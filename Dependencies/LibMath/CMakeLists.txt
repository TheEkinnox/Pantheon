# Download LibMath
FetchContent_Declare(
	LibMath
	GIT_REPOSITORY https://github.com/TheEkinnox/LibMath.git
	GIT_TAG origin/main
	GIT_SHALLOW ON
)

set(BUILD_SHARED_LIBS OFF)
set(LIBMATH_BUILD_TESTS OFF)

FetchContent_MakeAvailable(LibMath)

set(LIBMATH_NAME LibMath PARENT_SCOPE)
set(LIBMATH_INCLUDE_DIR ${LibMath_SOURCE_DIR}/LibMath/Header PARENT_SCOPE)