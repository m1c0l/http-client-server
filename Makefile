CXX=g++
CXXOPTIMIZE= -O2
CXXFLAGS= -g -Wall -pthread -std=c++11 $(CXXOPTIMIZE)
USERID=EDIT_MAKE_FILE

SERVER_CLASSES = web-server
CLIENT_CLASSES = web-client

all: web-server web-client

web-server: $(SERVER_CLASSES:=.cpp)
	$(CXX) -o $@ $^ $(CXXFLAGS)

web-client: $(CLIENT_CLASSES:=.cpp)
	$(CXX) -o $@ $^ $(CXXFLAGS)

clean:
	rm -rf *.o *~ *.gch *.swp *.dSYM web-server web-client *.tar.gz

tarball: clean
	tar -cvf $(USERID).tar.gz *
