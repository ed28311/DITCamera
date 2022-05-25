cmake_minimum_required(VERSION 3.1)

project(DITCamera)

SET(BASEDIR             ${CMAKE_SOURCE_DIR})
SET(THIRDPARTY 		    ${BASEDIR}/thirdparty)
SET(STR_DIR             ${BASEDIR}/str)
SET(ALG_DIR             ${STR_DIR}/method)
SET(JSON_DIR 			${THIRDPARTY}/nlohmann)

SET(LIBRARY_OUTPUT_DIRECTORY 	${PROJECT_BINARY_DIR}/lib)
SET(EXECUTABLE_OUTPUT_PATH 	    ${PROJECT_BINARY_DIR}/bin)

add_subdirectory(${JSON_DIR})
find_package(nlohmann_json 3.2.0 REQUIRED CONFIG PATHS ${JSON_DIR})
find_package(OpenCV REQUIRED PATHS "/usr/include/opencv2")
if(TEST_MODE)
    SET(DIT_SOURCE_MAIN algorithm_test.cpp)
else()
    SET(DIT_SOURCE_MAIN DITCameraTestApp.cpp)
endif()

file(GLOB DIT_SOURCE_FILES      "${STR_DIR}/*.cpp")
file(GLOB DIT_SOURCE_FILES_EXT  "${ALG_DIR}/*.cpp")
SET(DIT_SOURCE_LIB  ${DIT_SOURCE_FILES_EXT} ${DIT_SOURCE_FILES} ${DIT_SOURCE_MAIN})
# list(APPEND DIT_SOURCE_FILES    ${DIT_SOURCE_FILES_EXT})



add_executable(main ${DIT_SOURCE_LIB})
target_include_directories(main PUBLIC ${STR_DIR})
target_link_libraries(main nlohmann_json::nlohmann_json)
target_link_libraries(main ${OpenCV_LIBS})

