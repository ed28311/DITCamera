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
#define isWindowsToken
#include "windows.h"main_report
static const bool isWindows = true;
#else
static const bool isWindows = false;
#endif
#endif

void CheckTaskArgument(int, char **);

/*
Author: Edward Kuo(Chih-Rong Kuo)
Contact: edward_kuo@asus.com

**Check readme.md before use.**

Basic argument
main_executor -{mode}{configMode} {config path} {image path} -{mode}{configMode} {config path} {image path} 
*/

int main(int argc, char * argv[]) {
	try {
		CheckTaskArgument(argc, argv);
		DITCameraTool::ConfigLoader config_loader;
		/* main_report is to integrate all single report to report file. */
		DITCameraTool::Reporter* main_report = new DITCameraTool::Reporter();
		std::vector<std::string> global_config_members = {"ATD_OUTPUT_INFO", "ASUS_DebugInfo", "Project"};
		for (int i = 0; i < (argc / 3); i++) {
			std::string mode = std::string(argv[3 * i + 1]);
			std::string config_path = std::string(argv[3 * i + 2]);
			std::string image_path = std::string(argv[3 * i + 3]);
			DITCameraTool::Config config = config_loader.GetSPEConfig(config_path, mode, global_config_members);
			printf("---------------------------\n");
			_PrintVariable(mode);
			_PrintVariable(config_path);
			_PrintVariable(image_path);
			printf("---------------------------\n");

			DITCameraTool::AlgorithmDispatch* dispatcher = new DITCameraTool::AlgorithmDispatch(config, image_path);
			dispatcher->DispatchAlgorithm();
			dispatcher->ExecuteAlgorithm();
			DITCameraTool::Reporter reporter = dispatcher->GetReporter();
			if (dispatcher != NULL)
			{
				delete dispatcher;
				dispatcher = NULL;// In loop, it still need to be NULL.
			}
			main_report->MergeReporter(reporter);
		}
		main_report->GenerateCSV();
		if (main_report != NULL)
		{
			delete main_report;
			main_report = NULL;
		}
		
	}
	catch (std::invalid_argument& e) {
		std::cerr << e.what() << std::endl;
		#ifdef isWindowsToken
		system("pause");
		#endif
		return -1;
	}
	#ifdef isWindowsToken
	system("pause");
	#endif
	return 0;
}

/*
Description:
	Check argument numbers is valid.	
*/
void CheckTaskArgument(int argc, char ** argv) {
	if ((argc % 3) != 1) {
		throw std::invalid_argument("Invalid arguments number. (format: -mode[modeName] config_path image_path)");
	}
}