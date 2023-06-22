set(BASE_FOLDER ${CMAKE_FOLDER})

set(CMAKE_FOLDER ${BASE_FOLDER}/Engine)
add_subdirectory(Engine)

set(CMAKE_FOLDER ${BASE_FOLDER}/API)
add_subdirectory(API)

set(CMAKE_FOLDER ${BASE_FOLDER})
add_subdirectory(Editor)