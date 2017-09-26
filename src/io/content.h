//
//  content.h
//  libkge
//
//  Created by kevin on 17-9-26.
//
//

#ifndef __KGE_IO_CONTENT_H__
#define __KGE_IO_CONTENT_H__

#include "util/byte_buffer.h"

namespace kge
{
	class Content
	{
	public:
		Content();
		Content(ByteBuffer* buffer);
		~Content();

		inline ByteBuffer* CData() const { return _buffer; }
		inline uint32 CSize() const { return _buffer->Size(); }

		inline void Resize(uint32 size);
		inline bool Empty() { return _buffer->Size() == 0; }

	private:
		ByteBuffer* _buffer;
	};
}

#endif // __KGE_IO_CONTENT_H__