OS ?= $(shell uname)

# Assume Visual C++ on Windows
# Environments like Cygwin will hopefully fall through and pick up the
# appropriate Unix-like options
ifeq ($(OS),Windows_NT)
EXE := bin\qqq.exe
OUT := /out:$(EXE)
CXX := cl
CXXFLAGS := /EHsc /W4
else
EXE := bin/qqq
OUT := -o $(EXE)
CXXFLAGS := -Wall -O0
ifneq (,$(findstring $(OS),Darwin FreeBSD))
CXX := clang++
else
CXX := g++
endif # macOS or FreeBSD
endif # Windows

setup:
	-mkdir bin

cpp: setup
	$(CXX) cpp/qqq.cpp $(OUT) $(CXXFLAGS)

rust: setup
	rustc rust/main.rs $(OUT)

clean:
	-rm $(EXE)
