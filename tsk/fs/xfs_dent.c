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

/*
 * @param a_is_del Set to 1 if block is from a deleted directory
 * a_fs_dir = 채워야 할 것, 나머지는 채워져 있는 것
 */
static TSK_RETVAL_ENUM
xfs_dent_parse_block(XFS_INFO * xfs, TSK_FS_DIR * a_fs_dir,
    uint8_t a_is_del, TSK_LIST ** list_seen, char *buf, int len)
{
    TSK_FS_INFO *fs = &(xfs->fs_info);

    xfs_dir2_data_hdr_t *hdr;
    xfs_dir2_data_entry_t *dep;
    xfs_dir2_data_unused_t *dup;
    xfs_dir2_block_tail_t *btp;

    char *ptr; // current data entry
    char *endptr; // end of data entry

    int wantoff;
    xfs_off_t cook;

    int error;

    // dir3
    //error = xfs_dir3_block_read();

    // extract the byte offset
    //wantoff = xfs_dir2_dataptr_to_off(xfs, );
    //hdr = bp->b_addr; // b_addr == 가상 버퍼
    // we can skip check whether it is corrupted, we can recover it

    // dir2
    btp = xfs_dir2_block_tail_p(xfs, hdr);
    ptr = (char *)xfs_dir3_data_entry_p(hdr);
    endptr = xfs_dir2_block_leaf_p(xfs, btp);

    while (ptr < endptr) {
        uint8_t filetype;

        dup = (xfs_dir2_data_unused_t *)ptr;

        // to next iteration
        ptr += xfs_dir3_data_entsize(dep->namelen);

        // entry is before the desired starting point then skip it.
        if ((char *)dep - (char *)hdr < wantoff)
            continue;


    }

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
    fprintf(stderr, "xfs_dir_open_meta: called\n");

    XFS_INFO * xfs = (XFS_INFO *) a_fs;
    TSK_FS_DIR * fs_dir;
    TSK_LIST *list_seen = NULL;
    TSK_OFF_T size;

    char * dirbuf;
    
    /* If we get corruption in one of the blocks, then continue processing.
     * retval_final will change when corruption is detected.  Errors are
     * returned immediately. */
    TSK_RETVAL_ENUM retval_tmp;
    TSK_RETVAL_ENUM retval_final = TSK_OK;

    /* sanity check */
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
    if (a_addr == TSK_FS_ORPHANDIR_INUM(a_fs)) {
        return tsk_fs_dir_find_orphans(a_fs, fs_dir);
    }

    /* Get the inode and verify it has attributes */
    if ((fs_dir->fs_file =
        tsk_fs_file_open_meta(a_fs, NULL, a_addr)) == NULL) { // add_meta 함수 구현해줘야 함
        tsk_error_errstr2_concat("- xfs_dir_open_meta");
        return TSK_COR;
    }

    // We only read in and process a single block at a time
    if ((dirbuf = tsk_malloc((size_t)a_fs->block_size)) == NULL) {
        return TSK_ERR;
    }

    size = roundup(fs_dir->fs_file->meta->size, a_fs->block_size);
    TSK_OFF_T offset = 0;

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
            xfs_dent_parse_block(xfs, fs_dir,
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

    if (a_addr == a_fs->root_inum) {
        TSK_FS_NAME *fs_name = tsk_fs_name_alloc(256, 0);

        if (fs_name == NULL)
            return TSK_ERR;

        if (tsk_fs_dir_make_orphan_dir_name(a_fs, fs_name)) {
            tsk_fs_name_free(fs_name);
            return TSK_ERR;
        }

        if (tsk_fs_dir_add(fs_dir, fs_name)) {
            tsk_fs_name_free(fs_name);
            return TSK_ERR;
        }
    }

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