#ifndef ModelMgr_H
#define ModelMgr_H

#include "mgr.h"
#include "ogl/oglModel.h"
#include "d3d/d3dModel.h"

#include <memory>
#include <string>
#include <unordered_map>


namespace byhj
{
	class ModelMgr : public Mgr
	{
	public:
	  ModelMgr();
	  ~ModelMgr();

	  static std::shared_ptr<ModelMgr> getInstance();

	  int  getMeshCount(std::string modelName);
	  GLuint getMeshVAO(std::string modelName, int index);

	  void loadOGLModel(std::string modelName);
	  void loadD3DModel(std::string modelName, ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext);
	 

	  void render(ID3D11DeviceContext *pD3D11DeviceContext);
	  void render(std::string modelName, ID3D11DeviceContext *pD3D11DeviceContext);

	  void render(GLuint program);
	  void renderPatch(GLuint program);

	  void render(std::string modelName, GLuint program);
	  void renderInstance(GLuint program, GLuint amount);
	  void renderInstance(std::string modelName, GLuint program, GLuint amount);

	private:
		std::unordered_map<std::string, OGLModel> m_OGLModels;
		std::unordered_map<std::string, D3DModel> m_D3DModels;

	};

}
#endif