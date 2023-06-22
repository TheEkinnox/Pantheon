# Download PugiXML
FetchContent_Declare(
	pugixml
	GIT_REPOSITORY https://github.com/zeux/pugixml
	GIT_TAG v1.13
	GIT_SHALLOW ON
)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

set(BUILD_SHARED_LIBS OFF)

FetchContent_MakeAvailable(pugixml)

set(PUGIXML_NAME pugixml-static PARENT_SCOPE)
set(PUGIXML_INCLUDE_DIR ${pugixml_SOURCE_DIR}/src PARENT_SCOPE)