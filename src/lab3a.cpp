#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ext2_fs.h"

int fd;
int block_size;

void getSuperNodeInfo() {
	struct ext2_super_block* super_node = new struct ext2_super_block;
	pread(fd, super_node, 1024, 1024);
	int total_blocks = super_node -> s_blocks_count;
	int total_inodes = super_node -> s_inodes_count;
	block_size = EXT2_MIN_BLOCK_SIZE << super_node -> s_log_block_size;
	int inode_size = sizeof(struct ext2_inode);
	int blocks_per_group = super_node -> s_blocks_per_group;
	int inodes_per_group = super_node -> s_inodes_per_group;
	int first_inode = super_node -> s_first_ino;

	printf("%s,%d,%d,%d,%d,%d,%d,%d\n", "SUPERBLOCK", total_blocks, total_inodes, block_size, inode_size, blocks_per_group, inodes_per_group, first_inode);
	delete super_node;
}

void getGroupInfo() {
	struct ext2_group_desc group_descriptor_table[block_size/sizeof(struct ext2_group_desc)];
	// struct ext2_group_desc* group_descriptor = malloc(sizeof(struct ext2_group_desc));
	pread(fd, group_descriptor_table, block_size, block_size + 1024);
	int group_num = 0;
	while (1) {
		if (group_descriptor_table[group_num].bg_block_bitmap == 0)
			break;

		int free_blocks = group_descriptor_table[group_num].bg_free_blocks_count;
		int free_inodes = group_descriptor_table[group_num].bg_free_inodes_count;
		int block_bitmap = group_descriptor_table[group_num].bg_block_bitmap;
		int inode_bitmap = group_descriptor_table[group_num].bg_inode_bitmap;
		printf("GROUP,%d,%d,%d,%d,%d\n", group_num, free_blocks, free_inodes, block_bitmap, inode_bitmap);
		group_num++;
		// group_descriptor += ;
	}
}

int main(int argc, char** argv)
{
	/* code */
	(void) argc;
	(void) argv;

	fd = open("trivial.img", O_RDONLY);
	getSuperNodeInfo();
	getGroupInfo();
	return 0;
}