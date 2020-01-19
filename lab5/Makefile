CXX=g++
CXXFLAGS=-std=c++14 -Wall -pedantic
LIBS=-lcrypto -lssl
INCLUDE=-I.
run: compile
	./src/cert.out src/site.data src/cert.pem
compile: src/cert.cpp
	$(CXX) $(CXXFLAGS) -o src/cert.out $< $(LIBS) $(INCLUDE)
clean:
	rm -r src/*.out src/site.data src/cert.pem