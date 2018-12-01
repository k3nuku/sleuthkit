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
    fprintf(stderr, ">> xfs_dent_copy called.\n");
    
    TSK_FS_INFO *fs = &(xfs->fs_info);
    // if format 1 (short form)
    
    xfs_dir2_sf_t * dir2_sf = (xfs_dir2_sf_t*) xfs_dent;
    xfs_dir2_sf_hdr_t * hdr = (xfs_dir2_sf_hdr_t*) dir2_sf->hdr;
    xfs_dir2_sf_entry_t * ent = (xfs_dir2_sf_entry_t*) dir2_sf->entry;

    fs_name->meta_addr = (TSK_INUM_T)xfs_dir3_sfe_get_ino(hdr, ent);
    
    if(ent->namelen >= fs_name->name_size){
        tsk_error_reset();
        tsk_error_set_errno(TSK_ERR_FS_ARG);
        tsk_error_set_errstr
            ("xfs_dent_copy: Name Space too Small %d %" PRIuSIZE "",
           ent->namelen, fs_name->name_size);
        return 1;
    }
    
    strncpy(fs_name->name, ent->name, ent->namelen);
    fs_name->name[ent->namelen] = '\0';

    fs_name->type = TSK_FS_NAME_TYPE_UNDEF;

    switch (xfs_dir3_sfe_get_ftype(ent)) {
        case XFS_DE_REG:
            fs_name->type = TSK_FS_NAME_TYPE_REG;
            break;
        case XFS_DE_DIR:
            fs_name->type = TSK_FS_NAME_TYPE_DIR;
            break;
        case XFS_DE_CHR:
            fs_name->type = TSK_FS_NAME_TYPE_CHR;
            break;
        case XFS_DE_BLK:
            fs_name->type = TSK_FS_NAME_TYPE_BLK;
            break;
        case XFS_DE_FIFO:
            fs_name->type = TSK_FS_NAME_TYPE_FIFO;
            break;
        case XFS_DE_SOCK:
            fs_name->type = TSK_FS_NAME_TYPE_SOCK;
            break;
        case XFS_DE_LNK:
            fs_name->type = TSK_FS_NAME_TYPE_LNK;
            break;
        case XFS_DE_UNKNOWN:
        default:
            fs_name->type = TSK_FS_NAME_TYPE_UNDEF;
            break;
    }
    fs_name->flags = 0;

    fprintf(stderr, ">> xfs_dent_copy passed.\n");
    return 0;
}

static TSK_RETVAL_ENUM
xfs_dent_parse_shortform(XFS_INFO * xfs, TSK_FS_DIR * a_fs_dir,
    uint8_t a_is_del, TSK_LIST ** list_seen, char *buf, TSK_OFF_T offset)
{
    TSK_FS_INFO *fs = &(xfs->fs_info);
    
    TSK_FS_NAME * fs_name;
    xfs_dir2_sf_hdr_t *hdr;
    xfs_dir2_sf_entry_t *ent; 
    
    // dir2_sf = dir2_sf header + dir2_sf entry => to calculate inode offset in dent_copy 
    xfs_dir2_sf_t * dir2_sf = (xfs_dir2_sf_t *)tsk_malloc(sizeof(xfs_dir2_sf_t));
    hdr = (xfs_dir2_sf_hdr_t*)buf;
    dir2_sf->hdr = hdr;   
    
    uint8_t ftype;
    uint8_t namelen;
    uint64_t inode;
    
    if ((fs_name = tsk_fs_name_alloc(XFS_MAXNAMELEN + 1, 0)) == NULL)
        return TSK_ERR;

    ent = (char*)(hdr + 1) - (hdr->i8count==0) * 4; // code of miracle
    
    uint16_t num_entries = (hdr->i8count > 0) ? hdr->i8count : hdr->count;

    for (int i = 0; i < num_entries; i++)
    {
        dir2_sf->entry = ent;
        namelen = ent->namelen;
        inode = xfs_dir2_sf_get_ino(hdr, ent);
        if(inode > fs->last_inum ||
            namelen > XFS_MAXNAMELEN ||
            namelen == 0){
            fprintf(stderr, ">>>xfs_dent.c%d -> inode : %lx  namelen : %d  | last inum : %d\n", __LINE__, inode, namelen, fs->last_inum);
            //fprintf(stderr, "xfs_dent.c:%d ->xfs_dent_parse_shortform: Invalid inode.\n",__LINE__);
        }

        if (xfs_dent_copy(xfs, dir2_sf, fs_name)) {
            tsk_fs_name_free(fs_name);
            return TSK_ERR;
        }

        fs_name->flags = TSK_FS_NAME_FLAG_ALLOC;
        /* Do we have a deleted entry? */
        
        // if ((dellen > 0) || (inode == 0) || (a_is_del)) {
        //     fs_name->flags = TSK_FS_NAME_FLAG_UNALLOC;
        //     if (dellen > 0)
        //         dellen -= minreclen;
        // }
        
        if (tsk_fs_dir_add(a_fs_dir, fs_name)) {
            tsk_fs_name_free(fs_name);
            return TSK_ERR;
        }

        // tsk_fprintf(stderr, "[%lu] name: %s | type: %d | inode: %d | atoffset: 0x%lu\n",
        //     filename, filetype, inumdata, offset);

        // offset += sizeof(xfs_dir2_sf_entry_t) 
        //             + namelen * sizeof(char)
        //             + sizeof(TSK_INUM_T);
    }
    tsk_fs_name_free(fs_name);
    return TSK_OK;
}

static TSK_RETVAL_ENUM
xfs_dent_parse_btree(XFS_INFO * xfs, TSK_FS_DIR * a_fs_dir,
    uint8_t a_is_del, TSK_LIST ** list_seen, char *buf, TSK_OFF_T offset)
{
    // while nextents, nblocks
    //  di_bmx에서 n = offset, block, blockcount / 2n = leafoffset, block, blockcount
    //  while true
    //   if block[n.offset].magic == dir2_data_magic
    //    data_free_t*n개 지나기 (data_unused_t의 length, offset++)
    //    continue
    //   offset => dir22_data_entry_t
    //   break
    //  while nextents, nblocks
    //   dir2_data_entry_t 파싱 -> inumber, namelen, name, tag
    //  ~~이런식
    offset += 0; // 여기서 취해줄 수 있는게 없음
}

/*
 * @param a_is_del Set to 1 if block is from a deleted directory
 * a_fs_dir = 채워야 할 것, 나머지는 채워져 있는 것
 * parse_block = 최종목표: a_fs_dir 채우기
 * inode format = local -> shortform
 *              = block -> block
 *                      or leaf
 */
static TSK_RETVAL_ENUM
xfs_dent_parse_block(XFS_INFO * xfs, TSK_FS_DIR * a_fs_dir,
    uint8_t a_is_del, TSK_LIST ** list_seen, char *buf, TSK_OFF_T offset)
{
    // while valid at current offset is bmbt_rec
    //  bmbt_rec[n] 파싱 -> offset, block, blockcount 저장
    // offset 이동

    TSK_FS_INFO *fs = &(xfs->fs_info);
    // TSK_FS_NAME *fs_name;

    // xfs_dir2_data_hdr_t *hdr;
    // xfs_dir2_data_entry_t *dep;
    // xfs_dir2_data_unused_t *dup;
    // xfs_dir2_block_tail_t *btp;

    // char *ptr; // current data entry
    // char *endptr; // end of data entry

    // int wantoff;
    // xfs_off_t cook;

    // int error;

    // // dir3
    // //error = xfs_dir3_block_read();
    // if (error)
    //     return error;

    // // extract the byte offset
    // //wantoff = xfs_dir2_dataptr_to_off(xfs, );
    // //hdr = bp->b_addr; // b_addr == 가상 버퍼
    // // we can skip check whether it is corrupted, we can recover it

    // // dir2
    // btp = xfs_dir2_block_tail_p(xfs, hdr);
    // ptr = (char *)xfs_dir3_data_entry_p(hdr);
    // endptr = xfs_dir2_block_leaf_p(xfs, btp);

    // while (ptr < endptr) {
    //     uint8_t filetype;

    //     dup = (xfs_dir2_data_unused_t *)ptr;

    //     // to next iteration
    //     ptr += xfs_dir3_data_entsize(dep->namelen);

    //     // entry is before the desired starting point then skip it.
    //     if ((char *)dep - (char *)hdr < wantoff)
    //         continue;

    //     cook = xfs_dir2_db_to_dataptr(xfs, (char*)dep - (char*)hdr); // calc entry pointer
    //     filetype = xfs_dir3_data_get_ftype(dep);

    //     // read name, namelen, inumber, filetype and copy to fs_dir
    //     if (xfs_dent_copy(xfs, cook, fs_name)) {
    //         tsk_fs_name_free(fs_name);
    //         return TSK_ERR;
    //     }

    //     if (tsk_fs_dir_add(a_fs_dir, fs_name)) { // add filled fs_name to fs_dir
    //         tsk_fs_name_free(fs_name);
    //         return TSK_ERR;
    //     }
    // }

    return TSK_OK;
}

static TSK_RETVAL_ENUM
xfs_dent_parse(XFS_INFO * xfs, TSK_FS_DIR * a_fs_dir,
    uint8_t a_is_del, TSK_LIST ** list_seen, char *buf, TSK_OFF_T offset)
{
    TSK_FS_INFO* fs_info = (TSK_FS_INFO*) xfs;
    
    switch(a_fs_dir->fs_file->meta->content_type){
        case TSK_FS_META_CONTENT_TYPE_XFS_DATA_FORK_SHORTFORM:
            xfs_dent_parse_shortform(xfs, a_fs_dir, a_is_del, list_seen, buf, offset);
            break;

        case TSK_FS_META_CONTENT_TYPE_XFS_DATA_FORK_BLOCK:
            break;
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
        ssize_t len;
        ssize_t cnt;
        if(fs_dir->fs_file->meta->content_type == TSK_FS_META_CONTENT_TYPE_XFS_DATA_FORK_SHORTFORM){
            /// if short form - case of only diretory
            cnt = len = XFS_CONTENT_LEN_V5(xfs);
            memcpy(dirbuf, fs_dir->fs_file->meta->content_ptr, XFS_CONTENT_LEN_V5(xfs));
            //if(cnt != len){
            //     fprintf(stderr, "xfs_dent.c:%d  invalid datafork read size : cnt : %d   con_len : %d\n", __LINE__);            }
        }
        else{
            len = (a_fs->block_size < size) ? a_fs->block_size : size;
            cnt = tsk_fs_file_read(fs_dir->fs_file, offset, dirbuf, len, (TSK_FS_FILE_READ_FLAG_ENUM)0);
        }
                
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