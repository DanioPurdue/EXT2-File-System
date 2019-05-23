#include "FreeInodeEntries.h"

using namespace std;

unsigned int getInodeBitmapID(int fd) {
    int block_size = getBlockSize(fd);
    struct ext2_group_desc * group_descriptor = new ext2_group_desc;
    pread(fd, group_descriptor, sizeof(group_descriptor), SUPERBLOCK_OFFSET + block_size);
    unsigned int bg_inode_bitmap = group_descriptor->bg_inode_bitmap;
    delete group_descriptor;
    return bg_inode_bitmap;
}

void freeInodeEntries(int fd) {
    unsigned int inode_bitmap_id = getInodeBitmapID(fd);
    // cout << "inode_bitmap_id: " << inode_bitmap_id << endl;
    unsigned int block_size = getBlockSize(fd);
    unsigned int inode_num = getInodeNumber(fd);
    unsigned int bytes_required = (inode_num % 8 == 0) ? (inode_num / 8) : (inode_num / 8 + 1);
    __u8 * inode_bitmap = new __u8[bytes_required];
    pread(fd, inode_bitmap, bytes_required, SUPERBLOCK_OFFSET + block_size * (inode_bitmap_id - 1));
    for (unsigned int i = 0; i < bytes_required; i++) {
        bitAccess(inode_bitmap[i], i, inode_num, "IFREE");
    }
    return;
}
