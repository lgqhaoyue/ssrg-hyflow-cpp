/*
 * BenchmarkExecutor.h
 *
 *  Created on: Aug 30, 2012
 *      Author: mishras[at]vt.edu
 */

#ifndef BENCHMARKEXECUTOR_H_
#define BENCHMARKEXECUTOR_H_

#include <boost/thread/tss.hpp>
#include "HyflowBenchmark.h"
#include "tm/bank/BankBenchmark.h"
#include "tm/list/ListBenchmark.h"

namespace vt_dstm {

class Integer {
	int value;
public:
	Integer() {}
	Integer (int v) {value =v;}
	~Integer() {}

	int getValue() const {
		return value;
	}

	void setValue(int value) {
		this->value = value;
	}
};

class BenchmarkExecutor {
	static HyflowBenchmark *benchmark;
	static boost::thread **benchmarkThreads;

	static boost::thread_specific_ptr<Integer> threadId;
	static int calls;
	static int delay;
	static long timeout;
	static bool checkPoint;
	static int objectsCount;
	static int transactions;
	static int readPercent;
	static int threads;
	static bool isInitiated;
	static int threadCount;
	static int executionTime;
	static boost::mutex execMutex;

	static bool* transactionType;
	static std::string** argsArray;
	static std::string* ids;


	static void prepareArgs();

	static unsigned long long getTime();
	static void writeResults();
	static std::string& randomId();
	static void createObjects();
    static void execute(int id);
	static void addExecTime(unsigned long long time);
public:
	BenchmarkExecutor();
	virtual ~BenchmarkExecutor();

	static void initExecutor();

	/* TODO: Try to retrieve this using configuration file
	 * This function allows user to register its class so that it can be
	 * registered as HyflowObject derived class in boost::serialization
	 */
    template<class Archive>
	static void registerObjectTypes(Archive & ar) {
    	BankBenchmark::registerObjectTypes(ar);
//    	ListBenchmark::registerObjectTypes(ar);
    }

    static void executeThreads();
	static int getThreadId();

};

} /* namespace vt_dstm */

#endif /* BENCHMARKEXECUTOR_H_ */
