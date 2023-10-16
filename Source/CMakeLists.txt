set(BASE_FOLDER ${CMAKE_FOLDER})

if (MSVC)
  add_compile_options(/Zc:preprocessor)
endif()

set(CMAKE_FOLDER ${BASE_FOLDER}/Engine)
add_subdirectory(Engine)

set(CMAKE_FOLDER ${BASE_FOLDER}/API)
add_subdirectory(API)

set(CMAKE_FOLDER ${BASE_FOLDER})
add_subdirectory(Editor)