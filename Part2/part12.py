import csv
import sys

if sys.argv[1][0] == '-':
    sys.exit(1)

with open(sys.argv[1], 'r') as csvfile:
    reader = csv.reader(csvfile, delimiter=',')
    csv_list = [row for row in reader]


supernode = []
group_info = []
bfree_list = []
ifree_list = []
inode_list = []
dirent_list = []
indirect_list = []

for row in csv_list:
    if row[0] == 'SUPERBLOCK':
        supernode = row
    elif row[0] == 'GROUP':
        group_info = row
    elif row[0] == 'BFREE':
        bfree_list.append(row)
    elif row[0] == 'IFREE':
        ifree_list.append(row)
    elif row[0] == 'INODE':
        inode_list.append(row)
    elif row[0] == 'DIRENT':
        dirent_list.append(row)
    elif row[0] == 'INDIRECT':
        indirect_list.append(row)
        
block_size = int(supernode[3])
block_num = int(block_size / 4)
# calculate min_block (the first data block)
number_of_inodes = int(supernode[2])
min_block = int(group_info[-1]) + int(number_of_inodes*128 / block_size)
if (number_of_inodes * 128) % block_size != 0:
    min_block += 1
max_block = int(supernode[1]) - 1
offset_mapping = [i for i in range(13)] + [12 + block_num] + [12 + block_num + block_num ** 2]
level_mapping = {0: "", 1: "INDIRECT", 2: "DOUBLE INDIRECT", 3: "TRIPLE INDIRECT"}


freelist_blocks = set() # blocks on free list
allocated_blocks = set()
unallocated_blocks = set()

#part 1
# Examine unreferenced and allocated blocks
# free blocks according to BFREE
for bfree in bfree_list:
    freelist_blocks.add(int(bfree[1]))

# truly allocated blocks
for inode in inode_list:
    for i in range(12, len(inode)):
        allocated_blocks.add(int(inode[i]))
for indirect in indirect_list:
    allocated_blocks.add(int(indirect[-1]))

#truly free blocks
for i in range(min_block, max_block + 1):
    if i not in allocated_blocks:
        unallocated_blocks.add(i)


#print unreferenced blocks (free blocks which are not in freelist)
for i in unallocated_blocks - freelist_blocks:
    if i >= min_block:
        print("UNREFERENCED BLOCK %d"% (i))

#print allocated blocks on freelist
for i in allocated_blocks & freelist_blocks:
    print("ALLOCATED BLOCK %d ON FREELIST"% (i))

# invalid blocks and duplicate blocks
# mapping: block number -> (level, inode_num, offset)
block_ptr_info = dict()
# info from INODE
for inode in inode_list:
    inode_num = int(inode[1])
    # skip symbolic link smaller than 60 bytes
    if inode[2] == 's' and int(inode[10]) < 60:
        continue
    if inode[2] == '?':
        continue

    for i in range(12, len(inode)):
        block_id = int(inode[i])
        # skip empty pointer entry
        if block_id == 0:
            continue
        # level: 0-11(i = 12 - 23) -> 0; 12 (i = 24) -> 1; 13 (i = 25) -> 2; 14 (i = 26) -> 3
        level = max(i - 23, 0)
        if block_id not in block_ptr_info:
            block_ptr_info[block_id] = []
        block_ptr_info[block_id].append((level, inode_num, offset_mapping[i-12]))

# info from INDIRECT
for ind in indirect_list:
    inode_num = int(ind[1])
    level = int(ind[2])
    offset = int(ind[3])
    block_id = int(ind[-1])
    if block_id not in block_ptr_info:
        block_ptr_info[block_id] = []
    block_ptr_info[block_id].append((level, inode_num, offset))

# for block_id in block_ptr_info:
#     print(block_id)
#     print(block_ptr_info[block_id])

# find invalid/reserved ones
for block_id in block_ptr_info:
    if block_id < min_block:
        for level, inode_num, offset in block_ptr_info[block_id]:
            if level == 0:
                print("RESERVED BLOCK %d IN INODE %d AT OFFSET %d"% (block_id, inode_num, offset))
                continue
            print("RESERVED %s BLOCK %d IN INODE %d AT OFFSET %d"% (level_mapping[level], block_id, inode_num, offset))


    if block_id > max_block:
        for level, inode_num, offset in block_ptr_info[block_id]:
            if level == 0:
                print("INVALID BLOCK %d IN INODE %d AT OFFSET %d"% (block_id, inode_num, offset))
                continue
            print("INVALID %s BLOCK %d IN INODE %d AT OFFSET %d"% (level_mapping[level], block_id, inode_num, offset))

# find duplicates
for block_id in block_ptr_info:
    if len(block_ptr_info[block_id]) > 1:
        for level, inode_num, offset in block_ptr_info[block_id]:
            if level == 0:
                print("DUPLICATE BLOCK %d IN INODE %d AT OFFSET %d"% (block_id, inode_num, offset))
                continue
            print("DUPLICATE %s BLOCK %d IN INODE %d AT OFFSET %d"% (level_mapping[level], block_id, inode_num, offset))


# part 2
freelist_inodes = set()
allocated_inodes = set()
unallocated_inodes = set()
min_inode = int(supernode[-1])
max_inode = int(supernode[2])

# inodes on freelist
for inode in ifree_list:
    freelist_inodes.add(int(inode[1]))

# truly allocated inodes
for inode in inode_list:
    allocated_inodes.add(int(inode[1]))

# truly free inodes
for i in range(1, max_inode):
    if i not in allocated_inodes:
        unallocated_inodes.add(i)

# allocated but on freelist
for i in freelist_inodes & allocated_inodes:
    print("ALLOCATED INODE %d ON FREELIST" % (i))

# unallocated but not on freelist
for i in unallocated_inodes - freelist_inodes:
    # only print info about unreserved inodes
    if i >= min_inode:
        print("UNALLOCATED INODE %d NOT ON FREELIST" % (i))


