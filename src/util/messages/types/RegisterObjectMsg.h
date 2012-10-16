/*
 * RegisterObjectMsg.h
 *
 *  Created on: Aug 28, 2012
 *      Author: mishras[at]vt.edu
 */

#ifndef REGISTEROBJECTMSG_H_
#define REGISTEROBJECTMSG_H_

#include <string>
#include <cstddef>
#include "../BaseMessage.h"
#include "../../../core/HyflowObject.h"
#include "../HyflowMessage.h"

namespace vt_dstm {

class RegisterObjectMsg: public vt_dstm::BaseMessage {
	unsigned long long txnId;
	std::string objectId;
	int owner;
	bool request;
	bool waited; // To make sure one way or callback

	friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version);
public:
    RegisterObjectMsg() {request = true; waited=false;}
	RegisterObjectMsg(std::string id, unsigned long long tid);
	RegisterObjectMsg(std::string id, int owner, unsigned long long tid);
	virtual ~RegisterObjectMsg();

	static void registerObjectHandler(HyflowMessage & msg);
	HyflowObject *getObject();
	bool isRequest() const;
	void setRequest(bool request);
	void serializationTest();

	bool isWaited() const {
		return waited;
	}

	void setWaited(bool waited) {
		this->waited = waited;
	}
};

} /* namespace vt_dstm */

#endif /* REGISTEROBJECTMSG_H_ */
