//
//  byte_buffer.cpp
//  libkge
//
//  Created by kevin on 17-9-21.
//
//

#include "byte_buffer.h"

namespace kge
{
	ByteBuffer::ByteBuffer(int size /* = 0 */):_size(size)
		,_bytes(nullptr)
		,_weak_ref(false)
	{
		if (_size > 0)
		{
			_ref_count = RefMake<bool>(true);
			_bytes = (uint8*)malloc(_size);
		}
		else
		{
			_size = 0;
		}
	}

	ByteBuffer::ByteBuffer(const ByteBuffer& buffer)
	{
		_size = buffer._size;
		_bytes = buffer._bytes;
		_ref_count = buffer._ref_count;
		_weak_ref = buffer._weak_ref;
	}

	ByteBuffer::ByteBuffer(uint8* bytes, int size) :_size(size)
		, _bytes(bytes)
		, _weak_ref(true)
	{
	
	}

	ByteBuffer::~ByteBuffer()
	{
		Free();
	}

	ByteBuffer& ByteBuffer::operator =(const ByteBuffer& buffer)
	{
		Free();

		_size = buffer._size;
		_bytes = buffer._bytes;
		_ref_count = buffer._ref_count;
		_weak_ref = buffer._weak_ref;;

		return *this;
	}

	void ByteBuffer::Free()
	{
		if (!_weak_ref)
		{
			if (_ref_count && _ref_count.use_count() == 1)
			{
				if (_bytes != nullptr)
					free(_bytes);
			}
		}
	}

	uint8* ByteBuffer::Bytes() const
	{
		return _bytes;
	}

	int ByteBuffer::Size() const
	{
		return _size;
	}

	uint8& ByteBuffer::operator [](int index)
	{
		return _bytes[index];
	}

	const uint8& ByteBuffer::operator [](int index) const
	{
		return _bytes[index];
	}

} // end of namespace kge