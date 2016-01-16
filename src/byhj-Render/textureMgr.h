#ifndef textureMgr_H
#define textureMgr_H

#include "Mgr.h"
#include <unordered_map>
#include <GL/glew.h>
#include <d3d11.h>
#include <memory>

namespace byhj
{
	class TextureMgr : public Mgr
	{
	public:
		TextureMgr();
	   ~TextureMgr();

	   static std::shared_ptr<TextureMgr>  getInstance();

	   void   loadOGLTexture(std::string name);
	   void   loadD3DTexture(std::string name);

	   GLuint getOGLTextureByName(std::string name) ;
	   ID3D11ShaderResourceView * getD3DTextureByName(std::string name) ;

	private:
          std::unordered_map<std::string, GLuint> m_oglTextures;
		  std::unordered_map<std::string, ID3D11ShaderResourceView*> m_d3dTextures; 
	  
	};
}
#endif