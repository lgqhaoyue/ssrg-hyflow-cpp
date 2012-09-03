/*
 * BankAccount.h
 *
 *  Created on: Aug 20, 2012
 *      Author: mishras[at]vt.edu
 */

#ifndef BANKACCOUNT_H_
#define BANKACCOUNT_H_

#include <stdint.h>
#include "../../../core/HyflowObject.h"
#include "../../../core/context/HyflowContext.h"

namespace vt_dstm {

//template <class objType>
class BankAccount: public HyflowObject {
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version);
	uint64_t amount;

	void setAmount(uint64_t amount);
	uint64_t checkBalance();
	uint64_t checkBalance(HyflowContext* c);

	void deposit(uint64_t money);
	void deposit(uint64_t money, HyflowContext* c);

	void withdraw(uint64_t money);
	void withdraw(uint64_t money, HyflowContext* c);
	static uint64_t totalBalance(std::string id1, std::string id2, HyflowContext* c);
	static void transfer(std::string fromId, std::string toId, uint64_t money, HyflowContext* c);
public:
	BankAccount() {};
	BankAccount(uint64_t amount, const std::string & Id);
	BankAccount(uint64_t amount, const std::string & Id, int version);
	virtual ~BankAccount();


	static uint64_t totalBalance(std::string id1, std::string id2);
	static void transfer(std::string fromId, std::string toId, uint64_t money);
	void print();
	void getClone(HyflowObject **obj);
	void test();
};


} /* namespace vt_dstm */

#endif /* BANKACCOUNT_H_ */
