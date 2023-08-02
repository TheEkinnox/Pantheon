# set project
get_filename_component(CURRENT_FOLDER_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
set(PROJECT_NAME Pantheon${CURRENT_FOLDER_NAME})


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
set(PROJECT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
file(GLOB_RECURSE HEADER_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/*.inl
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${PROJECT_FILES})


###############################
#                             #
# Executable                  #
#                             #
###############################

add_library(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

include_directories(${PROJECT_INCLUDE_DIR}
    ${GLAD_INCLUDE_DIR} ${GLFW_INCLUDE_DIR}
    ${CORE_INCLUDE_DIR} ${RENDERING_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
	${GLAD_NAME}
	${GLFW_NAME}
	${CORE_NAME}
	${RENDERING_NAME}
)

if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
  target_link_options(${PROJECT_NAME} PRIVATE /FORCE:UNRESOLVED)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

set(APP_NAME ${PROJECT_NAME} PARENT_SCOPE)
set(APP_INCLUDE_DIR ${PROJECT_INCLUDE_DIR} PARENT_SCOPE)