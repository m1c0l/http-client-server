CXX=g++
CXXOPTIMIZE= -g
CXXFLAGS= -g -Wall -pthread -std=c++11 $(CXXOPTIMIZE)
USERID=EDIT_MAKE_FILE

CLASSES = HttpMessage HttpRequest HttpResponse

all: web-server web-client

web-server: $(CLASSES:=.cpp) $(CLASSES:=.h) web-server.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

web-client: $(CLASSES:=.cpp) $(CLASSES:=.h) web-client.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@


clean:
	rm -rf *.o *~ *.gch *.swp *.dSYM web-server web-client *.tar.gz

tarball: clean
	tar -cvf $(USERID).tar.gz *

.PHONY: clean tarball
