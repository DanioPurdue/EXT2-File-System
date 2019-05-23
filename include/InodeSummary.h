#pragma once

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <math.h>
#include <iostream>
#include <time.h>
#include "ext2_fs.h"
#include "FreeInodeEntries.h"

#define SUPERBLOCK_OFFSET 1024
#define EXT2_S_IFDIR 0x4000
#define EXT2_S_IFREG 0X8000
#define EXT2_S_IFLNK 0xA000
unsigned int getInodeTableID(int fd);
void getInodeSummary(int fd);