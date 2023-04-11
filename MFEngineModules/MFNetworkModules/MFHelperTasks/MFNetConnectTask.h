/*
 * MFNetConnectTask.h
 *
 *  Created on: 18.03.2020
 *      Author: michl
 */

#ifndef MFENGINEMODULES_MFNETWORKMODULES_MFHELPERTASKS_MFNETCONNECTTASK_H_
#define MFENGINEMODULES_MFNETWORKMODULES_MFHELPERTASKS_MFNETCONNECTTASK_H_

#include <vector>
#include <mutex>
#include <MFNetClientClasses/MFNetClient.h>
#include <MFTasks/MFAbstractTask.h>
#include <MFThreadSystem/MFTaskThread.h>

#include "../MFInterfacesNetwork/MFINetClientControl.h"

/**
 * This task connects added connect requests. Client and server must be specified
 * with a addConnectRequest call. The client must be local and the server should be in
 * a reachable network.
 * Warning: For every doWork run, a new connect request queue will be created (old
 * will be deleted)!
 */
class MFNetConnectTask: public MFAbstractTask {
	struct ConnectData{
		std::string address="";
		uint16_t port;
		MFNetClient* pNetClient=nullptr;
	};
private:
	std::mutex
		lockVecConDat;
	std::vector<ConnectData*>
		*mp_vecConnectData;
	MFINetClientControl
		*mp_clientCtl;
public:
	uint32_t m_timeout=1000000;
	MFNetConnectTask(MFINetClientControl *pClientCtl){
		mp_clientCtl=pClientCtl;
		mp_vecConnectData=new std::vector<ConnectData*>();
		setTaskName("MFNetConnectTask");
	};
	virtual ~MFNetConnectTask(){
		for(ConnectData* pD:*mp_vecConnectData){
			delete pD;
		}
		delete mp_vecConnectData;
	};

	void addConnectRequest(
			std::string address,
			uint16_t port,
			MFNetClient* pNetClient){
		printInfo("MFNetConnectTask::addConnectRequest - "
				"connection request:\n"
				""+address+":"+std::to_string(port));
		ConnectData* pConDat=new ConnectData();
		pConDat->address=address;
		pConDat->port=port;
		pConDat->pNetClient=pNetClient;
		lockVecConDat.lock();
		mp_vecConnectData->push_back(pConDat);
		lockVecConDat.unlock();
	}

	bool doWork(){
	  /*create new queue for storing connect requests*/
		std::vector<ConnectData*> *pNewVecDat=new std::vector<ConnectData*>();

		/*switch in out queue*/
		lockVecConDat.lock();
		std::vector<ConnectData*> *pVecDat=mp_vecConnectData;
		mp_vecConnectData=pNewVecDat;
		lockVecConDat.unlock();

		bool ret=true;

		/*iterate over all connect requests and try to connect*/
		for(ConnectData* pCD:*pVecDat){
			if(pCD!=nullptr && pCD->pNetClient!=nullptr){
				printInfo("MFNetConnectTask::doWork - connecting to:\n"
						""+pCD->address+":"+std::to_string(pCD->port));

				/*try to connect*/
				if(!pCD->pNetClient->connect(pCD->address, pCD->port,m_timeout)){
					printErr("MFNetConnectTask - failed to establish "
							"connection with:\n address:"+pCD->address+
							":"+std::to_string(pCD->port));
					ret=false;

					/*reenqueue the connect request*/
					addConnectRequest(pCD->address, pCD->port, pCD->pNetClient);
				}else{
					printInfo("MFNetConnectTask::doWork - client connected to server:"
							" "+pCD->address+":"+std::to_string(pCD->port));

					/*Add the client to the controling intreface*/
					mp_clientCtl->addClient(pCD->pNetClient);
					pCD->pNetClient->startDetached();
				}
				delete pCD;
			}else{
				printInfo("MFNetConnectTask::doWork - invalid resources!");
			}
		}
		pVecDat->clear();
		delete pVecDat;
		return ret;
	}
};

#endif /* MFENGINEMODULES_MFNETWORKMODULES_MFHELPERTASKS_MFNETCONNECTTASK_H_ */
