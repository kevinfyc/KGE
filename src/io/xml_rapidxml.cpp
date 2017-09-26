//
//  xml_rapidxml.h
//  orange
//
//  Created by Kevin on 17-9-25.
//

#include "xml_rapidxml.h"
#include "util/log.h"
#include "rapidxml/rapidxml_print.hpp"

#include "section_factory.h"

namespace rapidxml
{
    void parse_error_handler(const char *what, void *where)
    {
        KGE_LOG_ERROR("rapidxml parse error: %s", what);
    }
}

namespace kge
{

    RapidXmlImpl::RapidXmlImpl(const std::string & tag, const std::string & value)
        : doc_(new XmlDocumentType())
        , pNode_(&*doc_)
    {
        SetTag(tag);

        if (!value.empty())
            SetString(value);
    }

    RapidXmlImpl::RapidXmlImpl(XmlNodeType* node, ContentPtr content, XmlDocumentTypePtr doc)
        : content_(content)
        , doc_(doc)
        , pNode_(node)
    {
    }

    RapidXmlImpl::~RapidXmlImpl()
    {
    }
    
    
    void RapidXmlImpl::SetString(const std::string &value)
    {
        pNode_->value(doc_->allocate_string(value.c_str(), value.length()));
    }
    
    void RapidXmlImpl::SetInt(int value)
    {
        std::ostringstream ss;
        ss << value;
        SetString(ss.str());
    }
    
    void RapidXmlImpl::SetUint(uint32 value)
    {
        std::ostringstream ss;
        ss << value;
        SetString(ss.str());
    }
    
    void RapidXmlImpl::SetFloat(float value)
    {
        std::ostringstream ss;
        ss << value;
        SetString(ss.str());
    }
    
    void RapidXmlImpl::SetDouble(double value)
    {
        std::ostringstream ss;
        ss << value;
        SetString(ss.str());
    }
    
    void RapidXmlImpl::SetBool(bool value)
    {
        SetString(value ? StringTrue : StringFalse);
    }
    
    std::string RapidXmlImpl::GetString() const
    {
        return pNode_->value();
    }
    
    int RapidXmlImpl::GetInt() const
    {
        return atoi(pNode_->value());
    }
    
    uint32 RapidXmlImpl::GetUint() const
    {
        return (uint32) atoi(pNode_->value());
    }
    
    float RapidXmlImpl::GetFloat() const
    {
        return (float) atof(pNode_->value());
    }
    
    double RapidXmlImpl::GetDouble() const
    {
        return atof(pNode_->value());
    }
    
    bool RapidXmlImpl::GetBool() const
    {
        if(StringTrue == pNode_->value())
            return true;
        else if(StringFalse == pNode_->value())
            return false;
        
        return atoi(pNode_->value()) != 0;
    }
    
	Ref<ISection> RapidXmlImpl::Read(const std::string &tag) const
    {
        XmlNodeType *p = FindFirstNodeRecursive(tag.c_str());
        return MakeNode(p);
    }

	Ref<ISection> RapidXmlImpl::Write(const std::string &tag)
    {
        XmlNodeType *p = AddFirstNodeRecursive(tag.c_str());
        return MakeNode(p);
    }

    const char* RapidXmlImpl::CTag() const
    {
        assert(pNode_ && "pNode_ is NULL!");
        return pNode_->name();
    }

    const char * RapidXmlImpl::Cvalue() const
    {
        assert(pNode_ && "pNode_ is NULL!");
        return pNode_->value();
    }

    void RapidXmlImpl::SetTag(const std::string &tag)
    {
        pNode_->name(doc_->allocate_string(tag.c_str(), tag.length()));
    }

    
	Ref<ISection> RapidXmlImpl::NewChild(const std::string & tag)
    {
        XmlNodeType *pChild = doc_->allocate_node(rapidxml::node_element);
        pChild->name(doc_->allocate_string(tag.c_str(), tag.size()));
        pNode_->append_node(pChild);
        
        return MakeNode(pChild);
    }

	Ref<ISection> RapidXmlImpl::MakeNode(XmlNodeType* pNode) const
    {
        if (!pNode) return nullptr;

        return Ref<ISection>(new RapidXmlImpl(pNode, content_, doc_));
    }	

    RapidXmlImpl::XmlNodeType * RapidXmlImpl::FindFirstNodeRecursive(const char * pTag) const
    {
        XmlNodeType *pNode = pNode_;
        while (pNode)
        {
            const char * p = strchr(pTag, '/');
            if (p == nullptr)
                return pNode->first_node(pTag);

            pNode = pNode->first_node(pTag, p - pTag);
            pTag = p + 1;
        }
        return nullptr;
    }

    RapidXmlImpl::XmlNodeType* RapidXmlImpl::AddFirstNodeRecursive(const char * pTag)
    {
        XmlNodeType *pNode = pNode_;
        bool isFini = false;
        while (pNode && !isFini)
        {
            size_t n = 0;
            const char * p = strchr(pTag, '/');
            if (p == nullptr)
            {
                isFini = true;
                n = strlen(pTag);
            }
            else n = p - pTag;

            XmlNodeType *pChild = pNode->first_node(pTag, n);
            if (!pChild)
            {
                pChild = doc_->allocate_node(rapidxml::node_element);
                pChild->name(doc_->allocate_string(pTag, n));
                pNode->append_node(pChild);
            }

            pNode = pChild;
            pTag += n + 1;
        }
        return pNode;
    }

    void RapidXmlImpl::Print(std::ostream & stream)
    {
        rapidxml::print(std::ostream_iterator<char>(stream), *pNode_);
    }

    void RapidXmlImpl::GetChildren(SectionPtrVector & children)
    {
        for (XmlNodeType * p = pNode_->first_node(); p; p = p->next_sibling())
        {
            children.push_back(MakeNode(p));
        }
    }
    
    void RapidXmlImpl::ClearChildren()
    {
        pNode_->remove_all_nodes();
    }

	Ref<ISection> RapidXmlImpl::GetFirstChild() const
    {
        return MakeNode(pNode_->first_node());
    }

}// end namespace kge
