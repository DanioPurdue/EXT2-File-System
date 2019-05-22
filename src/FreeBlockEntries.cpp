#include "FreeBlockEntries.h"

using namespace std;

unsigned int getBlockBitmapID(int fd) {
    int block_size = getBlockSize(fd);
    struct ext2_group_desc * group_descriptor = new ext2_group_desc;
    pread(fd, group_descriptor, sizeof(group_descriptor), 1024 + block_size);
    return group_descriptor->bg_block_bitmap;
}

void bitAccess(__u8 val, unsigned int offset, unsigned int block_num, string tag_name) {
    __u8 mask = 1;
    for (int i = 0; i < 8; i++) {
        if (offset * 8 + i == block_num) return;
        if ((mask & val) == 0) {
            printf("%s, %u\n", tag_name.c_str(), i + offset * 8 + 1);
        }
        mask = mask << 1;
    }
}

void freeBlockEntries(int fd) {
    unsigned int block_bitmap_id = getBlockBitmapID(fd);
    unsigned int block_size = getBlockSize(fd);
    unsigned int block_num = getBlockNumber(fd);
    unsigned int bytes_required = (block_num % 8 == 0) ? (block_num / 8) : (block_num / 8 + 1);
    __u8 * block_bitmap = new __u8[bytes_required];
    pread(fd, block_bitmap, bytes_required, SUPERBLOCK_OFFSET + block_size * (block_bitmap_id-1));
    for (unsigned int i = 0; i < bytes_required; i++) {
        bitAccess(block_bitmap[i], i, block_num, "BFREE");
    }
    return;
}