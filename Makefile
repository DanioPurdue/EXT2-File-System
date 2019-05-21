CC = g++
CFLAGS = -Wall -Wextra -std=c++11 -I./include
SOURCES = src/*.cpp
OBJFILES = lab3a.o 
TARGET = lab3a

all:$(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) -o $(TARGET) $(OBJFILES)

lab3a.o: src/lab3a.cpp
	$(CC) $(CFLAGS) -c src/lab3a.cpp 