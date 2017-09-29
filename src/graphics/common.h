//
//  common.h
//  libkge
//
//  Created by kevin on 17-9-21.
//
//

#ifndef __KGE_GRAPHICS_COMMON_H__
#define __KGE_GRAPHICS_COMMON_H__

#if WIN32

#include <Windows.h>
#include "glew/glew.h"
#include "glew/wglew.h"

#endif

#include "util/log.h"

#include <assert.h>

/**
* GL assertion that can be used for any OpenGL function call.
*
* This macro will assert if an error is detected when executing
* the specified GL code. This macro will do nothing in release
* mode and is therefore safe to use for realtime/per-frame GL
* function calls.
*/
#ifdef _RETAIL
#define GL_ASSERT( gl_code ) gl_code
#else
#define GL_ASSERT( gl_code ) \
do{ \
	glGetError();\
    gl_code; \
    __gl_error_code = glGetError(); \
    if (__gl_error_code != GL_NO_ERROR) \
    { \
        KGE_LOG_ERROR("OpenGL Error: " #gl_code ": 0x%x", (int) __gl_error_code); \
        assert(0);  \
    } \
} while (0)
#endif

/**
* Executes the specified GL code and checks the GL error afterwards
* to ensure it succeeded.
*
* This macro should be used instead of GL_ASSERT for code that must
* be checked in both debug and release builds. The GL_LAST_ERROR
* macro can be used afterwards to check whether a GL error was
* encountered executing the specified code.
*/
#define GL_CHECK( gl_code ) \
do{ \
    while (glGetError() != GL_NO_ERROR); \
    gl_code; \
    __gl_error_code = glGetError(); \
    if (__gl_error_code != GL_NO_ERROR) \
    { \
        KGE_LOG_ERROR("OpenGL Error: " #gl_code ": %d", (int) __gl_error_code); \
    } \
} while (0)

// Global variable to hold GL errors
extern GLenum __gl_error_code;

/**
* Accesses the most recently set global GL error.
*/
#define GL_LAST_ERROR() __gl_error_code

#endif // __KGE_GRAPHICS_COMMON_H__