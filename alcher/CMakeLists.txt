cmake_minimum_required(VERSION 3.8)

project("alcher")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE ${PROJECT_NAME}_SOURCES "src/*.cpp")

add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_SOURCES})

set(CMAKE_SHARED_LINKER_FLAGS "-Os -s -m32 -static-libgcc -static-libstdc++ -static")

target_include_directories(${PROJECT_NAME} PUBLIC "include")

target_link_libraries(${PROJECT_NAME}
  bt-alpacabot
)

set_target_properties(${PROJECT_NAME}
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${ALPACABOT_ROOT_DIR}/Scripts/Local/${PROJECT_NAME}"
)

set_target_properties(${PROJECT_NAME}
  PROPERTIES PREFIX ""
  SUFFIX ".dll"
)