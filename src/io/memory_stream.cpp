//
//  memory_stream.cpp
//  libkge
//
//  Created by kevin on 17-9-23.
//
//

#include "memory_stream.h"

namespace kge
{
	MemoryStream::MemoryStream(const ByteBuffer& buffer) :_buffer(buffer)
	{
		_length = _buffer.Size();
	}

	uint32 MemoryStream::Read(void* buffer, uint32 size)
	{
		uint32 pos = _position;
		uint32 read = Stream::Read(buffer, size);

		if (read > 0 && buffer != nullptr)
			::memcpy(buffer, &_buffer[pos], read);

		return read;
	}

	uint32 MemoryStream::Write(void* buffer, uint32 size)
	{
		uint32 pos = _position;
		uint32 write = Stream::Write(buffer, size);

		if (write > 0 && buffer != nullptr)
			::memcpy(&_buffer[pos], buffer, pos);

		return write;
	}

	std::string MemoryStream::ReadString(uint32 size)
	{
		ByteBuffer buffer(size);
		Read(buffer.Bytes(), size);

		return std::string((const char*)buffer.Bytes(), buffer.Size());
	}


}

