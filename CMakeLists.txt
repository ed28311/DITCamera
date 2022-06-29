cmake_minimum_required(VERSION 3.14)

project(DITCameraTool CXX)

SET(BASEDIR             ${CMAKE_SOURCE_DIR})
SET(THIRDPARTY 		    ${BASEDIR}/thirdparty)
SET(STR_DIR             ${BASEDIR}/str)
SET(ALG_DIR             ${STR_DIR}/algorithm)
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
    SET(DIT_SOURCE_DITCamera DITCameraTest.cpp)
else()
    SET(DIT_SOURCE_DITCamera DITCameraTestApp.cpp)
endif()

file(GLOB DIT_SOURCE_FILES      "${STR_DIR}/*.cpp")
file(GLOB DIT_SOURCE_FILES_EXT  "${ALG_DIR}/*.cpp")
file(GLOB DIT_TOOL_EXT          "${TOOL_DIR}/*.cpp")
SET(DIT_SOURCE_LIB  ${DIT_SOURCE_FILES_EXT} ${DIT_SOURCE_FILES} ${DIT_SOURCE_DITCamera} ${DIT_TOOL_EXT})
add_executable(DITCamera ${DIT_SOURCE_LIB})

# ===== Set nlohmann/json environment variable =====
add_subdirectory(${JSON_GIT_DIR})
target_link_libraries(DITCamera nlohmann_json::nlohmann_json)
target_include_directories(DITCamera PUBLIC ${JSON_GIT_DIR}/include)

# # ===== Set OpenCV environment variable =====

add_subdirectory(${OPENCV_GIT_DIR})
target_link_libraries(DITCamera opencv_core opencv_highgui)
target_include_directories(DITCamera PUBLIC 
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
target_link_libraries(DITCamera ${OpenCV_LIBS})

#=========

target_include_directories(DITCamera PUBLIC ${STR_DIR})
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT DITCamera)
set_property(TARGET DITCamera PROPERTY VS_DEBUGGER_COMMAND_ARGUMENTS "-BL[Blemish] ${BASEDIR}/CameraSpec.spe ${BASEDIR}/test_pic/blemishNG.jpg -BL[Blemish] ${BASEDIR}/CameraSpec.spe ${BASEDIR}/test_pic/blemishOK.jpg -BL[Blemish] ${BASEDIR}/CameraSpec.spe ${BASEDIR}/test_pic/blemish.jpg -FL[Flare] ${BASEDIR}/CameraSpec.spe ${BASEDIR}/test_pic/flareNG.jpg -FL[Flare] ${BASEDIR}/CameraSpec.spe ${BASEDIR}/test_pic/flareOK.jpg -FL[Flare] ${BASEDIR}/CameraSpec.spe ${BASEDIR}/test_pic/flare.jpg -LS[Shading] ${BASEDIR}/CameraSpec.spe ${BASEDIR}/test_pic/shadingNG.jpg -LS[Shading] ${BASEDIR}/CameraSpec.spe ${BASEDIR}/test_pic/shadingOK.jpg -LS[Shading] ${BASEDIR}/CameraSpec.spe ${BASEDIR}/test_pic/shading.jpg")
# SET(VS_DEBUGGER_COMMAND_ARGUMENTS "-BL[Blemish] ${BASEDIR}/CameraSpec.spe ${BASEDIR}/test_pic/blemish.jpg -FL[Flare] ${BASEDIR}/CameraSpec.spe ${BASEDIR}/test_pic/flare.jpg -LS[Shading] ${BASEDIR}/CameraSpec.spe ${BASEDIR}/test_pic/shading.jpg")
