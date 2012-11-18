SRCBWT:=$(wildcard bwt*.cpp)
SRCUNBWT:=$(wildcard unbwt*.cpp)
SRCMTF:=$(wildcard mtf*.cpp)
SRCUNMTF:=$(wildcard unmtf*.cpp)
SRCAHM:=$(wildcard ahm*.cpp) Bitstream.cpp AdaptiveHuffmanTree.cpp CodeObject.cpp Node.cpp
SRCUNAHM:=$(wildcard unahm*.cpp) Bitstream.cpp AdaptiveHuffmanTree.cpp CodeObject.cpp Node.cpp
SRC:=$(wildcard common*.cpp)

OBJBWT:=$(SRCBWT:.cpp=.o)
OBJUNBWT:=$(SRCUNBWT:.cpp=.o)
OBJMTF:=$(SRCMTF:.cpp=.o)
OBJUNMTF:=$(SRCUNMTF:.cpp=.o)
OBJAHM:=$(SRCAHM:.cpp=.o)
OBJUNAHM:=$(SRCUNAHM:.cpp=.o)
OBJ:=$(SRC:.cpp=.o)

CFLAGS:=-ggdb -O3
CXXFLAGS:=$(CFLAGS) -std=c++11
CC:=gcc
CXX:=g++
LDFLAGS:=

EBWT:=bwt
EUNBWT:=unbwt
EMTF:=mtf
EUNMTF:=unmtf
EAHM:=ahm
EUNAHM:=unahm

all: BWT MTF AHM

BWT: $(EBWT) $(EUNBWT)
MTF: $(EMTF) $(EUNMTF)
AHM: $(EAHM) $(EUNAHM)

$(EBWT): $(OBJBWT) $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $(LIBS) $^
	
$(EUNBWT): $(OBJUNBWT) $(OBJ)
	$(CXX) $(LDFALGS) -o $@ $(LIBS) $^
	
$(EMTF): $(OBJMTF) $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $(LIBS) $^
	
$(EUNMTF): $(OBJUNMTF) $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $(LIBS) $^
	
$(EAHM):$(OBJAHM) $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $(LIBS) $^
	
$(EUNAHM):$(OBJUNAHM) $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $(LIBS) $^
	
clean:
	rm -f $(OBJBWT) $(OBJUNBWT) $(OBJMTF) $(OBJUNMTF) $(OBJAHM) $(OBJUNAHM)
	
clean-all: clean
	rm -f $(EBWT) $(EUNBWT) $(EMTF) $(EUNMTF) $(EAHM) $(EUNAHM)

%.o:%.cpp
	$(CXX) -c $(CXXFLAGS) $^ -o $*.o
