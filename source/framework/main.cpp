#include <iostream>
#include <memory>
#include <json/document.h>

#include "config_utils.h"

extern const char* taskname;
extern const char* version_string;

extern void run_testcase(rapidjson::Value& testcase, const std::string& output, int width, int height);

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cout << "Usage: ./" << taskname << " <config-file>\n" << std::endl;
    return -1;
  }

  std::cout << version_string << std::endl; // DO NOT REMOVE THIS LINE!!!

  try
  {
		Config cfg;
		cfg.open(argv[1]);

		rapidjson::Value& testcase = cfg.getJson();

		std::string output = getAttribute<std::string>(testcase, "output", "output.png");
		int width = getAttribute<int>(testcase, "width", 800);
		int height = getAttribute<int>(testcase, "height", 600);

		run_testcase(testcase, output, width, height);
  }
  catch (std::exception& e)
  {
    std::cout << "error: " << e.what() << std::endl;
    return -1;
  }

  return 0;
}
