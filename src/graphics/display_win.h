//
//  display_win.h
//  libkge
//
//  Created by kevin on 17-9-21.
//
//

#ifndef __KGE_DISPLAY_WIN_H__
#define __KGE_DISPLAY_WIN_H__

#include "display_base.h"
#include "common.h"

namespace kge
{
	class DisplayWin : public DisplayBase
	{
	public:
		DisplayWin();
		virtual ~DisplayWin();

		virtual bool Init(uint32 width, uint32 height) override;
		virtual void Fini() override;

		void ProcessEvent();

	private:
		bool CreateView();

	protected:
		HWND _hwnd;
#if KGE_GLES
		HDC _hdc;
		HGLRC _context;
		HGLRC _shared_context;
#endif
	};
}

#endif // __KGE_DISPLAY_WIN_H__
