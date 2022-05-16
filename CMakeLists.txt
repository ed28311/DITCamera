cmake_minimum_required(VERSION 3.1)

project(DITCamera)

SET(BASEDIR             ${CMAKE_SOURCE_DIR})
SET(THIRDPARTY 		    ${BASEDIR}/thirdparty)
SET(JSON_DIR 			${THIRDPARTY}/nlohmann)
SET(STR_DIR             ${BASEDIR}/str)
SET(LIBRARY_OUTPUT_DIRECTORY 	${PROJECT_BINARY_DIR}/lib)
SET(EXECUTABLE_OUTPUT_PATH 	    ${PROJECT_BINARY_DIR}/bin)


include_directories(${STR_DIR})
find_package(nlohmann_json 3.2.0 REQUIRED CONFIG PATHS ${JSON_DIR})
add_executable(main ${BASEDIR}/DITCameraTestApp.cpp)
target_link_libraries(main nlohmann_json::nlohmann_json)

