//
//  Application.h
//  libkge
//
//  Created by kevin on 17-9-21.
//
//

#include "application.h"
#include "graphics/graphics.h"

#include "util/log.h"

#include <assert.h>
#include "io/file_tool.h"
#include "io/file_tool_imp.h"
#include "io/section_factory.h"
#include "graphics/texture2D.h"
#include "graphics/shader_xml.h"
#include "graphics/material.h"

namespace kge
{
	IApplication* IApplication::_instance;
	/*static*/ IApplication* IApplication::GetInstance()
	{
		return _instance;
	}

	IApplication::IApplication():_width(1280)
		,_height(720)
	{
		_instance = this;
	}

	IApplication::~IApplication()
	{
		_instance = nullptr;
		World::Fini();
	}

	bool IApplication::Init()
	{

		FILE * f = new FILE();
		fopen_s(&f, "F:\\git\\KGE\\Assert\\test.xml", "r");
		size_t cur = ftell(f);
		fseek(f, 0, SEEK_END);
		size_t l = ftell(f);
		fseek(f, 0, SEEK_SET);

		SectionFactory::InitInstance();
		FileSystemMgr::InitInstance();

		Ref<IFileSystem> fileSystem = Ref<IFileSystem>(new FileSystemImp());
		if (!fileSystem->init())
		{
			KGE_LOG_ERROR("failed to init filesystem!");
			return false;
		}
		FileSystemMgr::Instance()->SetFileSystem(fileSystem.get());

		//add defualt resource path
		fileSystem->AddSearchPath("../../../Assert");

		//std::string content;
		//bool ret = ReadFile(content, "test.txt", false);

		auto x = SectionFactory::Load("test.xml", SectionType::Xml);
		//auto j = SectionFactory::Load("test.json", SectionType::Json);
		auto d = SectionFactory::Load("test.cfg", SectionType::DS);

		auto shader = x->Read("Shader");
		auto name = shader->GetFirstAttribute("name");
		auto queue = shader->GetFirstAttribute("queue");
		x = nullptr;

		auto t = Texture2D::LoadFromFile("test.png");

		ShaderXML* shaderx = new ShaderXML();
		shaderx->Load("test.xml");
		//delete shaderx;
		//shaderx = nullptr;

		if (!Graphics::Init(_width, _height))
		{
			KGE_LOG_ERROR("Graphics Init failed");
			return false;
		}

		if (!World::Init())
		{
			KGE_LOG_ERROR("World Init failed");
			return false;
		}

		auto mat = Material::Create("Base.shader.xml");

		Start();

		return true;
	}

	void IApplication::Fini()
	{
		Graphics::Fini();
	}
    
	void IApplication::Tick()
	{
		while (!_exit)
		{
			OnUpdate();
			OnDraw();
		}
	}

    int IApplication::Run()
    {
		assert(Init());

		Tick();

		Fini();

        return 0;
    }
    
	void IApplication::OnUpdate()
	{
		Graphics::Tick();

		Update();
		World::Tick();
	}

	void IApplication::OnDraw()
	{
		Graphics::Draw();
	}

} // end namespace kge
