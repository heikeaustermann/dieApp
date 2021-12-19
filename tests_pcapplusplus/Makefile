include /usr/local/etc/PcapPlusPlus.mk

aufruf = g++
aufruf_flags = -Wall -Wextra -pthread -std=c++17 -pedantic -fmax-errors=1
gtest_dir = ../../googletest/googletest

# All Target
all:
	$(aufruf) $(aufruf_flags) $(PCAPPP_BUILD_FLAGS) $(PCAPPP_INCLUDES) -c -o testpcappp.o testpcappp.cpp
	$(aufruf) $(PCAPPP_LIBS_DIR) -static-libstdc++  testpcappp.o $(gtest_dir)/src/gtest_main.a $(PCAPPP_LIBS) -o tester

# Clean Target
clean:
	rm *.o *.pcap *.pcapng tester