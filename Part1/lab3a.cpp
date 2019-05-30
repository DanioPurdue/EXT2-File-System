#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <getopt.h>
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
	int c;
	int option_index;
	static struct option long_options[] = {
		{0, 0, 0, 0}
	};

	while (1){
		c = getopt_long(argc, argv, "", long_options, &option_index);
		if (c == -1){
			break;
		}
		switch(c){
			case 0:
				break;

			default:
				fprintf(stderr, "<Usage> %s filename\n", argv[0]);
				exit(1);
				break;
		}
	}

	if (argc < 1) exit(1);
	int fd = open(argv[1], O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "Cannot open the file: %s\n", argv[1]);
		exit(3);
	}
	getSuperNodeInfo(fd);
	getGroupInfo(fd);
	freeBlockEntries(fd);
	freeInodeEntries(fd);
	getInodeSummary(fd);
	getDirectoryEntries(fd);
	IndirectBlockSummary(fd);
	return 0;
}