#pragma once
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <vector>
#include "ext2_fs.h"
#include "SuperblockSummary.h"

unsigned int getBlockSize(int fd);
struct ext2_group_desc* getGroupInfo(int fd);
unsigned int getBlockSize(int fd);
unsigned int getBlockNumber(int fd);
unsigned int getInodeNumber(int fd);
// std::vector<std::vector<unsigned int>> getGroupInfo(int fd);