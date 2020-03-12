CC = g++  -std=c++11

.DEFAULT_GOAL := make_tester

make_tester: tester.cpp
	${CC} -o tester tester.cpp

make_fs: filesystem.cpp filesystem.h
	g++  -std=c++11 -c filesystem.cpp

make_monitor: monitorentry.cpp monitorentry.h monitortable.cpp monitortable.h
	g++  -std=c++11 -c monitorentry.cpp
	g++  -std=c++11 -c monitortable.cpp

make_all: make_fs make_monitor tester.cpp
	${CC} -o tester filesystem.o monitorentry.o monitortable.o tester.cpp


make_fss: filesystem.cpp tester.cpp
	g++ -c filesystem.cpp
	g++ -o tester filesystem.o tester.cpp

clean_tester: tester
	@rm tester

clean_object:
	@rm *.o

clean: clean_tester clean_object

deep_clean:
	@rm *~


runner: tester.cpp
	${CC} -c monitorentry.cpp monitorentry.h
	${CC} -c monitortable.cpp monitortable.h
	${CC} -c filesystem.cpp filesystem.h
	${CC} -o runner monitorentry.o monitortable.o filesystem.o tester.cpp

real_clean: 
	@rm *.o
	@rm *.h.gch
	@rm runner
