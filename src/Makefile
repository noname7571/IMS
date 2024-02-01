CC = g++
CFLAGS = --std=c++17
LDFLAGS = -lsimlib -lm
SOURCE_FILE = main.cpp
OUTPUT_BINARY = main

all: $(OUTPUT_BINARY)

$(OUTPUT_BINARY): $(SOURCE_FILE)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

clean:
	rm -f $(OUTPUT_BINARY)
