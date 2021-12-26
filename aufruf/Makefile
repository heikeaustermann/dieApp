include ../ressourcen/PcapPlusPlus.mk

# Merker: zukünftig flag -std=c++17 auf -std=c++20 (bzw -std=c++2a)
CXX := g++
CXXFLAGS+= -Wall -Wextra -pthread -std=c++17 -pedantic -fmax-errors=1

# Bezeichnung des zu erzeugenden ausführbaren Programms
APP = dieapp
# Verzeichnisse mit den Programmdateien
WERKZEUG_DIR = ../modul_0_werkzeugkasten/
PROGRAM1_DIR = ../modul_1_erzeugung/
PROGRAM2_DIR = ../modul_2_ergaenzer/
PROGRAM3_DIR = ../modul_3_aenderer/
PROGRAM4_DIR = ../modul_4_parameter/

SOURCES := $(wildcard *.cpp) $(wildcard $(WERKZEUG_DIR)*.cpp) $(wildcard $(PROGRAM1_DIR)*.cpp) $(wildcard $(PROGRAM2_DIR)*.cpp) $(wildcard $(PROGRAM3_DIR)*.cpp) $(wildcard $(PROGRAM4_DIR)*.cpp)
HEADERS := $(wildcard *.h) $(wildcard $(WERKZEUG_DIR)*.cpp) $(wildcard $(PROGRAM1_DIR)*.h) $(wildcard $(PROGRAM2_DIR)*.h) $(wildcard $(PROGRAM3_DIR)*.h) $(wildcard $(PROGRAM4_DIR)*.h) 
OBJS := $(patsubst $(WERKZEUG_DIR)%.cpp,%.o,$(wildcard $(WERKZEUG_DIR)*.cpp)) $(patsubst $(PROGRAM1_DIR)%.cpp,%.o,$(wildcard $(PROGRAM1_DIR)*.cpp)) $(patsubst $(PROGRAM2_DIR)%.cpp,%.o,$(wildcard $(PROGRAM2_DIR)*.cpp)) $(patsubst $(PROGRAM3_DIR)%.cpp,%.o,$(wildcard $(PROGRAM3_DIR)*.cpp)) $(patsubst $(PROGRAM4_DIR)%.cpp,%.o,$(wildcard $(PROGRAM4_DIR)*.cpp))

all : $(APP)

new:
	rm -f *.pcap *.csv *.txt

clean:
	rm -f $(APP) *.o *.pcap *.csv *.txt

$(APP) : $(OBJS) dieapp.cpp
	-$(CXX) $(PCAPPP_LIBS_DIR) $(PCAPPP_INCLUDES) -lpthread $^ $(PCAPPP_LIBS) -o $(APP)

$(OBJS) : $(SOURCES) $(HEADERS) 
	-$(CXX) $(CXXFLAGS) $(PCAPPP_BUILD_FLAGS) $(PCAPPP_INCLUDES) $(INCLUDE) -c $(SOURCES)
