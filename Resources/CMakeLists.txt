# set target
get_filename_component(CURRENT_FOLDER_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
set(TARGET_NAME ${PROJECT_NAME}${CURRENT_FOLDER_NAME})

file(GLOB_RECURSE ASSETS_FILES ${PANTHEON_ASSETS_DIR}/*)

add_library(${TARGET_NAME} INTERFACE ${ASSETS_FILES})

message(STATUS "Added ${TARGET_NAME} resource target with files: ${ASSETS_FILES}")

set(CMAKE_FOLDER .utils)

add_custom_command(
	OUTPUT .resources.stamp
	DEPENDS ${ASSETS_FILES}
	COMMAND ${CMAKE_COMMAND} -E touch .resources.stamp
)

add_custom_target(${TARGET_NAME}_Stamp ALL
	DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/.resources.stamp
)

set(RESOURCES_STAMP ${TARGET_NAME}_Stamp PARENT_SCOPE)