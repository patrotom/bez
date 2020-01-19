CXX=g++
CXXFLAGS=-std=c++14 -Wall -pedantic
LIBS=-lcrypto -lssl
INCLUDE=-I.
run: compile
	./tmp/cert.out tmp/site.data tmp/cert.pem
compile: src/cert.cpp
	$(CXX) $(CXXFLAGS) -o tmp/cert.out $< $(LIBS) $(INCLUDE)
clean:
	rm -r tmp/*