CXX=g++
CXXOPTIMIZE= -O2
CXXFLAGS= -g -Wall -pthread -std=c++11 $(CXXOPTIMIZE)
USERID=EDIT_MAKE_FILE

CLASSES =
OBJDIR  = obj

all: web-server web-client

web-server: $(CLASSES)
	$(CXX) $(CXXFLAGS) -o $@ $(^:=.cpp) $@.cpp

web-client: $(CLASSES)
	$(CXX) $(CXXFLAGS) -o $@ $(^:=.cpp) $@.cpp

$(CLASSES): $(OBJDIR)
	$(CXX) -c $@.cpp -o $(OBJDIR)/$@.o

$(OBJDIR):
	mkdir -p $@

clean:
	rm -rf $(OBJDIR) *.o *~ *.gch *.swp *.dSYM web-server web-client *.tar.gz

tarball: clean
	tar -cvf $(USERID).tar.gz *
