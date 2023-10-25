# Download brotli
FetchContent_Declare(
	brotli
	GIT_REPOSITORY https://github.com/google/brotli
	GIT_TAG v1.0.9
	GIT_SHALLOW ON
)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

set(BUILD_SHARED_LIBS OFF)
set(BUILD_TESTING OFF)
set(BROTLI_DISABLE_TESTS ON)
set(BROTLI_EMSCRIPTEN ON) # Hack to disable shared libs
set(BROTLI_BUNDLED_MODE ON)

set(CMAKE_FOLDER ${CMAKE_FOLDER}/brotli)

FetchContent_MakeAvailable(brotli)

set(BROTLI_LIBRARIES brotlicommon-static brotlienc-static brotlidec-static PARENT_SCOPE)
set(BROTLI_INCLUDE_DIRS ${BROTLI_INCLUDE_DIRS} PARENT_SCOPE)