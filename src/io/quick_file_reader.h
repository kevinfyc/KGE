//
//  quick_file_reader.h
//  orange
//
//  Created by Kevin on 17-9-25.
//

#ifndef H_QUICK_FILE_READER_H
#define H_QUICK_FILE_READER_H

#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include "core/pre_define.h"

namespace kge
{

    class QuickFileReader
    {
    public:
        QuickFileReader(const uint8 *data, size_t len, size_t pos = 0);

        bool Empty() const
        {
            return pos_ >= len_;
        }
        bool Good() const
        {
            return good_;
        }

        int Remain() const
        {
            return int(len_) - int(pos_);
        }

        template <class T>
        QuickFileReader & operator >> ( T & t )
        {
            Read(&t, sizeof(T));
            return *this;
        }

        QuickFileReader & operator >> ( std::string & t );

        char Get();
        bool Read( void* buffer, size_t size );
        void Offset(int size);
        void Seek(int pos);

        const uint8 * CData() const
        {
            return data_;
        }
        size_t Length() const
        {
            return len_;
        }
        size_t Tell() const
        {
            return pos_;
        }

        template<typename TSeq>
        bool ReadSequence(TSeq & v, size_t n)
        {
            if(CheckSize(n))
            {
                v.resize(n);
                _read_no_check(&v[0], n * sizeof(v[0]));
                return true;
            }
            else
            {
                v.resize(0);
                pos_ += n;
                return false;
            }
        }

        template<typename T>
        bool ReadVector(std::vector<T> & v, size_t n)
        {
            return ReadSequence(v, n);
        }

        template<typename SizeType>
        bool ReadString(std::string & v)
        {
            SizeType len;
            (*this) >> len;

            return ReadSequence(v, len);
        }

    private:
        const uint8 *data_;
        size_t len_;
        size_t pos_;
        mutable bool good_;

        bool CheckSize(size_t n) const;
        void _read_no_check(void *pBuffer, size_t size);
    };

}//end namespace kge

#endif // H_QUICK_FILE_READER_H