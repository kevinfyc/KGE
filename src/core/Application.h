//
//  Application.h
//  libkge
//
//  Created by kevin on 17-9-21.
//
//

#ifndef __KGE_APPLICATION_H__
#define __KGE_APPLICATION_H__

namespace kge
{
    class IApplication
    {
    public:
		bool Init();
		void Fini();

		void Tick();

		void OnUpdate();
		void OnDraw();

        int Run();

		inline void Quit() { _exit = true; };

	private:
		bool _exit = false;
    };
}

#endif __KGE_APPLICATION_H__
