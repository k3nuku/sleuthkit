#ifndef _TSK_XFS_H
#define _TSK_XFS_H

#ifdef __cplusplus
extern "C" {
#endif

    typedef uint64_t XFS_AGNUM_T;
#define PRI_XFSAG PRIu64


/*
** Constants
*/
#define XFS_FIRSTINO    1    /* inode 1 contains the bad blocks */
#define XFS_ROOTINO     2    /* location of root directory inode */
#define XFS_SBOFF       0    // 슈퍼블록 오프셋
#define XFS_FS_MAGIC    0x58465342  // magic number
#define XFS_MAXNAMLEN    255    // 최대 파일이름길이
#define XFS_MAXPATHLEN   4096   // 최대 디렉토리 길이
#define XFS_MIN_BLOCK_SIZE   1024   // 최소 블록사이즈
#define XFS_MAX_BLOCK_SIZE   4096   // 최대 블록사이즈
#define XFS_FILE_CONTENT_LEN     ((XFS_NDADDR + XFS_NIADDR) * sizeof(TSK_DADDR_T)) // 파일컨텐츠 길이

/*
 * AG Header
 */
    typedef struct XFS_agheader {
        // agheader 채우기
    } XFS_agheader;

/*
** Super Block
*/
    typedef struct {
        // 슈퍼블록 기술
    } XFS_sb;

/*
** AG Free block info
*/
    typedef struct {
        //d
    } XFS_fbInfo;

/*
** AG B+ Tree Info
*/
    typedef struct {
        //d
    } XFS_bpTreeInfo;

/*
** AG Internal Free List
*/
    typedef struct {
        //d
    } XFS_intFreeList;

/*
 * Inodev2
 */
    typedef struct {

    } XFS_INODE_V2;

/*
 * Inodev3 :: 코드 최적화할때 다른부분만 union으로 합칠것
 */
    typedef struct {

    } XFS_INODE_V3;

/*
 * Inode core
 */
    typedef struct {

    } XFS_INODE_CORE;

/*
 * Inode Data Fork
 */
    typedef struct {

    } XFS_INODE_DATAFORK;

/*
 * Root of inode b+ tree (b+tree, num, count 병합)
 */
    typedef struct {
        
    } XFS_INODE_BPTREE;

/* MODE */
#define XFS_IN_FMT  0170000
#define XFS_IN_SOCK 0140000
#define XFS_IN_LNK  0120000
#define XFS_IN_REG  0100000
#define XFS_IN_BLK  0060000
#define XFS_IN_DIR  0040000
#define XFS_IN_CHR  0020000
#define XFS_IN_FIFO  0010000

#define XFS_IN_ISUID   0004000
#define XFS_IN_ISGID   0002000
#define XFS_IN_ISVTX   0001000
#define XFS_IN_IRUSR   0000400
#define XFS_IN_IWUSR   0000200
#define XFS_IN_IXUSR   0000100
#define XFS_IN_IRGRP   0000040
#define XFS_IN_IWGRP   0000020
#define XFS_IN_IXGRP   0000010
#define XFS_IN_IROTH   0000004
#define XFS_IN_IWOTH   0000002
#define XFS_IN_IXOTH   0000001


#define XFS_IN_SECDEL      0x00000001      /* Secure deletion */
#define XFS_IN_UNRM        0x00000002      /* Undelete */
#define XFS_IN_COMP        0x00000004      /* Compress file */
#define XFS_IN_SYNC        0x00000008      /* Synchronous updates */
#define XFS_IN_IMM         0x00000010      /* Immutable file */
#define XFS_IN_APPEND      0x00000020      /* writes to file may only append */
#define XFS_IN_NODUMP      0x00000040      /* do not dump file */
#define XFS_IN_NOA         0x00000080      /* do not update atime */
#define XFS_IN_DIRTY                   0x00000100
#define XFS_IN_COMPRBLK                0x00000200      /* One or more compressed clusters */
#define XFS_IN_NOCOMPR                 0x00000400      /* Don't compress */
#define XFS_IN_ECOMPR                  0x00000800      /* Compression error */
#define XFS_IN_INDEX                   0x00001000      /* hash-indexed directory */
#define XFS_IN_IMAGIC                  0x00002000      /* AFS directory */
#define XFS_IN_JOURNAL_DATA            0x00004000      /* file data should be journaled */
#define XFS_IN_NOTAIL                  0x00008000      /* file tail should not be merged */
#define XFS_IN_DIRSYNC                 0x00010000      /* dirsync behaviour (directories only) */
#define XFS_IN_TOPDIR                  0x00020000      /* Top of directory hierarchies */
#define XFS_IN_HUGE_FILE               0x00040000      /* Set to each huge file */
#define XFS_IN_EXTENTS                 0x00080000      /* Inode uses extents */
#define XFS_IN_EA_INODE                0x00200000      /* Inode used for large EA */
#define XFS_IN_EOFBLOCKS               0x00400000      /* Blocks allocated beyond EOF */
#define XFS_IN_RESERVED                0x80000000      /* reserved for XFS lib */
#define XFS_IN_USER_VISIBLE            0x004BDFFF      /* User visible flags */
#define XFS_IN_USER_MODIFIABLE         0x004B80FF      /* User modifiable flags */


/*
 * shortform directory entry
 */
    typedef struct {

    } XFS_sfentry;

/*
 * shortform attribute
 */
    typedef struct {

    } XFS_sfattr;


/* XFS directory file types  */
#define XFS_DE_UNKNOWN         0
#define XFS_DE_REG        1
#define XFS_DE_DIR             2
#define XFS_DE_CHR          3
#define XFS_DE_BLK          4
#define XFS_DE_FIFO            5
#define XFS_DE_SOCK            6
#define XFS_DE_LNK         7
#define XFS_DE_MAX             8



/************** JOURNAL ******************/

/* These values are always in big endian */

#define XFS_JMAGIC 0xC03b3998

/*JBD2 Feature Flags */
#define JBD2_FEATURE_COMPAT_CHECKSUM        0x00000001

#define JBD2_FEATURE_INCOMPAT_REVOKE        0x00000001
#define JBD2_FEATURE_INCOMPAT_64BIT         0x00000002
#define JBD2_FEATURE_INCOMPAT_ASYNC_COMMIT  0x00000004

    typedef struct {
        // 저널 슈퍼블록
    } XFS_journ_sb;


#define XFS_J_ETYPE_DESC   1       /* descriptor block */
#define XFS_J_ETYPE_COM    2       /* commit */
#define XFS_J_ETYPE_SB1    3       /* super block v1 */
#define XFS_J_ETYPE_SB2    4       /* sb v2 */
#define XFS_J_ETYPE_REV    5       /* revoke */


/* Header that is used for all structures */
    typedef struct {
        // 저널 헤더
    } XFS_journ_head;

/* JBD2 Checksum types */
#define JBD2_CRC32_CHKSUM   1
#define JBD2_MD5_CHKSUM     2
#define JBD2_SHA1_CHKSUM    3

#define JBD2_CRC32_CHKSUM_SIZE  4
#define JBD2_CHECKSUM_BYTES (32/ sizeof(unsigned int))

#define NSEC_PER_SEC 1000000000L

/* Header for XFS commit blocks */
    typedef struct {
        // 저널 커밋헤드
    } XFSfs_journ_commit_head;


/* dentry flags */
#define XFS_J_DENTRY_ESC   1       /* The orig block starts with magic */
#define XFS_J_DENTRY_SAMEID    2       /* Entry is for same id, so do not skip 16 ahead */
#define XFS_J_DENTRY_DEL   4       /* not currently used in src */
#define XFS_J_DENTRY_LAST  8       /* Last tag */

/* Entry in the descriptor table */
    typedef struct {
        // 저널 디스크립터
    } XFS_journ_dentry;


/* Journal Info */
    typedef struct {

        TSK_FS_FILE *fs_file;
        TSK_INUM_T j_inum;

        uint32_t bsize;
        TSK_DADDR_T first_block;
        TSK_DADDR_T last_block;

        uint32_t start_seq;
        TSK_DADDR_T start_blk;

    } XFS_JINFO;



    /*
     * Structure of an XFS file system handle.
     */
    typedef struct {
        TSK_FS_INFO fs_info;    /* super class */
        XFS_sb *fs;          /* super block */

        /* lock protects */
        tsk_lock_t lock;

        // one of the below will be allocated and populated by XFS_group_load depending on the FS type
        XFS_agheader *XFS_ag_buf; /* cached AG header for XFS r/w shared - lock */
        XFS_AGNUM_T ag_num;  /* cached AG number r/w shared - lock */

        uint8_t *bmap_buf;      /* cached block allocation bitmap r/w shared - lock */
        XFS_AGNUM_T bmap_grp_num;     /* cached block bitmap nr r/w shared - lock */

        uint8_t *imap_buf;      /* cached inode allocation bitmap r/w shared - lock */
        XFS_AGNUM_T imap_grp_num;     /* cached inode bitmap nr r/w shared - lock */

        TSK_OFF_T ags_offset;        /* offset to first group desc */
        XFS_AGNUM_T ags_count;     /* nr of descriptor group blocks */
        
        uint16_t inode_size;    /* size of each inode */
        TSK_DADDR_T first_data_block;

        XFS_JINFO *jinfo;
    } XFS_INFO;


    extern TSK_RETVAL_ENUM
        XFS_dir_open_meta(TSK_FS_INFO * a_fs, TSK_FS_DIR ** a_fs_dir,
        TSK_INUM_T a_addr);
    extern uint8_t XFS_jentry_walk(TSK_FS_INFO *, int,
        TSK_FS_JENTRY_WALK_CB, void *);
    extern uint8_t XFS_jblk_walk(TSK_FS_INFO *, TSK_DADDR_T,
        TSK_DADDR_T, int, TSK_FS_JBLK_WALK_CB, void *);
    extern uint8_t XFS_jopen(TSK_FS_INFO *, TSK_INUM_T);

#ifdef __cplusplus
}
#endif
#endif
