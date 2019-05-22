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

using namespace std;

int main()
{
	int fd = open("trivial.img", O_RDONLY);
	getSuperNodeInfo(fd);
	struct ext2_group_desc* group_desc = getGroupInfo(fd);
	(void) group_desc;
	// cout << "bitmap: " << group_desc -> bg_block_bitmap << endl;
	// printf("Main\n");
	return 0;
}