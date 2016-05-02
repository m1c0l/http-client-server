CXX=g++
CXXOPTIMIZE= -g
CXXFLAGS= -g -Wall -pthread -std=c++11 $(CXXOPTIMIZE)
DISTDIR= CS118Project1

CLASSES = HttpMessage HttpRequest HttpResponse

all: web-server web-client

web-server: $(CLASSES:=.cpp) web-server.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

web-client: $(CLASSES:=.cpp) web-client.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

$(CLASSES:=.cpp): $(CLASSES:=.h)

clean:
	rm -rf *.o *~ *.gch *.swp *.dSYM web-server web-client *.tar.gz

dist: clean
	tar cf - --transform='s|^|$(DISTDIR)/|' *.cpp *.h *.pdf Makefile Vagrantfile | gzip -9 > $(DISTDIR).tar.gz

.PHONY: clean dist
