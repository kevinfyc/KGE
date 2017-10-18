//
//  Application.h
//  libkge
//
//  Created by kevin on 17-9-21.
//
//

#include <assert.h>

#include "application.h"

#include "io/file_tool.h"
#include "io/file_tool_imp.h"
#include "io/section_factory.h"

#include "graphics/texture2D.h"
#include "graphics/shader_xml.h"
#include "graphics/material.h"
#include "graphics/graphics.h"

#include "util/log.h"
#include "util/time.h"
#include "input.h"

namespace kge
{
	IApplication* IApplication::_instance;
	/*static*/ IApplication* IApplication::GetInstance()
	{
		return _instance;
	}

	IApplication::IApplication() :_width(1280)
		, _height(720)
		, _pre_taskloop(RefMake<TaskLoop>())
		, _post_taskloop(RefMake<TaskLoop>())
		, _thread_pool_tick(RefMake<ThreadPool>(4))
	{
		_instance = this;
	}

	IApplication::~IApplication()
	{
		fileSystem = nullptr;
		_instance = nullptr;
		World::Fini();
	}

	bool IApplication::Init()
	{
		SectionFactory::InitInstance();
		FileSystemMgr::InitInstance();

		fileSystem = Ref<IFileSystem>(new FileSystemImp());
		if (!fileSystem->init())
		{
			KGE_LOG_ERROR("failed to init filesystem!");
			return false;
		}
		FileSystemMgr::Instance()->SetFileSystem(fileSystem.get());

		//add defualt resource path
		fileSystem->AddSearchPath("../../../");

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
		Time::Tick();

		_pre_taskloop->Tick();

		Update();
		World::Tick();

		_post_taskloop->Tick();
		_thread_pool_tick->Wait();

		Input::Tick();
	}

	void IApplication::OnDraw()
	{
		Graphics::Draw();
	}

	void IApplication::OnResize(uint32 width, uint32 height)
	{
		Graphics::OnResize(width, height);
	}

	void IApplication::AddAsyncTickTask(Thread::TaskNode task)
	{
		_thread_pool_tick->AddTask(task);
	}

	void IApplication::RunTaskInPreLoop(TaskLoop::TaskNode task)
	{
		_pre_taskloop->Add(task);
	}

	void IApplication::RunTaskInPostLoop(TaskLoop::TaskNode task)
	{
		_pre_taskloop->Add(task);
	}

} // end namespace kge
