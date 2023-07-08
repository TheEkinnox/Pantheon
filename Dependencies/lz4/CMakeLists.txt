# Download lz4
FetchContent_Declare(
	lz4
	GIT_REPOSITORY https://github.com/lz4/lz4
	GIT_TAG v1.9.4
	GIT_SHALLOW ON
)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

set(BUILD_SHARED_LIBS OFF)
set(BUILD_SHARED_LIBS OFF)
set(LZ4_BUILD_CLI OFF)
set(LZ4_BUILD_LEGACY_LZ4C OFF)

set(CMAKE_FOLDER ${CMAKE_FOLDER}/lz4)

FetchContent_MakeAvailable(lz4)

add_subdirectory(${lz4_SOURCE_DIR}/build/cmake build)

set(LZ4_NAME lz4_static PARENT_SCOPE)
set(LZ4_INCLUDE_DIR ${lz4_SOURCE_DIR}/lib PARENT_SCOPE)