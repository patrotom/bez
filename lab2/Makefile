CXX=g++
CXXFLAGS=-std=c++14 -Wall -pedantic -g
LIBS=-lcrypto
INCLUDE=-I.
FILES=src/hexa_byte_convertor.cpp

uloha1: compile1
	./a.out
uloha2: compile2
	./a.out
compile1: ./src/uloha1.cpp
	$(CXX) $(CXXFLAGS) $< $(LIBS) $(INCLUDE)
compile2: ./src/uloha2.cpp
	$(CXX) $(CXXFLAGS) $< $(FILES) $(LIBS) $(INCLUDE)
clean:
	rm -r *.out