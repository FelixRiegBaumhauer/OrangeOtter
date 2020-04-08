
#ifndef M_TABLE_H
#define M_TABLE_H

#include <vector>
#include <string>
#include <iostream>

#include "monitorentry.h"



class MonitorTable{
public:
	std::vector<MonitorEntry> list;

	MonitorTable();
	~MonitorTable();
	void insert(MonitorEntry me);
	void print();
	std::vector<MonitorEntry> findAll(std::string fp);
	std::vector<MonitorEntry> sweep(std::string fp);
	uint findRemove(uint clientId);

	//same as other but also cleans by removing the old entries
	std::vector<MonitorEntry> sweepClean(std::string fp);

};

#endif