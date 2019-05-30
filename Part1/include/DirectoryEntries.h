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


bool isValidDirectNode(struct ext2_inode * inode);
void getDirectoryEntries(int fd);