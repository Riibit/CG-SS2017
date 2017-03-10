#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>
#include "config_utils.h"
#include "scene_builder.h"
#include <json/document.h>

namespace
{
  template <typename T>
  T ref(const std::vector<T>& c, int i)
  {
    return i > 0 ? c[i - 1] : c[c.size() + i];
  }

  template <typename T>
  T ref(const std::vector<T>& c, int i, const T& default_value)
  {
    return i > 0 ? c[i - 1] : (i == 0 ? default_value : c[c.size() + i]);
  }
}

void SceneBuilder::Load(SceneBuilder& builder, rapidjson::Value& test)
{
	rapidjson::Value& scene = test["scene"];

	float3 background = getAttribute<float3>(scene, "background", float3(0.6f, 0.8f, 1.0f));
	builder.SetBackground(background);	

	rapidjson::Value& camera = test["camera"];
	float3 eye = getAttribute<float3>(camera, "eye", float3(0, 0, 0));
	float3 lookat = getAttribute<float3>(camera, "lookat", float3(0.0f, 0.0f, 0.0f));
	float3 up = getAttribute<float3>(camera, "up", float3(0.0f, 1.0f, 0.0f));
	float f = getAttribute<float>(camera, "f", 0.1f);
	float beta = getAttribute<float>(camera, "beta", 60.0f) * math::constants<float>::pi() / 180.0f;

	builder.SetCamera(eye, lookat, up, f, beta);

	std::unordered_map<std::string, int> material_map;

	if (scene.HasMember("materials"))
	{
		rapidjson::Value& materials = scene["materials"];

		size_t num_materials = materials.Size();

		for (int i = 0; i < num_materials; i++)
		{
			rapidjson::Value& mat = materials[i];

			std::string name = getAttribute<std::string>(mat, "name", "");

			bool reflecting = getAttribute<bool>(mat, "reflection", false);

			float3 k_d = getAttribute<float3>(mat, "k_d", reflecting ? float3(0.0f, 0.0f, 0.0f) : float3(0.8f, 0.8f, 0.8f));

			float shininess = getAttribute<float>(mat, "shininess", 1.0f);

			float3 k_s = getAttribute<float3>(mat, "k_s", reflecting ? float3(1.0f, 1.0f, 1.0f) : float3(0.0f, 0.0f, 0.0f));

			std::string tex = getAttribute<std::string>(mat, "texture", "");

			if (!tex.empty())
				material_map.insert(std::make_pair(name, builder.AddTexturedMaterial(tex.c_str(), k_s, shininess)));
			else if (reflecting)
				material_map.insert(std::make_pair(name, builder.AddReflectiveMaterial(k_d, k_s)));
			else
				material_map.insert(std::make_pair(name, builder.AddUntexturedMaterial(k_d, k_s, shininess)));
		}
	}

	if (scene.HasMember("objects"))
	{
		rapidjson::Value& objects = scene["objects"];

		size_t num_objects = objects.Size();

		for (int i = 0; i < num_objects; i++)
		{
			rapidjson::Value& obj = objects[i];
			std::string src = getAttribute<std::string>(obj, "src", "");
			std::cout << "loading " << src.c_str() << std::endl;

			float3x4 M = getAttribute<float3x4>(obj, "transform", float3x4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f));
			float3x3 M_n = inverse(float3x3(M._11, M._21, M._31, M._12, M._22, M._32, M._13, M._23, M._33));

			std::vector<float3> v;
			std::vector<float3> vn;
			std::vector<float2> vt;
			int material_index = -1;

			std::ifstream file(src.c_str());
			std::string cmd;

			while (file >> cmd)
			{
				if (cmd[0] == '#')
				{
					while (file && file.get() != '\n')
						;
				}
				else if (cmd == "v")
				{
					float3 p;
					file >> p.x >> p.y >> p.z;
					v.push_back(M * float4(p, 1.0f));
				}
				else if (cmd == "vn")
				{
					float3 n;
					file >> n.x >> n.y >> n.z;
					vn.push_back(normalize(M_n * n));
				}
				else if (cmd == "vt")
				{
					float2 t;
					file >> t.x >> t.y;
					t.y = 1.0f - t.y;
					vt.push_back(t);
				}
				else if (cmd == "f")
				{
					int indices[3][3];
					for (int i = 0; i < 3; ++i)
					{
						file >> indices[i][0];
						if (file.get() == '/')
						{
							if (file.peek() != '/')
							{
								file >> indices[i][1];
								if (file.get() == '/')
									file >> indices[i][2];
								else
									indices[i][2] = 0;
							}
							else
							{
								file.get();
								indices[i][1] = 0;
								file >> indices[i][2];
							}
						}
						else
							indices[i][1] = indices[i][2] = 0;
					}

					auto v1 = ref(v, indices[0][0]);
					auto v2 = ref(v, indices[1][0]);
					auto v3 = ref(v, indices[2][0]);
					auto n = normalize(cross(v2 - v1, v3 - v1));
					auto vn1 = ref(vn, indices[0][2], n);
					auto vn2 = ref(vn, indices[1][2], n);
					auto vn3 = ref(vn, indices[2][2], n);
					auto vt1 = ref(vt, indices[0][1], float2(0.0f, 0.0f));
					auto vt2 = ref(vt, indices[1][1], float2(0.0f, 0.0f));
					auto vt3 = ref(vt, indices[2][1], float2(0.0f, 0.0f));

					builder.AddTriangle(v1, v2, v3, vn1, vn2, vn3, vt1, vt2, vt3, material_index);
				}
				else if (cmd == "usemtl")
				{
					std::string name;
					file >> name;
					auto f = material_map.find(name);
					if (f != end(material_map))
						material_index = f->second;
					else
						material_index = -1;
				}
			}
		}
	}

	if (scene.HasMember("lights"))
	{
		rapidjson::Value& lights = scene["lights"];
		size_t num_lights = lights.Size();

		for (int j = 0; j < num_lights; ++j)
		{
			const rapidjson::Value& light = lights[j];

			float3 pos = getAttribute<float3>(light, "position", float3(0.0f));
			float3 color = getAttribute<float3>(light, "color", float3(1.0f));

			builder.AddPointlight(pos, color);
		}
	}
}
