# set target
get_filename_component(CURRENT_FOLDER_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
set(TARGET_NAME Pantheon${CURRENT_FOLDER_NAME})


###############################
#                             #
# Sources                     #
#                             #
###############################

# Add source files
file(GLOB_RECURSE SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/*.c
	${CMAKE_CURRENT_SOURCE_DIR}/*.cc # C with classe
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/*.cxx
	${CMAKE_CURRENT_SOURCE_DIR}/*.c++
)

# Add header files
set(TARGET_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
file(GLOB_RECURSE HEADER_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/*.inl
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TARGET_FILES})


###############################
#                             #
# Executable                  #
#                             #
###############################

add_library(${TARGET_NAME} ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(${TARGET_NAME} PRIVATE ${TARGET_INCLUDE_DIR}
	${GLAD_INCLUDE_DIR} ${GLFW_INCLUDE_DIR} ${LIBMATH_INCLUDE_DIR}
	${CORE_INCLUDE_DIR} ${RENDERING_INCLUDE_DIR}
)

target_link_libraries(${TARGET_NAME}
	PRIVATE
	${GLFW_NAME}
	${LIBMATH_NAME}
	${CORE_NAME}
	${RENDERING_NAME}
)

if(MSVC)
	target_compile_options(${TARGET_NAME} PRIVATE /W4 /WX)
else()
	target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

set(APP_NAME ${TARGET_NAME} PARENT_SCOPE)
set(APP_INCLUDE_DIR ${TARGET_INCLUDE_DIR} PARENT_SCOPE)