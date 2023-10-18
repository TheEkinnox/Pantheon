set(BASE_FOLDER ${CMAKE_FOLDER})

if (MSVC)
  add_compile_options(/Zc:preprocessor)
endif()

option(PANTHEON_BUILD_TEST "Enable to include engine test application" OFF)
option(PANTHEON_BUILD_RUNTIME "Enable to include engine base runtime application" ON)
option(PANTHEON_BUILD_EDITOR "Enable to include engine editor" ON)

set(PANTHEON_BUILD_RUNTIME ${PANTHEON_BUILD_RUNTIME} OR ${PANTHEON_BUILD_EDITOR})

set(CMAKE_FOLDER ${BASE_FOLDER}/Engine)
add_subdirectory(Engine)

set(CMAKE_FOLDER ${BASE_FOLDER})

if (PANTHEON_BUILD_RUNTIME)
	#add_subdirectory(Runtime)
endif()

if (PANTHEON_BUILD_EDITOR)
	#add_subdirectory(Editor)
	#set(STARTUP_PROJECT ${EDITOR_NAME})
endif()

if (PANTHEON_BUILD_TEST)
	add_subdirectory(Test)
	set(STARTUP_PROJECT ${TEST_NAME})
endif()

if (DEFINED STARTUP_PROJECT)
	set (STARTUP_PROJECT ${STARTUP_PROJECT} PARENT_SCOPE)
endif()