/*
 * ListNode.h
 *
 *  Created on: Sep 8, 2012
 *      Author: mishras[at]vt.edu
 */

#ifndef LISTNODE_H_
#define LISTNODE_H_

#include <string>
#include "../../../core/HyflowObject.h"
#include "../../../core/context/HyflowContext.h"
#include "../../../core/HyflowObjectFuture.h"

namespace vt_dstm {

class ListNode: public vt_dstm::HyflowObject {
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version);

	std::string nextId;
	int value;
	static void addNode(int value, HyflowContext *c);
	static void deleteNode(int value, HyflowContext *c);
	static void sumNodes(HyflowContext *c);
	static void findNode(int value, HyflowContext *c);
public:
	ListNode();
	virtual ~ListNode();

	std::string getNextId() const;
	void setNextId(std::string nextId);
	int getValue() const;
	void setValue(int value);
	void print();
	void getClone(HyflowObject **obj);
	/*
	 * Adds the given value in the list
	 */
	static void addNode(int value);
	/*
	 * Deletes the first occurrence of value in list
	 */
	static void deleteNode(int value);
	/*
	 * Sums all the values in the list
	 */
	static void sumNodes();
	/*
	 * Finds the first occurrence of value in list
	 */
	static void findNode(int value);

};

} /* namespace vt_dstm */

#endif /* LISTNODE_H_ */
