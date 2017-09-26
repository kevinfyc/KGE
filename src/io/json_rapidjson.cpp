//
//  json_rapidjson.cpp
//  orange
//
//  Created by Kevin on 17-9-25.
//
//

#include "json_rapidjson.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/filestream.h"
#include "util/log.h"

#include "section_factory.h"

namespace kge
{

    RapidJsonImpl::RapidJsonImpl(const std::string & tag)
        : _doc(new JsonDocumentType())
    {
        _node = new JsonNodeType();
        _doc->SetObject();
        _node->value.SetObject();
        SetTag(tag);
    }
    
    RapidJsonImpl::RapidJsonImpl(JsonNodeType* node, JsonNodeTypePtr root, JsonDocumentTypePtr doc)
        : _root(root)
        , _doc(doc)
        , _node(node)
    {
    }
    
    RapidJsonImpl::~RapidJsonImpl()
    {
    }
    
    Ref<ISection> RapidJsonImpl::NewChild(const std::string & tag)
    {
        JsonNodeType node;
        node.name.SetString(tag.c_str(), tag.size(), _doc->GetAllocator());
        _node->value.AddMember(node.name, node.value, _doc->GetAllocator());
        JsonNodeType *pNode = _node->value.MemberEnd() - 1;
        
        return MakeNode(pNode);
    }
    
	Ref<ISection> RapidJsonImpl::MakeNode(JsonNodeType *node) const
    {
        if ( !node )
            return NULL;
        
        return Ref<ISection>(new RapidJsonImpl(node, _root, _doc));
    }
    
	Ref<ISection> RapidJsonImpl::Read(const std::string & tag) const
    {
        return MakeNode(FindFirstNodeRecursive(tag.c_str()));
    }

	Ref<ISection> RapidJsonImpl::Write(const std::string & tag)
    {
        return MakeNode(AddFirstNodeRecursive(tag.c_str()));
    }
    
    void RapidJsonImpl::SetString(const std::string &value)
    {
        _node->value.SetString(value.c_str(), value.length(), _doc->GetAllocator());
    }
    
    void RapidJsonImpl::SetInt(int value)
    {
        _node->value.SetInt(value);
    }
    
    void RapidJsonImpl::SetUint(uint32 value)
    {
        _node->value.SetUint(value);
    }
    
    void RapidJsonImpl::SetFloat(float value)
    {
        _node->value.SetDouble(double(value));
    }
    
    void RapidJsonImpl::SetDouble(double value)
    {
        _node->value.SetDouble(value);
    }
    
    void RapidJsonImpl::SetBool(bool value)
    {
        _node->value.SetBool(value);
    }
    
    std::string RapidJsonImpl::GetString() const
    {
        return _node->value.GetString();
    }
    
    int RapidJsonImpl::GetInt() const
    {
        return _node->value.GetInt();
    }
    
    uint32 RapidJsonImpl::GetUint() const
    {
        return _node->value.GetUint();
    }
    
    float RapidJsonImpl::GetFloat() const
    {
        return (float) _node->value.GetDouble();
    }
    
    double RapidJsonImpl::GetDouble() const
    {
        return _node->value.GetDouble();
    }
    
    bool RapidJsonImpl::GetBool() const
    {
        return _node->value.GetBool();
    }
    
    
    const char * RapidJsonImpl::Ctag() const
    {
        assert(_node && "_node is NULL!");
        return _node->name.GetString();
    }
    
    void RapidJsonImpl::SetTag(const std::string &tag)
    {
        _node->name.SetString(tag.c_str(), tag.length(), _doc->GetAllocator());
    }
        
    void RapidJsonImpl::Print(std::ostream &stream)
    {
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        _node->value.Accept(writer);
        
        stream << buffer.GetString();
    }
    
    void RapidJsonImpl::GetChildren(SectionPtrVector &children)
    {
        for (JsonDocumentType::MemberIterator iter = _node->value.MemberBegin();
             iter != _node->value.MemberEnd(); ++iter) {
            children.push_back(MakeNode(iter));
        }
    }
    
    void RapidJsonImpl::ClearChildren()
    {
        _node->value.Clear();
    }
    
	Ref<ISection> RapidJsonImpl::GetFirstChild() const
    {
        return MakeNode(_node->value.MemberBegin());
    }
    
    RapidJsonImpl::JsonNodeType * RapidJsonImpl::FindFirstNodeRecursive(const char *tag) const
    {
        if ( _node->value.FindMember(tag) == 0 )
            return nullptr;
        
        return _node->value.FindMember(tag);
    }
    
    RapidJsonImpl::JsonNodeType * RapidJsonImpl::AddFirstNodeRecursive(const char *tag) const
    {
        if ( !_node->value.IsObject() )
            _node->value.SetObject();
        
        JsonNodeType * node = _node->value.FindMember(tag);
        if ( node == 0)
        {
            JsonNodeType node;
            node.name.SetString(tag, strlen(tag), _doc->GetAllocator());
            _node->value.AddMember(node.name, node.value, _doc->GetAllocator());
            return _node->value.MemberEnd()-1;
        }
        else
        {
            return node;
        }
    }
    
}; // end of namespace kge