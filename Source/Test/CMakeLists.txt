# set target
get_filename_component(CURRENT_FOLDER_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
set(TARGET_NAME Pantheon${CURRENT_FOLDER_NAME})

option(PANTHEON_HEADLESS_TEST "Enable to run the tests in headless mode" OFF)

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

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${HEADER_FILES} ${SOURCE_FILES})


###############################
#                             #
# Executable                  #
#                             #
###############################

add_executable(${TARGET_NAME} ${HEADER_FILES} ${SOURCE_FILES})
add_compile_definitions(PTH_VERBOSE_LOG)

target_include_directories(${TARGET_NAME} PRIVATE ${TARGET_INCLUDE_DIR}
	${LIBMATH_INCLUDE_DIR} ${ENGINE_INCLUDE_DIRS}
)

target_link_libraries(${TARGET_NAME}
	PRIVATE
	${LIBMATH_NAME}
	${ENGINE_TARGETS}
)

if (PANTHEON_HEADLESS_TEST)
	target_compile_definitions(${TARGET_NAME} PRIVATE PTH_HEADLESS_TEST)
endif()

if(MSVC)
	target_compile_options(${TARGET_NAME} PRIVATE /W4 /WX)
else()
	target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()


###############################
#                             #
# CTest                       #
#                             #
###############################

add_test(
	NAME ${TARGET_NAME}
	COMMAND $<TARGET_FILE:${TARGET_NAME}>
)

copy_resources(${TARGET_NAME})

set(TEST_NAME ${TARGET_NAME} PARENT_SCOPE)