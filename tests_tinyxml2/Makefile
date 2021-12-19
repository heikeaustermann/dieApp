aufruf = g++
aufruf_flags = -Wall -Wextra -pthread -std=c++17 -pedantic -fmax-errors=1
gtest_dir = ../../googletest/googletest
prog_dir = ../proof_of_concept


all : erz

erz : erz_objekte erz_tester

erz_objekte : xmltest.cpp $(prog_dir)/tinyxml2.cpp
	$(aufruf) -isystem $(gtest_dir)/include $(aufruf_flags) -c xmltest.cpp $(prog_dir)/tinyxml2.cpp
    
erz_tester : xmltest.o tinyxml2.o $(gtest_dir)/src/gtest_main.a
	$(aufruf) -lpthread xmltest.o tinyxml2.o $(gtest_dir)/src/gtest_main.a -o tester

clean :
	rm -f tester *.o
