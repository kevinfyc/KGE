//
//  vertex_bufer.h
//  libkge
//
//  Created by kevin on 17-9-23.
//
//

#ifndef __KGE_IO_MEMORY_STREAM_H__
#define __KGE_IO_MEMORY_STREAM_H__

#include "stream.h"
#include "util/byte_buffer.h"

namespace kge
{
	class MemoryStream : public Stream
	{
	public:
		MemoryStream(const ByteBuffer& buffer);

		virtual uint32 Read(void* buffer, uint32 size)override;
		virtual uint32 Write(void* buffer, uint32 size)override;

		template<class T>
		T Read();
		template<class T>
		void Write(const T& t);

		// 有IO
		std::string ReadString(uint32 size);

	private:
		ByteBuffer _buffer;
	};

	template<class T>
	T MemoryStream::Read()
	{
		T t;
		Read((void*)&t, sizeof(T));
		return t;
	}

	template<class T>
	void MemoryStream::Write(const T& t)
	{
		Write((void*)&t, sizeof(T));
	}
}

#endif // __KGE_IO_MEMORY_STREAM_H__