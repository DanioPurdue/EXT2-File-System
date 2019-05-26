#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include "ext2_fs.h"
#include "SuperblockSummary.h"
#include "GroupSummary.h"
#include "FreeBlockEntries.h"
#include "FreeInodeEntries.h"
#include "InodeSummary.h"
#include "DirectoryEntries.h"
#include "IndirectBlockSummary.h"

using namespace std;

int main(int argc, char * argv[])
{
	if (argc < 1) exit(1);
	int fd = open(argv[1], O_RDONLY);
	getSuperNodeInfo(fd);
	getGroupInfo(fd);
	freeBlockEntries(fd);
	freeInodeEntries(fd);
	getInodeSummary(fd);
	getDirectoryEntries(fd);
	IndirectBlockSummary(fd);
	return 0;
}