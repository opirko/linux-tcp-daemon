PROGNAME = LinuxTcpDaemon

CXX = g++
CPPFLAGS = -std=c++14 -Wall -pedantic

CODEDIR = src
BUILDDIR = build
SRCFILES = $(wildcard ${CODEDIR}/*.cpp)
OBJECTS  = $(patsubst ${CODEDIR}%.cpp, ${BUILDDIR}%.o, $(SRCFILES))


all: buildDir $(BUILDDIR)/$(PROGNAME)

buildDir:
	mkdir -p $(BUILDDIR)

$(BUILDDIR)/$(PROGNAME): $(OBJECTS)
	$(info Linking objects to ELF.)
	$(info $$OBJECTS is ${OBJECTS})
	$(CXX) $(CPPFLAGS) -o $@ $^

$(OBJECTS): $(BUILDDIR)/%.o : $(SRCFILES)
	$(info Creating object files.)
	$(info Source files are [${SRCFILES}])
	$(CXX) $(CPPFLAGS) -c $< -o $@

format:
	find src/ -iname *.hpp -o -iname *.cpp | xargs clang-format -i

clean:
	rm -rf $(BUILDDIR)
