#ifndef FontMgr_H
#define FontMgr_H

#include <iostream>
#include <memory>
#include "mgr.h"

#include "ogl/oglFTFont.h"
#include "d3d/d3dFWFont.h"
#include "d3d/d3dSPFont.h"

#include "utility.h"

namespace byhj
{
	class FontMgr : public Mgr {
	public:
		 FontMgr(); 
		~FontMgr();

		static std::shared_ptr<FontMgr> getInstance();

		void initD3DFWFont();
		void initD3DSPFont();
		void initOGLFTFont();

		void renderD3DFWFont();
		void renderD3DSPFont();
		void renderOGLFTFont();

		void shutdown();


	private:
		std::shared_ptr<OGLFTFont> m_pOGLFTFont;
		std::shared_ptr<D3DFWFont> m_pD3DFWFont;
		std::shared_ptr<D3DSPFont> m_pD3DSPFont;
	};
}
#endif