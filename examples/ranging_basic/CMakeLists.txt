cmake_minimum_required(VERSION 3.9)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

list(APPEND EXTRA_COMPONENT_DIRS ../..)
project(vl53l5cx_ranging_basic)

