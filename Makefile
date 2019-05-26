CC = g++
CFLAGS = -Wall -Wextra -std=c++11 -I./include
SOURCES = src/*.cpp
OBJFILES = lab3a.o SuperblockSummary.o GroupSummary.o FreeBlockEntries.o FreeInodeEntries.o InodeSummary.o DirectoryEntries.o IndirectBlockSummary.o
TARGET = lab3a
TARFILE = lab3a-005229714.tar.gz
SUMFILES = $(TARGET) $(SOURCES) include/*.h Makefile README

all:$(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) -o $(TARGET) $(OBJFILES)

lab3a.o: src/lab3a.cpp
	$(CC) $(CFLAGS) -c src/lab3a.cpp

SuperblockSummary.o: src/SuperblockSummary.cpp
	$(CC) $(CFLAGS) -c src/SuperblockSummary.cpp

GroupSummary.o: src/GroupSummary.cpp
	$(CC) $(CFLAGS) -c src/GroupSummary.cpp

FreeBlockEntries.o: src/FreeBlockEntries.cpp
	$(CC) $(CFLAGS) -c src/FreeBlockEntries.cpp

FreeInodeEntries.o: src/FreeInodeEntries.cpp
	$(CC) $(CFLAGS) -c src/FreeInodeEntries.cpp

InodeSummary.o: src/InodeSummary.cpp
	$(CC) $(CFLAGS) -c src/InodeSummary.cpp

DirectoryEntries.o: src/DirectoryEntries.cpp
	$(CC) $(CFLAGS) -c src/DirectoryEntries.cpp

IndirectBlockSummary.o: src/IndirectBlockSummary.cpp
	$(CC) $(CFLAGS) -c src/IndirectBlockSummary.cpp

clean:
	rm -f $(OBJFILES) $(TARGET) $(TARFILE) *~

dist:
	tar -cvzf $(TARFILE) $(SUMFILES)
	