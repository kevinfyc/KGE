//
//  data_stream.h
//  orange
//
//  Created by Kevin on 17-9-25.
//

#ifndef __KGE_IO_DATA_STREAM__
#define __KGE_IO_DATA_STREAM__


#include "quick_file_reader.h"
#include "section.h"

namespace kge
{
    std::string ValueStringFormat(const std::string & input);
    std::string ValueStringUnformat(const std::string & input);


    class DataStream : public ISection
    {
    public:
        typedef std::vector<Ref<DataStream>> Children;

        DataStream(void);
        explicit DataStream(const std::string & tag);
        DataStream(const std::string & tag, const std::string & value);
        ~DataStream(void);
        
        virtual SectionType GetType() const{ return SectionType::DS; }
        virtual bool Valid() const { return true; }
        
        virtual const char* CTag() const { return tag_.c_str(); }
        virtual std::string GetTag() const { return tag_; }
        virtual void SetTag(const std::string &tag){ tag_ = tag; }
        
        virtual void SetString(const std::string & value);
        virtual void SetInt(int value);
        virtual void SetUint(kge::uint32 value);
        virtual void SetFloat(float value);
        virtual void SetDouble(double value);
        virtual void SetBool(bool value);
        
        virtual std::string GetString() const;
        virtual int         GetInt() const;
        virtual uint32      GetUint() const;
        virtual float       GetFloat() const;
        virtual double      GetDouble() const;
        virtual bool        GetBool() const;
        
		virtual std::string GetFirstAttribute(const std::string& attr_name) const override;

        virtual Ref<ISection> Read(const std::string &tag) const;
        virtual Ref<ISection> Write(const std::string &tag);
        virtual Ref<ISection> GetFirstChild() const;
        virtual Ref<ISection> NewChild(const std::string & tag);
        virtual void GetChildren(SectionPtrVector & children);


    public:
    
        void AddChild(Ref<DataStream> child);
        void ClearChildren();

        Ref<DataStream> GetChild(const std::string & tag, bool createIfMiss);

        Children::iterator begin() { return children_.begin(); }
        Children::iterator end() { return children_.end(); }

        void Print(std::ostream & stream);
        void Print(std::string & stream);
        void Print(std::ostream & stream, int depth);

        bool Scan(const std::string & stream);
        bool Scan(QuickFileReader & reader);
        
        bool Load(const std::string & filename);
        bool Save(const std::string & filename);

    private:

        std::string tag_;
        std::string value_;
        Children children_;
    };

}//end namepace kge


#endif // __KGE_IO_DATA_STREAM__

