/**
 * @file lv_fs_posix.c
 *
 */


/*********************
 *      INCLUDES
 *********************/
#include "lv_fs_if.h"
#if LV_USE_FS_IF
#if LV_FS_IF_POSIX != '\0'

#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#ifdef WIN32
#include <windows.h>
#endif

/*********************
 *      DEFINES
 *********************/
#ifndef LV_FS_POSIX_PATH
# ifndef WIN32
#  define LV_FS_POSIX_PATH "./" /*Project root*/
# else
#  define LV_FS_POSIX_PATH ".\\" /*Project root*/
# endif
#endif /*LV_FS_PATH*/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void * fs_open (lv_fs_drv_t * drv, const char * path, lv_fs_mode_t mode);
static lv_fs_res_t fs_close (lv_fs_drv_t * drv, void * file_p);
static lv_fs_res_t fs_read (lv_fs_drv_t * drv, void * file_p, void * buf, uint32_t btr, uint32_t * br);
static lv_fs_res_t fs_write(lv_fs_drv_t * drv, void * file_p, const void * buf, uint32_t btw, uint32_t * bw);
static lv_fs_res_t fs_seek (lv_fs_drv_t * drv, void * file_p, uint32_t pos, lv_fs_whence_t whence);
static lv_fs_res_t fs_tell (lv_fs_drv_t * drv, void * file_p, uint32_t * pos_p);
static void * fs_dir_open (lv_fs_drv_t * drv, const char *path);
static lv_fs_res_t fs_dir_read (lv_fs_drv_t * drv, void * dir_p, char *fn);
static lv_fs_res_t fs_dir_close (lv_fs_drv_t * drv, void * dir_p);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * Register a driver for the File system interface
 */
void lv_fs_if_posix_init(void)
{
    /*---------------------------------------------------
     * Register the file system interface  in LittlevGL
     *--------------------------------------------------*/

    /* Add a simple drive to open images */
    static lv_fs_drv_t fs_drv;                         /*A driver descriptor*/
    lv_fs_drv_init(&fs_drv);

    /*Set up fields...*/
    fs_drv.letter = LV_FS_IF_POSIX;
    fs_drv.open_cb = fs_open;
    fs_drv.close_cb = fs_close;
    fs_drv.read_cb = fs_read;
    fs_drv.write_cb = fs_write;
    fs_drv.seek_cb = fs_seek;
    fs_drv.tell_cb = fs_tell;

    fs_drv.dir_close_cb = fs_dir_close;
    fs_drv.dir_open_cb = fs_dir_open;
    fs_drv.dir_read_cb = fs_dir_read;

    lv_fs_drv_register(&fs_drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * Open a file
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to a file_t variable
 * @param path path to the file beginning with the driver letter (e.g. S:/folder/file.txt)
 * @param mode read: FS_MODE_RD, write: FS_MODE_WR, both: FS_MODE_RD | FS_MODE_WR
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static void * fs_open (lv_fs_drv_t * drv, const char * path, lv_fs_mode_t mode)
{
    (void) drv;     /*Unused*/
    errno = 0;

    uint32_t flags = 0;
    if(mode == LV_FS_MODE_WR) flags = O_WRONLY;
    else if(mode == LV_FS_MODE_RD) flags = O_RDONLY;
    else if(mode == (LV_FS_MODE_WR | LV_FS_MODE_RD)) flags = O_RDWR;

    /*Make the path relative to the current directory (the projects root folder)*/
    char buf[256];
    sprintf(buf, LV_FS_POSIX_PATH "%s", path);


    int f = open(buf, flags);
    if(f < 0) return NULL;

    /*Be sure we are the beginning of the file*/
    lseek(f, 0, SEEK_SET);

    int * fp = lv_mem_alloc(sizeof(int));
    if(fp == NULL) return NULL;
    *fp = f;

    return fp;
}


/**
 * Close an opened file
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to a file_t variable. (opened with lv_ufs_open)
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_close (lv_fs_drv_t * drv, void * file_p)
{
    (void) drv;     /*Unused*/
    int * fp = file_p;
    close(*fp);
    lv_mem_free(file_p);
    return LV_FS_RES_OK;
}

/**
 * Read data from an opened file
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to a file_t variable.
 * @param buf pointer to a memory block where to store the read data
 * @param btr number of Bytes To Read
 * @param br the real number of read bytes (Byte Read)
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_read (lv_fs_drv_t * drv, void * file_p, void * buf, uint32_t btr, uint32_t * br)
{
    (void) drv;     /*Unused*/
    int * fp = file_p;
    *br = read(*fp, buf,  btr);
    return LV_FS_RES_OK;
}

/**
 * Write into a file
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to a file_t variable
 * @param buf pointer to a buffer with the bytes to write
 * @param btr Bytes To Write
 * @param br the number of real written bytes (Bytes Written). NULL if unused.
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_write(lv_fs_drv_t * drv, void * file_p, const void * buf, uint32_t btw, uint32_t * bw)
{
    (void) drv;     /*Unused*/
    int * fp = file_p;
    *bw = write(*fp, buf, btw);
    return LV_FS_RES_OK;
}

/**
 * Set the read write pointer. Also expand the file size if necessary.
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to a file_t variable. (opened with lv_ufs_open )
 * @param pos the new position of read write pointer
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_seek (lv_fs_drv_t * drv, void * file_p, uint32_t pos, lv_fs_whence_t whence)
{
    (void) drv;     /*Unused*/
    int * fp = file_p;
    lseek(*fp, pos, whence);
    return LV_FS_RES_OK;
}

/**
 * Give the position of the read write pointer
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to a file_t variable.
 * @param pos_p pointer to to store the result
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_tell (lv_fs_drv_t * drv, void * file_p, uint32_t * pos_p)
{
    (void) drv;     /*Unused*/
    int * fp = file_p;
    *pos_p = lseek(*fp, 0, SEEK_CUR);
    return LV_FS_RES_OK;
}


#ifdef WIN32
static char next_fn[256];
#endif

/**
 * Initialize a 'fs_read_dir_t' variable for directory reading
 * @param drv pointer to a driver where this function belongs
 * @param dir_p pointer to a 'fs_read_dir_t' variable
 * @param path path to a directory
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static void * fs_dir_open (lv_fs_drv_t * drv, const char *path)
{
    (void) drv;     /*Unused*/
#ifndef WIN32
    /*Make the path relative to the current directory (the projects root folder)*/
    char buf[256];
    sprintf(buf, LV_FS_POSIX_PATH "/%s", path);
    return opendir(buf);
#else
    HANDLE d = INVALID_HANDLE_VALUE;
    WIN32_FIND_DATA fdata;

    /*Make the path relative to the current directory (the projects root folder)*/
    char buf[256];
    sprintf(buf, LV_FS_PC_PATH "\\%s\\*", path);

    strcpy(next_fn, "");
    d = FindFirstFile(buf, &fdata);
    do {
        if (strcmp(fdata.cFileName, ".") == 0 || strcmp(fdata.cFileName, "..") == 0) {
            continue;
        } else {

            if (fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                sprintf(next_fn, "/%s", fdata.cFileName);
            } else {
                sprintf(next_fn, "%s", fdata.cFileName);
            }
            break;
        }
    } while(FindNextFileA(d, &fdata));

    return d;
#endif
}

/**
 * Read the next filename form a directory.
 * The name of the directories will begin with '/'
 * @param drv pointer to a driver where this function belongs
 * @param dir_p pointer to an initialized 'fs_read_dir_t' variable
 * @param fn pointer to a buffer to store the filename
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_dir_read (lv_fs_drv_t * drv, void * dir_p, char *fn)
{
//     (void) drv;     /*Unused*/

// #ifndef WIN32
//     struct dirent *entry;
//     do {
//         entry = readdir(dir_p);

//         if(entry) {
//             if(entry->d_type == DT_DIR) sprintf(fn, "/%s", entry->d_name);
//             else strcpy(fn, entry->d_name);
//         } else {
//             strcpy(fn, "");
//         }
//     } while(strcmp(fn, "/.") == 0 || strcmp(fn, "/..") == 0);
// #else
//     strcpy(fn, next_fn);

//     strcpy(next_fn, "");
//     WIN32_FIND_DATA fdata;

//     if(FindNextFile(dir_p, &fdata) == false) return LV_FS_RES_OK;
//     do {
//         if (strcmp(fdata.cFileName, ".") == 0 || strcmp(fdata.cFileName, "..") == 0) {
//             continue;
//         } else {

//             if (fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
//             {
//                 sprintf(next_fn, "/%s", fdata.cFileName);
//             } else {
//                 sprintf(next_fn, "%s", fdata.cFileName);
//             }
//             break;
//         }
//     } while(FindNextFile(dir_p, &fdata));

// #endif
    return LV_FS_RES_OK;
}

/**
 * Close the directory reading
 * @param drv pointer to a driver where this function belongs
 * @param dir_p pointer to an initialized 'fs_read_dir_t' variable
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_dir_close (lv_fs_drv_t * drv, void * dir_p)
{
    (void) drv;     /*Unused*/
#ifndef WIN32
    closedir(dir_p);
#else
    FindClose(dir_p);
#endif
    return LV_FS_RES_OK;
}

#endif  /*LV_USE_FS_IF*/
#endif  /*LV_FS_IF_FATFS*/
