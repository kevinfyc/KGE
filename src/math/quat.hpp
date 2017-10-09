#ifndef QUAT_HPP
#define QUAT_HPP

#include "matrix.hpp"

namespace kge
{
    /**
     *	This class is used to represent a quaternion. Quaternions are useful when
     *	working with angles in 3 dimensions.
     *
     *	@ingroup Math
     */
    class Quaternion
    {
    public:

        float x, y, z, w;

        /// @name Construction/Destruction
        //@{
        Quaternion();
        Quaternion( const Matrix &m );
        Quaternion( float x, float y, float z, float w );
        Quaternion( const Vector3 &v, float w );
        //@}

        void			setZero();
        void            setIdentity();
        void			set( float x, float y, float z, float w );
        void			set( const Vector3 &v, float w );

        void			fromAngleAxis( float angle, const Vector3 &axis );

        void			fromMatrix( const Matrix &m );

        void			normalise();
        void			invert();
		static Quaternion inverse(const Quaternion& q) { return Quaternion(-q.x, -q.y, -q.z, q.w); };
        void			minimise();

        void			slerp( const Quaternion& qStart, const Quaternion &qEnd,
                               float t );

        void			multiply( const Quaternion& q1, const Quaternion& q2 );
        void			preMultiply( const Quaternion& q );
        void			postMultiply( const Quaternion& q );

        float			dotProduct( const Quaternion& q ) const;
        float			length() const;
        float			lengthSquared() const;

        operator float *()				{ return (float *)&x; }
        operator const float *() const	{ return (float *)&x; }
		Vector3 operator *(const Vector3& p) const;
		bool operator !=(const Quaternion& v) const;
		bool operator ==(const Quaternion& v) const;

        static const Quaternion& identity() { return s_identity; }
        static const Quaternion& zero(){ return s_zero; }

        bool isIdentity() const;
        bool isZero() const;


    private:
        static const Quaternion s_identity;
        static const Quaternion s_zero;
    };

    Quaternion  operator *( const Quaternion& q1, const Quaternion& q2 );
    //bool		operator ==( const Quaternion& q1, const Quaternion& q2 );

} // end namespace kge

#ifdef CODE_INLINE
#include "quat.ipp"
#endif


#endif // QUAT_HPP
/*quat.hpp*/
