#include "IndirectBlockSummary.h"

using namespace std;
void printIndirectBlock(int fd, unsigned int inode_number, unsigned int block_id, int level, int logical_block) {
	// this is a data block, no indirect pointer
	if (level == 0) return;

	unsigned int block_size = getBlockSize(fd);
	unsigned int block_num = block_size / sizeof(unsigned int);
	unsigned int blocks[block_num];
	pread(fd, blocks, block_size, block_id * block_size);
	unsigned int i;
	// cout << "Before Loop" << endl;
	for (i = 0; i < block_num; i++) {
		// this entry of the block is empty
		if (blocks[i] == 0)
			continue;

		printf("INDIRECT,%u,%d,%d,%u,%u\n",
			inode_number,
			level,
			logical_block + i * (int) pow(block_num, level - 1),
			block_id,
			blocks[i]);
		// cout << "i: " << i << endl;
		printIndirectBlock(fd, inode_number, blocks[i], level - 1, logical_block + i * pow(block_num, level - 1));
	}
}

void IndirectBlockSummary(int fd) {
	unsigned int inode_table_id = getInodeTableID(fd);
    // cout << "inode table id: " << inode_table_id << endl;
    unsigned int inode_num = getInodeNumber(fd);
    unsigned int block_size = getBlockSize(fd);
    unsigned int block_num = block_size / sizeof(unsigned int);
    // cout << "Inode number: " << inode_num << endl;
    struct ext2_inode * inodes = new struct ext2_inode[inode_num];
    pread(fd, inodes, inode_num * sizeof(struct ext2_inode), SUPERBLOCK_OFFSET + block_size * (inode_table_id - 1));
    
    for (unsigned int i = 0; i < inode_num; i++) {
    	struct ext2_inode* inode = inodes + i;

        if (inode == nullptr) 
        	continue;

    	if (inode->i_mode == 0 || inode->i_links_count == 0)
    		continue;

    	// cout << "i: " << i << endl;
    	// cout << "Single: " << inode -> i_block[EXT2_IND_BLOCK] << endl;
    	// cout << "Double: " << inode -> i_block[EXT2_DIND_BLOCK] << endl;
    	// cout << "Triple: " << inode -> i_block[EXT2_TIND_BLOCK] << endl;
    	if (inode -> i_block[EXT2_IND_BLOCK] != 0) {
    		printIndirectBlock(fd, i + 1, inode -> i_block[EXT2_IND_BLOCK], 1, 12);
    	}

    	if (inode -> i_block[EXT2_DIND_BLOCK] != 0) {
    		printIndirectBlock(fd, i + 1, inode -> i_block[EXT2_DIND_BLOCK], 2, 12 + block_num);
    	}

    	if (inode -> i_block[EXT2_TIND_BLOCK] != 0) {
    		printIndirectBlock(fd, i + 1, inode -> i_block[EXT2_TIND_BLOCK], 3, 12 + block_num + block_num * block_num);
    	}
    }

}