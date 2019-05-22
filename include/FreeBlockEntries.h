#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include "ext2_fs.h"
#include "SuperblockSummary.h"
#include "GroupSummary.h"

unsigned int getBlockBitmapID(int fd);
void freeBlockEntries(int fd);
void bitAccess(__u8 val, unsigned int offset, unsigned int block_num);