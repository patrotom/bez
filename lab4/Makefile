CXX=g++
CXXFLAGS=-std=c++14 -Wall -pedantic
LIBS=-lcrypto
INCLUDE=-I.
cipher: compile1
	touch src/data.enc
	./src/cipher.out src/data.bin src/pubkey.pem src/data.enc
	rm tmp
decipher: compile2
	touch src/data.dec
	wc -c src/data.bin
	./src/decipher.out src/data.enc src/privkey.pem src/data.dec
run: cipher decipher
	diff -s src/data.bin src/data.dec
compile1: src/cipher.cpp
	$(CXX) $(CXXFLAGS) -o src/cipher.out $< $(LIBS) $(INCLUDE)
compile2: src/decipher.cpp
	$(CXX) $(CXXFLAGS) -o src/decipher.out $< $(LIBS) $(INCLUDE)
clean:
	rm -r src/*.out src/data.enc src/data.dec