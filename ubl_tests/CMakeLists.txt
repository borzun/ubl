cmake_minimum_required(VERSION 2.8)

message("START downloading googletest")

###################################################################################################

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

message("FINISHED downloading googletest")

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/gtest/src
                 ${CMAKE_CURRENT_BINARY_DIR}/gtest/build
                 EXCLUDE_FROM_ALL)

message(STATUS "gtest_SOURCE_DIR: ${gtest_SOURCE_DIR}")

###################################################################################################
message("START UBL_TESTS configuring")

find_library(UBL_LIBRARY ubl)

message(STATUS "UBL library is: ${UBL_LIBRARY}")

link_directories(${PROJECT_SOURCE_DIR}/bin)

file(GLOB_RECURSE UBL_TESTS_HEADER_FILES "*.h")
file(GLOB_RECURSE UBL_TESTS_SOURCE_FILES "*.cpp")

message(STATUS "UBL_TESTS_HEADER_FILES: ${UBL_TESTS_HEADER_FILES}")
message(STATUS "UBL_TESTS_SOURCE_FILES: ${UBL_TESTS_SOURCE_FILES}")

add_executable(ubl_tests ${UBL_TESTS_HEADER_FILES} ${UBL_TESTS_SOURCE_FILES})

target_include_directories(ubl_tests PRIVATE ${PROJECT_SOURCE_DIR}/ubl ${gtest_SOURCE_DIR}/include)
add_dependencies(ubl_tests ubl gtest)
target_link_libraries(ubl_tests ubl)
target_link_libraries(ubl_tests gtest)
# target_link_libraries(ubl_tests ${GTEST_BINARY_DIR1}/libgtest.a)
# target_link_libraries(ubl_tests ${GTEST_BINARY_DIR1}/libgtest_main.a)

set_target_properties(ubl_tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
install (TARGETS ubl_tests RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/_install)

message(STATUS "FINISH UBL_TESTS configuring")

