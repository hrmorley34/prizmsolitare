.PHONY: all exe dummy test clean

all: exe

BUILD_DIR := build
CXXFLAGS = -Os -Wall $(INCLUDE)

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

$(BUILD_DIR)/%.o: %.cpp
	$(CXX) -MMD -MP -MF $(BUILD_DIR)/$*.d $(CXXFLAGS) -c $< -o $@ $(ERROR_FILTER)

$(BUILD_DIR)/catch.o: catch2/catch_amalgamated.cpp
	$(CXX) -MMD -MP -MF $*.d $(CXXFLAGS) -c $< -o $@ $(ERROR_FILTER)

ifeq ($(OS),Windows_NT)
EXECUTABLE := runtests.exe
else
EXECUTABLE := runtests
endif

SOURCES := .
CPPFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))

OFILES := $(addprefix $(BUILD_DIR)/,$(CPPFILES:.cpp=.o)) $(BUILD_DIR)/catch.o
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
