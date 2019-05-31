import csv
import sys

def rawListParser(data_list):
    inode_list = [entry for entry in data_list if entry[0] == 'INODE']
    direc_list = [entry for entry in data_list if entry[0] == 'DIRENT']
    group_summary = [entry for entry in data_list if entry[0] == 'GROUP']
    return (inode_list, direc_list, group_summary[0])

def linkCountValidation(inode_list, direc_list):
    for one_inode in inode_list:
        inode_num = int(one_inode[1])
        link_cnt = int(one_inode[6])
        act_cnt = findLinkRef(inode_num, direc_list)
        if link_cnt != act_cnt:
            print "INODE", inode_num, "HAS", act_cnt, "LINKS BUT LINKCOUNT IS", link_cnt

def findLinkRef(inode_num, direct_list):
    act_cnt = 0
    for one_entry in direc_list:
        if int(one_entry[3]) == int(inode_num):
            act_cnt += 1
    return act_cnt
    
def returnDataList(filename):
    data_list = list()
    with open(filename) as csv_f:
        f = csv.reader(csv_f, delimiter=',')
        for one_row in f:
            data_list.append(one_row)
    return data_list

def foundParentInodeNum(inode_num, direct_list):
    for direc_entry in direc_list:
        entry_inode_num = int(direc_entry[3])
        direc_inode_num = int(direc_entry[1])
        if direc_inode_num == entry_inode_num:
            return direc_inode_num
    return None

def directoryReferenceValidation(inode_list, direc_list, total_inodes):
    for direc_entry in direc_list:
        direc_inode_num = int(direc_entry[1])
        entry_inode_num = int(direc_entry[3])
        entry_name = direc_entry[6]

        #check whether the inode is allocated or not
        found_inode = False
        inode_info = None
        for one_inode in inode_list:
            inode_num = int(one_inode[1])
            if inode_num == entry_inode_num:
                # found the allocated block
                found_inode = True
                inode_info = one_inode
                break
        if entry_inode_num < 1 or entry_inode_num > total_inodes:
            print "DIRECTORY INODE", direc_inode_num, "NAME", entry_name, "INVALID INODE", entry_inode_num
        elif found_inode == False:
            print "DIRECTORY INODE", direc_inode_num, "NAME", entry_name, "UNALLOCATED INODE", entry_inode_num
        
        if entry_name == "'.'" and entry_inode_num != direc_inode_num:
            print "DIRECTORY INODE", direc_inode_num, "NAME", entry_name, "LINK TO INODE", inode_num, "SHOULD BE", direc_inode_num
        if entry_name == "'..'":
            parent_direc_inode_num = foundParentInodeNum(direc_inode_num, direc_list)
            if parent_direc_inode_num != entry_inode_num:
                print "DIRECTORY INODE", direc_inode_num, "NAME", entry_name, "LINK TO INODE", inode_num, "SHOULD BE", direc_inode_num
    return
#linke count: 7
if len(sys.argv) < 2:
    print "Usage: ./DCP.py filename"
    exit(1)
filename = sys.argv[1]
data_list = returnDataList(filename)
inode_list, direc_list, group_summary = rawListParser(data_list)
total_inodes = int(group_summary[3])
linkCountValidation(inode_list, direc_list)
directoryReferenceValidation(inode_list, direc_list, total_inodes)
