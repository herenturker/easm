CC = gcc
CFLAGS = -O2 \
  -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wfloat-equal -Wcast-align \
  -Wpointer-arith -Wstrict-overflow=5 -Wwrite-strings -Wmissing-declarations \
  -Wmissing-prototypes -Wno-unused-parameter -Wstack-protector \
  -Wconversion -Wsign-conversion -Wdouble-promotion -Wnull-dereference \
  -Wduplicated-cond -Wlogical-op -Wjump-misses-init -Wstrict-prototypes \
  -fstack-protector-strong -fPIC -pipe \
  -I./include \
  -static-libgcc \

SRC = $(wildcard src/*.c) $(wildcard src/arch/*.c)

OBJ = $(patsubst src/%.c,output/%.o,$(filter-out src/arch/%, $(SRC))) \
      $(patsubst src/arch/%.c,output/arch/%.o,$(filter src/arch/%, $(SRC)))

TARGET = easm

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

output/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

output/arch/%.o: src/arch/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all clean

clean:
	rm -rf output/*.o output/arch/*.o $(TARGET)

dll:
	objdump -p easm.exe | findstr "DLL"
