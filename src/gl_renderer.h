#pragma once

#include "plungs_lib.h"
#include "glcorearb.h"

static PFNGLCREATEPROGRAMPROC glCreateProgram_ptr;

void gl_load_functions()
{
    PROC proc = wglGetProcAddress("glCreateProgram");
    if(!proc)
    {
        SM_ASSERT(false, "Failed to load gl function %s","glCreateProgram");
    }

    glCreateProgram_ptr = (PFNGLCREATEPROGRAMPROC)proc;
}

GLAPI GLuint APIENTRY glCreateProgram(void)
{
    return glCreateProgram_ptr();
}