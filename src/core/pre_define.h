//
//  pre_define.h
//  libkge
//
//  Created by kevin on 17-9-21.
//
//

#ifndef __KGE_PRE_DEFINE_H__
#define __KGE_PRE_DEFINE_H__

#include <string>
#include <vector>
#include <list>
#include <map>

#include <memory>

#define Ref std::shared_ptr
#define RefMake std::make_shared

#define WeakRef std::weak_ptr

namespace kge
{
	typedef signed char     int8;
	typedef unsigned char   uint8;
	typedef short           int16;
	typedef unsigned short  uint16;
	typedef int             int32;
	typedef unsigned int    uint32;
}

#endif // __KGE_PRE_DEFINE_H__
