namespace ora
{

    /**
     *	This method sets the values of this quaternion to the input values.
     */
    INLINE
    void Quaternion::set( const Vector3 &v, float _w )
    {
        this->x	    = v.x;
        this->y     = v.y;
        this->z     = v.z;
        this->w		= _w;
    }

    /**
     *	The default constructor.
     */
    INLINE
    Quaternion::Quaternion()
    {
    }


    /**
     *	This constructor sets up this quaternion based on the input matrix.
     *
     *	@see fromMatrix
     */
    INLINE
    Quaternion::Quaternion( const Matrix &m )
    {
        this->fromMatrix( m );
    }

	INLINE
    Quaternion::Quaternion( float x_, float y_, float z_, float w_ ):
		x (x_),
		y (y_),
		z (z_),
		w (w_)
	{
	}

    /**
     *	This constructor sets up this quaternion based on the input values.
     */
    INLINE
    Quaternion::Quaternion( const Vector3 &v, float w )
    {
        this->set( v, w );
    }


    /**
     *	This method sets all values of this quaternion to 0.
     */
    INLINE
    void Quaternion::setZero()
    {
        x = y = z = w = 0.0f;
    }

	INLINE
	void Quaternion::setIdentity()
	{
		*this = s_identity;
	}


    /**
     *	This method sets the values of this quaternion to the input values.
     */
    INLINE
    void Quaternion::set( float _x, float _y, float _z, float _w )
    {
        this->x	    = _x;
        this->y     = _y;
        this->z     = _z;
        this->w		= _w;
    }

    /**
     *	This method pre-multiplies this quaternion by the input quaternion. This
     *	quaternion is set to the result.
     *
     *	@see multiply
     */
    INLINE
    void Quaternion::preMultiply( const Quaternion& q )
    {
        this->multiply( q, *this );
    }


    /**
     *	This method post-multiplies this quaternion by the input quaternion. This
     *	quaternion is set to the result.
     *
     *	@see multiply
     */
    INLINE
    void Quaternion::postMultiply( const Quaternion& q )
    {
        this->multiply( *this, q );
    }

    /**
     *	This method returns the dot product of this quaternion with the input
     *	quaternion.
     *
     *	@param q	The quaternion to perform the dot product with.
     *
     *	@return	The dot product of this quaternion.
     */
    INLINE
    float Quaternion::dotProduct( const Quaternion& q ) const
    {
#ifdef EXT_MATH
        return XPQuaternionDot( this, &q );
#else
        Vector4 v1( x, y, z, w );
        Vector4 v2( q.x, q.y, q.z, q.w );
        return v1.dotProduct( v2 );
#endif
    }

    /**
     * This function returns the length of this quaternion.
     */
    INLINE
    float Quaternion::length() const
    {
        Vector4 vec4(x, y, z, w);
        return vec4.length();
    }

    /**
     * This function returns the squared length of this quaternion.
     */
    INLINE
    float Quaternion::lengthSquared() const
    {
        Vector4 vec4(x, y, z, w);
        return vec4.lengthSquared();
    }

    /**
     *	This function implements a shorthand method of multiplying two quaternions.
     */
    INLINE
    Quaternion operator *( const Quaternion& q1, const Quaternion& q2 )
    {
        Quaternion q;
        q.multiply( q1, q2 );
        return q;
    }


    /**
     *	This method returns whether or not two quaternions are equal. Two
     *	quaternions are equal if all of their elements are equal.
     */
    INLINE
    bool operator ==( const Quaternion& q1, const Quaternion& q2 )
    {
        return ( q1.x == q2.x &&
                 q1.y == q2.y &&
                 q1.z == q2.z &&
                 q1.w == q2.w );
    }

    /**
     *	This method makes the w member of this quaternion non-negative.
     */
    INLINE
    void Quaternion::minimise()
    {
        if (w < 0)
        {
            x = -x;
            y = -y;
            z = -z;
            w = -w;
        }
    }

	INLINE
    bool Quaternion::isIdentity() const
    {
        return x == 0.0f && y == 0.0f && z == 0.0f && w == 1.0f;
    }

	INLINE
    bool Quaternion::isZero() const
    {
        return x == 0.0f && y == 0.0f && z == 0.0f && z == 0.0f;
    }

} // end namespace ora

/*quat.ipp*/
