#include "GroupSummary.h"
unsigned int getBlockSize(int fd) {
	struct ext2_super_block* super_node = new struct ext2_super_block;
	pread(fd, super_node, sizeof(struct ext2_super_block), SUPERBLOCK_OFFSET);
	unsigned int block_size = EXT2_MIN_BLOCK_SIZE << (super_node -> s_log_block_size);
	return block_size;
}

void getGroupInfo(int fd) {
	unsigned int block_size = getBlockSize(fd);
	struct ext2_group_desc group_descriptor_table[block_size/sizeof(struct ext2_group_desc)];
	// struct ext2_group_desc* group_descriptor = malloc(sizeof(struct ext2_group_desc));
	pread(fd, group_descriptor_table, block_size, block_size + 1024);
	int group_num = 0;
	while (1) {
		if (group_descriptor_table[group_num].bg_block_bitmap == 0)
			break;

		unsigned int free_blocks = group_descriptor_table[group_num].bg_free_blocks_count;
		unsigned int free_inodes = group_descriptor_table[group_num].bg_free_inodes_count;
		unsigned int block_bitmap = group_descriptor_table[group_num].bg_block_bitmap;
		unsigned int inode_bitmap = group_descriptor_table[group_num].bg_inode_bitmap;
		printf("GROUP,%u,%u,%u,%u,%u\n", group_num, free_blocks, free_inodes, block_bitmap, inode_bitmap);
		group_num++;
		// group_descriptor += ;
	}
}