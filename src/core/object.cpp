//
//  object.cpp
//  libkge
//
//  Created by kevin on 17-9-22.
//
//

#include "object.h"

namespace kge
{

	Object::Object() :_name("Object")
	{
		static int s_id = 0;
		_id = s_id++;
	}

	Object::~Object()
	{

	}

} // end namespace ora
