//
//  json_rapidjson.h
//  orange
//
//  Created by Kevin on 17-9-25.
//
//

#ifndef __KGE_IO_JSON_RAPIDJSON_H__
#define __KGE_IO_JSON_RAPIDJSON_H__

#include "section.h"
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"

namespace kge
{

    class RapidJsonImpl : public ISection
	{
    public:
        typedef rapidjson::Document         JsonDocumentType;
        typedef JsonDocumentType::Member    JsonNodeType;
        
        typedef std::shared_ptr<JsonDocumentType>   JsonDocumentTypePtr;
        typedef std::shared_ptr<JsonNodeType>       JsonNodeTypePtr;
        
    public:
        explicit RapidJsonImpl(const std::string & tag);
        RapidJsonImpl(JsonNodeType * node, JsonNodeTypePtr root, JsonDocumentTypePtr doc);
        ~RapidJsonImpl();
        
        virtual SectionType GetType() const{ return SectionType::Json; }
        virtual bool Valid() const { return !!_doc; };
        
        virtual const char* Ctag() const;

        virtual std::string GetTag() const { return Ctag(); }
        virtual void SetTag(const std::string &tag);
    public:
        virtual void SetString(const std::string & value);
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
		Ref<ISection> MakeNode(JsonNodeType * node) const;
        JsonNodeType* FindFirstNodeRecursive(const char * tag) const;
        JsonNodeType* AddFirstNodeRecursive(const char * tag) const;
        
    public:
        JsonNodeTypePtr     _root;
        JsonDocumentTypePtr _doc;
        JsonNodeType*       _node;
    };
    
}; // end of namespace kge

#endif // __KGE_IO_JSON_RAPIDJSON_H__
