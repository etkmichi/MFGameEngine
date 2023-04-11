/*
 * MFRenderModule.h
 *
 *  Created on: 28.10.2019
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MODULERENDER_MFRENDERMODULE_H_
#define MFENGINEMODULES_MODULERENDER_MFRENDERMODULE_H_
#include "../MFBaseModule.h"
#include <GLFW/glfw3.h>
#include <MFTime/MFTickCounter.h>

#include "MFVulkan/MFVkContext/MFVkContext.h"
#include "MFVulkan/MFVkResources/MFVkResources.h"
#include "MFVulkan/MFVkObjects/MFVkShader.h"
#include "MFVulkan/MFVkSceneManager/MFVkObjectManager.h"
#include "MFVulkan/MFVkScene/MFVkRenderLoop.h"
#include "MFVulkan/MFVkObjects/MFVkBuffer.h"
#include "vulkan/vulkan.h"
#include <glm/glm.hpp>
#include <MFVulkan/MFVkSceneObject/MFVkSceneObject.h>
#include "../MFInterfacesModules/MFIModuleCreator.h"
#include "../../MFStaticSetupData.h"
#include "../ModuleRender/MFRenderModuleObject.h"

class MFRenderModule:
		public MFBaseModule,
		public MFIModuleCreator{
private:
	MFVkContext
		*mp_context;
	MFVkResources
		*mp_resources;
	MFVkRenderLoop
		*mp_renderLoop;
	MFVkObjectManager
		*mp_vkObjectManager;
	MFSyncObject
		*mp_camObject;
	std::vector<MFVkBuffer*>
		*mp_vecViewBuffers;//TODO implement muliview in one window.
	MFVkBuffer
		*mp_viewBuffer=nullptr;
	glm::mat4x4
		*mp_viewMatrix;
	std::string
		m_windowTitle,
		m_dataPath,
		m_glslCompilerPath;
	MFTickCounter
		*mp_tickCounter;
//	MFQtPrinter
//	*mp_renderLog;

protected:
	bool initModuleObject(
			MFSyncObject* syncObject,
			MFModuleObject* pObject);
public:
	virtual MFModuleObject* createModuleObject(){
		MFRenderModuleObject* pRenderModule=new MFRenderModuleObject();
		return pRenderModule;
	};
	void updateResources(MFModuleResources* pRes);
	virtual MFBaseModule* createModule(){
		MFRenderModule* pMod=nullptr;
		if(mp_camObject!=nullptr)//TODO test if mp cam object is ok as param.
			pMod=new MFRenderModule(
					"Window",
					MFStaticSetupData::sm_engineDataPath,
					MFStaticSetupData::sm_glslCompilerPath,
					mp_camObject,
					mp_syncRes);
		else
			pMod=new MFRenderModule(
					"Window",
					MFStaticSetupData::sm_engineDataPath,
					MFStaticSetupData::sm_glslCompilerPath,
					new MFSyncObject(mp_syncRes),
					mp_syncRes);
		pMod->setWindowTitle("glfw vulkan window");
		return pMod;
	};

  virtual void initRecycling(MFModuleObject* pMO);
	/**
	 * If this constructor is used, the setter functions for window title, data path and
	 * glsl compiler path should be set manually. If needed other setters can be used too.
	 */
	MFRenderModule(MFSyncObjectResources* pSyncRes);

	/**
	 *
	 * @param windowTitle
	 * @param dataPath
	 * @param glslCompilerPath - path to the shader compiler
	 */
	MFRenderModule(
			std::string windowTitle,
			std::string dataPath,
			std::string glslCompilerPath,
			MFSyncObject* camObject,
			MFSyncObjectResources* pSyncRes);

	virtual ~MFRenderModule();

//	bool disableObject(MFSyncObject *pObject);//TODO
	/*inherited functions*/
	bool initModule();
//	bool initRenderModule();
//	bool syncInputData(std::vector<MFSyncObject*>* pVecInputData);
	bool executeModule();
	bool syncOutputData();
	void setWindowTitle(std::string title){m_windowTitle=title;};
	void setDataPath(std::string path){m_dataPath=path;};
	void setGLSLCompilerPath(std::string path){m_glslCompilerPath=path;};
	void setCamObject(MFSyncObject* pCam){mp_camObject=pCam;};
	MFSyncObject* getCamObject(){return mp_camObject;};
	GLFWwindow* getWindow();
};

#endif /* MFENGINEMODULES_MODULERENDER_MFRENDERMODULE_H_ */
