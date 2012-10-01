/*
 * MessageMaps.cpp
 *
 *  Created on: Sep 18, 2012
 *      Author: mishras[at]vt.edu
 */

#include "MessageMaps.h"
#include "../concurrent/ConcurrentHashMap.h"
#include "../logging/Logger.h"

namespace vt_dstm {

tbb::concurrent_hash_map<HyMessageType, void (*)(HyflowMessage &)> MessageMaps::handlerMap;
tbb::concurrent_hash_map<std::string, HyflowMessageFuture *> MessageMaps::trackerCallbackMap;
tbb::concurrent_hash_map<std::string, HyflowMessageFuture *> MessageMaps::objCallbackMap;
tbb::concurrent_hash_map<std::string, HyflowMessageFuture *> MessageMaps::syncCallbackMap;
tbb::concurrent_hash_map<std::string, HyflowMessageFuture *> MessageMaps::lockCallbackMap;
tbb::concurrent_hash_map<std::string, HyflowMessageFuture *> MessageMaps::readValidCallbackMap;
tbb::concurrent_hash_map<std::string, HyflowMessageFuture *> MessageMaps::registerCallbackMap;

MessageMaps::MessageMaps() {}

MessageMaps::~MessageMaps() {}

void MessageMaps::registerHandler(HyMessageType msg_t, void (*handlerFunc)(HyflowMessage &)){
	tbb::concurrent_hash_map<HyMessageType, void (*)(HyflowMessage &)>::accessor a;
	handlerMap.insert(a, msg_t);
	a->second = handlerFunc;
}


void (*MessageMaps::getMessageHandler(HyMessageType msg_t))(HyflowMessage & hmsg) {
	tbb::concurrent_hash_map<HyMessageType, void (*)(HyflowMessage &)>::const_accessor a;
	if (handlerMap.find(a,msg_t)) {
		return a->second;
	} else{
		throw "No Handler found for this message type " + msg_t ;
	}
}

/*LESSON: Don't throw the exception if same id is used as same thread can use same id
 * as for small intervals time stamp will be same. Make sure while assigning assigning
 * HyflowMessage future id use nodeId, thread Id and objectId + timeStamp */
void MessageMaps::registerMessageFuture(const std::string & m_id, HyMessageType t, HyflowMessageFuture & fu) {
	tbb::concurrent_hash_map<std::string, HyflowMessageFuture*>::accessor a;
	switch (t)
	{
	case MSG_TRK_OBJECT:
		if (!trackerCallbackMap.insert(a, m_id)) {
			Logger::fatal("TrackerCallback already exist with same m_id %s\n",m_id.c_str());
		}else {
			a->second = &fu;
		}
		break;
	case MSG_ACCESS_OBJECT:
		if (!objCallbackMap.insert(a, m_id)) {
			Logger::fatal("ObjectCallback already exist with same m_id %s\n",m_id.c_str());
		}else {
			a->second = &fu;
		}
		break;
	case MSG_GRP_SYNC:
		if (!syncCallbackMap.insert(a, m_id)) {
			Logger::fatal("SyncCallback already exist with same m_id %s\n",m_id.c_str());
		}else {
			a->second = &fu;
		}
		break;
	case MSG_LOCK_ACCESS:
		if (!lockCallbackMap.insert(a, m_id)) {
			Logger::fatal("LockCallback already exist with same m_id %s\n",m_id.c_str());
		}else {
			a->second = &fu;
		}
		break;
	case MSG_READ_VALIDATE:
		if (!readValidCallbackMap.insert(a, m_id)) {
			Logger::fatal("ReadValidateCallback already exist with same m_id %s\n",m_id.c_str());
		}else {
			a->second = &fu;
		}
		break;
	case MSG_REGISTER_OBJ:
		if (!registerCallbackMap.insert(a, m_id)) {
			Logger::fatal("RegisterCallback already exist with same m_id %s\n",m_id.c_str());
		}else {
			a->second = &fu;
		}
		break;
	case MSG_TYPE_INVALID:
		break;
	default:
		Logger::fatal("MSCN :registerMessageFuture :Invalid type message request to getbyId %s\n",m_id.c_str());
		break;
	}
}

HyflowMessageFuture* MessageMaps::getMessageFuture(const std::string & m_id, HyMessageType t) {
	HyflowMessageFuture* future = NULL;
	tbb::concurrent_hash_map<std::string, HyflowMessageFuture*>::const_accessor a;

	switch (t)
	{
	case MSG_TRK_OBJECT:
		if (trackerCallbackMap.find(a, m_id)) {
			future = a->second;
		}else {
			Logger::fatal("Future don't exist for trackerCallbackMap m_id %s\n",m_id.c_str());
		}
		break;
	case MSG_ACCESS_OBJECT:
		if (objCallbackMap.find(a, m_id)) {
			future = a->second;
		}else {
			Logger::fatal("Future don't exist for ObjectCallbackMap m_id %s\n",m_id.c_str());
		}
		break;
	case MSG_GRP_SYNC:
		if (syncCallbackMap.find(a, m_id)) {
			future = a->second;
		}else {
			Logger::fatal("Future don't exist for SyncCallbackMap m_id %s\n",m_id.c_str());
		}
		break;
	case MSG_LOCK_ACCESS:
		if (lockCallbackMap.find(a, m_id)) {
			future = a->second;
		}else {
			Logger::fatal("Future don't exist for LockCallbackMap m_id %s\n",m_id.c_str());
		}
		break;
	case MSG_READ_VALIDATE:
		if (readValidCallbackMap.find(a, m_id)) {
			future = a->second;
		}else {
			Logger::fatal("Future don't exist for ReadValidCallbackMap m_id %s\n",m_id.c_str());
		}
		break;
	case MSG_REGISTER_OBJ:
		if (registerCallbackMap.find(a, m_id)) {
			future = a->second;
		}else {
			Logger::fatal("Future don't exist for RegisterCallbackMap m_id %s\n",m_id.c_str());
		}
		break;
	case MSG_TYPE_INVALID:
		break;
	default:
		Logger::fatal("MSCN :GetMessageFuture :Invalid type message request to getbyId %s\n", m_id.c_str());
		break;
	}
	return future;
}

void MessageMaps::removeMessageFuture(const std::string & m_id, HyMessageType t) {
	switch(t){
	case MSG_TRK_OBJECT:
		if (!trackerCallbackMap.erase(m_id))
			Logger::fatal("Tracker Object already deleted for m_id %s\n",m_id.c_str());
		break;
	case MSG_ACCESS_OBJECT:
		if (!objCallbackMap.erase(m_id))
			Logger::fatal("Object Access already deleted for m_id %s\n",m_id.c_str());
		break;
	case MSG_GRP_SYNC:
		if (!syncCallbackMap.erase(m_id))
			Logger::fatal("Sync Callback already deleted for m_id %s\n",m_id.c_str());
		break;
	case MSG_LOCK_ACCESS:
		if(!lockCallbackMap.erase(m_id))
			Logger::fatal("Lock Callback already deleted for m_id %s\n",m_id.c_str());
		break;
	case MSG_READ_VALIDATE:
		if (!readValidCallbackMap.erase(m_id))
			Logger::fatal("Read Validate already deleted for m_id %s\n",m_id.c_str());
		break;
	case MSG_REGISTER_OBJ:
		if(!registerCallbackMap.erase(m_id))
			Logger::fatal("Register Callback already deleted for m_id %s\n",m_id.c_str());
		break;
	case MSG_TYPE_INVALID:
		break;
	default:
		Logger::fatal("MSCN :RemoveMessageFuture :Invalid type message request to getbyId %s\n", m_id.c_str());
		break;
	}
}

} /* namespace vt_dstm */
