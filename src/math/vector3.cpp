

#include "vector3.hpp"

#ifndef CODE_INLINE
    #include "vector3.ipp"
#endif

#include <sstream>

namespace kge
{
    /*static*/ const Vector3 Vector3::s_zero(0.0f, 0.0f, 0.0f);
    /*static*/ const Vector3 Vector3::s_one(1.0f, 1.0f, 1.0f);

	/**
	 *	This function returns a description of the vector
	 *
	 */
	std::string Vector3::desc() const
	{
		std::stringstream ss;

		ss << *this;

		return ss.str();
	}


	/**
	 *	This function implements the output streaming operator for Vector3.
	 *
	 *	@relates Vector3
	 */
	std::ostream& operator <<( std::ostream& o, const Vector3& t )
	{
        o << "(" << t.x << ", " << t.y << ", " << t.z << ")";
	    return o;
	}


	/**
	 *	This function implements the input streaming operator for Vector3.
	 *
	 *	@relates Vector3
	 */
	std::istream& operator >>( std::istream& i, Vector3& t )
	{
		char dummy;
	    i >> dummy >> t.x >> dummy >> t.y >> dummy >> t.z >>  dummy;

	    return i;
	}

} // end namespace kge

// vector3.cpp
