#include "xfs.h"

/**
 * \internal
 * Open part of a disk image as a XFS file system.
 *
 * @param img_info Disk image to analyze
 * @param offset Byte offset where file system starts
 * @param ftype Specific type of file system
 * @param test NOT USED
 * @returns NULL on error or if data is not an XFS file system
 */
TSK_FS_INFO *
xfs_open(TSK_IMG_INFO * img_info, TSK_OFF_T offset,
    TSK_FS_TYPE_ENUM ftype, uint8_t test)
{
    XFS_INFO *xfs;

    return (fs);
}

/** \internal
 * Test if block group has a super block in it.
 *
 * @return 1 if block group has superblock, otherwise 0
*/
static uint32_t
xfs_is_super_bg(uint32_t feature_ro_compat, uint32_t group_block)
{
    return 0;
}


/** \internal
 * Add a single extent -- that is, a single data ran -- to the file data attribute.
 * @return 0 on success, 1 on error.
 */
static TSK_OFF_T
xfs_make_data_run_extent(TSK_FS_INFO * fs_info, TSK_FS_ATTR * fs_attr,
    xfs_extent * extent)
{
    TSK_FS_ATTR_RUN *data_run;
    data_run = tsk_fs_attr_run_alloc();
    if (data_run == NULL) {
        return 1;
    }

    // TODO: data run on xfs

    // save the run
    if (tsk_fs_attr_add_run(fs_info, fs_attr, data_run)) {
        return 1;
    }

    return 0;
}


/** \internal
 * Given a block that contains an extent node (which starts with extent_header),
 * walk it, and add everything encountered to the appropriate attributes.
 * @return 0 on success, 1 on error.
 */
static TSK_OFF_T
xfs_make_data_run_extent_index(TSK_FS_INFO * fs_info,
    TSK_FS_ATTR * fs_attr, TSK_FS_ATTR * fs_attr_extent,
    TSK_DADDR_T idx_block)
{
    return 0;
}

/** \internal
 * Get the number of extent blocks rooted at the given extent_header.
 * The count does not include the extent header passed as a parameter.
 *
 * @return the number of extent blocks, or -1 on error.
 */
static int32_t
xfs_extent_tree_index_count(TSK_FS_INFO * fs_info,
    TSK_FS_META * fs_meta, ext2fs_extent_header * header)
{
    int count = 0;

    return count;
}


/**
 * \internal
 * Loads attribute for XFS Extents-based storage method.
 * @param fs_file File system to analyze
 * @returns 0 on success, 1 otherwise
 */
static uint8_t
xfs_load_attrs_extents(TSK_FS_FILE *fs_file)
{
    return 0;
}

/** \internal
 * Add the data runs and extents to the file attributes.
 *
 * @param fs_file File system to analyze
 * @returns 0 on success, 1 otherwise
 */
static uint8_t
xfs_load_attrs(TSK_FS_FILE * fs_file)
{
    return -1;
}

//inode walk
uint8_t xfs_inode_walk(TSK_FS_INFO * fs, TSK_INUM_T start, TSK_INUM_T end,
    TSK_FS_META_FLAG_ENUM flags, TSK_FS_META_WALK_CB cb, void *ptr)
{
    return -1;
}

//block walk
uint8_t xfs_block_walk(TSK_FS_INFO * fs, TSK_DADDR_T start, TSK_DADDR_T end, 
    TSK_FS_BLOCK_WALK_FLAG_ENUM flags, TSK_FS_BLOCK_WALK_CB cb, void *ptr)
{
    return -1;
}

//block_getflags
TSK_FS_BLOCK_FLAG_ENUM xfs_block_getflags(TSK_FS_INFO * a_fs, TSK_DADDR_T a_addr)
{
    int flags = 0;

    return flags;
}

//load_attrs
uint8_t xfs_load_attrs(TSK_FS_FILE *)
{
    return -1;
}

//file_add_meta
uint8_t xfs_inode_lookup(TSK_FS_INFO * fs, TSK_FS_FILE * fs_file, TSK_INUM_T addr)
{
    return -1;
}

//dir_open_meta
TSK_RETVAL_ENUM xfs_dir_open_meta(TSK_FS_INFO * fs, TSK_FS_DIR ** a_fs_dir, TSK_INUM_T inode)
{
    return TSK_OK;
}

//fsstat
uint8_t xfs_fsstat(TSK_FS_INFO * fs, FILE * hFile)
{
    return -1;
}

//fscheck
uint8_t xfs_fscheck(TSK_FS_INFO *, FILE *)
{
    return -1;
}

//istat
uint8_t xfs_istat(TSK_FS_INFO * fs, TSK_FS_ISTAT_FLAG_ENUM flags, FILE * hFile, TSK_INUM_T inum,
            TSK_DADDR_T numblock, int32_t sec_skew)
{
    return -1;
}

//close
void xfs_close(TSK_FS_INFO * fs)
{
    return;
}

//jblk_walk
uint8_t xfs_jblk_walk(TSK_FS_INFO *, TSK_DADDR_T, TSK_DADDR_T, int, TSK_FS_JBLK_WALK_CB, void *)
{
    return -1;
}

//jentry_walk
uint8_t xfs_jentry_walk(TSK_FS_INFO *, int, TSK_FS_JENTRY_WALK_CB, void *)
{
    return -1;
}

//jopen
uint8_t xfs_jopen(TSK_FS_INFO *, TSK_INUM_T)
{
    return -1;
}