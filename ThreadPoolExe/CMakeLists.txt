cmake_minimum_required(VERSION 2.8)

find_library(UBL_LIBRARY ubl)
link_directories(${PROJECT_SOURCE_DIR}/bin)

add_executable(ThreadPoolExe main.cpp)

target_include_directories(ThreadPoolExe PRIVATE ${PROJECT_SOURCE_DIR}/ubl)

add_dependencies(ThreadPoolExe ubl)
target_link_libraries(ThreadPoolExe ubl)

set_target_properties(ThreadPoolExe PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
install (TARGETS ThreadPoolExe RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/_install)

