

/*
#ifndef M_TABLE_H
#define M_TABLE_H

#include <vector>
#include <string>

#include "monitorentry.h"



class MonitorTable{
public:
	MonitorEntry* head;
	int length;

	MonitorTable();
	~MonitorTable();
	void insert(MonitorEntry * me);
	//void find(char * fp);
	void find(std::string fp);

	void print();
	//void findAll(char * fp, std::vector<std::string> * vec);
	void findAll(std::string fp, std::vector<MonitorEntry> * vec);

	std::vector<MonitorEntry> findAll(std::string fp);

};

#endif
*/


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

};

#endif