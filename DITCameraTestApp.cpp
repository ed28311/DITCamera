#include <iostream>
#include <regex>
#include <stdlib.h>
#include "./str/configLoader.hpp"
#include "./str/algorithmDispatcher.hpp"
#include "./str/tool/displayer.hpp"

#ifndef WindowsDetermined
#define WindowsDetermined
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define isWindowsBlock
#include "windows.h"
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
		DITCameraTool::ConfigLoader configLoader;
		DITCameraTool::Logger totalLogger;
		std::vector<std::string> argvDebug(argv, argv + argc);
		for (int i = 0; i < (argc / 3); i++) {
			std::string mode = std::string(argv[3 * i + 1]);
			std::string configPath = std::string(argv[3 * i + 2]);
			std::string imagePath = std::string(argv[3 * i + 3]);
			printf("---------------------------\n");
			_PrintVariable(mode);
			_PrintVariable(configPath);
			_PrintVariable(imagePath);
			printf("---------------------------\n");
			std::vector<std::string> modeVector(2);
			modeVector = parseDITMode(mode);
			DITCameraTool::Config DITConfig = configLoader.getSPEConfig(configPath, modeVector);
			json globalConfig = DITConfig.getGlobalConf();
			DITCameraTool::Logger logger(globalConfig);
			DITCameraTool::AlgorithmDispatch* dispatcher = new DITCameraTool::AlgorithmDispatch(DITConfig, imagePath, logger);
			dispatcher->executeAlgorithm();
			totalLogger.mergeLogger(logger);
			delete dispatcher;
		}
		bool logResult = totalLogger.generateCSV();
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

std::vector<std::string> parseDITMode(std::string DITMode) {
	std::vector<std::string> modeVector(2);
	std::regex reg("^-.*[\\w]+\\[.*[\\w]+\\]");
	std::string mode = DITMode;
	if (std::regex_match(mode, reg)) {
		std::smatch sm;
		std::regex matchstring("[\\w]+");
		int i = 0;
		while (std::regex_search(mode, sm, matchstring)) {
			for (std::string x : sm) {
				modeVector[i] = x;
			}
			mode = sm.suffix().str();
			i++;
			if (i > 2) {
				throw std::invalid_argument("Invalid arguments format (" + mode + "Wrong mode format.). (format: -mode[modeName] configPath imagePath)");
				break;
			}
		}
	}
	else {
		throw std::invalid_argument("Invalid arguments format (" + mode + "Wrong mode format.). (format: -mode[modeName] configPath imagePath)");
	}
	return modeVector;
}
void checkDITArgument(int argc, char ** argv) {
	if ((argc % 3) != 1 || (argc < 4)) {
		throw std::invalid_argument("Invalid arguments format (Too little arguments.). (format: -mode[modeName] configPath imagePath)");
	}
}