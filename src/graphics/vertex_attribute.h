//
//  vertex_attribute.h
//  libkge
//
//  Created by kevin on 17-9-26.
//
//

#ifndef __KGE_GRAPHICS_VERTEX_ATTRIBUTE_H__
#define __KGE_GRAPHICS_VERTEX_ATTRIBUTE_H__

#include "color.h"
#include "math/vector2.hpp"
#include "math/vector3.hpp"
#include "math/vector4.hpp"

namespace kge
{
	enum class VertexAttributeType
	{
		None = -1,

		Vertex = 0,
		Color,
		Texcoord,
		Texcoord2,
		Normal,
		Tangent,
		BlendWeight,
		BlendIndices,

		Count
	};

	struct Vertex
	{
		Vector3 vertex;
		Color color;
		Vector2 uv;
		Vector2 uv2;
		Vector3 normal;
		Vector4 tangent;
		Vector4 bone_weight;
		Vector4 bone_indices;
	};

	extern const char* VERTEX_ATTR_TYPES[(int) VertexAttributeType::Count];
	extern const int VERTEX_ATTR_SIZES[(int) VertexAttributeType::Count];
	extern const int VERTEX_ATTR_OFFSETS[(int) VertexAttributeType::Count];
	extern const uint32 VERTEX_STRIDE;
} // end of namespace kge

#endif // __KGE_GRAPHICS_VERTEX_ATTRIBUTE_H__
