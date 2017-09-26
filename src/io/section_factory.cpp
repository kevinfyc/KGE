//
//  section_factory.cpp
//  libkge
//
//  Created by kevin on 17-9-25.
//
//

#include "section_factory.h"

#include "util/log.h"
#include "xml_rapidxml.h"
#include "file_tool.h"
#include "json_rapidjson.h"
#include "data_stream.h"

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
		RapidXmlImpl::XmlDocumentTypePtr doc(new RapidXmlImpl::XmlDocumentType());

		RapidXmlImpl::ContentPtr c(new std::string(content));

		const int flag = rapidxml::parse_trim_whitespace;
		doc->parse<flag>(const_cast<char *>(c->c_str()));

		Ref<ISection> root = Ref<ISection>(new RapidXmlImpl(&*doc, c, doc));

		return root;
	}

	/*static*/ Ref<ISection> SectionFactory::ParseJson(const std::string &content)
	{
		RapidJsonImpl::JsonDocumentTypePtr document(new RapidJsonImpl::JsonDocumentType);

		RapidJsonImpl::JsonNodeTypePtr node(new RapidJsonImpl::JsonNodeType);
		if (document->Parse<0>(content.c_str()).HasParseError())
		{
			KGE_LOG_ERROR("Json parse error %s.", content.c_str());
			return nullptr;
		}

		node->value = dynamic_cast<rapidjson::Value &> (*document);
		Ref<ISection> root = Ref<ISection>(new RapidJsonImpl(&*node, node, document));

		return root;
	}

	/*static*/ Ref<ISection> SectionFactory::ParseDS(const std::string & content)
	{
		Ref<DataStream> root = Ref<DataStream>(new DataStream("root"));
		root->Scan(content);
		return root;
	}

	/*static*/ Ref<ISection> SectionFactory::LoadXml(const std::string & filename)
	{
		Ref<ISection> root = Instance()->FindWithDefault(filename);
		if (root) return root;

		RapidXmlImpl::ContentPtr content(new std::string());
		if (!ReadFile(*content, filename, true))
			return nullptr;

		RapidXmlImpl::XmlDocumentTypePtr doc(new RapidXmlImpl::XmlDocumentType());

		const int flag = rapidxml::parse_trim_whitespace;
		doc->parse<flag>(const_cast<char *>(content->c_str()));

		root = Ref<ISection>(dynamic_cast<ISection*>(new RapidXmlImpl(&*doc, content, doc)));

		Instance()->Set(filename, root);
		return root;
	}

	/*static*/ bool SectionFactory::SaveXml(const std::string & filename, Ref<ISection> root)
	{
		Instance()->Set(filename, root);

		RapidXmlImpl *pRoot = dynamic_cast<RapidXmlImpl*>(root.get());

		std::ostringstream ss;
		pRoot->Print(ss);

		return WriteFile(ss.str(), filename, true);
	}


	/*static*/ Ref<ISection> SectionFactory::LoadJson(const std::string &filename)
	{
		Ref<ISection> root = Instance()->FindWithDefault(filename);
		if (root) return root;

		std::string content;
		if (!ReadFile(content, filename, true))
			return nullptr;

		RapidJsonImpl::JsonDocumentTypePtr document(new RapidJsonImpl::JsonDocumentType);

		RapidJsonImpl::JsonNodeTypePtr node(new RapidJsonImpl::JsonNodeType);
		if (document->Parse<0>(content.c_str()).HasParseError())
		{
			KGE_LOG_ERROR("Json parse error %s.", filename.c_str());
			return nullptr;
		}

		node->value = dynamic_cast<rapidjson::Value &> (*document);
		root = Ref<ISection>(new RapidJsonImpl(&*node, node, document));

		Instance()->Set(filename, root);
		return root;
	}

	/*static*/ bool SectionFactory::SaveJson(const std::string &filename, Ref<ISection> root)
	{
		Instance()->Set(filename, root);

		RapidJsonImpl *pRoot = dynamic_cast<RapidJsonImpl*>(root.get());

		std::ostringstream ss;
		pRoot->Print(ss);

		return WriteFile(ss.str(), filename, true);
	}


	/*static*/ Ref<ISection> SectionFactory::LoadDS(const std::string & filename)
	{
		Ref<ISection> root = Instance()->FindWithDefault(filename);
		if (root) return root;

		Ref<DataStream> realRoot = Ref<DataStream>(new DataStream("root"));
		if (!realRoot->Load(filename))
			return nullptr;

		Instance()->Set(filename, realRoot);
		return realRoot;
	}

	/*static*/ bool SectionFactory::SaveDS(const std::string & filename, Ref<ISection> root)
	{
		Instance()->Set(filename, root);

		DataStream *pRoot = dynamic_cast<DataStream*>(root.get());
		return pRoot->Save(filename);
	}
}

