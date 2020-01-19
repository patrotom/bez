CXX=g++
CXXFLAGS=-std=c++14 -Wall -pedantic
LIBS=-lcrypto
INCLUDE=-I.
ecb: compile
	echo "ecb" > src/ecb
	./src/a.out < src/ecb
	rm src/ecb
	gnome-open src/Mad_scientist_ecb.bmp
cbc: compile
	echo "cbc" > src/cbc
	./src/a.out < src/cbc
	rm src/cbc
	gnome-open src/Mad_scientist_cbc.bmp
compile: src/uloha.cpp
	$(CXX) $(CXXFLAGS) $< $(LIBS) -o src/a.out $(INCLUDE)
clean:
	rm -r src/*.out src/Mad_scientist_*