#include <iostream>
#include <regex>
#include <stdlib.h>
#include "./str/config_loader.hpp"
#include "./str/algorithm_dispatcher.hpp"
#include "./str/tool/displayer.hpp"
#include "./str/reporter.hpp"

#ifndef WindowsDetermined
#define WindowsDetermined
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define isWindowsBlock
#include "windows.h"main_report
static const bool isWindows = true;
#else
static const bool isWindows = false;
#endif
#endif

void checkDITArgument(int, char **);
std::vector<std::string>  parseDITMode(std::string);

int main(int argc, char * argv[]) {
	try {
		checkDITArgument(argc, argv);
		DITCameraTool::ConfigLoader config_loader;
		DITCameraTool::Reporter main_report;
		std::vector<std::string> global_config_members = {"ATD_OUTPUT_INFO", "ASUS_DebugInfo", "Project"};
		std::vector<std::string> report_basic_property = {"ITEM", "STATUS", "VALUE", "UCL", "LCL", "RESULT", "SPEC_NAME", "DATE_TIME", "OBJ_NAME", "COMMENT", "OTHERS", "IMG" };
		for (int i = 0; i < (argc / 3); i++) {
			std::string mode = std::string(argv[3 * i + 1]);
			std::string config_path = std::string(argv[3 * i + 2]);
			std::string image_path = std::string(argv[3 * i + 3]);
			DITCameraTool::Config config = config_loader.GetSPEConfig(config_path, mode, global_config_members);
			DITCameraTool::Reporter reporter(config, report_basic_property);
			DITCameraTool::AlgorithmDispatch* dispatcher = new DITCameraTool::AlgorithmDispatch(config, image_path, reporter);
			printf("---------------------------\n");
			_PrintVariable(mode);
			_PrintVariable(config_path);
			_PrintVariable(image_path);
			printf("---------------------------\n");
			dispatcher->ExecuteAlgorithm();
			delete dispatcher;
			main_report.MergeReporter(reporter);
		}
		main_report.GenerateCSV();
	}
	catch (std::invalid_argument& e) {
		std::cerr << e.what() << std::endl;
		#ifdef isWindowsBlock
		system("pause");
		#endif
		return -1;
	}
	#ifdef isWindowsBlock
	system("pause");
	#endif
	return 0;
}


void checkDITArgument(int argc, char ** argv) {
	if ((argc % 3) != 1 || (argc < 4)) {
		throw std::invalid_argument("Invalid arguments number. (format: -mode[modeName] config_path image_path)");
	}
}