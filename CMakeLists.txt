cmake_minimum_required (VERSION 2.6)
project (DHBWCC)

# We want a debug build by default
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Debug")
endif()

# We want all warnings and pedantic programming-style
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic -Wno-unused-variable -Wno-unused-function -Wno-unused-parameter -std=gnu99")

# Disable compiler warnings on different sign compare (for flex)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-sign-compare")

# Define locations of sources and binaries
set(INPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/")
set(OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/src/")
file(MAKE_DIRECTORY ${OUTPUT_DIR})

# Find uthash library
include(cmake/Finduthash.cmake)
if(NOT UTHASH_FOUND)
	message(FATAL_ERROR "uthash library not found, CMake will exit." )
endif()

# Find and run bison and flex
find_package(BISON 3.0.4)
find_package(FLEX 2.6.0)
bison_target(parser ${INPUT_DIR}/parser.y ${OUTPUT_DIR}/parser.y.c
	COMPILE_FLAGS "-v -t -rall --report-file=${OUTPUT_DIR}/parser.y.out")
flex_target(scanner ${INPUT_DIR}/scanner.l ${OUTPUT_DIR}/scanner.y.c)
add_flex_bison_dependency(scanner parser)

# Target to build dhbwcc from source, generated files (bison and flex) and uthash library
file(GLOB_RECURSE DHBWCC_SRC "${INPUT_DIR}/*.c")
add_executable(dhbwcc
	${DHBWCC_SRC}
	${UTHASH_INCLUDE_DIR}
	${BISON_parser_OUTPUTS}
	${FLEX_scanner_OUTPUTS})
target_include_directories(dhbwcc
	PUBLIC "${INPUT_DIR}" "${INPUT_DIR}/include" "${OUTPUT_DIR}"
)

# New target to build doxygen from source
find_package(Doxygen)
set(DOXYGEN_CONFIG "doxygen/config")
add_custom_target(dhbwdoc
	COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_CONFIG}
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	COMMENT "Generating API documentation with Doxygen"
	VERBATIM
)
