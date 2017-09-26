//
//  section_factory.cpp
//  libkge
//
//  Created by kevin on 17-9-25.
//
//

#include "section_factory.h"

#include "util/log.h"

namespace kge
{
	IMPLEMENT_SINGLETON(SectionFactory);


	SectionFactory::SectionFactory()
	{

	}

	SectionFactory::~SectionFactory()
	{

	}

	Ref<ISection> SectionFactory::FindWithDefault(const std::string & name, Ref<ISection> def)
	{
		CacheMap::iterator it = _cache.find(name);
		if (it != _cache.end())
			return Ref<ISection>(it->second);

		_cache[name] = def.get();
		return def;
	}

	void SectionFactory::Set(const std::string & name, Ref<ISection> section)
	{
		_cache[name] = section.get();
	}


	/*static*/ Ref<ISection> SectionFactory::Load(const std::string & filename, SectionType type)
	{
		switch (type)
		{
		case SectionType::Xml:
			return LoadXml(filename);

		case SectionType::Json:
			return LoadJson(filename);

		case SectionType::DS:
			return LoadDS(filename);

		default:
			return nullptr;
		}
	}

	/*static*/ bool SectionFactory::Save(const std::string & filename, Ref<ISection> root)
	{
		assert(root);

		switch (root->GetType())
		{
		case SectionType::Xml:
			return SaveXml(filename, root);

		case SectionType::Json:
			return SaveJson(filename, root);

		case SectionType::DS:
			return SaveDS(filename, root);

		default:
			return false;
		};
	}


	/*static*/ void SectionFactory::Purge(ISection * p)
	{
		if (HasInstance())
			Instance()->PurgeByPtr(p);
	}

	void SectionFactory::PurgeByPtr(ISection * p)
	{
		CacheMap::iterator it;
		for (it = _cache.begin(); it != _cache.end(); ++it)
		{
			if (p == it->second)
			{
				_cache.erase(it);
				break;
			}
		}
	}

	void SectionFactory::PurgeByName(const std::string & name)
	{
		CacheMap::iterator it = _cache.find(name);
		if (it != _cache.end())
		{
			_cache.erase(it);
		}
	}

	/*static*/ Ref<ISection> SectionFactory::ParseXml(const std::string & content)
	{
		return nullptr;
	}

	/*static*/ Ref<ISection> SectionFactory::ParseJson(const std::string &content)
	{
		return nullptr;
	}

	/*static*/ Ref<ISection> SectionFactory::ParseDS(const std::string & content)
	{
		return nullptr;
	}

	/*static*/ Ref<ISection> SectionFactory::LoadXml(const std::string & filename)
	{
		return nullptr;
	}

	/*static*/ bool SectionFactory::SaveXml(const std::string & filename, Ref<ISection> root)
	{
		return nullptr;
	}


	/*static*/ Ref<ISection> SectionFactory::LoadJson(const std::string &filename)
	{
		return nullptr;
	}

	/*static*/ bool SectionFactory::SaveJson(const std::string &filename, Ref<ISection> root)
	{
		return nullptr;
	}


	/*static*/ Ref<ISection> SectionFactory::LoadDS(const std::string & filename)
	{
		return nullptr;
	}

	/*static*/ bool SectionFactory::SaveDS(const std::string & filename, Ref<ISection> root)
	{
		return nullptr;
	}
}

