include /usr/local/etc/PcapPlusPlus.mk

# Merker: zukünftig flag -std=c++17 auf -std=c++20 (bzw -std=c++2a)
CXX := g++
CXXFLAGS+= -Wall -Wextra -pthread -std=c++17 -pedantic -fmax-errors=1

# Verzeichnis mit googletest
GTEST_DIR = ../../googletest/googletest
# Bezeichnung des zu erzeugenden ausführbaren Programms
TEST = tester
# Verzeichnis mit den Programmdateien
PROGRAM_DIR = ../modul_3_aenderer/
CONFIG_DIR = ../modul_0_werkzeugkasten/

CPPFLAGS += -isystem $(GTEST_DIR)/include

GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h $(GTEST_DIR)/include/gtest/internal/*.h
GTEST = $(GTEST_DIR)/src/gtest
GTESTALL = $(GTEST_DIR)/src/gtest-all
GTESTMAIN = $(GTEST_DIR)/src/gtest_main

GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)
SOURCES := $(wildcard *.cpp) $(wildcard $(PROGRAM_DIR)*.cpp) $(wildcard $(CONFIG_DIR)*.cpp)
HEADERS := $(wildcard *.h) $(wildcard $(PROGRAM_DIR)*.h) $(wildcard $(CONFIG_DIR)*.h)
OBJS := $(patsubst %.cpp,%.o,$(wildcard *.cpp)) $(patsubst $(PROGRAM_DIR)%.cpp,%.o,$(wildcard $(PROGRAM_DIR)*.cpp)) $(patsubst $(CONFIG_DIR)%.cpp,%.o,$(wildcard $(CONFIG_DIR)*.cpp))
TEMPLATES := $(wildcard *.t) $(wildcard $(PROGRAM_DIR)*.t) $(wildcard $(CONFIG_DIR)*.t)

all : $(TEST)

clean:
	rm -f $(TEST) *.o *.pcap *.csv

cleanall:
	rm -rf $(TEST) *.o *.pcap *.csv $(GTEST_DIR)/lib/*.o

$(GTESTALL).o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
	$(GTEST_DIR)/src/gtest-all.cc -o $(GTESTALL).o

$(GTESTMAIN).o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
	$(GTEST_DIR)/src/gtest_main.cc -o $(GTESTMAIN).o 

$(GTESTALL).a : $(GTESTALL).o 
	$(AR) $(ARFLAGS) $@ $^

$(GTESTMAIN).a : $(GTESTALL).o $(GTESTMAIN).o
	$(AR) $(ARFLAGS) $@ $^

$(TEST) : $(OBJS) $(GTEST_DIR)/src/gtest_main.a
	-$(CXX)  $(PCAPPP_LIBS_DIR) -lpthread $^ $(PCAPPP_LIBS) -o $(TEST)

$(OBJS) : $(SOURCES) $(HEADERS) $(TEMPLATES) $(GTEST_HEADERS)
	-$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(PCAPPP_BUILD_FLAGS) $(PCAPPP_INCLUDES) $(INCLUDE) -c $(SOURCES)
