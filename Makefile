PROGNAME = LinuxTcpDaemon
CLIENTNAME = ClientTcp

CXX = g++
CPPFLAGS = -std=c++14 -Wall -pedantic -pthread

CODEDIR = src/server
BUILDDIR = build
SRCFILES = $(wildcard ${CODEDIR}/*.cpp)
OBJECTS  = $(patsubst ${CODEDIR}%.cpp, ${BUILDDIR}%.o, $(SRCFILES))
# $@ - target; $< - first prereq; $^ - all prereq


all: buildDir $(BUILDDIR)/$(PROGNAME) $(BUILDDIR)/$(CLIENTNAME)

buildDir:
	mkdir -p $(BUILDDIR)

# client
$(BUILDDIR)/$(CLIENTNAME) : $(BUILDDIR)/client.o
	$(CXX) $(CPPFLAGS) -o $@ $^
	$(info Linked objects to Client ELF.)

$(BUILDDIR)/client.o : src/client/client.cpp
	$(CXX) $(CPPFLAGS) -c $< -o $@

# server
$(BUILDDIR)/$(PROGNAME) : $(OBJECTS)
	$(CXX) $(CPPFLAGS) -o $@ $^
	$(info Linked objects to Server ELF.)

$(BUILDDIR)/%.o: $(CODEDIR)/%.cpp
	$(CXX) $(CPPFLAGS) -c $< -o $@

format:
	find src/ -iname *.hpp -o -iname *.cpp | xargs clang-format -i

clean:
	rm -rf $(BUILDDIR)
