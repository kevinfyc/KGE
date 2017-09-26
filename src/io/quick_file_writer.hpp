//
//  quick_file_write.hpp
//  orange
//
//  Created by Kevin on 17-9-25.
//


#ifndef __KGE_IO_QUICK_FILE_WRITER_HPP__
#define __KGE_IO_QUICK_FILE_WRITER_HPP__

#include <string>

namespace kge
{

    class QuickFileWriter
    {
    public:
        QuickFileWriter() { }

        template <class T> QuickFileWriter & operator <<( const T & t )
        {
            int len = sizeof( T );
            data_.append( (char*)&t, len );
            return *this;
        }


        QuickFileWriter & operator << (const std::string & s)
        {
            (*this) << (size_t)s.length();
            if(!s.empty()) this->write(s.c_str(), s.length());
            return *this;
        }

        QuickFileWriter & operator << (const char *p)
        {
            size_t len = p ? strlen(p) : 0;
            *this << len;
            if(len > 0) this->write(p, len);
            return *this;
        }

        template <class E> QuickFileWriter & operator <<( std::vector<E> & v )
        {
            this->writeV( v );
            return *this;
        }

        template <class E> QuickFileWriter & operator <<( const std::vector<E> & v )
        {
            this->writeV( v );
            return *this;
        }

        QuickFileWriter& write( const void* buffer, unsigned int size )
        {
            data_.append( (char*) buffer, size );
            return *this;
        }

        const std::string & getData() const { return data_; }

    private:
        std::string data_;

        template <class E> void writeV( const std::vector<E> & v )
        {
            int elen = sizeof( E ); // Quenya: star :)
            int vlen = elen * v.size();
            data_.append( (char*) &v.front(), vlen );
        }
    };

}// end namespace kge

#endif // __KGE_IO_QUICK_FILE_WRITER_HPP__
