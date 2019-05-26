CC = g++
CFLAGS = -Wall -Wextra -std=c++11 -I./include
SOURCES = *.cpp
OBJFILES = lab3a.o SuperblockSummary.o GroupSummary.o FreeBlockEntries.o FreeInodeEntries.o InodeSummary.o DirectoryEntries.o IndirectBlockSummary.o
TARGET = lab3a
TARFILE = lab3a-005229714.tar.gz
SUMFILES = $(SOURCES) include/*.h Makefile README

all:$(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) -o $(TARGET) $(OBJFILES)

lab3a.o: lab3a.cpp
	$(CC) $(CFLAGS) -c lab3a.cpp

SuperblockSummary.o: SuperblockSummary.cpp
	$(CC) $(CFLAGS) -c SuperblockSummary.cpp

GroupSummary.o: GroupSummary.cpp
	$(CC) $(CFLAGS) -c GroupSummary.cpp

FreeBlockEntries.o: FreeBlockEntries.cpp
	$(CC) $(CFLAGS) -c FreeBlockEntries.cpp

FreeInodeEntries.o: FreeInodeEntries.cpp
	$(CC) $(CFLAGS) -c FreeInodeEntries.cpp

InodeSummary.o: InodeSummary.cpp
	$(CC) $(CFLAGS) -c InodeSummary.cpp

DirectoryEntries.o: DirectoryEntries.cpp
	$(CC) $(CFLAGS) -c DirectoryEntries.cpp

IndirectBlockSummary.o: IndirectBlockSummary.cpp
	$(CC) $(CFLAGS) -c IndirectBlockSummary.cpp

clean:
	rm -f $(OBJFILES) $(TARGET) $(TARFILE) *~

dist:
	tar -cvzf $(TARFILE) $(SUMFILES)
	