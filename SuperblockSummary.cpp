#include "SuperblockSummary.h"

void getSuperNodeInfo(int fd) {
	struct ext2_super_block* super_node = new struct ext2_super_block;
	pread(fd, super_node, sizeof(struct ext2_super_block), SUPERBLOCK_OFFSET);
	unsigned int total_blocks = super_node -> s_blocks_count;
	unsigned int total_inodes = super_node -> s_inodes_count;
	unsigned int block_size = EXT2_MIN_BLOCK_SIZE << (super_node -> s_log_block_size);
	unsigned int inode_size = sizeof(struct ext2_inode);
	unsigned int blocks_per_group = super_node -> s_blocks_per_group;
	unsigned int inodes_per_group = super_node -> s_inodes_per_group;
	unsigned int first_inode = super_node -> s_first_ino;

	printf("%s,%u,%u,%u,%u,%u,%u,%u\n", "SUPERBLOCK", total_blocks, total_inodes, block_size, inode_size, blocks_per_group, inodes_per_group, first_inode);
	delete super_node;
}