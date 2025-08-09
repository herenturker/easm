CC  = gcc
CXX = g++

CFLAGS = -O2 \
  -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wfloat-equal -Wcast-align \
  -Wpointer-arith -Wstrict-overflow=5 -Wwrite-strings -Wmissing-declarations \
  -Wmissing-prototypes -Wno-unused-parameter -Wstack-protector \
  -Wconversion -Wsign-conversion -Wdouble-promotion -Wnull-dereference \
  -Wduplicated-cond -Wlogical-op -Wjump-misses-init -Wstrict-prototypes \
  -fstack-protector-strong -fPIC -pipe -g \
  -I./include

CXXFLAGS = -O2 \
  -std=c++17 \
  -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wfloat-equal -Wcast-align \
  -Wpointer-arith -Wstrict-overflow=5 -Wwrite-strings \
  -Wno-unused-parameter -Wstack-protector \
  -Wconversion -Wsign-conversion -Wdouble-promotion -Wnull-dereference \
  -Wduplicated-cond -Wlogical-op \
  -fstack-protector-strong -fPIC -pipe -g \
  -I./include

SRC_C   = $(wildcard src/*.c) $(wildcard src/arch/*.c)
SRC_CPP = $(wildcard src/*.cpp) $(wildcard src/arch/*.cpp)

OBJ_C   = $(patsubst src/%.c,output/%.o,$(filter-out src/arch/%, $(SRC_C))) \
          $(patsubst src/arch/%.c,output/arch/%.o,$(filter src/arch/%, $(SRC_C)))

OBJ_CPP = $(patsubst src/%.cpp,output/%.o,$(filter-out src/arch/%, $(SRC_CPP))) \
          $(patsubst src/arch/%.cpp,output/arch/%.o,$(filter src/arch/%, $(SRC_CPP)))

OBJ = $(OBJ_C) $(OBJ_CPP)

TARGET = easm

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

output/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

output/arch/%.o: src/arch/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

output/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

output/arch/%.o: src/arch/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: all clean

clean:
	rm -rf output/*.o output/arch/*.o $(TARGET)

dll:
	objdump -p easm.exe | findstr "DLL"
