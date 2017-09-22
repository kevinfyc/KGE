
#include "vector4.hpp"

#ifndef CODE_INLINE
    #include "vector4.ipp"
#endif

#include <iomanip>

namespace kge
{
	Vector4 Vector4::s_zero( 0.f, 0.f, 0.f, 0.f );


	/**
	 *	This function implements the output streaming operator for Vector4.
	 *
	 *	@relates Vector4
	 */
	std::ostream& operator <<( std::ostream& o, const Vector4& t )
	{
		const int fieldWidth = 8;

		o.put('(');
		o.width( fieldWidth );
		o << t[0];
		o.put(',');
		o.width( fieldWidth );
		o << t[1];
		o.put(',');
		o.width( fieldWidth );
		o << t[2];
		o.put(',');
		o.width( fieldWidth );
		o << t[3];
		o.put(')');

	    return o;
	}


	/**
	 *	This function implements the input streaming operator for Vector2.
	 *
	 *	@relates Vector4
	 */
	std::istream& operator >>( std::istream& i, Vector4& t )
	{
		char dummy;
	    i >>	dummy >> t[0] >>
				dummy >> t[1] >>
				dummy >> t[2] >>
				dummy >> t[3] >> dummy;

	    return i;
	}

} // end namespace kge

// vector4.cpp
