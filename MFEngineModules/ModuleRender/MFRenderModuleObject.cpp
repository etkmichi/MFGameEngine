/*
 * MFRenderModuleObject.cpp
 *
 *  Created on: 28.10.2019
 *      Author: michl
 */

#include "../ModuleRender/MFRenderModuleObject.h"

/*static vars:*/
MFVkSceneObject* MFRenderModuleObject::smp_dummySceneObject=
		new MFVkSceneObject();
UBOMVPMatrix4x4* MFRenderModuleObject::smp_uboContainerDummy=
		new UBOMVPMatrix4x4();
glm::mat4x4* MFRenderModuleObject::smp_modelMatrixDummy=new glm::mat4x4();
glm::vec4* MFRenderModuleObject::smp_colorVectorDummy=new glm::vec4();

MFRenderModuleObject::MFRenderModuleObject() {
	mp_sceneObject=smp_dummySceneObject;
	smp_uboContainerDummy->model=glm::translate(glm::mat4x4(1.0), glm::vec3(0,0,0));
	smp_uboContainerDummy->view = glm::lookAt(
			glm::vec3(2.0f, 2.0f, 2.0f), //Position of cam (eye)
			glm::vec3(0.0f, 0.0f, 0.0f), //look at this position (center)
			glm::vec3(0.0f, 0.0f, 1.0f));//upward direction (up);
	smp_uboContainerDummy->projection=calcPerspective(
			75.0f, 0.75f, 0.1, 201.0f);
	mp_syncModelMatrix=&smp_uboContainerDummy->model;
	mp_modelMat=&smp_uboContainerDummy->model;
	mp_viewMat=&smp_uboContainerDummy->view;
	mp_perspectiveMat=&smp_uboContainerDummy->projection;
	mp_colorVector=smp_colorVectorDummy;
	mp_sceneObjectBuffers=nullptr;
	setSetupName("MFRenderModuleObject");
}

bool MFRenderModuleObject::updateData(){
	bool ret=true;
	if(mp_dbModel!=nullptr && mp_dbModel->getData()!=nullptr){
		if(!mp_modelBuffer->updateBuffer()){
			MFObject::printErr("MFRenderModuleObject::updateObject - "
					"vulkan model mat. buffer update failed!");
			ret=false;
		}
	}else{
		MFObject::printErr("MFRenderModuleObject::updateObject - "
				"(mp_dbModel!=nullptr && mp_dbModel->getData()!=nullptr)==false!");
		ret=false;
	}

	if(mp_dbView!=nullptr && mp_dbView->getData()!=nullptr){
		if(!mp_viewBuffer->updateBuffer()){
			MFObject::printErr("MFRenderModuleObject::updateObject - "
					"vulkan view buffer update failed!");
			ret=false;
		}
	}else{
		MFObject::printErr("MFRenderModuleObject::updateObject - "
				"(mp_dbView!=nullptr && mp_dbView->getData()!=nullptr)==false!");
		ret=false;
	}

	if(mp_dbProjection!=nullptr && mp_dbProjection->getData()!=nullptr){
		if(!mp_perspectiveBuffer->updateBuffer()){
			MFObject::printErr("MFRenderModuleObject::updateObject - "
					"vulkan perspective buffer update failed!");
			ret=false;
		}
	}else{
		MFObject::printErr("MFRenderModuleObject::updateObject - "
				"(mp_dbProjection!=nullptr && mp_dbProjection->getData()!=nullptr)"
				"==false!");
		ret=false;
	}

	if(mp_dbColor!=nullptr && mp_dbColor->getData()!=nullptr){
		if(!mp_colorBuffer->updateBuffer()){
			MFObject::printErr("MFRenderModuleObject::updateObject - "
					"vulkan color buffer update failed!");
			ret=false;
		}
	}else{
		MFObject::printErr("MFRenderModuleObject::updateObject - "
				"(mp_dbColor!=nullptr && mp_dbColor->getData()!=nullptr)==false!");
		ret=false;
	}
	return ret;
}

bool MFRenderModuleObject::updateObject(){
	bool ret=true;
	if(mp_dbModel!=nullptr && mp_dbModel->getData()!=nullptr){
		memcpy(mp_modelBuffer->getInData(),mp_dbModel->getData(),sizeof(glm::mat4x4));
		mp_modelBuffer->mapData(mp_dbModel->getData());
//		if(!mp_modelBuffer->updateBuffer()){
//			MFObject::printErr("MFRenderModuleObject::updateObject - "
//					"vulkan model mat. buffer update failed!");
//			ret=false;
//		}
	}else{
		MFObject::printErr("MFRenderModuleObject::updateObject - "
				"(mp_dbModel!=nullptr && mp_dbModel->getData()!=nullptr)==false!");
		ret=false;
	}

	if(mp_dbView!=nullptr && mp_dbView->getData()!=nullptr){
		if(!mp_viewBuffer->updateBuffer()){
			MFObject::printErr("MFRenderModuleObject::updateObject - "
					"vulkan view buffer update failed!");
			ret=false;
		}
	}else{
		MFObject::printErr("MFRenderModuleObject::updateObject - "
				"(mp_dbView!=nullptr && mp_dbView->getData()!=nullptr)==false!");
		ret=false;
	}

	if(mp_dbProjection!=nullptr && mp_dbProjection->getData()!=nullptr){
		if(!mp_perspectiveBuffer->updateBuffer()){
			MFObject::printErr("MFRenderModuleObject::updateObject - "
					"vulkan perspective buffer update failed!");
			ret=false;
		}
	}else{
		MFObject::printErr("MFRenderModuleObject::updateObject - "
				"(mp_dbProjection!=nullptr && mp_dbProjection->getData()!=nullptr)"
				"==false!");
		ret=false;
	}

	if(mp_dbColor!=nullptr && mp_dbColor->getData()!=nullptr){
		if(!mp_colorBuffer->updateBuffer()){
			MFObject::printErr("MFRenderModuleObject::updateObject - "
					"vulkan color buffer update failed!");
			ret=false;
		}
	}else{
		MFObject::printErr("MFRenderModuleObject::updateObject - "
				"(mp_dbColor!=nullptr && mp_dbColor->getData()!=nullptr)==false!");
		ret=false;
	}
	return ret;
}

void MFRenderModuleObject::setColor(glm::vec4 color){
  mp_dbColor->writeData(&color);
}

void MFRenderModuleObject::setupObjectDataStructure(){
	//TODO setup shared buffer
	MFDataObject* pDat=add4x4Matrix(nullptr);
	pDat->setObjectName("4x4ModelMatrix");
	mp_dbModel=pDat;

	pDat=add4x4Matrix(nullptr);
	pDat->setObjectName("4x4ViewMatrix");
	mp_dbView=pDat;

	pDat=add4x4Matrix(nullptr);
	pDat->setObjectName("4x4ProjectionMatrix");
	mp_dbProjection=pDat;

	pDat=addNBitValue(sizeof(glm::vec4)*8, nullptr);
	pDat->setObjectName("4DColorVector");
	mp_dbColor=pDat;
}

bool MFRenderModuleObject::writeData(
		void* pDestination,
		void* pInput,
		const uint32_t &dataByteSize){
	if(pDestination==nullptr || pInput == nullptr || dataByteSize==0){
		MFObject::printErr("MFRenderModuleObject::writeData - "
				"pDestination==nullptr || pInput == nullptr || dataByteSize==0");
		return false;
	}
	void** ppVKBuffer=(void**)pDestination;
	MFVkBuffer* pVkBuffer=(MFVkBuffer*)(*ppVKBuffer);
	if(pVkBuffer->getSize()<dataByteSize){
		MFObject::printErr("MFRenderModuleObject::writeData - input data to big"
				"for writing to vulkan buffer!");
		return false;
	}
	//TODO dont map data, just add vkbuffer to vec input buffer update, which will
	//update input data during input sync
	if(pVkBuffer->getInData()!=nullptr){
		memcpy(pVkBuffer->getInData(),pInput,dataByteSize);
		return pVkBuffer->updateBuffer();
	}
	MFObject::printWarning("MFRenderModuleObject::writeData - mapping to unkowen "
			"data ptr! If vulkan data will be updated, the same pointer is used!");
	//TODO bad implementation of vulkan buffer, change somehow.
	//The vulkan buffer was pre allocated with pre allocated memory as input data.
	//change class -> dont overwrite input data ptr, just copy then map
	//			-> create function for set up the described buffer behavior
	return pVkBuffer->mapData(pInput);
}

glm::mat4x4 MFRenderModuleObject::calcPerspective(float fov,float aspectRation,float nearPlane,float farPlane){
	glm::mat4x4 mat={};
	float c= 1.0f / tan(((1.0f*3.1415*fov)/360));
	mat[0][0]=c/aspectRation;
	mat[0][1]=.0f;
	mat[0][2]=.0f;
	mat[0][3]=.0f;

	mat[1][0]=.0f;
	mat[1][1]=-c;
	mat[1][2]=.0f;
	mat[1][3]=.0f;

	mat[2][0]=.0f;
	mat[2][1]=.0f;
	mat[2][2]=farPlane/(nearPlane-farPlane);
	mat[2][3]=-1.0f;

	mat[3][0]=.0f;
	mat[3][1]=.0f;
	mat[3][2]=(nearPlane*farPlane)/(nearPlane-farPlane);
	mat[3][3]=.0f;
	return mat;
}

MFRenderModuleObject::~MFRenderModuleObject() {
	// TODO Auto-generated destructor stub
}

void MFRenderModuleObject::setSceneObject(MFVkSceneObject *pSceneObject){
	mp_sceneObject=pSceneObject;
	if(mp_sceneObject!=smp_dummySceneObject)
	  validSceneObject=true;
	mp_modelBuffer=mp_sceneObject->getBufferCollection()->getModelMatrix();
	mp_viewBuffer=mp_sceneObject->getBufferCollection()->getViewMatrix();
	mp_perspectiveBuffer=mp_sceneObject->getBufferCollection()->getPerspectiveMatrix();
	mp_colorBuffer=mp_sceneObject->getBufferCollection()->getColor();
	if(mp_modelBuffer->getSize()!=sizeof(glm::mat4x4)){
		P_WRN(
				"mp_modelBuffer vulkan buffer doesnt match with glm::mat4x4!"
				"Usage may overwrite important data!");
	}
	if(mp_viewBuffer->getSize()!=sizeof(glm::mat4x4)){
    P_WRN(
				"mp_viewBuffer vulkan buffer doesnt match with glm::mat4x4!"
				"Usage may overwrite important data!");
	}
	if(mp_perspectiveBuffer->getSize()!=sizeof(glm::mat4x4)){
    P_WRN(
				"mp_perspectiveBuffer vulkan buffer doesnt match with glm::mat4x4!"
				"Usage may overwrite important data!");
	}

	mp_modelMat=(glm::mat4x4*)mp_modelBuffer->getInData();
	mp_viewMat=(glm::mat4x4*)mp_viewBuffer->getInData();
	mp_colorVector=(glm::vec4*)mp_colorBuffer->getInData();
	mp_perspectiveMat=(glm::mat4x4*)mp_perspectiveBuffer->getInData();

	mp_dbModel->setData(mp_modelMat);
	mp_dbView->setData(mp_viewMat);
	mp_dbColor->setData(mp_colorVector);
	mp_dbProjection->setData(mp_perspectiveMat);
};

void MFRenderModuleObject::updateSyncObject(MFSyncObject* syncObject){
	/*TODO make thread safe, lock the vars!*/
//	mp_syncModelMatrix=syncObject->getGlobalModelMatrix();
};

bool MFRenderModuleObject::synchronizeInput(){
	/*Read the syncObjects position rotation and write it to this object*/
	if(smp_dummySceneObject==mp_sceneObject){
		//TODO must not be null use a dummy if not set!
		MFObject::printErr("MFRenderModuleObject::synchronizeInput - "
		    "mp_sceneObject == dummy!");
		return false;//TODO delete this if
	}
	mp_syncModelMatrix=mp_syncObject->getGlobalModelMatrix();
	memcpy(mp_modelMat,mp_syncModelMatrix,sizeof(glm::mat4x4));
	mp_modelBuffer->updateBuffer();
	mp_colorBuffer->updateBuffer();
	return true;
}

bool MFRenderModuleObject::synchronizeOutput(){
	return true;/*No output*/
}

void MFRenderModuleObject::disable(){
	mp_sceneObject->disable();
}
void MFRenderModuleObject::enable(){
	mp_sceneObject->enable();
}

void MFRenderModuleObject::update(){
	if(!mp_sceneObject->getVertexBuffer()->mapData(
			mp_syncObject->getGeometry()->
			getGeometryData().mp_sharedObjectBuffer->getBuffer())){
		MFObject::printErr("MFRenderModuleObject::update() - failed!");
	}

	if(!mp_sceneObject->getIndexBuffer()->mapData(
			mp_syncObject->getGeometry()->getVecIndices().data())){
		MFObject::printErr("MFRenderModuleObject::update() - failed!");
	}
}
