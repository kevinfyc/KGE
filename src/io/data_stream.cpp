
#include "data_stream.h"
#include "file_tool.h"
#include "section_factory.h"

namespace kge
{


    bool ignoreAllSpace(QuickFileReader & reader)
    {
        char ch;
        do
        {
            ch = reader.Get();
            if(!reader.Good())  return false;
        }
        while(IsWhiteSpace(ch));

        reader.Offset(-1);
        return true;
    }

    bool ignoreComment(QuickFileReader & reader)
    {
        char ch;
        do
        {
            ch = reader.Get();
            if(!reader.Good()) return false;
        }
        while(ch != '\n');
        return true;
    }

    char translateChar(char ch)
    {
        switch(ch)
        {
        case 'n':
            return '\n';
        case '0':
            return '\0';
        default:
            return ch;
        };
    }

    std::string ValueStringFormat(const std::string & input)
    {
        static const std::string StrNeedFormat("={}#\n\\");

        char ch;
        std::string  output;
        size_t len = input.length();
        for(size_t i = 0; i < len; ++i)
        {
            ch = input[i];
            if(ch == '\0' || StrNeedFormat.find(ch) != StrNeedFormat.npos)
            {
                output += '\\';
                switch(ch)
                {
                case '\n':
                    ch = 'n';
                    break;
                case '\0':
                    ch = '0';
                    break;
                default:
                    break;
                };
            }

            output += ch;
        }

        return output;
    }

    std::string ValueStringUnformat(const std::string & input)
    {
        char ch;
        std::string output;
        for(size_t i = 0; i < input.size(); ++i)
        {
            ch = input[i];
            if(ch == '\\')
            {
                if(++i >= input.size()) break;

                ch = translateChar(input[i]);
            }

            output += ch;
        }
        return output;
    }

    //////////////////////////////////////////////////////////////////////////


    DataStream::DataStream(void)
    {
    }

    DataStream::~DataStream(void)
    {
    }

    DataStream::DataStream(const std::string & tag)
        : tag_(tag)
    {

    }

    DataStream::DataStream(const std::string & tag, const std::string & value)
        : tag_(tag)
        , value_(value)
    {

    }
    
    
    void DataStream::SetString(const std::string &value)
    {
        value_ = value;
    }
    
    void DataStream::SetInt(int value)
    {
        std::ostringstream ss;
        ss << value;
        value_ = ss.str();
    }
    
    void DataStream::SetUint(kge::uint32 value)
    {
        std::ostringstream ss;
        ss << value;
        value_ = ss.str();
    }
    
    void DataStream::SetFloat(float value)
    {
        std::ostringstream ss;
        ss << value;
        value_ = ss.str();
    }
    
    void DataStream::SetDouble(double value)
    {
        std::ostringstream ss;
        ss << value;
        value_ = ss.str();
    }
    
    void DataStream::SetBool(bool value)
    {
        value_ = value ? StringTrue : StringFalse;
    }
    
    std::string DataStream::GetString() const
    {
        return value_;
    }
    
    int DataStream::GetInt() const
    {
        return atoi(value_.c_str());
    }
    
    uint32 DataStream::GetUint() const
    {
        return (uint32) atoi(value_.c_str());
    }
    
    float DataStream::GetFloat() const
    {
        return (float) atof(value_.c_str());
    }
    
    double DataStream::GetDouble() const
    {
        return atof(value_.c_str());
    }
    
    bool DataStream::GetBool() const
    {
        if(StringTrue == value_)
            return true;
        else if(StringFalse == value_)
            return false;
        
        return atoi(value_.c_str()) != 0;
    }
    
    
    Ref<ISection> DataStream::GetFirstChild() const
    {
        if(children_.empty()) return nullptr;
        
        return children_[0];
    }
    
    void DataStream::GetChildren(SectionPtrVector & children)
    {
        children.resize(children_.size());
        std::copy_n(children_.begin(), children_.size(), children.begin());
    }

    void DataStream::AddChild(Ref<DataStream> child)
    {
        children_.push_back(child);
    }

    Ref<DataStream> DataStream::GetChild(const std::string & tag, bool createIfMiss)
    {
        for(Children::iterator it = children_.begin();
                it != children_.end(); ++it)
        {
            if((*it)->GetTag() == tag) return *it;
        }

        if(createIfMiss)
        {
            return Ref<DataStream>(dynamic_cast<DataStream*>(NewChild(tag).get()));
        }

        return nullptr;
    }

    Ref<ISection> DataStream::NewChild(const std::string & tag)
    {
		Ref<DataStream> p = Ref<DataStream>(new DataStream(tag));
        AddChild(p);
        return p;
    }

    void DataStream::ClearChildren()
    {
        children_.clear();
    }

    Ref<ISection> DataStream::Write(const std::string & tag)
    {
        size_t pos = tag.find('/');
        if(pos == tag.npos)
        {
            return GetChild(tag, true);
        }
        
        Ref<DataStream> child = GetChild(tag.substr(0, pos), true);
        if(!child) return NULL;
        
        return child->Write(tag.substr(pos + 1));
    }

    Ref<ISection> DataStream::Read(const std::string & tag) const
    {
        DataStream *pThis = const_cast<DataStream*>(this);
        
        size_t pos = tag.find('/');
        if(pos == tag.npos)
        {
            return pThis->GetChild(tag, false);
        }

        Ref<DataStream> child = pThis->GetChild(tag.substr(0, pos), false);
        if(!child) return NULL;

        return child->Read(tag.substr(pos + 1));
    }

    void DataStream::Print(std::ostream & stream, int depth)
    {
        stream << TabProxy(depth) << ValueStringFormat(tag_);
        if(!value_.empty())
        {
            stream  << " = " << ValueStringFormat(value_);
        }
        stream  << '\n';

        if(!children_.empty())
        {
            stream << TabProxy(depth) << "{\n";

            for(Children::iterator it = begin(); it != end(); ++it)
            {
                (*it)->Print(stream, depth + 1);
            }

            stream << TabProxy(depth) << "}\n";
        }
    }

    void DataStream::Print(std::ostream & stream)
    {
        for(Children::iterator it = begin(); it != end(); ++it)
        {
            (*it)->Print(stream, 0);
        }
    }

    void DataStream::Print(std::string & stream)
    {
        std::ostringstream out;

        Print(out);

        stream = out.str();
    }

    bool DataStream::Load(const std::string & filename)
    {
        ClearChildren();
        Content content;

        if (!ReadFile(content, filename, false)) return false;

        return Scan(content);
    }
    
    bool DataStream::Save(const std::string & filename)
    {
        std::ostringstream ss;
        Print(ss);
		Ref<ByteBuffer> buffer = Ref<ByteBuffer>(new ByteBuffer((uint8*)ss.str().c_str(), ss.str().size()));
		Ref<Content> content = Ref<Content>(new Content(buffer.get()));
        
        return WriteFile(content, filename, false);
    }

    bool DataStream::Scan(const Content & stream)
    {
		if (!stream.CBuffer())
			return false;

        QuickFileReader reader(stream.CBuffer()->Bytes(), stream.CSize());
        return Scan(reader);
    }

    bool DataStream::Scan(QuickFileReader & reader)
    {
        std::string ctag;
        std::string cvalue;
        char ch;
        Ref<DataStream> lastChild;

        while(reader.Good())
        {
            if(!ignoreAllSpace(reader)) break;

            ch = reader.Get();
            if(ch == '}') //一次解析完成
            {
                break;
            }
            else if(ch == '{')//新的节点
            {
                if(!lastChild)//无效的节点
                {
                    return false;
                }

                lastChild->Scan(reader);
                lastChild = nullptr;
            }
            else if(ch == '#')
            {
                ignoreComment(reader);
            }
            else
            {
                reader.Offset(-1);
                ctag.clear();
                cvalue.clear();

                //解析tag
                while(reader.Good())
                {
                    ch = reader.Get();
                    if(ch == '\\')
                    {
                        ch = translateChar(reader.Get());
                    }
                    else if(ch == '=')
                    {
                        break;
                    }
                    else if(ch == '{' || ch == '}' || ch == '#' || ch == '\n')
                    {
                        reader.Offset(-1);
                        break;
                    }

                    ctag += ch;
                }

                TrimString(ctag);
                if(ctag.empty()) return false;

                //解析value
                while(reader.Good())
                {
                    ch = reader.Get();
                    if(ch == '\\')
                    {
                        ch = translateChar(reader.Get());
                    }
                    else if(ch == '{' || ch == '}' || ch == '#' || ch == '\n')
                    {
                        reader.Offset(-1);
                        break;
                    }

                    cvalue += ch;
                }

                TrimString(cvalue);

                lastChild = Ref<DataStream>(dynamic_cast<DataStream*>(NewChild(ctag).get()));
                lastChild->SetString(cvalue);
            }
        }

        return true;
    }

}//end namespace kge
