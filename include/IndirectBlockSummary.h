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
#include "InodeSummary.h"

void printIndirectBlock(int fd, unsigned int inode_number, unsigned int block_id, int level, int logical_block);
void IndirectBlockSummary(int fd);