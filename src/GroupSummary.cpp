#include "GroupSummary.h"

int countIntegerOnes (unsigned int n) {
	int count = 0;
	while (n != 0) {
		count += n & 1;
		n >>= 1;
	}

	return count;
}

unsigned int getBlockSize(int fd) {
	struct ext2_super_block* super_node = new struct ext2_super_block;
	pread(fd, super_node, sizeof(struct ext2_super_block), SUPERBLOCK_OFFSET);
	unsigned int block_size = EXT2_MIN_BLOCK_SIZE << (super_node -> s_log_block_size);
	return block_size;
}

unsigned int getBlockNumber(int fd) {
	struct ext2_super_block* super_node = new struct ext2_super_block;
	pread(fd, super_node, sizeof(struct ext2_super_block), SUPERBLOCK_OFFSET);
	unsigned int total_blocks = super_node -> s_blocks_count;
	return total_blocks;
}

unsigned int getInodeNumber(int fd) {
	struct ext2_super_block* super_node = new struct ext2_super_block;
	pread(fd, super_node, sizeof(struct ext2_super_block), SUPERBLOCK_OFFSET);
	unsigned int total_inodes = super_node -> s_inodes_count;
	return total_inodes;
}

int countBlockOnes (unsigned char* bitmap, int size) {
	int i;
	int count = 0;

	for (i = 0; i < size; i++) {
		count += countIntegerOnes(bitmap[i]);
	}

	return count;
}

std::vector<std::vector<unsigned int>> getGroupInfo(int fd) {
	unsigned int block_size = getBlockSize(fd);
	struct ext2_group_desc group_descriptor_table[block_size/sizeof(struct ext2_group_desc)];
	// struct ext2_group_desc* group_descriptor = malloc(sizeof(struct ext2_group_desc));
	pread(fd, group_descriptor_table, block_size, block_size + 1024);
	int group_num = 0;

	std::vector<std::vector<unsigned int>> result = std::vector<std::vector<unsigned int>>();
	unsigned int total_blocks = getBlockNumber(fd);
	unsigned int total_inodes = getInodeNumber(fd);
	unsigned char block_bitmap_content[total_blocks];
	unsigned char inode_bitmap_content[total_inodes];

	/* Return the pair of these two vectors, which stores the block numbers of bitmaps*/
	std::vector<unsigned int> block_bitmap_locations = std::vector<unsigned int>();
	std::vector<unsigned int> inode_bitmap_locations = std::vector<unsigned int>();
	while (1) {
		if (group_descriptor_table[group_num].bg_block_bitmap == 0)
			break;

		unsigned int free_blocks = group_descriptor_table[group_num].bg_free_blocks_count;
		unsigned int free_inodes = group_descriptor_table[group_num].bg_free_inodes_count;
		unsigned int block_bitmap = group_descriptor_table[group_num].bg_block_bitmap;
		unsigned int inode_bitmap = group_descriptor_table[group_num].bg_inode_bitmap;

		/* Append the locations of block bitmap and inode bitmap to the vectors*/
		block_bitmap_locations.push_back(block_bitmap);
		inode_bitmap_locations.push_back(inode_bitmap);


		// compute number of blocks, inodes in group
		pread(fd, block_bitmap_content, total_blocks / 8, block_size * block_bitmap);
		pread(fd, inode_bitmap_content, total_inodes / 8, block_size * inode_bitmap);
		unsigned int occupied_blocks = countBlockOnes(block_bitmap_content, total_blocks / 8);
		unsigned int occupied_inodes = countBlockOnes(inode_bitmap_content, total_inodes / 8);
		unsigned int total_blocks = occupied_blocks + free_blocks;
		unsigned int total_inodes = occupied_inodes + free_inodes;

		unsigned int free_inode = group_descriptor_table[group_num].bg_inode_table;

		printf("GROUP,%u,%u,%u,%u,%u,%u,%u,%u\n", group_num, total_blocks, total_inodes, free_blocks, free_inodes, block_bitmap, inode_bitmap, free_inode);
		group_num++;
	}
	printf("Loop Finished\n");

	result.push_back(block_bitmap_locations);
	result.push_back(inode_bitmap_locations);
	return result;
	// return std::make_pair(block_bitmap_locations, inode_bitmap_locations);
}
