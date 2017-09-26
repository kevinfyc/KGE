//
//  section.cpp
//  libkge
//
//  Created by kevin on 17-9-25.
//
//

#include "section.h"
#include "section_factory.h"

#include <cstdarg>

namespace kge
{
	ISection::ISection()
	{
	}

	ISection::~ISection()
	{
		SectionFactory::Purge(this);
	}

	bool ISection::Reads(const std::string & tag, SectionPtrVector & vector)
	{
		vector.clear();

		std::string tailTag = tag;
		Ref<ISection> ptr;

		size_t pos = tag.find_last_of('/');
		if (pos != tag.npos)
		{
			ptr = Read(tag.substr(0, pos));
			if (!ptr)
				return false;

			tailTag = tag.substr(pos + 1);
		}
		else
		{
			ptr = Ref<ISection>(this);
		}

		SectionPtrVector children;
		ptr->GetChildren(children);
		GetChildren(children);

		for (Ref<ISection> child : children)
		{
			if (child->GetTag() == tag)
				vector.push_back(ptr);
		}

		return !vector.empty();
	}


	void ISection::GetFormat(const char * pFormat, ...)
	{
		va_list va;
		va_start(va, pFormat);
		vsscanf(GetString().c_str(), pFormat, va);
		va_end(va);
	}

	void ISection::SetFormat(const char * pFormat, ...)
	{
		const size_t maxSize = 1024;
		char buffer[maxSize];

		va_list va;
		va_start(va, pFormat);
		int n = vsnprintf(buffer, maxSize, pFormat, va);
		va_end(va);

		if (n >= 0) SetString(buffer);
	}

}

