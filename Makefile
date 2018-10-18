# Variables
SRCDIR = ./src
TESTSDIR = ./tests
BUILDDIR = ./build

TARGET = $(BUILDDIR)/fleet
TESTTARGET = $(BUILDDIR)/testFleet

CC = g++
CFLAGS = -c -Wall -Werror -Wextra -std=c++17
TESTSCFLAGS = -iquote $(SRCDIR)
DEBUGCFLAGS = -g

CFILES = $(addprefix $(SRCDIR)/,ParseError.cpp Token.cpp TokenStream.cpp \
	TokenTree.cpp Context.cpp TypeError.cpp Value.cpp)
OFILES = $(addprefix $(BUILDDIR)/,ParseError.o Token.o TokenStream.o \
	TokenTree.o Context.o TypeError.o Value.o)
TESTCFILES = $(addprefix $(TESTSDIR)/,TestToken.cpp TestTokenStream.cpp \
	TestTokenTree.cpp Tester.cpp tests.cpp)
TESTOFILES = $(addprefix $(BUILDDIR)/,TestToken.o TestTokenStream.o \
	TestTokenTree.o Tester.o tests.o)

# Basic Targets
.PHONY: default
default: $(TARGET)

.PHONY: debug
debug: CFLAGS += $(DEBUGCFLAGS)
debug: $(TARGET)

$(TARGET): $(BUILDDIR) $(OFILES)
	$(CC) -o $(TARGET) $(OFILES)

tests: CFLAGS += $(TESTSCFLAGS)
tests: $(BUILDDIR) $(OFILES) $(TESTOFILES)
	$(CC) -o $(TESTTARGET) $(OFILES) $(TESTOFILES)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

.PHONY: clean
clean:
	rm -rf $(BUILDDIR)

# Source Directory Object Files
$(BUILDDIR)/ParseError.o: $(SRCDIR)/ParseError.cpp $(SRCDIR)/ParseError.hpp
	$(CC) $(CFLAGS) $(SRCDIR)/ParseError.cpp -o $(BUILDDIR)/ParseError.o

$(BUILDDIR)/Token.o: $(SRCDIR)/Token.cpp $(SRCDIR)/Token.hpp
	$(CC) $(CFLAGS) $(SRCDIR)/Token.cpp -o $(BUILDDIR)/Token.o

$(BUILDDIR)/TokenStream.o: $(addprefix $(SRCDIR)/,TokenStream.cpp \
TokenStream.hpp ParseError.hpp Token.hpp)
	$(CC) $(CFLAGS) $(SRCDIR)/TokenStream.cpp -o $(BUILDDIR)/TokenStream.o

$(BUILDDIR)/TokenTree.o: $(addprefix $(SRCDIR)/,TokenTree.cpp TokenTree.hpp \
ParseError.hpp Token.hpp TokenStream.hpp TokenTreeVisitor.hpp)
	$(CC) $(CFLAGS) $(SRCDIR)/TokenTree.cpp -o $(BUILDDIR)/TokenTree.o

$(BUILDDIR)/Context.o: $(addprefix $(SRCDIR)/,Context.cpp Context.hpp \
TypeError.hpp Value.hpp)
	$(CC) $(CFLAGS) $(SRCDIR)/Context.cpp -o $(BUILDDIR)/Context.o

$(BUILDDIR)/TypeError.o: $(SRCDIR)/TypeError.cpp $(SRCDIR)/TypeError.hpp
	$(CC) $(CFLAGS) $(SRCDIR)/TypeError.cpp -o $(BUILDDIR)/TypeError.o

$(BUILDDIR)/Value.o: $(SRCDIR)/Value.cpp $(SRCDIR)/Value.hpp
	$(CC) $(CFLAGS) $(SRCDIR)/Value.cpp -o $(BUILDDIR)/Value.o

# Tests Directory Object Files
$(BUILDDIR)/TestToken.o: $(addprefix $(TESTSDIR)/,TestToken.cpp TestToken.hpp \
Tester.hpp) $(SRCDIR)/Token.hpp
	$(CC) $(CFLAGS) $(TESTSDIR)/TestToken.cpp -o $(BUILDDIR)/TestToken.o

$(BUILDDIR)/TestTokenStream.o: $(addprefix $(TESTSDIR)/,TestTokenStream.cpp \
TestTokenStream.hpp Tester.hpp) $(SRCDIR)/Token.hpp $(SRCDIR)/TokenStream.hpp
	$(CC) $(CFLAGS) $(TESTSDIR)/TestTokenStream.cpp -o \
	$(BUILDDIR)/TestTokenStream.o

$(BUILDDIR)/TestTokenTree.o: $(addprefix $(TESTSDIR)/,TestTokenTree.cpp \
TestTokenTree.hpp Tester.hpp) $(addprefix $(SRCDIR)/,Token.hpp TokenStream.hpp \
TokenTree.hpp)
	$(CC) $(CFLAGS) $(TESTSDIR)/TestTokenTree.cpp -o $(BUILDDIR)/TestTokenTree.o

$(BUILDDIR)/Tester.o: $(TESTSDIR)/Tester.cpp $(TESTSDIR)/Tester.hpp
	$(CC) $(CFLAGS) $(TESTSDIR)/Tester.cpp -o $(BUILDDIR)/Tester.o

$(BUILDDIR)/tests.o: $(addprefix $(TESTSDIR)/,tests.cpp TestToken.hpp \
TestTokenStream.hpp TestTokenTree.hpp)
	$(CC) $(CFLAGS) $(TESTSDIR)/tests.cpp -o $(BUILDDIR)/tests.o
