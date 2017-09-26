//
//  quick_file_reader.cpp
//  orange
//
//  Created by Kevin on 17-9-25.
//

#include "quick_file_reader.h"
#include "core/pre_define.h"
#include <assert.h>

namespace kge
{

    QuickFileReader::QuickFileReader(const uint8 *data, size_t len, size_t pos)
        : data_(data)
        , len_(len)
        , pos_(pos)
        , good_(true)
    {
        CheckSize(0);
    }

    QuickFileReader & QuickFileReader::operator >> ( std::string & t )
    {
        uint32 len = 0;
        (*this) >> len;

        if(len > 0)
        {
            if(CheckSize(len))
            {
                t.resize(len);
                Read(&t[0], len);
            }
            else pos_ += len;
        }

        return *this;
    }

    char QuickFileReader::Get()
    {
        char ch = 0;
        if(CheckSize(1))
        {
            ch = data_[pos_];
        }

        pos_ += 1;
        return ch;
    }

    bool QuickFileReader::Read( void* buffer, size_t size )
    {
        if(CheckSize(size))
        {
            _read_no_check(buffer, size);
        }
        else pos_ += size;

        return good_;
    }

    void QuickFileReader::Offset(int size)
    {
        pos_ += size;
    }

    void QuickFileReader::Seek(int pos)
    {
        pos_ = pos;
    }

    bool QuickFileReader::CheckSize(size_t n) const
    {
        if(pos_ + n > len_)
        {
            good_ = false;
        }

        return good_;
    }

    void QuickFileReader::_read_no_check(void *pBuffer, size_t size)
    {
        assert(pBuffer && pos_ + size <= len_);

        memcpy(pBuffer, data_ + pos_, size);
        pos_ += size;
    }

}// end namespace kge