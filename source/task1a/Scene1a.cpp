
#include "config_utils.h"
#include "Scene1a.h"

Scene1a::Scene1a(rapidjson::Value& testcase)
{
	viewBox = getAttribute<float2>(testcase, "viewBox", float2(0, 1));

  SceneBuilder::Load(*this, testcase);
  buildBVH();
}

void Scene1a::SetCamera(const float3& eye, const float3& lookat, const float3& up, float f, float beta)
{
  camera.eye = eye;
  camera.lookat = lookat;
  camera.up = up;
  camera.f = f;
  camera.beta = beta;
}

float Scene1a::scaleDistance(float d) const
{
  return 1.0f - ((float)((1.0 / (viewBox.y - viewBox.x)) * (d - viewBox.x)));
}
