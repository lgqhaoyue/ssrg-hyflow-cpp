/*
 * HyflowContextFactory.h
 *
 *  Created on: Oct 29, 2012
 *      Author: mishras[at]vt.edu
 */

#ifndef HYFLOWCONTEXTFACTORY_H_
#define HYFLOWCONTEXTFACTORY_H_

#include <vector>
#include "HyflowContext.h"

namespace vt_dstm {

class HyflowContextFactory {
	std::vector<HyflowContext*> contextStack;
	int contextStackIndex;
	HyflowContext* getContextFromStack();
	HyflowContext* getFreshContext();
public:
	HyflowContextFactory();
	virtual ~HyflowContextFactory();

	HyflowContext* getContextInstance();
	void releaseContextInstance();
};

} /* namespace vt_dstm */

#endif /* HYFLOWCONTEXTFACTORY_H_ */