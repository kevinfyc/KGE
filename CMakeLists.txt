
cmake_minimum_required(VERSION 2.8)

project(kge)

##################################################
#prepare
##################################################
set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)
include(common)

message(STATUS "cmake kge src dir: " ${PROJECT_SOURCE_DIR})
message(STATUS "cmake kge bin dir: " ${PROJECT_BINARY_DIR})

##################################################
#third party
##################################################
set(THIRD_PARTY_LIBS FreeType)

foreach (TARGET ${THIRD_PARTY_LIBS})
    add_subdirectory (src/third_party/${TARGET})
endforeach ()

LINK_LIBRARIES(${PROJECT_SOURCE_DIR}/lib/glew/win32/glew32.lib)

##################################################
#engine source
##################################################

add_subdirectory(src)
add_subdirectory(app)
