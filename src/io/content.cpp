//
//  content.cpp
//  libkge
//
//  Created by kevin on 17-9-26.
//
//

#include "content.h"

namespace kge
{
	Content::Content() :_buffer(new ByteBuffer())
	{

	}

	Content::Content(ByteBuffer* buffer):_buffer(buffer)
	{
	}

	Content::~Content()
	{
		if (_buffer)
			delete _buffer;
		_buffer = nullptr;
	}

	void Content::Resize(uint32 size)
	{
		if (_buffer)
			delete _buffer;

		_buffer = new ByteBuffer(size);
	}
}

