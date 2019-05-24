#include "InodeSummary.h"

using namespace std;
unsigned int getInodeTableID(int fd) {
    int block_size = getBlockSize(fd);
    struct ext2_group_desc * group_descriptor = new ext2_group_desc;
    pread(fd, group_descriptor, sizeof(ext2_group_desc), SUPERBLOCK_OFFSET + block_size);
    unsigned int bg_inode_table = group_descriptor->bg_inode_table;
    delete group_descriptor;
    return bg_inode_table;
}

unsigned long long fileSize(struct ext2_inode * inode) {
    unsigned long long f_size = inode->i_size;
    if ((inode->i_mode & EXT2_S_IFREG) == 0) { // regular file
        unsigned long long upper_bits = inode->i_file_acl;
        f_size = (upper_bits << 32) | f_size;
    }
    return f_size;
}

char * getTime(unsigned int time_secs) {
    time_t raw_time = time_secs;
    struct tm * ptm;
    ptm = gmtime(&raw_time);
    char * time_str = new char[64];
    strftime(time_str, 64, "%D %T", ptm);
    return time_str;
}

void printFreeInode(struct ext2_inode * inode, unsigned int inode_id) {
    if (inode == nullptr) return;
    if (inode->i_mode == 0 || inode->i_links_count == 0) return;
    char file_type = '?';
    unsigned int mode_mask = 0xF000;
    if ((inode->i_mode & mode_mask) == EXT2_S_IFDIR) file_type = 'd';
    else if ((inode->i_mode & mode_mask) == EXT2_S_IFREG) file_type = 'f';
    else if ((inode->i_mode & mode_mask) == EXT2_S_IFLNK) file_type = 's';
    __u16 mask = 0x0FFF; //set the higher order bits to 0
    __u16 mode_val = inode->i_mode & mask;
    unsigned long long f_size = fileSize(inode);
    printf("INODE,%u,%c,%o,%u,%u,%u,%s,%s,%s,%llu,%u", 
                    inode_id,       //inode number 2
                    file_type,      //file type 3
                    mode_val,       //mode 4
                    inode->i_uid,   // owner 5
                    inode->i_gid,   //group 6
                    inode->i_links_count, //links count 7
                    getTime(inode->i_ctime), //inode mod time 9
                    getTime(inode->i_mtime), //modification time 9
                    getTime(inode->i_atime), //last access time 10
                    f_size, // file size 11
                    inode->i_blocks); // access time 12

    if (f_size < 60) {
        cout << endl;
        return;
    }
    //print the indirect blocks
    for (int i = 0; i < EXT2_N_BLOCKS; i++) {
        printf(",%u", inode->i_block[i]);
    }
    cout << endl;
    return;
}


void getInodeSummary(int fd) {
    unsigned int inode_table_id = getInodeTableID(fd);
    // cout << "inode table id: " << inode_table_id << endl;
    unsigned int inode_num = getInodeNumber(fd);
    unsigned int block_size = getBlockSize(fd);
    // cout << "Inode number: " << inode_num << endl;
    struct ext2_inode * inodes = new struct ext2_inode[inode_num];
    pread(fd, inodes, inode_num * sizeof(struct ext2_inode), SUPERBLOCK_OFFSET + block_size * (inode_table_id - 1));
    for (unsigned int i = 0; i < inode_num; i++) {
        printFreeInode(inodes+i, i+1);
    }
    delete inodes;
}