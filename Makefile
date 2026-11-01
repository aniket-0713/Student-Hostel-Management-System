CC     = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic
SRC    = main.c hostel.c storage.c utils.c
TARGET = hostel

all: $(TARGET)

$(TARGET): $(SRC) hostel.h
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET) $(TARGET).exe hostel.dat occupants_report.txt

.PHONY: all clean
