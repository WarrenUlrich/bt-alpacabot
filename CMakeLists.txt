cmake_minimum_required(VERSION 3.19)

project(bt-alpacabot)

add_subdirectory(bt)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT DEFINED ALPACABOT_ROOT_DIR)
  set(ALPACABOT_ROOT_DIR $ENV{USERPROFILE}\\AlpacaBot)
endif()

if (NOT DEFINED ALPACABOT_LIB_DIR)
  set(ALPACABOT_LIB_DIR $ENV{USERPROFILE}\\AlpacaBot\\Library)
endif()

if (NOT DEFINED ALPACABOT_INCLUDE_DIR)
  set(ALPACABOT_INCLUDE_DIR $ENV{USERPROFILE}\\AlpacaBot\\Include)
endif()

add_library(bt-alpacabot INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${ALPACABOT_INCLUDE_DIR}
)

target_link_directories(${PROJECT_NAME} INTERFACE
  ${ALPACABOT_LIB_DIR}
)

target_link_libraries(${PROJECT_NAME} INTERFACE
  bt
  AlpacaLibrary
)

add_subdirectory(alcher)