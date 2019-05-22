#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <string.h>
#include "ext2_fs.h"
#include "SuperblockSummary.h"
#include "GroupSummary.h"
#include "FreeBlockEntries.h"

void freeInodeEntries(int fd);
unsigned int getInodeBitmapID(int fd);