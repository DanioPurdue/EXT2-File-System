#include "DirectoryEntries.cpp"

bool isValidDirectNode(struct ext2_inode * inode) {
    if (inode == nullptr) return;
    //check whether is is empty
    if (inode->i_mode == 0 || inode->i_links_count == 0) return false;
    if (inode->i_size == 0) return false;
    unsigned int mode_mask = 0xF000;
    //check inode type
    if ((inode->i_mode & mode_mask) == EXT2_S_IFDIR) return false;
    return true;
}

void printOneEntry(struct ext2_dir_entry * one_entry, unsigned int offset) {
    printf("DIRENT,%u,%u,%u,%u,%u,%s",
                        parent_inode_num, //2
                        offset, //3
                        one_entry->inode, //4
                        one_entry->rec_len, //5
                        one_entry->name_len, //6
                        one_entry->name);//7
    return;
}

void printOneDirectory(struct ext2_inode * inode, int fd, unsigned int block_size) {
    unsigned int file_size = fileSize(inode);
    char * data_block = new char[block_size * (file_size / 512 + 1)];
    unsigned int data_offset = inode->i_block[i];
    pread(fd, data_block, block_size, SUPERBLOCK_OFFSET + block_size * (data_offset-1));
    struct ext2_dir_entry * cur = data_block;
    int block_cnt = 1;
    unsigned int read_size = 0;
    while (read_size < file_size) {
        if (inode != 0) {
            printOneEntry(cur, read_size);  
        }
        cur = (void * ) cur + cur->rec_len;
        read_size += cur->rec_len;
        if (read_size < )
    }
}

print

void getDirectoryEntries(int fd) {
    unsigned int inode_table_id = getInodeTableID(fd);
    unsigned int inode_num = getInodeNumber(fd);
    unsigned int block_size = getBlockSize(fd);
    unsigned int data_block_id = getInodeTableID(fd);
    struct ext2_inode * inodes = new struct ext2_inode[inode_num];
    pread(fd, inodes, inode_num * sizeof(struct ext2_inode), SUPERBLOCK_OFFSET + block_size * (inode_table_id - 1));
    for (unsigned int i = 0; i < inode_num; i++) {
        if (isValidDirectNode(inodes[i])) { //valid directory node
        }
    }
}