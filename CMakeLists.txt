cmake_minimum_required(VERSION 3.0)
project(ecs)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# SFML
if (WIN32)
	set(SFML_STATIC_LIBRARIES TRUE)
	set(SFML_DIR "${SFML_DIR}/lib/cmake/SFML")
	set(CMAKE_SUPPRESS_REGENERATION true)
endif (WIN32)

get_filename_component(PROJECT_DIR "${CMAKE_CURRENT_SOURCE_DIR}" ABSOLUTE)
find_package(SFML 2.5.1 COMPONENTS system window graphics audio REQUIRED)
find_library (SFML REQUIRED sfml-window sfml-system sfml-graphics sfml-audio)
include_directories(${SFML_INCLUDE_DIR})


# # ImGui-SFML
# set(ImGui-SFML_DIR "${ImGui-SFML_DIR}/lib/cmake/ImGui-SFML")
# find_package(ImGui-SFML REQUIRED)

find_package(GLM REQUIRED)
include_directories(${GLM_INCLUDE_DIR})

file(GLOB_RECURSE src
	"src/*.hpp"
	"src/*.cpp"
)

add_executable(main src/main.cpp ${src})

target_compile_definitions(main PRIVATE SFML_STATIC)

target_link_libraries(main sfml-window sfml-system sfml-graphics sfml-audio)

