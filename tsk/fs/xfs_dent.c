#include "tsk_fs_i.h"
#include "tsk_xfs.h"

//statit uint8_t
//xfs_dent_copyXFS_INFO * xfs,
//char ^ )

static struct xfs_dir2_data_entry *
xfs_dir3_data_entry_p(struct xfs_dir2_data_hdr *hdr)
{
    return (struct xfs_dir2_data_entry *)
        ((char *)hdr + sizeof(struct xfs_dir3_data_hdr));
}

static int
xfs_dir3_data_entsize(
    int         n)
{
    return XFS_DIR3_DATA_ENTSIZE(n);
}

static uint8_t
xfs_dir3_data_get_ftype(
    struct xfs_dir2_data_entry *dep)
{
    uint8_t     ftype = dep->name[dep->namelen];

    if (ftype >= XFS_DIR3_FT_MAX)
        return XFS_DIR3_FT_UNKNOWN;
    return ftype;
}

static uint8_t
xfs_dent_copy(XFS_INFO * xfs,
    char *xfs_dent, TSK_FS_NAME * fs_name)
{
    // 덴트 카피를 해야함.
    // 타입은 3개. 숏폼, 블럭, 리프
    // xfs_dent에서 넘어오는 데이터를 적절히 deserialization해서 넣어야 함
    // 적절히 어떻게?
    // 매직넘버?

    // TSK_FS_INFO *fs = &(xfs->fs_info);

    // ext2fs_dentry2 *dir = (ext2fs_dentry2 *) ext2_dent;

    // fs_name->meta_addr = tsk_getu32(fs->endian, dir->inode);

    // /* ext2 does not null terminate */
    // if (dir->name_len >= fs_name->name_size) {
    //     tsk_error_reset();
    //     tsk_error_set_errno(TSK_ERR_FS_ARG);
    //     tsk_error_set_errstr
    //         ("xfs_dent_copy: Name Space too Small %d %" PRIuSIZE "",
    //         dir->name_len, fs_name->name_size);
    //     return 1;
    // }

    // /* Copy and Null Terminate */
    // strncpy(fs_name->name, dir->name, dir->name_len);
    // fs_name->name[dir->name_len] = '\0';

    // switch (dir->type) {
    // case EXT2_DE_REG:
    //     fs_name->type = TSK_FS_NAME_TYPE_REG;
    //     break;
    // case EXT2_DE_DIR:
    //     fs_name->type = TSK_FS_NAME_TYPE_DIR;
    //     break;
    // case EXT2_DE_CHR:
    //     fs_name->type = TSK_FS_NAME_TYPE_CHR;
    //     break;
    // case EXT2_DE_BLK:
    //     fs_name->type = TSK_FS_NAME_TYPE_BLK;
    //     break;
    // case EXT2_DE_FIFO:
    //     fs_name->type = TSK_FS_NAME_TYPE_FIFO;
    //     break;
    // case EXT2_DE_SOCK:
    //     fs_name->type = TSK_FS_NAME_TYPE_SOCK;
    //     break;
    // case EXT2_DE_LNK:
    //     fs_name->type = TSK_FS_NAME_TYPE_LNK;
    //     break;
    // case EXT2_DE_UNKNOWN:
    // default:
    //     fs_name->type = TSK_FS_NAME_TYPE_UNDEF;
    //     break;
    // }

    // fs_name->flags = 0;

    return 0;
}

// static TSK_RETVAL_ENUM
// xfs_dent_parse_shortform(XFS_INFO * xfs, TSK_FS_DIR * a_fs_dir,
//     uint8_t a_is_del, TSK_LIST ** list_seen, char *buf, TSK_OFF_T offset)
// {
//     xfs_dir2_sf_hdr_t *hdr;

//     uint64_t bit64_count = 0;
//     uint32_t bit32_count = 0;
    
//     TSK_INUM_T parent;

//     char* filename;
//     uint16_t eoffset;
//     uint8_t namelen;
//     uint8_t filetype;
//     TSK_INUM_T inumdata;

//     hdr = (xfs_dir2_sf_hdr_t*)offset;
    
//     // sf header 파싱 -> count, parent 저장
//     parent = tsk_getu64(xfs->fs_info.endian, hdr->parent);

//     if (hdr->i8count > 0)
//         bit64_count = tsk_getu64(xfs->fs_info.endian, hdr->parent);
//     else
//         bit32_count = hdr->count;

//     offset += sizeof(xfs_dir2_sf_hdr_t); // +80

//     // 이제 sf_entry_t로 해석
//     for (int i = 0; i < (bit64_count > 0 ? bit64_count : bit32_count); i++)
//     {
//         xfs_dir2_sf_entry_t *entry = (xfs_dir2_sf_entry_t*)offset;

//         namelen = entry->namelen;
//         eoffset = tsk_getu16(xfs->fs_info.endian, entry->offset);
//         filename = entry->name;
//         filetype = xfs_dir3_sfe_get_ftype(entry);
//         inumdata = xfs_dir3_sfe_get_ino(hdr, entry);

//         tsk_fprintf(stderr, "[%lu] name: %s | type: %d | inode: %d | atoffset: 0x%lu\n",
//             filename, filetype, inumdata, offset);

//         offset += sizeof(xfs_dir2_sf_entry_t) 
//                     + namelen * sizeof(char)
//                     + sizeof(TSK_INUM_T);
//     }

//     return TSK_OK;
// }

// static TSK_RETVAL_ENUM
// xfs_dent_parse_btree(XFS_INFO * xfs, TSK_FS_DIR * a_fs_dir,
//     uint8_t a_is_del, TSK_LIST ** list_seen, char *buf, TSK_OFF_T offset)
// {
//     // while nextents, nblocks
//     //  di_bmx에서 n = offset, block, blockcount / 2n = leafoffset, block, blockcount
//     //  while true
//     //   if block[n.offset].magic == dir2_data_magic
//     //    data_free_t*n개 지나기 (data_unused_t의 length, offset++)
//     //    continue
//     //   offset => dir2_data_entry_t
//     //   break
//     //  while nextents, nblocks
//     //   dir2_data_entry_t 파싱 -> inumber, namelen, name, tag
//     //  ~~이런식
//     offset += 0; // 여기서 취해줄 수 있는게 없음
// }


//  * @param a_is_del Set to 1 if block is from a deleted directory
//  * a_fs_dir = 채워야 할 것, 나머지는 채워져 있는 것
//  * parse_block = 최종목표: a_fs_dir 채우기
//  * inode format = local -> shortform
//  *              = block -> block
//  *                      or leaf
 
// static TSK_RETVAL_ENUM
// xfs_dent_parse_block(XFS_INFO * xfs, TSK_FS_DIR * a_fs_dir,
//     uint8_t a_is_del, TSK_LIST ** list_seen, char *buf, TSK_OFF_T offset)
// {
//     // while valid at current offset is bmbt_rec
//     //  bmbt_rec[n] 파싱 -> offset, block, blockcount 저장
//     // offset 이동

//     // TSK_FS_INFO *fs = &(xfs->fs_info);
//     // TSK_FS_NAME *fs_name;

//     // xfs_dir2_data_hdr_t *hdr;
//     // xfs_dir2_data_entry_t *dep;
//     // xfs_dir2_data_unused_t *dup;
//     // xfs_dir2_block_tail_t *btp;

//     // char *ptr; // current data entry
//     // char *endptr; // end of data entry

//     // int wantoff;
//     // xfs_off_t cook;

//     // int error;

//     // // dir3
//     // //error = xfs_dir3_block_read();
//     // if (error)
//     //     return error;

//     // // extract the byte offset
//     // //wantoff = xfs_dir2_dataptr_to_off(xfs, );
//     // //hdr = bp->b_addr; // b_addr == 가상 버퍼
//     // // we can skip check whether it is corrupted, we can recover it

//     // // dir2
//     // btp = xfs_dir2_block_tail_p(xfs, hdr);
//     // ptr = (char *)xfs_dir3_data_entry_p(hdr);
//     // endptr = xfs_dir2_block_leaf_p(xfs, btp);

//     // while (ptr < endptr) {
//     //     uint8_t filetype;

//     //     dup = (xfs_dir2_data_unused_t *)ptr;

//     //     // to next iteration
//     //     ptr += xfs_dir3_data_entsize(dep->namelen);

//     //     // entry is before the desired starting point then skip it.
//     //     if ((char *)dep - (char *)hdr < wantoff)
//     //         continue;

//     //     cook = xfs_dir2_db_to_dataptr(xfs, (char*)dep - (char*)hdr); // calc entry pointer
//     //     filetype = xfs_dir3_data_get_ftype(dep);

//     //     // read name, namelen, inumber, filetype and copy to fs_dir
//     //     if (xfs_dent_copy(xfs, cook, fs_name)) {
//     //         tsk_fs_name_free(fs_name);
//     //         return TSK_ERR;
//     //     }

//     //     if (tsk_fs_dir_add(a_fs_dir, fs_name)) { // add filled fs_name to fs_dir
//     //         tsk_fs_name_free(fs_name);
//     //         return TSK_ERR;
//     //     }
//     // }

//     return TSK_OK;
// } deprecated

static TSK_RETVAL_ENUM
xfs_dent_parse(XFS_INFO * xfs, TSK_FS_DIR * a_fs_dir,
    uint8_t a_is_del, TSK_LIST ** list_seen, char *buf, TSK_OFF_T offset)
{
    // do parse about directory entries
    fprintf(stderr, "xfs_dent_parse: called\n");


    fprintf(stderr, "xfs_dent_parse: passed\n");

    return TSK_OK;
}

/** \internal
* Process a directory and load up FS_DIR with the entries. If a pointer to
* an already allocated FS_DIR structure is given, it will be cleared.  If no existing
* FS_DIR structure is passed (i.e. NULL), then a new one will be created. If the return
* value is error or corruption, then the FS_DIR structure could
* have entries (depending on when the error occurred).
*
* @param a_fs File system to analyze
* @param a_fs_dir Pointer to FS_DIR pointer. Can contain an already allocated
* structure or a new structure.
* @param a_addr Address of directory to process.
* @returns error, corruption, ok etc.
*/
TSK_RETVAL_ENUM 
xfs_dir_open_meta(TSK_FS_INFO * a_fs, TSK_FS_DIR ** a_fs_dir,
    TSK_INUM_T a_addr)
{
    // directory inode로부터 tsk_fs_dir -> tsk_fs_file 채우는거
    fprintf(stderr, "xfs_dir_open_meta: called\n");

    XFS_INFO * xfs = (XFS_INFO *) a_fs;
    TSK_FS_DIR * fs_dir;
    TSK_LIST *list_seen = NULL;
    TSK_OFF_T size;

    char *dirbuf;
    
    TSK_RETVAL_ENUM retval_tmp;
    TSK_RETVAL_ENUM retval_final = TSK_OK;

    if (a_addr < a_fs->first_inum || a_addr > a_fs->last_inum) {
        tsk_error_reset();
        tsk_error_set_errno(TSK_ERR_FS_WALK_RNG);
        tsk_error_set_errstr("xfs_dir_open_meta: inode value: %" PRIuINUM
            "\n", a_addr);
        return TSK_ERR;
    }
    else if (a_fs_dir == NULL) {
        tsk_error_reset();
        tsk_error_set_errno(TSK_ERR_FS_ARG);
        tsk_error_set_errstr
            ("xfs_dir_open_meta: NULL fs_attr argument given");
        return TSK_ERR;
    }

    if (tsk_verbose) {
        tsk_fprintf(stderr,
            "xfs_dir_open_meta: Processing directory %" PRIuINUM
            "\n", a_addr);
    }

    fs_dir = *a_fs_dir;

    if (fs_dir) {
        tsk_fs_dir_reset(fs_dir);
        fs_dir->addr = a_addr;
    }
    else {
        if((*a_fs_dir = fs_dir =
                tsk_fs_dir_alloc(a_fs, a_addr, 128)) == NULL) {
            return TSK_ERR;
        }
    }

    // handle the orphan directory if its contents were requested
    // if (a_addr == TSK_FS_ORPHANDIR_INUM(a_fs)) {
    //     return tsk_fs_dir_find_orphans(a_fs, fs_dir);
    // }

    if ((fs_dir->fs_file =
        tsk_fs_file_open_meta(a_fs, NULL, a_addr)) == NULL) { // inode_lookup -> content_ptr 채움
        fprintf(stderr, "xfs_fs_dir_open_meta: failed to obtain fs_file meta info\n");
        tsk_error_errstr2_concat("- xfs_dir_open_meta");
        return TSK_COR;
    }

    // We only read in and process a single block at a time
    if ((dirbuf = tsk_malloc((size_t)a_fs->block_size)) == NULL) {
        return TSK_ERR;
    }

    size = roundup(fs_dir->fs_file->meta->size, a_fs->block_size);
    TSK_OFF_T offset = 0;

    fprintf(stderr, "metasize: %d\n", fs_dir->fs_file->meta->size);

    while (size > 0) {
        ssize_t len = (a_fs->block_size < size) ? a_fs->block_size : size;
        ssize_t cnt = tsk_fs_file_read(fs_dir->fs_file, offset, dirbuf, len, (TSK_FS_FILE_READ_FLAG_ENUM)0);
        
        if (cnt != len) {
            tsk_error_reset();
            tsk_error_set_errno(TSK_ERR_FS_FWALK);
            tsk_error_set_errstr
            ("xfs_dir_open_meta: Error reading directory contents: %"
                PRIuINUM "\n", a_addr);
            free(dirbuf);
            return TSK_COR;
        }

        retval_tmp =
            xfs_dent_parse(xfs, fs_dir,
            (fs_dir->fs_file->meta->
                flags & TSK_FS_META_FLAG_UNALLOC) ? 1 : 0, &list_seen,
            dirbuf, len);

        if (retval_tmp == TSK_ERR) {
            retval_final = TSK_ERR;
            break;
        }
        else if (retval_tmp == TSK_COR) {
            retval_final = TSK_COR;
        }

        size -= len;
        offset += len;
    }

    free(dirbuf);

    fprintf(stderr, "xfs_dir_open_meta: passed\n");
    return TSK_OK;
}

uint8_t xfs_jentry_walk(TSK_FS_INFO *info, int a,
        TSK_FS_JENTRY_WALK_CB c, void *b)
{
    return -1;
}

uint8_t xfs_jblk_walk(TSK_FS_INFO *a, TSK_DADDR_T b,
        TSK_DADDR_T c, int d, TSK_FS_JBLK_WALK_CB e, void *f)
{
    return -1;
}

uint8_t xfs_jopen(TSK_FS_INFO *a, TSK_INUM_T b)
{
    return -1;
}