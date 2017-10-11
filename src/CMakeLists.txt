
set(TARGET_NAME kge)

set(THIRD_PARTY_DIR ${PROJECT_SOURCE_DIR}/src/third_party)

set(FREETYPE_FT2BUILD_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src/third_party/FreeType/include)
include_directories(
	${FREETYPE_INCLUDE_DIRS} ${FREETYPE_FT2BUILD_INCLUDE_DIR}
)

set(INCLUDE_DIRS_ONLY 
	${PROJECT_SOURCE_DIR}/src
	${PROJECT_SOURCE_DIR}/src/zlib
	${THIRD_PARTY_DIR}
)

if (WIN32)
	list(APPEND INCLUDE_DIRS_ONLY ${THIRD_PARTY_DIR}/glew)
endif()

set(SOURCES core math graphics ui util io rapidjson rapidxml jpeg png zlib)

foreach(SOURCE ${SOURCES})
	add_subdirectory(${SOURCE})
	source_group ("${SOURCE}" FILES ${${SOURCE}_H_FILES} ${${SOURCE}_CPP_FILES})

    list (APPEND SOURCE_FILES ${${SOURCE}_H_FILES} ${${SOURCE}_CPP_FILES})
    list (APPEND ENGINE_INCLUDE_DIRS_ONLY ${SOURCE})
    
endforeach()

add_definitions(-DCODE_INLINE)
add_definitions(-DKGE_GLES)

setup_library()
