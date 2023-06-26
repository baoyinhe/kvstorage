cmake_minimum_required(VERSION 3.12)
project(kvSDK)

set(CMAKE_CXX_STANDARD 11)
option(BUILD_SHARED_LIBS "Specifies the type of libraries (SHARED or STATIC) to build" ON)

set(CMAKE_DEBUG_POSTFIX "_d") 
set(CMAKE_RELEASE_POSTFIX "_r") 

# if(NOT CMAKE_BUILD_TYPE)
#     set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
# endif()

# # 添加一个选项，用于指定构建类型
# set(BUILD_TYPE "Release" CACHE STRING "Build type (Debug or Release)")
# set_property(CACHE BUILD_TYPE PROPERTY STRINGS "Debug" "Release")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/libs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/libs)

# Add the include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/sqlite)

# list of sources files of the library
set(KVSDK_SRC
 ${PROJECT_SOURCE_DIR}/src/realkvstorage.cpp
)

if (BUILD_SHARED_LIBS)
    add_library(kvSDK SHARED ${KVSDK_SRC})
    target_compile_definitions(kvSDK PUBLIC -DKVSTORAGE_SHARED_BUILD)
    target_compile_definitions(kvSDK PRIVATE -DKVSTORAGE_EXPORTS)
else()
    add_library(kvSDK STATIC ${KVSDK_SRC})
endif()

target_include_directories(kvSDK
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link the sqlite library
target_link_libraries(kvSDK PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/sqlite/sqlite3.lib)

set_target_properties(kvSDK PROPERTIES DEBUG_POSTFIX "_d") 
set_target_properties(kvSDK PROPERTIES RELEASE_POSTFIX "_r") 

# Set the public headers
set(PUBLIC_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/kvstorage.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/kvstorage_export.h
)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/distribution)

# Install the public headers
install(FILES ${PUBLIC_HEADERS} TYPE INCLUDE)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/sqlite/sqlite3.dll TYPE BIN)

if (BUILD_SHARED_LIBS)
  # Install the shared library
  install(TARGETS kvSDK
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
  )
else()
  install(TARGETS kvSDK
    ARCHIVE DESTINATION lib
  )
endif()

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

option(KVSDK_BUILD_EXAMPLES "Build examples." ON)
if (KVSDK_BUILD_EXAMPLES)
  add_subdirectory(examples/example1)
  file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/sqlite/sqlite3.dll DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug)
endif()