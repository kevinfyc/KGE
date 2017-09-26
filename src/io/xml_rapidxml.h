//
//  xml_rapidxml.h
//  orange
//
//  Created by Kevin on 17-9-25.
//

#ifndef __KGE_IO_XML_RAPIDJXML_H__
#define __KGE_IO_XML_RAPIDJXML_H__

#include <memory>

#include "section.h"

#define RAPIDXML_NO_EXCEPTIONS

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"

namespace kge
{

    class RapidXmlImpl : public ISection
    {
    public:

        typedef rapidxml::xml_document<char>    XmlDocumentType;
        typedef rapidxml::xml_node<char>        XmlNodeType;
        typedef rapidxml::xml_attribute<char>   XmlAttributeType;

        typedef std::shared_ptr<XmlDocumentType>    XmlDocumentTypePtr;
        typedef std::shared_ptr<std::string>        ContentPtr;

    public:
        explicit RapidXmlImpl(const std::string & tag, const std::string & value = "");
        RapidXmlImpl(XmlNodeType* node, ContentPtr content, XmlDocumentTypePtr doc);
        ~RapidXmlImpl();
        
        virtual SectionType GetType() const{ return SectionType::Xml; }

        virtual bool Valid() const { return doc_ && pNode_; }
        virtual const char* Ctag() const;
        virtual const char * Cvalue() const;
        
        virtual std::string GetTag() const { return Ctag(); }
        virtual void SetTag(const std::string &tag);

    public:
        virtual void SetString( const std::string & value );
        virtual void SetInt(int value);
        virtual void SetUint(uint32 value);
        virtual void SetFloat(float value);
        virtual void SetDouble(double value);
        virtual void SetBool(bool value);
        
        virtual std::string GetString() const;
        virtual int         GetInt() const;
        virtual uint32      GetUint() const;
        virtual float       GetFloat() const;
        virtual double      GetDouble() const;
        virtual bool        GetBool() const;
        
        virtual Ref<ISection> Read(const std::string &tag) const;
        virtual Ref<ISection> Write(const std::string &tag);
        virtual Ref<ISection> GetFirstChild() const;
        virtual Ref<ISection> NewChild(const std::string & tag);
        virtual void GetChildren(SectionPtrVector & children);
        virtual void ClearChildren();
        virtual void Print(std::ostream & stream);

    private:
		Ref<ISection> MakeNode(XmlNodeType* pNode) const;

        XmlNodeType * FindFirstNodeRecursive(const char * pTag) const;
        XmlNodeType * AddFirstNodeRecursive(const char * pTag);

    private:
        ContentPtr          content_;
        XmlDocumentTypePtr  doc_;
        XmlNodeType*        pNode_;

        friend class SectionFactory;
    };
    
}// end namespace kge

#endif//  __KGE_IO_XML_RAPIDJXML_H__
