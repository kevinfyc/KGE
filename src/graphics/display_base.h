﻿//
//  display_base.h
//  libkge
//
//  Created by kevin on 17-9-21.
//
//

#ifndef __KGE_DISPLAY_BASE_H__
#define __KGE_DISPLAY_BASE_H__

#include "core/pre_define.h"

namespace kge
{
    class DisplayBase
    {
	public:
		virtual bool Init(uint32 width, uint32 height);
		virtual void Fini();

	protected:
		uint32 _width;
		uint32 _height;
    };
}

#endif __KGE_DISPLAY_BASE_H__
