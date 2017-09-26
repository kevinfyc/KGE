//
//  section_factory.h
//  libkge
//
//  Created by kevin on 17-9-25.
//
//

#ifndef __KGE_IO_SECTION_FACTORY_H__
#define __KGE_IO_SECTION_FACTORY_H__

#include "section.h"
#include "util/singleton.h"

namespace kge
{
	class SectionFactory :
		public Singleton<SectionFactory>
	{
	public:
		SectionFactory();
		~SectionFactory();

		static Ref<ISection> Load(const std::string & filename, SectionType type);
		static bool Save(const std::string & filename, Ref<ISection> root);

		static Ref<ISection> LoadXml(const std::string & filename);
		static Ref<ISection> LoadJson(const std::string & filename);
		static Ref<ISection> LoadDS(const std::string & filename);

		static Ref<ISection> ParseXml(const std::string & content);
		static Ref<ISection> ParseJson(const std::string & content);
		static Ref<ISection> ParseDS(const std::string & content);

		static bool SaveXml(const std::string & filename, Ref<ISection> root);
		static bool SaveJson(const std::string & filename, Ref<ISection> root);
		static bool SaveDS(const std::string & filename, Ref<ISection> root);

		static void Purge(ISection * p);

	private:

		Ref<ISection> FindWithDefault(const std::string & name, Ref<ISection> def = nullptr);
		void Set(const std::string & name, Ref<ISection> section);
		void PurgeByPtr(ISection * p);
		void PurgeByName(const std::string & name);

	private:
		typedef std::map<std::string, ISection*> CacheMap;
		CacheMap _cache;
	};
}

#endif // __KGE_IO_SECTION_FACTORY_H__