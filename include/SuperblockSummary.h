#pragma once

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include "ext2_fs.h"

#define SUPERBLOCK_OFFSET 1024

void getSuperNodeInfo(int fd);