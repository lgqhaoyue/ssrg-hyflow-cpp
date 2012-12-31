/*
 * ContentionManager.cpp
 *
 *  Created on: Dec 30, 2012
 *      Author: mishras[at]vt.edu
 */

#include "ContentionManager.h"

#include <cstdlib>

#include "../../util/logging/Logger.h"
#include "../../util/networking/NetworkManager.h"
#include "../context/types/DTL2Context.h"
#include <unistd.h>

#define HYFLOW_BACKOFF_BARRIER 3

#ifdef RELEASE
#define HYFLOW_BASE_BACKOFF 2000
#else
#define HYFLOW_BASE_BACKOFF 10000
#endif

namespace vt_dstm {

ContentionManager::ContentionManager() {}

ContentionManager::~ContentionManager() {}

void ContentionManager::init(void* metaData) {}

// Performs Random back-off if too many aborts are happening
void ContentionManager::deInit(void* metaData) {
	DTL2Context* context = (DTL2Context*)metaData;
	int aborts = context->getAbortCount();
	int baseSleepTime = HYFLOW_BASE_BACKOFF*NetworkManager::getThreadCount()*NetworkManager::getNodeCount();
	int sleepTime = ((aborts)/HYFLOW_BACKOFF_BARRIER)*baseSleepTime;
	sleepTime = sleepTime + abs(Logger::getCurrentMicroSec())%(sleepTime+1);
	// Do random back-Off
	if (sleepTime && context->isIsWrite()) {
		LOG_DEBUG("CONMAN :Performing back-off after %d aborts for %d ms\n", aborts, sleepTime/1000);
		usleep(sleepTime);
	}
}

} /* namespace vt_dstm */
