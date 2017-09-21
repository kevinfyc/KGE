

if (IOS)
	add_definitions(-DIOS=1)
elseif (ANDROID)
	add_definitions(-DANDROID=1)
endif ()

set (CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG")
set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG")

set (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG -D_DEBUG")
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG -D_DEBUG")


macro (collect_source_files)

    set (ARG_GLOB_CPP_PATTERNS *.c *.cpp *.mm)
    set (ARG_GLOB_H_PATTERNS *.h *.hpp *.ipp)

    file (GLOB CPP_FILES ${ARG_GLOB_CPP_PATTERNS})
    file (GLOB H_FILES ${ARG_GLOB_H_PATTERNS})
    
    set (SOURCE_FILES ${H_FILES} ${CPP_FILES})

endmacro()


macro (set_source_files_to_parent)
    get_filename_component (DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
    set (${DIR_NAME}_CPP_FILES ${CPP_FILES} PARENT_SCOPE)
    set (${DIR_NAME}_H_FILES ${H_FILES} PARENT_SCOPE)
endmacro()


macro (define_source_files)
    
    collect_source_files()
    set_source_files_to_parent()
    
endmacro ()


macro (setup_target)

    include_directories (${LIBS} ${INCLUDE_DIRS_ONLY})

    if(${TARGET_NAME} STREQUAL ${TARGET_NAME})
        target_link_libraries(${TARGET_NAME} ${ALL_LIBRARY_TARGETS} ${LINK_LIBS_ONLY})
    endif()
	
	if(${TARGET_NAME} MATCHES ${TARGET_NAME}|glew)
		add_definitions(-DGLEW_STATIC -DGLEW_NO_GLU)
	endif()

    if (XCODE)

        set_property_xcode(${TARGET_NAME} CLANG_CXX_LANGUAGE_STANDARD "c++0x")
        set_property_xcode(${TARGET_NAME} CLANG_CXX_LIBRARY "libc++")
        set_property_xcode(${TARGET_NAME} GCC_GENERATE_DEBUGGING_SYMBOLS "YES")

        file (MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/build)
        get_target_property (LOCATION ${TARGET_NAME} LOCATION)
        string (REGEX REPLACE "^.*\\$\\(CONFIGURATION\\)" $(CONFIGURATION) SYMLINK ${LOCATION})
        get_filename_component (DIRECTORY ${SYMLINK} PATH)
        add_custom_command (TARGET ${TARGET_NAME} POST_BUILD
            COMMAND mkdir -p ${DIRECTORY} && ln -s -f $<TARGET_FILE:${TARGET_NAME}> ${DIRECTORY}/$<TARGET_FILE_NAME:${TARGET_NAME}>
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/build)

    endif ()

endmacro ()


macro (setup_library)
    add_library (${TARGET_NAME} ${ARGN} ${SOURCE_FILES})
    setup_target ()

    set(ALL_LIBRARY_TARGETS ${ALL_LIBRARY_TARGETS} ${TARGET_NAME} PARENT_SCOPE)
    set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
endmacro ()


macro (setup_executable)
    
    add_executable (${TARGET_NAME} ${ARG_UNPARSED_ARGUMENTS} ${SOURCE_FILES})
	set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
    
    setup_target ()

    if (IOS)
        set_target_properties (${TARGET_NAME} PROPERTIES XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2")
    elseif (CMAKE_CROSSCOMPILING AND NOT ANDROID AND URHO3D_SCP_TO_TARGET)
        add_custom_command (TARGET ${TARGET_NAME} POST_BUILD COMMAND scp $<TARGET_FILE:${TARGET_NAME}> ${URHO3D_SCP_TO_TARGET} || exit 0)
    endif ()

    if (DEST_RUNTIME_DIR)
        # Need to check if the variable is defined first because this macro could be called by CMake project outside of Urho3D that does not wish to install anything
        install (TARGETS ${TARGET_NAME} RUNTIME DESTINATION ${DEST_RUNTIME_DIR} BUNDLE DESTINATION ${DEST_RUNTIME_DIR})
    endif ()

endmacro ()

# Macro for setting up linker flags for Mac OS X desktop build
macro (setup_macosx_linker_flags LINKER_FLAGS)
    set (${LINKER_FLAGS} "${${LINKER_FLAGS}} -framework AudioUnit -framework Carbon -framework Cocoa -framework CoreAudio -framework ForceFeedback -framework IOKit -framework OpenGL -framework CoreServices")
endmacro ()

# Macro for setting up linker flags for IOS build
macro (setup_ios_linker_flags LINKER_FLAGS)
    set (${LINKER_FLAGS} "${${LINKER_FLAGS}} -framework AudioToolbox -framework CoreAudio -framework CoreGraphics -framework Foundation -framework OpenGLES -framework QuartzCore -framework UIKit")
endmacro ()

macro (set_property_xcode TARGET XCODE_PROPERTY XCODE_VALUE)
    set_property (TARGET ${TARGET} PROPERTY XCODE_ATTRIBUTE_${XCODE_PROPERTY} ${XCODE_VALUE})
endmacro (set_property_xcode)

