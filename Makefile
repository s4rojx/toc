CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = nfa_to_dfa
SOURCES = main.c nfa.c dfa.c

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET) $(TARGET).exe
