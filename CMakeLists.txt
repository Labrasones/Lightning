cmake_minimum_required (VERSION 3.0)
project(Lightning)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(Lightning/3rdparty/glfw)

option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(Lightning/3rdparty/assimp)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++11")
    if(NOT WIN32)
        set(GLAD_LIBRARIES dl)
    endif()
endif()

set(LIGHTNING_SOURCE_DIR ${CMAKE_BINARY_DIR}/Lightning/sources)
set(LIGHTNING_HEADERS_DIR ${CMAKE_BINARY_DIR}/Lightning/headers)

include_directories(Lightning/headers/
                    Lightning/3rdparty/assimp/include/
                    Lightning/3rdparty/glad/include/
                    Lightning/3rdparty/glfw/include/
                    Lightning/3rdparty/glm/
                    Lightning/3rdparty/stb/)
					
file(GLOB_RECURSE TMP_SOURCE_FILES	Lightning/sources/*.cpp
									Lightning/sources/*.c)
foreach(f ${TMP_SOURCE_FILES})
	file(RELATIVE_PATH SRCGR ${LIGHTNING_SOURCE_DIR} ${f})
	set(SRCGR "src/${SRCGR}")
	
	string(REGEX REPLACE "(.*)(/[^/]*)$" "\\1" SRCGR ${SRCGR})
	
	string(REPLACE / \\ SRCGR ${SRCGR})
	source_group("${SRCGR}" FILES ${f})
endforeach()

file(GLOB_RECURSE TMP_HEADER_FILES	Lightning/headers/*.hpp
									Lightning/headers/*.h)
foreach(f ${TMP_HEADER_FILES})
	file(RELATIVE_PATH SRCGR ${LIGHTNING_HEADERS_DIR} ${f})
	set(SRCGR "header/${SRCGR}")
	
	string(REGEX REPLACE "(.*)(/[^/]*)$" "\\1" SRCGR ${SRCGR})
	
	string(REPLACE / \\ SRCGR ${SRCGR})
	source_group("${SRCGR}" FILES ${f})
endforeach()

file(GLOB THIRD_PARTY_SOURCE Lightning/third_party/glad/src/glad.c)

file(GLOB LIGHTNING_SHADERS Lightning/shaders/*.comp
							Lightning/shaders/*.frag
							Lightning/shaders/*.geom
							Lightning/shaders/*.vert)
file(GLOB LIGHTNING_CONFIG 	CMakeLists.txt
							Readme.md
							.gitattributes
							.gitignore
							.gitmodules)

source_group("shaders" FILES ${LIGHTNING_SHADERS})

add_definitions(-DGLFW_INCLUDE_NONE
				-DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")
add_executable(${PROJECT_NAME} ${TMP_SOURCE_FILES} ${TMP_HEADER_FILES}
								${LIGHTNING_SHADERS} ${LIGHTNING_CONFIG})
target_link_libraries(${PROJECT_NAME} assimp glfw ${GLFW_LIBRARIES} ${GLAD_LIBRARIES})
set_target_properties(${PROJECT_NAME} PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Build/${PROJECT_NAME})