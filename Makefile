CC = g++

.DEFAULT_GOAL := tester

tester: tester.cpp
	${CC} -o tester tester.cpp

clean:
	@rm tester

deep_clean:
	@rm *.~
