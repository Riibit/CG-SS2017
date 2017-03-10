

#ifndef INCLUDED_CONFIG_UTILS
#define INCLUDED_CONFIG_UTILS

#pragma once

#include <stdexcept>
#include <array>
#include <vector>
#include <fstream>
#include <iostream>

#include "vector.h"
#include "matrix.h"
#include <json/document.h>

class Config
{
private:
	rapidjson::Value test_case_;
	rapidjson::Document doc;

public:
	void open(const char* fname)
	{
		std::ifstream fs(fname);
		std::string str((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());

		rapidjson::ParseResult check = doc.Parse<0>(str.c_str());

		if (check)
		{
			test_case_ = doc["test"];
		}
		else
		{
			std::cout << "Error parsing file!" << std::endl;
			getchar();
		}
	}

	rapidjson::Value& getJson()
	{
		return test_case_;
	}
};

template <typename Type>
inline Type getAttribute(const rapidjson::Value& testcase, const std::string& name);

template <>
inline int32_t getAttribute(const rapidjson::Value& testcase, const std::string& name)
{
	return testcase[name.c_str()].GetInt();
}

template <>
inline float getAttribute(const rapidjson::Value& testcase, const std::string& name)
{
	return static_cast<float>(testcase[name.c_str()].GetDouble());
}

template <>
inline bool getAttribute(const rapidjson::Value& testcase, const std::string& name)
{
	return testcase[name.c_str()].GetBool();
}

template <>
inline std::string getAttribute(const rapidjson::Value& testcase, const std::string& name)
{
	return std::string(testcase[name.c_str()].GetString());
}

template <>
inline float2 getAttribute(const rapidjson::Value& testcase, const std::string& name)
{
	const rapidjson::Value& v = testcase[name.c_str()];
	return float2(static_cast<float>(v[0].GetDouble()), static_cast<float>(v[1].GetDouble()));
}

template <>
inline float3 getAttribute(const rapidjson::Value& testcase, const std::string& name)
{
	const rapidjson::Value& v = testcase[name.c_str()];
	return float3(static_cast<float>(v[0].GetDouble()), static_cast<float>(v[1].GetDouble()), static_cast<float>(v[2].GetDouble()));
}

template <>
inline float3x4 getAttribute(const rapidjson::Value& testcase, const std::string& name)
{
	float3x4 mat;
	const rapidjson::Value& v = testcase[name.c_str()];

	mat._11 = static_cast<float>(v[0].GetDouble());
	mat._12 = static_cast<float>(v[1].GetDouble());
	mat._13 = static_cast<float>(v[2].GetDouble());
	mat._14 = static_cast<float>(v[3].GetDouble());

	mat._21 = static_cast<float>(v[4].GetDouble());
	mat._22 = static_cast<float>(v[5].GetDouble());
	mat._23 = static_cast<float>(v[6].GetDouble());
	mat._24 = static_cast<float>(v[7].GetDouble());

	mat._31 = static_cast<float>(v[8].GetDouble());
	mat._32 = static_cast<float>(v[9].GetDouble());
	mat._33 = static_cast<float>(v[10].GetDouble());
	mat._34 = static_cast<float>(v[11].GetDouble());

	return mat;
}

template <typename Type>
inline Type getAttribute(const rapidjson::Value& testcase, const std::string& name, Type default_value)
{
	if (testcase.HasMember(name.c_str()))
	{
		return getAttribute<Type>(testcase, name);
	}
	return default_value;//; testcase.getAttribute<Type>(name);
}
//
//template <typename Type>
//inline Type getAttribute(const CgcvConfig::Container& container, const std::string& name, Type default_value)
//{
//  try
//  {
//    return container.getAttribute<Type>(name);
//  }
//  catch (const CgcvConfig::ConfigError&)
//  {
//  }
//
//  return default_value;
//}
//
//template <typename T, unsigned int N>
//math::vector<T, N> make_vector(const T (&v)[N]);
//
//template <typename T>
//inline math::vector<T, 2> make_vector(const T (&v)[2])
//{
//  return math::vector<T, 2>(v[0], v[1]);
//}
//
//template <typename T>
//inline math::vector<T, 3> make_vector(const T (&v)[3])
//{
//  return math::vector<T, 3>(v[0], v[1], v[2]);
//}
//
//template <typename T, unsigned int N>
//math::vector<T, N> make_vector(const std::array<T, N>& v);
//
//template <typename T>
//inline math::vector<T, 2> make_vector(const std::array<T, 2>& v)
//{
//  return math::vector<T, 2>(v[0], v[1]);
//}
//
//template <typename T>
//inline math::vector<T, 3> make_vector(const std::array<T, 3>& v)
//{
//  return math::vector<T, 3>(v[0], v[1], v[2]);
//}
//
//template <typename T>
//inline math::matrix<T, 2, 2> make_matrix(const std::array<std::array<T, 2>, 2>& v)
//{
//  return math::matrix<T, 3, 4>(v[0][0], v[0][1], v[1][0], v[1][1]);
//}
//
//template <typename T>
//inline math::matrix<T, 3, 3> make_matrix(const std::array<std::array<T, 3>, 3>& v)
//{
//  return math::matrix<T, 3, 4>(v[0][0], v[0][1], v[0][2], v[1][0], v[1][1], v[1][2], v[2][0], v[2][1], v[2][2]);
//}
//
//template <typename T>
//inline math::matrix<T, 3, 4> make_matrix(const std::array<std::array<T, 4>, 3>& v)
//{
//  return math::matrix<T, 3, 4>(v[0][0], v[0][1], v[0][2], v[0][3], v[1][0], v[1][1], v[1][2], v[1][3], v[2][0], v[2][1], v[2][2], v[2][3]);
//}
//
//template <typename T>
//inline math::matrix<T, 4, 4> make_matrix(const std::array<std::array<T, 4>, 4>& v)
//{
//  return math::matrix<T, 4, 4>(v[0][0], v[0][1], v[0][2], v[0][3], v[1][0], v[1][1], v[1][2], v[1][3], v[2][0], v[2][1], v[2][2], v[2][3], v[3][0], v[3][1], v[3][2], v[3][3]);
//}
//
//template <typename T, unsigned int N>
//inline math::vector<T, N> getVector(const CgcvConfig::Container& container, const std::string& name, const math::vector<T, N>& default_value)
//{
//  try
//  {
//    std::array<T, N> v;
//    container.getVector(v, name);
//    return make_vector(v);
//  }
//  catch (const CgcvConfig::ConfigError&)
//  {
//  }
//
//  return default_value;
//}
//
//template <typename T, unsigned int M, unsigned int N>
//inline math::matrix<T, M, N> getMatrix(const CgcvConfig::Container& container, const std::string& name, const math::matrix<T, M, N>& default_value)
//{
//  try
//  {
//    std::array<std::array<T, N>, M> m;
//    container.getMatrix(m, name);
//    return make_matrix(m);
//  }
//  catch (const CgcvConfig::ConfigError&)
//  {
//  }
//
//  return default_value;
//}

#endif // INCLUDED_CONFIG_UTILS
