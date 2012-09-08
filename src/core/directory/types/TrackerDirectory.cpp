/*
 * TrackerDirectory.cpp
 *
 *  Created on: Aug 28, 2012
 *      Author: mishras[at]vt.edu
 */

#include "TrackerDirectory.h"
#include "../../../util/networking/NetworkManager.h"
#include "../../../util/messages/types/ObjectTrackerMsg.h"
#include "../../../util/messages/types/RegisterObjectMsg.h"
#include "../../../util/logging/Logger.h"

namespace vt_dstm {
ConcurrentHashMap<std::string, int> TrackerDirectory::directory;
ConcurrentHashMap<std::string, HyflowObject*> TrackerDirectory::local;

TrackerDirectory::TrackerDirectory() {}

TrackerDirectory::~TrackerDirectory() {}

int TrackerDirectory::getTracker(std::string objectId) {
	int end = objectId.find('-');
	return atoi((objectId.substr(0,end)).c_str());
}

HyflowObject* TrackerDirectory::locate(std::string id, bool rw, unsigned long long txn){
	HyflowObjectFuture fu(id, rw, txn);
	locateAsync(id, rw, txn, fu);
	return fu.waitOnObject();
}

void TrackerDirectory::locateAsync(std::string id, bool rw, unsigned long long txn, HyflowObjectFuture & fu){
	ObjectTrackerMsg msg(id, rw);
	int trackerNode = getTracker(id);

	HyflowMessage hmsg;
	hmsg.msg_t = MSG_TRK_OBJECT;
	hmsg.isCallback = true;	// isReplied false by default
	hmsg.setMsg(&msg);
	NetworkManager::sendCallbackMessage(trackerNode,hmsg,fu.getMessageFuture());
}

/*
 * Sends a one way message to register object.
 * Mainly used by commit function to register objects
 * If current node is already owner of object don't send the request only update owner
 * and old owner values (Used in unlock).
 */
void TrackerDirectory::registerObject(HyflowObject & object, unsigned long long txn) {
	std::string & id = object.getId();
	int oldOwner = object.getOwnerNode();
	int owner  = NetworkManager::getNodeId();
	object.setOwnerNode(owner);
	object.setOldOwnerNode(oldOwner);
	// Save the object in local cache, create a clone and update in local cache
	// Currently pointed objected is attached to message future and will be destroyed
	// on its destructor call
	HyflowObject *objectCopy = NULL;
	object.getClone(&objectCopy);
	std::pair<std::string, HyflowObject*> p;
	p.first = object.getId();
	p.second = objectCopy;
	local.updatePointerValue(p);

	Logger::debug("DIR : New owner of %s is %d\n", object.getId().c_str(), local.getValue(object.getId())->getOwnerNode());

	if ( owner != oldOwner ) {
		Logger::debug("DIR : Registering %s owner %d->%d\n", id.c_str(), oldOwner, owner);

		//Register the object with tracker
		RegisterObjectMsg romsg(id, owner, txn);
		HyflowMessage hmsg;
		hmsg.msg_t = MSG_REGISTER_OBJ;	// Not callback by default
		hmsg.setMsg(&romsg);

		NetworkManager::sendMessage(getTracker(id), hmsg);
	} else {
		Logger::debug("DIR :Local object %s no re-registration %d -> %d\n", object.getId().c_str(), oldOwner, owner);
	}
}

/*
 * This function allows the sender to wait on registration response
 * Mainly used by createObject in benchmark for synchronization purpose
 * If current node is already owner of object don't send the request only update owner
 * and old owner values (Used in unlock).
 */
void TrackerDirectory::registerObjectWait(HyflowObject & object, unsigned long long txn) {
	std::string & id = object.getId();
	int oldOwner = object.getOwnerNode();
	int owner  = NetworkManager::getNodeId();
	object.setOwnerNode(owner);
	object.setOldOwnerNode(oldOwner);
	// Save the object in local cache, create a clone and update in local cache
	// Currently pointed objected is attached to message future and will be destroyed
	// on its destructor call
	HyflowObject *objectCopy = NULL;
	object.getClone(&objectCopy);
	std::pair<std::string, HyflowObject*> p;
	p.first = object.getId();
	p.second = objectCopy;
	local.updatePointerValue(p);

	Logger::debug("DIR : New owner of %s is %d\n", object.getId().c_str(), local.getValue(object.getId())->getOwnerNode());

	if ( owner != oldOwner ) {
		Logger::debug("DIR : Registering %s owner %d->%d\n", id.c_str(), oldOwner, owner);

		//Register the object with tracker
		RegisterObjectMsg romsg(id, owner, txn);
		HyflowMessage hmsg;
		hmsg.msg_t = MSG_REGISTER_OBJ;	// Not callback by default
		hmsg.setMsg(&romsg);

		HyflowMessageFuture mFu;
		NetworkManager::sendCallbackMessage(getTracker(id), hmsg, mFu);
		mFu.waitOnFuture();
	} else {
		Logger::debug("DIR :Local object %s no re-registration %d -> %d\n", object.getId().c_str(), oldOwner, owner);
	}
}

void TrackerDirectory::registerObjectLocally(std::string objId, int owner, unsigned long long txn) {
	std::pair<std::string, int> p;
	p.first = objId;
	p.second = owner;
	Logger::debug("DIR : Object %s owner %d\n", objId.c_str(), owner);
	directory.insertValue(p);
}

void TrackerDirectory::unregisterObject(HyflowObject & object, unsigned long long txn) {
	RegisterObjectMsg romsg(object.getId(),txn);
	HyflowMessage hmsg;
	hmsg.msg_t = MSG_REGISTER_OBJ;	// Not callback by default
	hmsg.setMsg(&romsg);
	int tracker = getTracker(object.getId());
	if ( tracker != NetworkManager::getNodeId()) {
		NetworkManager::sendMessage(tracker, hmsg);
	} else {	// Local object
		unregisterObjectLocally(object.getId(),txn);
	}
}

// TODO: Proper clean up will require object to be deleted from all local caches
void TrackerDirectory::unregisterObjectLocally(std::string objId, unsigned long long txn) {
	directory.deletePair(objId);
}

HyflowObject* TrackerDirectory::getObjectLocally(std::string id, bool rw){
	return local.getValue(id);
}


void TrackerDirectory::updateObjectLocally(HyflowObject & obj){
	std::pair<std::string, HyflowObject*> p;
	p.first = obj.getId();
	p.second = &obj;
	local.updateValue(p);
}

int TrackerDirectory::getObjectLocation(std::string id){
	return directory.getValue(id);
}

} /* namespace vt_dstm */
