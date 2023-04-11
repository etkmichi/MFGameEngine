/*
 * MFRenderModuleObject.h
 *
 *  Created on: 28.10.2019
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MODULERENDER_MFRENDERMODULEOBJECT_H_
#define MFENGINEMODULES_MODULERENDER_MFRENDERMODULEOBJECT_H_
#include <MFVulkan/MFVkSceneObject/MFVkSceneObject.h>
#include <MFData/MFInterfacesData/MFIDataObjectWrite.h>

#include "../MFModuleObject.h"
#include "glm/glm.hpp"
class MFRenderModuleObject:
		public MFModuleObject,
		public MFIDataObjectWrite{
private:
	static MFVkSceneObject
		*smp_dummySceneObject;
	static UBOMVPMatrix4x4
		*smp_uboContainerDummy;
	static glm::mat4x4
		*smp_modelMatrixDummy;
	static glm::vec4
		*smp_colorVectorDummy;

	MFVkSceneObject
		*mp_sceneObject;
	MFVkSceneObjectBuffers
		*mp_sceneObjectBuffers;

	MFVkBuffer
		*mp_modelBuffer=nullptr,
		*mp_viewBuffer=nullptr,
		*mp_perspectiveBuffer=nullptr,
		*mp_colorBuffer=nullptr;

	/* (   *)(  *)
	 * db=datrabuffer The data object will be a pointer to MFVkBuffer* .*/
	MFDataObject
		*mp_dbModel,
		*mp_dbView,
		*mp_dbProjection,
		*mp_dbColor;

	glm::vec4
		*mp_colorVector;
	glm::mat4x4
		*mp_syncModelMatrix,
		*mp_modelMat,
		*mp_viewMat,
		*mp_perspectiveMat;
	bool
	validSceneObject=false;

protected://virtual functions MFStructurableDataSetup
  virtual void updateSyncObject(MFSyncObject* syncObject);
  virtual void setupObjectDataStructure();

public://virtual functions of MFModuleObject:
  virtual void initData(){};

  /**
   * Don't need to update the data structures, the render module has no ouput data!
   * @return
   */
  virtual bool updateData();
  virtual bool updateObject();
  virtual void disable();
  virtual void enable();
	virtual bool synchronizeInput();
	virtual bool synchronizeOutput();

public:
	MFRenderModuleObject();
	virtual ~MFRenderModuleObject();

	glm::mat4x4 calcPerspective(
			float fov,
			float aspectRation,
			float nearPlane,
			float farPlane);

	bool writeData(
			void* pDestination,
			void* pInput,
			const uint32_t &dataByteSize);

	void setSceneObject(MFVkSceneObject *pSceneObject);

	/**
	 * The render modules update function will reload the used data into the render
	 * system.
	 */
	void update();
	void setColor(glm::vec4 color);
	bool hasValidSceneObject(){return validSceneObject;};
	/*update geometry*/
	/*static default geometry*/
};

#endif /* MFENGINEMODULES_MODULERENDER_MFRENDERMODULEOBJECT_H_ */
