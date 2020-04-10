CC = g++  -std=c++11

.DEFAULT_GOAL := all


fs: filesystem.cpp filesystem.h
	$(CC) -c filesystem.cpp filesystem.h

message: message.cpp message.h
	$(CC) -c message.cpp message.h

marshal: marshal.cpp marshal.h message
	$(CC) -c marshal.cpp marshal.h

sender: sender.cpp sender.h
	$(CC) -c sender.cpp sender.h

m_entry: messageentry.cpp messageentry.h
	$(CC) -c messageentry.cpp messageentry.h

c_entry: cliententry.cpp cliententry.h
	$(CC) -c cliententry.cpp cliententry.h

c_map: clientmap.cpp clientmap.h
	$(CC) -c clientmap.cpp clientmap.h

monitor: monitorentry.cpp monitorentry.h monitortable.cpp monitortable.h
	$(CC) -c monitorentry.cpp monitorentry.h
	$(CC) -c monitortable.cpp monitortable.h

cache: cache.cpp cache.h cacheentry.cpp cacheentry.h
	$(CC) -c cacheentry.cpp cacheentry.h
	$(CC) -c cache.cpp cache.h


server: fs marshal sender m_entry c_entry c_map monitor server.cpp server.h
	$(CC) -o server filesystem.o message.o marshal.o sender.o messageentry.o cliententry.o clientmap.o monitorentry.o monitortable.o server.cpp

client: fs marshal sender m_entry cache monitor client.cpp client.h
	$(CC) -o client filesystem.o message.o marshal.o sender.o messageentry.o cache.o cacheentry.o monitorentry.o monitortable.o client.cpp



all: server client
	@mkdir cli1
	@mkdir cli2
	@mkdir cli3
	@cp client cli1/client
	@cp client cli2/client
	@cp client cli3/client


clean: 
	@rm *.h.gch
	@rm *.o
	@rm client
	@rm server
	@rm -r cli