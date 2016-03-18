
#ifndef _D3DCEGUI_H
#define _D3DCEGUI_H

/**************************************************************************
* The following libs (and corresponding headers) are needed to compile and to link:
* CEGUIBase
* CEGUIOpenGLRenderer
* CEGUICoreWindowRendererSet
* default CEGUI xml parser (and dependencies)
* GLFW3
* OpengGL
* glm headers (as part of CEGUIBase)
***************************************************************************/

#include <iostream>
#include <memory>

#include <d3d11.h>

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Direct3D11/Renderer.h>

#pragma  comment(lib, "CEGUIBase-0_d.lib")
#pragma  comment(lib, "CEGUICoreWindowRendererSet_d.lib")
#pragma  comment(lib, "CEGUIDirect3D11Renderer-0_d.lib")


using namespace CEGUI;

namespace byhj {

	class D3DCEGUI {
	public:
		void setCallback(UINT uMsg, WPARAM wParam, LPARAM lParam);

		void init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext);
		void render();
		void shutdown();

		static std::shared_ptr<D3DCEGUI> getInstance();

	private:
		Direct3D11Renderer* renderer;
		static std::shared_ptr<D3DCEGUI> pInstance;

	};
}


#endif