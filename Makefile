CC = g++  -std=c++11

.DEFAULT_GOAL := make_tester

make_tester: tester.cpp
	${CC} -o tester tester.cpp

make_fs: filesystem.cpp filesystem.h
	$(CC) -c filesystem.cpp filesystem.h

make_monitor: monitorentry.cpp monitorentry.h monitortable.cpp monitortable.h
	$(CC) -c monitorentry.cpp monitorentry.h
	$(CC) -c monitortable.cpp monitortable.h

make_all: make_fs make_monitor tester.cpp
	${CC} -o tester filesystem.o monitorentry.o monitortable.o tester.cpp


make_fss: filesystem.cpp tester.cpp
	g++ -c filesystem.cpp
	g++ -o tester filesystem.o tester.cpp

clean_tester: tester
	@rm tester

clean_object:
	@rm *.o

clean: 
	@rm *.o
	@rm *.h.gch
	@rm client
	@rm server


marshal: marshal.h marshal.cpp
	${CC} -o marshal marshal.cpp marshal.h

clean_marshal:
	@rm marshal

deep_clean:
	@rm *~


server: make_fs make_monitor server.cpp server.h
	$(CC) -c sender.cpp sender.h
	$(CC) -c marshal.cpp marshal.h
	$(CC) -c messageentry.cpp messageentry.h
	$(CC) -c cliententry.cpp cliententry.h	
	$(CC) -c clientmap.cpp clientmap.h
	$(CC) -c filesystem.cpp filesystem.h	
	$(CC) -o server marshal.o sender.o messageentry.o cliententry.o clientmap.o filesystem.o monitorentry.o monitortable.o server.cpp

client: client.cpp client.h
	$(CC) -c sender.cpp sender.h
	$(CC) -c marshal.cpp marshal.h
	$(CC) -c messageentry.cpp messageentry.h
	$(CC) -o client marshal.o sender.o messageentry.o client.cpp

runner: tester.cpp
	${CC} -c monitorentry.cpp monitorentry.h
	${CC} -c monitortable.cpp monitortable.h
	${CC} -c filesystem.cpp filesystem.h
	${CC} -o runner monitorentry.o monitortable.o filesystem.o tester.cpp

real_clean: 
	@rm *.o
	@rm *.h.gch
	@rm runner
