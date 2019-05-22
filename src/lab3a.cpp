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
	getGroupInfo(fd);
	printf("Main\n");
	return 0;
}