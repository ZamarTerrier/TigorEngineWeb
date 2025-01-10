cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0015 NEW)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

set(CMAKE_C_COMPILER emcc)
set(CMAKE_CXX_COMPILER em++)

SET(CMAKE_C_FLAGS " --js-library ../libs/lib_yandex.js --bind -sUSE_SDL=2 -sUSE_SDL_IMAGE=2 -sFULL_ES2=1  -sWASM=0 --preload-file assets --use-preload-plugins ") 


set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
project(TestEngine LANGUAGES C)

add_compile_definitions(__EMSCRIPTEN__)

set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build.emscripten)


file(GLOB_RECURSE CORE_SRC src/*.c)

#
# Trying usual cmake project compiler execution
#

include_directories(libs includes)
add_executable(${PROJECT_NAME} ${CORE_SRC})
#target_link_libraries(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/build/libTigorEngineWeb.a)
