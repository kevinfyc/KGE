//
//  Application.h
//  libkge
//
//  Created by kevin on 17-9-21.
//
//

#ifndef __KGE_APPLICATION_H__
#define __KGE_APPLICATION_H__

#include "pre_define.h"
#include "world.h"

namespace kge
{
    class IApplication
    {
    public:
		IApplication();
		virtual ~IApplication();

		bool Init();
		void Fini();

		void Tick();

		void OnUpdate();
		void OnDraw();

        int Run();

		inline void Quit() { _exit = true; };

		static IApplication* GetInstance();

		inline void SetName(const std::string& name) { _name = name; }
		inline bool GetName(std::string& name) { name = _name; return true; }

		inline void SetInitSize(uint32 width, uint32 height) { _width = width; _height = height; }

		virtual void Start() {}
		virtual void Update() {}

	private:
		static IApplication* _instance;
		std::string _name;

		bool _exit = false;

		uint32 _width;
		uint32 _height;
    };
}

#endif // __KGE_APPLICATION_H__
