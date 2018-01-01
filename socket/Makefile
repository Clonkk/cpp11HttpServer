RCol='\e[0m'

Red='\e[0;31m'
Gre='\e[0;32m'
Blu='\e[0;34m'
Pur='\e[0;35m'

BRed='\e[1;31m'
BGre='\e[1;32m'
BBlu='\e[1;34m'
BPur='\e[1;35m'
CXX=g++
#CXX:=clang++ 
#--analyze -Xanalyzer -analyzer-output=text #-Weverything
#CLANG_PATH=/usr/lib/clang/3.4.2
#LD_LIBRARY_PATH:=$(CLANG_PATH)/lib
#LIBRARY_PATH:=$(CLANG_PATH)/lib
#CPATH:=$(CLANG_PATH)/include

CXXFLAGS=-std=c++11 -Wall -fPIC -g
WS:=$(shell echo $(CURDIR))
SRCDIR=src
OBJDIR=obj
SRC:=$(wildcard $(SRCDIR)/*.cpp)
#SRC:=$(filter-out $(SRCDIR)/%pattern%.cpp, $(SRC))
OBJ:=$(SRC:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

LIB_NAME:=lib_$(shell basename $(CURDIR))
LIB:=$(OBJDIR)/$(LIB_NAME)

TUDIR:=tu
TU:=$(wildcard $(TUDIR)/tu*.cpp)
TU_OBJ:=$(wildcard $(TUDIR)/*.cpp)
TU_OBJ:=$(filter-out $(TUDIR)/tu%, $(TU_OBJ))

EXEDIR:=exe
EXE:=$(TU:$(TUDIR)/%.cpp=$(EXEDIR)/%.out)
EXE_OBJ:=$(TU_OBJ:$(TUDIR)/%.cpp=$(EXEDIR)/%.o)

INCL_LIBS+=$(shell basename $(CURDIR))
CPPFLAGS+=-I $(SRCDIR) #-I $(INCL_LIBS)/src
LDLIBS+= -lpthread -lrt -L $(WS)/$(OBJDIR) -Wl,-rpath=$(WS)/$(OBJDIR) -l_$(INCL_LIBS) 

all:$(LIB)
	@
	@echo -e  $(BGre) "=== END LIB COMPILATION ===" $(RCol)

full:clean 
	@$(MAKE) -j all
	@$(MAKE) -j test
	@echo -e $(BGre) "=== END FULL COMPILATION ===" $(RCol)

test:$(EXE)
	@
	@echo -e  $(BGre) "=== END TESTS COMPILATION ===" $(RCol)

$(EXE):$(EXEDIR)/%.out:$(TUDIR)/$(1)%.cpp $(EXE_OBJ) $(LIB)
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) $< $(EXE_OBJ) -o $@ $(LDLIBS) 
	@echo -e  $(Gre)"Executable " $< " compiled successfully !" $(RCol)

$(LIB):$(OBJ)
	@$(CXX) $(CXXFLAGS) -shared -o $@.so $^
	@echo -e $(BBlu)"Compile "$@" successfully !"$(RCol)

$(EXE_OBJ):$(EXEDIR)/%.o:$(TUDIR)/$(1)%.cpp
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@ $(LDLIBS) 
	@echo -e  $(Gre)"Object " $< " compiled successfully !" $(RCol)

$(OBJ):$(OBJDIR)/%.o:$(SRCDIR)/%.cpp
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@ 
	@echo -e  $(Gre)"Object " $< " compiled successfully !" $(RCol)

clean:
	@-rm -f $(OBJDIR)/*.o $(OBJDIR)/*.so $(EXEDIR)/*.out $(EXEDIR)/*.o
	@echo -e  $(BPur)"$(OBJDIR)/ cleaned" $(Rcol)
	@echo -e  $(BPur)"$(EXEDIR)/ cleaned" $(RCol)
