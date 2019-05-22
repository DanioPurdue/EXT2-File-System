#include "GroupSummary.h"
using namespace std;

unsigned int getBlockSize(int fd) {
	struct ext2_super_block* super_node = new struct ext2_super_block;
	pread(fd, super_node, sizeof(struct ext2_super_block), SUPERBLOCK_OFFSET);
	unsigned int block_size = EXT2_MIN_BLOCK_SIZE << (super_node -> s_log_block_size);
	delete super_node;
	return block_size;
}

unsigned int getBlockNumber(int fd) {
	struct ext2_super_block* super_node = new struct ext2_super_block;
	pread(fd, super_node, sizeof(struct ext2_super_block), SUPERBLOCK_OFFSET);
	unsigned int total_blocks = super_node -> s_blocks_count;
	delete super_node;
	return total_blocks;
}

unsigned int getInodeNumber(int fd) {
	struct ext2_super_block* super_node = new struct ext2_super_block;
	pread(fd, super_node, sizeof(struct ext2_super_block), SUPERBLOCK_OFFSET);
	unsigned int total_inodes = super_node -> s_inodes_count;
	delete super_node;
	return total_inodes;
}

struct ext2_group_desc* getGroupInfo(int fd) {
	unsigned int block_size = getBlockSize(fd);
	struct ext2_group_desc* group_descriptor_table = new struct ext2_group_desc;
	pread(fd, group_descriptor_table, sizeof(struct ext2_group_desc), block_size + 1024);
	int group_num = 0;
	unsigned int total_blocks = getBlockNumber(fd);
	unsigned int total_inodes = getInodeNumber(fd);

	unsigned int free_blocks = group_descriptor_table[group_num].bg_free_blocks_count;
	unsigned int free_inodes = group_descriptor_table[group_num].bg_free_inodes_count;
	unsigned int block_bitmap = group_descriptor_table[group_num].bg_block_bitmap;
	unsigned int inode_bitmap = group_descriptor_table[group_num].bg_inode_bitmap;
	unsigned int free_inode = group_descriptor_table[group_num].bg_inode_table;

	printf("GROUP,%u,%u,%u,%u,%u,%u,%u,%u\n", group_num, total_blocks, total_inodes, free_blocks, free_inodes, block_bitmap, inode_bitmap, free_inode);
	
	return group_descriptor_table;
}
