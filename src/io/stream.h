//
//  stream.h
//  libkge
//
//  Created by kevin on 17-9-23.
//
//

#ifndef __KGE_IO_STREAM_H__
#define __KGE_IO_STREAM_H__

#include "core/pre_define.h"

namespace kge
{
	class Stream
	{
	public:
		Stream();
		virtual ~Stream() {}
		virtual void Close();
		virtual uint32 Read(void* buffer, uint32 size);
		virtual uint32 Write(void* buffer, uint32 size);

	protected:
		uint32 _position;
		bool _close;
		uint32 _length;
	};
}

#endif // __KGE_IO_STREAM_H__