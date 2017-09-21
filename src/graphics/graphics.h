//
//  graphics.h
//  libkge
//
//  Created by kevin on 17-9-21.
//
//

#ifndef __KGE_GRAPHICS_H__
#define __KGE_GRAPHICS_H__

#include "core/pre_define.h"

namespace kge
{

	class Display;

    class Graphics
    {
    public:
		static bool Init(uint32 width, uint32 height);
		static void Tick();
		static void Fini();

		static void Draw();

		inline static Display* GetDisplay() { return s_display.get(); }

	private:
		static Ref<Display> s_display;
    };
}

#endif __KGE_GRAPHICS_H__
