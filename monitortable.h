
#ifndef M_TABLE_H
#define M_TABLE_H

#include <vector>
#include <string>
#include <iostream>

#include "monitorentry.h"



class MonitorTable{

	/* The monitor table tracks all the clients monitoring files */

public:
	std::vector<MonitorEntry> list;

	MonitorTable();
	~MonitorTable();

	/* Insert the entry into the map */
	void insert(MonitorEntry me);
	void print();

	/* find all the entries */
	std::vector<MonitorEntry> findAll(std::string fp);
	
	/* sweep across and remove any old ones that we find */
	std::vector<MonitorEntry> sweep(std::string fp);

	/* find and remove */
	uint findRemove(uint clientId);

	//same as other but also cleans by removing the old entries
	std::vector<MonitorEntry> sweepClean(std::string fp);

};

#endif