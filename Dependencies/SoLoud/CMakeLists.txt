# Download SoLoud
FetchContent_Declare(
	soloud
	GIT_REPOSITORY https://github.com/jarikomppa/soloud
	GIT_TAG e82fd32c1f62183922f08c14c814a02b58db1873
)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

set(BUILD_SHARED_LIBS OFF)

set(BUILD_NULL ON)

if (APPLE)
	option(SOLOUD_BACKEND_COREAUDIO "Set to ON for building CoreAudio backend" ON)
	set(BUILD_NULL OFF)
else()
	set(SOLOUD_BACKEND_COREAUDIO OFF)
endif()

if (WIN32)
	option(SOLOUD_BACKEND_WINMM "Set to ON for building WINMM backend" ON)
	set(BUILD_NULL OFF)
else()
	set(SOLOUD_BACKEND_WINMM OFF)
endif()

option(SOLOUD_BACKEND_NULL "Set to ON for building NULL backend" ${BUILD_NULL})
option(SOLOUD_BACKEND_SDL2 "Set to ON for building SDL2 backend (SDL2 is NOT included by default, you must install it yourself)" OFF)

FetchContent_MakeAvailable(soloud)

add_subdirectory(${soloud_SOURCE_DIR}/contrib build)

set(SOLOUD_NAME soloud PARENT_SCOPE)
set(SOLOUD_INCLUDE_DIR ${soloud_SOURCE_DIR}/include PARENT_SCOPE)