//
//  section.h
//  libkge
//
//  Created by kevin on 17-9-25.
//
//

#ifndef __KGE_IO_SECTION_H__
#define __KGE_IO_SECTION_H__

#include "core/pre_define.h"

namespace kge
{

#define SECTION_RW_DECLARATION(METHOD_NAME, VALUE_TYPE)                         \
void Write##METHOD_NAME(const std::string & tag, const VALUE_TYPE & value)      \
{                                                                               \
    Write(tag)->Set##METHOD_NAME(value);                                        \
}                                                                               \
VALUE_TYPE Read##METHOD_NAME(const std::string & tag,                           \
    const VALUE_TYPE & defaultV=VALUE_TYPE()) const                             \
{                                                                               \
    Ref<ISection> child = Read(tag);                                            \
    if(!child) return defaultV;                                                 \
    return child->Get##METHOD_NAME();                                           \
}
	enum class SectionType
	{
		Xml,
		Json,
		DS,
	};

	const std::string StringTrue("true");
	const std::string StringFalse("false");

	typedef std::vector<Ref<class ISection>> SectionPtrVector;

	class ISection
	{
	public:
		ISection();
		virtual ~ISection();

		virtual bool Valid() const = 0;
		virtual SectionType GetType() const = 0;

		virtual const char* CTag() const = 0;
		virtual std::string GetTag() const = 0;
		virtual void SetTag(const std::string &tag) = 0;

	public:
		virtual void SetString(const std::string & value) = 0;
		virtual void SetInt(int value) = 0;
		virtual void SetUint(uint32 value) = 0;
		virtual void SetFloat(float value) = 0;
		virtual void SetDouble(double value) = 0;
		virtual void SetBool(bool value) = 0;

		virtual std::string GetString() const = 0;
		virtual int         GetInt() const = 0;
		virtual uint32      GetUint() const = 0;
		virtual float       GetFloat() const = 0;
		virtual double      GetDouble() const = 0;
		virtual bool        GetBool() const = 0;

		virtual Ref<ISection> Read(const std::string &tag) const = 0;
		virtual Ref<ISection> Write(const std::string &tag) = 0;

		virtual Ref<ISection> NewChild(const std::string & tag) = 0;
		virtual Ref<ISection> GetFirstChild() const = 0;

		virtual void GetChildren(SectionPtrVector& children) = 0;
		virtual void ClearChildren() = 0;
		virtual void Print(std::ostream & stream) = 0;

	public:

		void GetFormat(const char * pFormat, ...);
		void SetFormat(const char * pFormat, ...);

		bool Reads(const std::string & tag, SectionPtrVector & vector);

		SECTION_RW_DECLARATION(String, std::string)
			SECTION_RW_DECLARATION(Int, int)
			SECTION_RW_DECLARATION(Uint, uint32)
			SECTION_RW_DECLARATION(Float, float)
			SECTION_RW_DECLARATION(Double, double)
			SECTION_RW_DECLARATION(Bool, bool)
	};
}

#endif // __KGE_IO_SECTION_H__