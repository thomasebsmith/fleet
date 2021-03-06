# Variables
SRCDIR = ./src
TESTSDIR = ./tests
BUILDDIR = ./build

TARGET = $(BUILDDIR)/fleet
TESTTARGET = $(BUILDDIR)/testFleet

CC = g++
CFLAGS = -c -Wall -Werror -Wextra -pedantic -std=c++17
TESTSCFLAGS = -iquote $(SRCDIR)
DEBUGCFLAGS = -g
LFLAGS = 

SRCEXT = cpp

CFILES = $(addprefix $(SRCDIR)/,ParseError.cpp Token.cpp TokenStream.cpp \
	TokenTree.cpp Context.cpp TypeError.cpp NumberValue.cpp Evaluator.cpp \
	DefaultContext.cpp IdentifierValue.cpp Value.cpp MaybeSharedPtr.cpp \
	Type.cpp)
OFILES = $(addprefix $(BUILDDIR)/,ParseError.o Token.o TokenStream.o \
	TokenTree.o Context.o TypeError.o NumberValue.o Evaluator.o \
	DefaultContext.o IdentifierValue.o Value.o MaybeSharedPtr.o Type.o)
EXECCFILES = $(addprefix $(SRCDIR)/,execute.cpp)
EXECOFILES = $(addprefix $(BUILDDIR)/,execute.o)
TESTCFILES = $(addprefix $(TESTSDIR)/,TestToken.cpp TestTokenStream.cpp \
	TestTokenTree.cpp Tester.cpp TestEvaluator.cpp TestContext.cpp tests.cpp)
TESTOFILES = $(addprefix $(BUILDDIR)/,TestToken.o TestTokenStream.o \
	TestTokenTree.o Tester.o TestEvaluator.o TestContext.o tests.o)

# Basic Targets
.PHONY: default
default: $(TARGET)

.PHONY: debug
debug: CFLAGS += $(DEBUGCFLAGS)
debug: $(TARGET)

.PHONY: debugtests
debugtests: CFLAGS += $(DEBUGCFLAGS)
debugtests: tests

$(TARGET): $(BUILDDIR) $(OFILES) $(EXECOFILES)
	$(CC) $(LFLAGS) -o $(TARGET) $(OFILES) $(EXECOFILES)

tests: CFLAGS += $(TESTSCFLAGS)
tests: $(BUILDDIR) $(OFILES) $(TESTOFILES)
	$(CC) $(LFLAGS) -o $(TESTTARGET) $(OFILES) $(TESTOFILES)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

.PHONY: clean
clean:
	rm -rf $(BUILDDIR)

$(OFILES) $(EXECOFILES):
	$(CC) $(CFLAGS) $(SRCDIR)/$(notdir $(basename $@)).$(SRCEXT) -o $@

$(TESTOFILES):
	$(CC) $(CFLAGS) $(TESTSDIR)/$(notdir $(basename $@)).$(SRCEXT) -o $@

# Source Directory Object Files
$(BUILDDIR)/ParseError.o: $(SRCDIR)/ParseError.cpp $(SRCDIR)/ParseError.hpp

$(BUILDDIR)/Token.o: $(SRCDIR)/Token.cpp $(SRCDIR)/Token.hpp

$(BUILDDIR)/TokenStream.o: $(addprefix $(SRCDIR)/,TokenStream.cpp \
TokenStream.hpp ParseError.hpp Token.hpp)

$(BUILDDIR)/TokenTree.o: $(addprefix $(SRCDIR)/,TokenTree.cpp TokenTree.hpp \
ParseError.hpp Token.hpp TokenStream.hpp TokenTreeVisitor.hpp)

$(BUILDDIR)/Context.o: $(addprefix $(SRCDIR)/,Context.cpp Context.hpp \
TypeError.hpp Value.hpp IdentifierValue.hpp MaybeSharedPtr.hpp)

$(BUILDDIR)/TypeError.o: $(SRCDIR)/TypeError.cpp $(SRCDIR)/TypeError.hpp

$(BUILDDIR)/NumberValue.o: $(addprefix $(SRCDIR)/,NumberValue.cpp \
NumberValue.hpp TypeError.hpp Value.hpp)

$(BUILDDIR)/Evaluator.o: $(addprefix $(SRCDIR)/,Evaluator.cpp Evaluator.hpp \
Context.hpp NumberValue.hpp ParseError.hpp Token.hpp TokenTree.hpp Value.hpp \
FunctionValue.hpp)

$(BUILDDIR)/DefaultContext.o: $(addprefix $(SRCDIR)/,DefaultContext.cpp \
DefaultContext.hpp Context.hpp FunctionValue.hpp NumberValue.hpp TypeError.hpp \
Value.hpp)

$(BUILDDIR)/IdentifierValue.o: $(addprefix $(SRCDIR)/,IdentifierValue.cpp \
TokenTree.cpp Value.hpp)

$(BUILDDIR)/Value.o: $(addprefix $(SRCDIR)/,Value.cpp Value.hpp TokenTree.hpp \
Evaluator.hpp)

$(BUILDDIR)/Type.o: $(addprefix $(SRCDIR)/,Type.cpp Type.hpp Value.hpp)

$(BUILDDIR)/execute.o: $(addprefix $(SRCDIR)/,execute.cpp TokenStream.hpp \
TokenTree.hpp Evaluator.hpp DefaultContext.hpp)

# Tests Directory Object Files
$(BUILDDIR)/TestToken.o: $(addprefix $(TESTSDIR)/,TestToken.cpp TestToken.hpp \
Tester.hpp) $(SRCDIR)/Token.hpp

$(BUILDDIR)/TestTokenStream.o: $(addprefix $(TESTSDIR)/,TestTokenStream.cpp \
TestTokenStream.hpp Tester.hpp) $(SRCDIR)/Token.hpp $(SRCDIR)/TokenStream.hpp

$(BUILDDIR)/TestTokenTree.o: $(addprefix $(TESTSDIR)/,TestTokenTree.cpp \
TestTokenTree.hpp Tester.hpp) $(addprefix $(SRCDIR)/,Token.hpp TokenStream.hpp \
TokenTree.hpp)

$(BUILDDIR)/Tester.o: $(TESTSDIR)/Tester.cpp $(TESTSDIR)/Tester.hpp

$(BUILDDIR)/TestEvaluator.o: $(addprefix $(TESTSDIR)/,TestEvaluator.cpp \
TestEvaluator.hpp Tester.hpp) $(addprefix $(SRCDIR)/,Context.hpp Evaluator.hpp \
NumberValue.hpp TokenTree.hpp Value.hpp DefaultContext.hpp)

$(BUILDDIR)/TestContext.o: $(addprefix $(TESTSDIR)/,TestContext.cpp \
TestContext.hpp Tester.hpp) $(addprefix $(SRCDIR)/,Context.hpp NumberValue.hpp \
Value.hpp IdentifierValue.hpp Token.hpp)

$(BUILDDIR)/tests.o: $(addprefix $(TESTSDIR)/,tests.cpp TestToken.hpp \
TestTokenStream.hpp TestTokenTree.hpp TestContext.hpp)
