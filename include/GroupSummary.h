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
// std::pair<std::vector<unsigned int>, std::vector<unsigned int>> getGroupInfo(int fd);
// void getGroupInfo(int fd);
std::vector<std::vector<unsigned int>> getGroupInfo(int fd);
unsigned int getBlockNumber(int fd);