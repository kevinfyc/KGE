//
//  display.h
//  libkge
//
//  Created by kevin on 17-9-21.
//
//

#ifndef __KGE_DISPLAY_H__
#define __KGE_DISPLAY_H__

#if KGE_GLES
#include "display_GLES.h"
#endif

namespace kge
{
#if KGE_GLES
    class Display : public DisplayGLES
#endif
    {
    };
}

#endif // __KGE_DISPLAY_H__
