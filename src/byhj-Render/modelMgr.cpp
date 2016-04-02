#include "modelMgr.h"

namespace byhj
{
	ModelMgr::ModelMgr()
	{
		m_dir += "objects/";
	}

	ModelMgr::~ModelMgr()
	{

	}

	std::shared_ptr<ModelMgr> ModelMgr::getInstance()
	{
		static std::shared_ptr<ModelMgr>  pInstance = std::make_shared<ModelMgr>();

		if (pInstance != nullptr) {
			return pInstance;
		}
	}
	int  ModelMgr::getMeshCount(std::string modelName) 
	{
		if (m_OGLModels.find(modelName) == m_OGLModels.end() ) {
		   std::cerr << "The file of " << modelName << " is not load" << std::endl; 
		   return 0;
		}
		int meshCnt = m_OGLModels[modelName].getMeshCount();
		return meshCnt;

	}
	GLuint  ModelMgr::getMeshVAO(std::string modelName, int index)
	{
		if (m_OGLModels.find(modelName) == m_OGLModels.end()) {
			std::cerr << "The file of " << modelName << " is not load" << std::endl;
			return 0;
		}
		GLuint meshVAO = m_OGLModels[modelName].getMeshVAO(index);
		return meshVAO;
	}

	void ModelMgr::loadOGLModel(std::string modelName) {

		if (m_OGLModels.find(modelName) != m_OGLModels.end()) {
			return;
		}
		OGLModel oglModel;
		oglModel.loadModel(modelName);
		m_OGLModels.insert(std::make_pair(modelName, oglModel) );
		
	}

	void ModelMgr::loadD3DModel(std::string modelName, ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext)
	{
		if (m_D3DModels.find(modelName) != m_D3DModels.end()) {
			return;
		}

		D3DModel d3dModel;
		d3dModel.loadModel(modelName);
		d3dModel.init(pD3D11Device, pD3D11DeviceContext);
		m_D3DModels.insert(std::make_pair(modelName, d3dModel));
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ModelMgr::render( ID3D11DeviceContext *pD3D11DeviceContext)
	{
		for (auto iter = m_D3DModels.begin(); iter != m_D3DModels.end(); ++iter) {
			iter->second.render(pD3D11DeviceContext);
		}
	}

	void ModelMgr::render(std::string modelName, ID3D11DeviceContext *pD3D11DeviceContext)
	{
		if (m_D3DModels.find(modelName) == m_D3DModels.end()) {
			std::cerr << "Error, not load the model:" << modelName << std::endl;
			return;
		}
		m_D3DModels[modelName].render(pD3D11DeviceContext);
	}

	void ModelMgr::render(GLuint program)
	{
		for (auto iter = m_OGLModels.begin(); iter != m_OGLModels.end(); ++iter) {
			iter->second.render(program);
		}
	}
	void ModelMgr::render(std::string modelName, GLuint program)
	{
		if (m_OGLModels.find(modelName) == m_OGLModels.end()) {
			std::cerr << "Error, not load the model:" << modelName << std::endl;
			return;
		}
		m_OGLModels[modelName].render(program);
	}
	void ModelMgr::renderInstance(GLuint program, GLuint amount)
	{
		for (auto iter = m_OGLModels.begin(); iter != m_OGLModels.end(); ++iter) {
			iter->second.renderInstance(program, amount);
		}
	}
	void ModelMgr::renderInstance(std::string modelName, GLuint program, GLuint amount)
	{
		if (m_OGLModels.find(modelName) == m_OGLModels.end()) {
			std::cerr << "Error, not load the model:" << modelName << std::endl;
			return;
		}
			m_OGLModels[modelName].renderInstance(program, amount);
	}

}