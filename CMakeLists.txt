cmake_minimum_required(VERSION 3.0)

project(DITCameraTool CXX)

SET(BASEDIR             ${CMAKE_SOURCE_DIR})
SET(THIRDPARTY 		    ${BASEDIR}/thirdparty)
SET(STR_DIR             ${BASEDIR}/str)
SET(ALG_DIR             ${STR_DIR}/method)
SET(JSON_GIT_DIR 		${THIRDPARTY}/nlohmann)
SET(OPENCV_GIT_DIR      ${THIRDPARTY}/opencv)
SET(OPENCV_SOURCE_DIR      ${THIRDPARTY}/opencv2/sources)
SET(OPENCV_BUILD_DIR      ${THIRDPARTY}/opencv2/build)
SET(TOOL_DIR            ${STR_DIR}/tool)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    SET(CMAKE_CXX_FLAGS     "-lstdc++fs -std=c++17")
endif()
# ===== Build Execute Distination =====
include_directories(${OPENCV_GIT_DIR})
if(TEST_MODE)
    SET(DIT_SOURCE_MAIN algorithm_test.cpp)
else()
    SET(DIT_SOURCE_MAIN DITCameraTestApp.cpp)
endif()

file(GLOB DIT_SOURCE_FILES      "${STR_DIR}/*.cpp")
file(GLOB DIT_SOURCE_FILES_EXT  "${ALG_DIR}/*.cpp")
file(GLOB DIT_TOOL_EXT          "${TOOL_DIR}/*.cpp")
SET(DIT_SOURCE_LIB  ${DIT_SOURCE_FILES_EXT} ${DIT_SOURCE_FILES} ${DIT_SOURCE_MAIN} ${DIT_TOOL_EXT})
add_executable(main ${DIT_SOURCE_LIB})

# ===== Set nlohmann/json environment variable =====
add_subdirectory(${JSON_GIT_DIR})
target_link_libraries(main nlohmann_json::nlohmann_json)
target_include_directories(main PUBLIC ${JSON_GIT_DIR}/include)

# # ===== Set OpenCV environment variable =====

# add_subdirectory(${OPENCV_GIT_DIR})
# find_package(OpenCV REQUIRED)
# INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})
# target_link_libraries(main ${OpenCV_LIBS})


# target_link_libraries(main opencv_core opencv_highgui opencv_imgproc)
add_subdirectory(${OPENCV_GIT_DIR})
target_link_libraries(main opencv_core opencv_highgui)
target_include_directories(main PUBLIC 
    ${CMAKE_CURRENT_BINARY_DIR}
    ${OPENCV_GIT_DIR}/include
    ${OPENCV_GIT_DIR}/modules/calib3d/include
    ${OPENCV_GIT_DIR}/modules/core/include
    ${OPENCV_GIT_DIR}/modules/cudaarithm/include
    ${OPENCV_GIT_DIR}/modules/cudabgsegm/include
    ${OPENCV_GIT_DIR}/modules/cudacodec/include
    ${OPENCV_GIT_DIR}/modules/cudafeatures2d/include
    ${OPENCV_GIT_DIR}/modules/cudafilters/include
    ${OPENCV_GIT_DIR}/modules/cudaimgproc/include
    ${OPENCV_GIT_DIR}/modules/cudalegacy/include
    ${OPENCV_GIT_DIR}/modules/cudaobjdetect/include
    ${OPENCV_GIT_DIR}/modules/cudaoptflow/include
    ${OPENCV_GIT_DIR}/modules/cudastereo/include
    ${OPENCV_GIT_DIR}/modules/cudawarping/include
    ${OPENCV_GIT_DIR}/modules/cudev/include
    ${OPENCV_GIT_DIR}/modules/dnn/include
    ${OPENCV_GIT_DIR}/modules/features2d/include
    ${OPENCV_GIT_DIR}/modules/flann/include
    ${OPENCV_GIT_DIR}/modules/highgui/include
    ${OPENCV_GIT_DIR}/modules/imgcodecs/include
    ${OPENCV_GIT_DIR}/modules/imgproc/include
    ${OPENCV_GIT_DIR}/modules/ml/include
    ${OPENCV_GIT_DIR}/modules/objdetect/include
    ${OPENCV_GIT_DIR}/modules/photo/include
    ${OPENCV_GIT_DIR}/modules/shape/include
    ${OPENCV_GIT_DIR}/modules/stitching/include
    ${OPENCV_GIT_DIR}/modules/superres/include
    ${OPENCV_GIT_DIR}/modules/ts/include
    ${OPENCV_GIT_DIR}/modules/video/include
    ${OPENCV_GIT_DIR}/modules/videoio/include
    ${OPENCV_GIT_DIR}/modules/videostab/include
    ${OPENCV_GIT_DIR}/modules/viz/include
    ${OPENCV_GIT_DIR}/modules/world/include
)
target_link_libraries(main ${OpenCV_LIBS})

#=========

target_include_directories(main PUBLIC ${STR_DIR})

