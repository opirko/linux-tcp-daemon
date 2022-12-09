PROGNAME = LinuxTcpDaemon

CXX = g++
CPPFLAGS = -std=c++14 -Wall -pedantic

CODEDIR = src
BUILDDIR = build
SRCFILES = $(wildcard ${CODEDIR}/*.cpp)
OBJECTS  = $(patsubst ${CODEDIR}%.cpp, ${BUILDDIR}%.o, $(SRCFILES))
# $@ - target; $< - first prereq; $^ - all prereq


all: buildDir $(BUILDDIR)/$(PROGNAME)

buildDir:
	mkdir -p $(BUILDDIR)

$(BUILDDIR)/$(PROGNAME) : $(OBJECTS)
	$(info Linking objects to ELF.)
	$(CXX) $(CPPFLAGS) -o $@ $^

$(BUILDDIR)/%.o: $(CODEDIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

format:
	find src/ -iname *.hpp -o -iname *.cpp | xargs clang-format -i

clean:
	rm -rf $(BUILDDIR)
