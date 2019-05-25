#include "DirectoryEntries.h"

using namespace std;

bool isValidDirectNode(struct ext2_inode * inode) {
    if (inode == nullptr) return false;
    //check whether is is empty
    if (inode->i_mode == 0 || inode->i_links_count == 0) return false;
    if (inode->i_size == 0) return false;
    //check inode type
    if ((inode->i_mode & EXT2_S_IFDIR) == EXT2_S_IFDIR) return true;
    return false;
}

void printOneEntry(struct ext2_dir_entry * one_entry, unsigned int parent_inode, unsigned int offset) {
    printf("DIRENT,%u,%u,%u,%u,%u,%s\n",
                        parent_inode, //2
                        offset, //3
                        one_entry->inode, //4
                        one_entry->rec_len, //5
                        one_entry->name_len, //6
                        one_entry->name);//7
    return;
}

void printDirectoryBlock(int fd, unsigned int parent_inode, unsigned int block_id, unsigned int initial_offset, int level) {
    unsigned int block_size = getBlockSize(fd);
    if (level == 0) {
        char block_content[block_size];
        pread(fd, block_content, block_size, block_id * block_size);
        char* byte_ptr = &block_content[0];
        struct ext2_dir_entry* dir_entry = (struct ext2_dir_entry*) byte_ptr;
        unsigned int local_offset = 0;
        // cout << "Direct Block: " << block_id << endl;
        while (dir_entry -> rec_len != 0) {
            if (dir_entry -> inode != 0) 
                printOneEntry(dir_entry, parent_inode, initial_offset + local_offset);
            local_offset += dir_entry -> rec_len;
            byte_ptr += dir_entry -> rec_len;
            dir_entry = (struct ext2_dir_entry*) byte_ptr;
        }
        return;
    }

    // indirect block
    int block_num = block_size/sizeof(unsigned int);
    unsigned int blocks[block_num];
    pread(fd, blocks, block_size, block_id * block_size);
    for (int i = 0; i < block_num; i++) {
        if (blocks[i] != 0)
            printDirectoryBlock(fd, parent_inode, blocks[i], initial_offset + i * pow(block_size, level - 1), level - 1);
    }
}

void printOneDirectory(struct ext2_inode * inode, unsigned int parent_inode, int fd) {
    // cout << "Inode" << endl;
    unsigned int block_size = getBlockSize(fd);
    for (unsigned int i = 0; i < EXT2_IND_BLOCK; i++) 
        printDirectoryBlock(fd, parent_inode, inode -> i_block[i], i * block_size, 0);

    printDirectoryBlock(fd, parent_inode, inode -> i_block[EXT2_IND_BLOCK], EXT2_IND_BLOCK * block_size, 1);
    printDirectoryBlock(fd, parent_inode, inode -> i_block[EXT2_DIND_BLOCK], EXT2_IND_BLOCK * block_size + (unsigned int) pow(block_size,2), 2);
    printDirectoryBlock(fd, parent_inode, inode -> i_block[EXT2_TIND_BLOCK], EXT2_IND_BLOCK * block_size + (unsigned int) pow(block_size,2) + (unsigned int) pow(block_size, 3), 3);

}


void getDirectoryEntries(int fd) {
    unsigned int inode_table_id = getInodeTableID(fd);
    unsigned int inode_num = getInodeNumber(fd);
    unsigned int block_size = getBlockSize(fd);
    // unsigned int data_block_id = getInodeTableID(fd);
    struct ext2_inode * inodes = new struct ext2_inode[inode_num];
    pread(fd, inodes, inode_num * sizeof(struct ext2_inode), SUPERBLOCK_OFFSET + block_size * (inode_table_id - 1));
    for (unsigned int i = 0; i < inode_num; i++) {
        if (isValidDirectNode(&inodes[i])) { //valid directory node
            // cout << "Start: " << i + 1 << endl;
            // cout << "Mode: " << inodes[i].i_mode << endl;
            printOneDirectory(&inodes[i], i + 1, fd);
            // cout << "Finished: " << i + 1 << endl;
        }
    }
    delete inodes;
    // printOneDirectory(&inodes[1], 2, fd);
}