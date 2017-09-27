//
//  buffer_type.h
//  libkge
//
//  Created by kevin on 17-9-23.
//
//

#ifndef __KGE_GRAPHICS_BUFFER_TYPE_H__
#define __KGE_GRAPHICS_BUFFER_TYPE_H__

namespace kge
{
	enum class BufferType
	{
		Unknow = 0,
		Vertex,
		Index,
		Uniform,
		Max
	};
} // end of namespace kge

#endif // __KGE_GRAPHICS_BUFFER_TYPE_H__
