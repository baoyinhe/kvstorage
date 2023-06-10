# CMakeLists.txt in SDK/

cmake_minimum_required(VERSION 3.12)
project(kvSDK)

set(CMAKE_CXX_STANDARD 11)
option(BUILD_SHARED_LIBS "Specifies the type of libraries (SHARED or STATIC) to build" ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/lib)

# Add the include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

if (BUILD_SHARED_LIBS)
    add_library(kvSDK SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/kvstorage.cpp)
    target_compile_definitions(kvSDK PUBLIC -DKVSTORAGE_SHARED_BUILD)
    target_compile_definitions(kvSDK PRIVATE -DKVSTORAGE_EXPORTS)
else()
    add_library(kvSDK STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/kvstorage.cpp)
endif()

# Link the sqlite library
target_link_libraries(kvSDK PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/sqlite/sqlite3.lib)

# Set the public headers
set(PUBLIC_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/kvstorage.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/kvstorage_export.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/sqlite3.h
)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/dist)

# Install the public headers
install(FILES ${PUBLIC_HEADERS} TYPE INCLUDE)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/sqlite/sqlite3.dll TYPE BIN)

if (BUILD_SHARED_LIBS)
  # Install the shared library
  install(TARGETS kvSDK
    RUNTIME DESTINATION bin
  )
else()
  install(TARGETS kvSDK
    ARCHIVE DESTINATION lib
  )
endif()


set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})