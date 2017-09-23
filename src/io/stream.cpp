//
//  stream.cpp
//  libkge
//
//  Created by kevin on 17-9-23.
//
//

#include "stream.h"

namespace kge
{

	Stream::Stream() :_position(0)
		, _close(0)
		, _length(0)
	{

	}

	void Stream::Close()
	{
		_close = true;
	}

	uint32 Stream::Read(void* buffer, uint32 size)
	{
		uint32 read = (_position + size <= _length) ? size : _length - _position;

		_position += read;

		return read;
	}

	uint32 Stream::Write(void* buffer, uint32 size)
	{
		uint32 write = (_position + size <= _length) ? size : _length - _position;

		_position += write;

		return write;
	}
}

