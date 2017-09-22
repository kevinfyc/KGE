#ifndef VECTOR2_HPP
#define VECTOR2_HPP

#include "_math.hpp"

namespace kge
{

    class Vector2
    {
    public:

        float x, y;

        Vector2();
        Vector2( float a, float b );

        // Use the default compiler implementation for these methods. It is faster.
        // Vector2( const Vector2& v );
        // Vector2& operator  = ( const Vector2& v );

        void setZero( );
        void set( float a, float b ) ;
        void scale( const Vector2 &v, float s );
        float length() const;
        float lengthSquared() const;
        void normalise();
        Vector2 unitVector() const;

        float dotProduct( const Vector2& v ) const;
        float crossProduct( const Vector2& v ) const;

        void projectOnto( const Vector2& v1, const Vector2& v2 );
        Vector2 projectOnto( const Vector2 & v ) const;

        operator float *()				{ return (float *)&x; }
        operator const float *() const	{ return (float *)&x; }

        INLINE void operator += ( const Vector2& v );
        INLINE void operator -= ( const Vector2& v );
        INLINE void operator *= ( float s );
        INLINE void operator /= ( float s );

        static const Vector2 & zero()			{ return s_zero; }

    private:
        friend std::ostream& operator <<( std::ostream&, const Vector2& );
        friend std::istream& operator >>( std::istream&, Vector2& );

        static Vector2	s_zero;
    };

    INLINE Vector2 operator +( const Vector2& v1, const Vector2& v2 );
    INLINE Vector2 operator -( const Vector2& v1, const Vector2& v2 );
    INLINE Vector2 operator *( const Vector2& v, float s );
    INLINE Vector2 operator *( float s, const Vector2& v );
    INLINE Vector2 operator *( const Vector2& v1, const Vector2& v2 );
    INLINE Vector2 operator /( const Vector2& v, float s );
    INLINE bool operator   ==( const Vector2& v1, const Vector2& v2 );
    INLINE bool operator   !=( const Vector2& v1, const Vector2& v2 );
    INLINE bool operator   < ( const Vector2& v1, const Vector2& v2 );

    inline
    bool operator   > ( const Vector2& v1, const Vector2& v2 ) { return v2 < v1; }
    inline
    bool operator   >=( const Vector2& v1, const Vector2& v2 ) { return !(v1 < v2); }
    inline
    bool operator   <=( const Vector2& v1, const Vector2& v2 ) { return !(v2 < v1); }

    bool almostEqual( const Vector2& v1, const Vector2& v2, const float epsilon = 0.0004f );

} // end namespace kge

#ifdef CODE_INLINE
#include "vector2.ipp"
#endif

#endif // VECTOR2_HPP
