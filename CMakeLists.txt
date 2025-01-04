cmake_minimum_required(VERSION 3.15)

# Set the project name and version
project(pfnet_miner VERSION 1.0 LANGUAGES C CXX)

# Set the build type to Debug by default
set(CMAKE_BUILD_TYPE Debug)

# Define the compilers
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

message(STATUS "Configuring pfnet_miner project")

# OpenCL settings
set(OPENCL_VERSION_STRING "0.1.0")
set(OPENCL_VERSION_MAJOR 0)
set(OPENCL_VERSION_MINOR 1)
set(OPENCL_VERSION_PATCH 0)

# Find OpenCL
find_package(OpenCL REQUIRED)

if (OpenCL_FOUND)
    message(STATUS "OpenCL found:")
    message(STATUS "  Includes: ${OpenCL_INCLUDE_DIRS}")
    message(STATUS "  Libraries: ${OpenCL_LIBRARIES}")
else()
    message(FATAL_ERROR "OpenCL not found. Please ensure OpenCL is installed.")
endif()

# Find CURL
find_package(CURL REQUIRED)
if (CURL_FOUND)
    message(STATUS "CURL found:")
    message(STATUS "  Includes: ${CURL_INCLUDE_DIRS}")
    message(STATUS "  Libraries: ${CURL_LIBRARIES}")
else()
    message(FATAL_ERROR "CURL not found. Please ensure CURL is installed.")
endif()

# Find OpenSSL
find_package(OpenSSL REQUIRED)
if (OpenSSL_FOUND)
    message(STATUS "OpenSSL found:")
    message(STATUS "  Includes: ${OPENSSL_INCLUDE_DIR}")
    message(STATUS "  Libraries: ${OPENSSL_LIBRARIES}")
else()
    message(FATAL_ERROR "OpenSSL not found. Please ensure OpenSSL is installed.")
endif()

# Include directories
include_directories(
    ${OpenCL_INCLUDE_DIRS}
    ${CURL_INCLUDE_DIRS}
    ${OPENSSL_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}/compat/jansson
    ${PROJECT_SOURCE_DIR}/algo
)

# Source files
set(SRC_LIST
    pfnet_miner.c
    pfnet_compiler.c
    util.c
    ocl.c
    ./pfnetPL/pfnetPL.c
    ./pfnetPL/pfnetPLTokenManager.c
    ./pfnetPL/pfnetPLParser.c
    ./pfnetPL/pfnetPLInterpreter.c
    ./pfnetPL/pfnetPLMath.c
    ./pfnetPL/pfnetPLConvert.c
    ./crypto/curve25519-donna.c
    ./crypto/sha2.c
)

# Add executable
add_executable(${PROJECT_NAME} ${SRC_LIST})

# Link libraries
if (OpenCL_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenCL_LIBRARIES})
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${CURL_LIBRARIES}
    ${OPENSSL_LIBRARIES}
    pthread
    m
    dl
    jansson
)

# Set target properties
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}
)

# Compiler flags for specific platforms
if (${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm")
    target_compile_options(${PROJECT_NAME} PRIVATE -Ofast -fPIC -g)
elseif (WIN32)
    target_compile_options(${PROJECT_NAME} PRIVATE -Ofast -msse -msse2 -msse3 -mmmx -m3dnow -DHAVE_STRUCT_TIMESPEC)
    target_link_libraries(${PROJECT_NAME} PRIVATE Ws2_32)
elif (${CMAKE_SYSTEM_PROCESSOR} MATCHES "(x86)|(X86)|(amd64)|(AMD64)")
    target_compile_options(${PROJECT_NAME} PRIVATE -O0 -fPIC -g)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -O0 -fPIC -g)
endif()

message(STATUS "Configuration complete. Ready to build.")

