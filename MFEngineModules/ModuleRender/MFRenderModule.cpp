/*
 * MFRenderModule.cpp
 *
 *  Created on: 28.10.2019
 *      Author: michl
 */

#include "../ModuleRender/MFRenderModule.h"

#include <MFRenderLibPrinter.h>
uint32_t scounter=0;
double millisCounter=0;
MFRenderModule::MFRenderModule(MFSyncObjectResources* pSyncRes) :
		MFBaseModule(pSyncRes,"MFRenderModule"),
		MFIModuleCreator("MFRenderModuleCreator"){
	enableOutputSynchronisation(false);
	mp_vecViewBuffers=new std::vector<MFVkBuffer*>();
	mp_vkObjectManager=nullptr;
	mp_viewMatrix=nullptr;
	mp_context=nullptr;
	mp_resources=nullptr;
	mp_renderLoop=nullptr;
	mp_camObject=nullptr;
	mp_tickCounter=new MFTickCounter();
//	mp_renderLog=nullptr;
}

MFRenderModule::MFRenderModule(
		std::string windowTitle,
		std::string dataPath,
		std::string glslCompilerPath,
		MFSyncObject* camObject,
		MFSyncObjectResources* pSyncRes) :
		MFBaseModule(pSyncRes,"MFRenderModule"){
	m_windowTitle=windowTitle;
	m_dataPath=dataPath;
	m_glslCompilerPath=glslCompilerPath;
	mp_context=new MFVkContext(windowTitle,dataPath,glslCompilerPath);
	mp_resources=new MFVkResources(mp_context);
	mp_renderLoop=new MFVkRenderLoop(mp_context,mp_resources);
	if(camObject!=nullptr){
		mp_camObject=camObject;
	}else{
		mp_camObject=new MFSyncObject(mp_syncRes);
	}

	mp_renderLoop->setCameraPosition(glm::lookAt(
			*camObject->getModelPosition(),
			*camObject->getGlobalLookAt(),
			*camObject->getUpwardsDirection()));

	enableOutputSynchronisation(false);
	mp_vecViewBuffers=new std::vector<MFVkBuffer*>();
	mp_vkObjectManager=nullptr;
	mp_viewMatrix=nullptr;
	mp_tickCounter=new MFTickCounter();
//  mp_renderLog=nullptr;
}

MFRenderModule::~MFRenderModule() {
	// TODO Auto-generated destructor stub
}

/*inherited functions*/
bool MFRenderModule::initModule(){
//  mp_renderLog=MFModuleResources::smp_guiManager->addPrinterWindow(
//      "RenderModule"+std::to_string(getModuleID()),
//      mp_syncRes->mp_logWindow);
//  mp_resources->getPrinter()->addSubTarget(mp_renderLog);

	if(mp_context==nullptr)
	  mp_context=new MFVkContext(
      m_windowTitle,
	    m_dataPath,
	    m_glslCompilerPath);

	if(mp_resources==nullptr)
	  mp_resources=new MFVkResources(mp_context);
	if(mp_renderLoop==nullptr)
	  mp_renderLoop=new MFVkRenderLoop(
	      mp_context,
	      mp_resources);
	if(mp_camObject!=nullptr){
		mp_renderLoop->setCameraPosition(glm::lookAt(
				*mp_camObject->getModelPosition(),
				*mp_camObject->getGlobalLookAt(),
				*mp_camObject->getUpwardsDirection()));
	}else{
		printWarning("MFRenderModule::initModule() - No cam object specified!");
	}

	if(!mp_context->initContext()){
		return false;
	}

	mp_context->setResources(mp_resources);
	if(!mp_resources->initResources()){
		return false;
	}

	if(!mp_renderLoop->initRenderLoop()){
		return false;
	}

	mp_viewBuffer=mp_renderLoop->getViewBuffer();
	mp_viewMatrix=(glm::mat4x4*)mp_viewBuffer->getInData();
	if(mp_res!=nullptr){
		mp_res->m_vecWindows.push_back(mp_context->getWindow());
		if(mp_res->mp_vecCamObjects!=nullptr){
			mp_res->mp_vecCamObjects->push_back(getCamObject());
		}
	}
	mp_tickCounter->start();
	return true;
}

void MFRenderModule::updateResources(MFModuleResources* pRes){
	pRes->m_vecWindows.push_back(mp_context->getWindow());
}

void MFRenderModule::initRecycling(MFModuleObject* pMO){
}

bool MFRenderModule::initModuleObject(
		MFSyncObject* syncObject,
		MFModuleObject* pObject){
  if(syncObject->getGeometry()==nullptr){
    P_ERR("geometry==nullptr!");
    return false;
  }

	if(!syncObject->getGeometry()->isCreated())
	  syncObject->getGeometry()->initGeometry();

  MFRenderModuleObject* pRenderObject=(MFRenderModuleObject*) pObject;
	if(syncObject->getRecycleState()==MFSyncObject::RECYCLE_STATE_NONE ||
	    !pRenderObject->hasValidSceneObject()){
    MFVkSceneObject* pSceneObject=//add to vulkan object manager
        mp_resources->getObjectManager()->addDefaultObject(
          syncObject->getGeometry(),
          *syncObject->getInitialSetup()->mp_color);
    if(pSceneObject==nullptr){
      printErr("MFRenderModule::initModuleObject - "
          "no object added! pSceneObject==nullptr");
      return false;
    }
    pRenderObject->setSceneObject(pSceneObject);
	}
	syncObject->updateModelMatrix();
	pRenderObject->synchronizeInput();
	pRenderObject->updateObject();
	pRenderObject->updateData();
	mp_renderLoop->triggerCommandBufferUpdate();
	return true;
}

bool MFRenderModule::executeModule(){
	bool ret=true;
	if(mp_tickCounter->stop()>=16){//TODO to get rid of object flickering (glitching between two posirtionsm
		//synchronize module execution with physics module execution. -> sometimes the uniformbuffer of
		//one frame uses the old data bc. it was not updated yet and next frame will show updated value
		if(mp_camObject!=nullptr)
			*mp_viewMatrix = mp_camObject->calculateGlobalLookAtMat4x4();
		else
			printWarning("MFRenderModule::executeModule() - cam object =="
					" nullptr!!");
		mp_viewBuffer->updateBuffer();
		ret=mp_renderLoop->renderLoopStep();
		mp_tickCounter->start();
	}

	return ret;
}

bool MFRenderModule::syncOutputData(){
	return true;
}

GLFWwindow* MFRenderModule::getWindow(){
	return mp_context->getWindow();
}
