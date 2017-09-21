//
//  byte_buffer.h
//  libkge
//
//  Created by kevin on 17-9-21.
//
//

#ifndef __KGE_UTIL_BYTE_BUFFER_H__
#define __KGE_UTIL_BYTE_BUFFER_H__

#include "core/pre_define.h"

namespace kge
{
	class ByteBuffer
	{
	public:
		ByteBuffer(int size = 0);
		ByteBuffer(const ByteBuffer& buffer);
		ByteBuffer(uint8* bytes, int size);
		~ByteBuffer();

		uint8* Bytes() const;
		int Size() const;

		ByteBuffer& operator =(const ByteBuffer& buffer);
		uint8& operator[](int index);
		const uint8& operator[](int index)const;

	private:
		void Free();

		int _size;
		uint8* _bytes;
		Ref<bool> _ref_count;
		bool _weak_ref;
	};
} // end of namespace kge

#endif // __KGE_UTIL_BYTE_BUFFER_H__