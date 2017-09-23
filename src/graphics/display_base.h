//
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

		uint32 GetWidth() const { return _width; }
		uint32 GetHeight() const { return _height; }

		virtual void BeginRecord(const std::string& file) { _recording = true; }
		virtual void EndRecord() { _recording = false; }
		bool IsRecording() const { return _recording; }

	protected:
		uint32 _width;
		uint32 _height;
		bool _recording;
    };
}

#endif // __KGE_DISPLAY_BASE_H__
