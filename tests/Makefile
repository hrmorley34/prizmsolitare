.PHONY: all exe dummy test clean

all: exe

BUILD_DIR := build
CXXFLAGS = -Os -Wall $(INCLUDE)

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

$(BUILD_DIR)/%.o: %.cpp
	$(CXX) -MMD -MP -MF $(BUILD_DIR)/$*.d $(CXXFLAGS) -c $< -o $@ $(ERROR_FILTER)

SRC_DIR := ../src
SRC_PREFIX := src_

$(BUILD_DIR)/$(SRC_PREFIX)%.o: $(SRC_DIR)/%.cpp
	$(CXX) -MMD -MP -MF $(BUILD_DIR)/$(SRC_PREFIX)$*.d $(CXXFLAGS) -c $< -o $@ $(ERROR_FILTER)

$(BUILD_DIR)/catch.o: catch2/catch_amalgamated.cpp
	$(CXX) -MMD -MP -MF $*.d $(CXXFLAGS) -c $< -o $@ $(ERROR_FILTER)

ifeq ($(OS),Windows_NT)
EXECUTABLE := runtests.exe
else
EXECUTABLE := runtests
endif

SOURCES := .
# Test .cpp files
TESTCPPFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
# .cpp files in ../src/
SRCCPPFILES = cardclasses.cpp game.cpp savedata.cpp

OFILES := $(addprefix $(BUILD_DIR)/,$(TESTCPPFILES:.cpp=.o))
OFILES += $(addprefix $(BUILD_DIR)/$(SRC_PREFIX),$(SRCCPPFILES:.cpp=.o))
OFILES += $(BUILD_DIR)/catch.o
DEPENDS	:=	$(OFILES:.o=.d)

$(EXECUTABLE): $(OFILES)
	$(CXX) $^ -o $@

-include $(DEPENDS)

exe: $(BUILD_DIR) $(EXECUTABLE)

force:

test: exe force
	$(abspath $(EXECUTABLE))

clean:
	rm -f $(EXECUTABLE)
	rm -rf $(BUILD_DIR)
