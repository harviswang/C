#include <sys/stat.h>
#include <stdio.h>

static void lstat_test(void);

int main(int argc, char **argv)
{
    lstat_test();
	return 0;
}

static void stat_dump(struct stat *sp)
{
	printf("st_dev = %ld\n", sp->st_dev);
	printf("st_ino = %ld\n", sp->st_ino);
	printf("st_mode = %d\n", sp->st_mode);
	printf("st_nlink = %ld\n", sp->st_nlink);
	printf("st_uid = %d\n", sp->st_uid);
	printf("st_gid = %d\n", sp->st_gid);
	printf("st_rdev = %ld\n", sp->st_rdev);
	printf("st_size = %ld\n", sp->st_size);
	printf("st_blksize = %ld\n", sp->st_blksize);
	printf("st_blocks = %ld\n", sp->st_blocks);
	printf("st_atime = %ld\n", sp->st_atime);
	printf("st_mtime = %ld\n", sp->st_mtime);
	printf("st_ctime = %ld\n", sp->st_ctime);
	if (S_ISLNK(sp->st_mode)) {
		printf("stat.st_mode indicate a link file\n");
	}
	if (S_ISREG(sp->st_mode)) {
		printf("stat.st_mode indicate a regular file\n");
	}
	if (S_ISDIR(sp->st_mode)) {
		printf("stat.st_mode indicate a directory\n");
	}
	if (S_ISCHR(sp->st_mode)) {
		printf("stat.st_mode indicate a char device\n");
	}
	if (S_ISBLK(sp->st_mode)) {
		printf("stat.st_mode indicate a block device\n");
	}
/*
 * not support S_ISFIFIO in CentOS 7
	if (S_ISFIFIO(sp->st_mode)) {
		printf("stat.st_mode indicate a FIFO file\n");
	}
*/
	if (S_ISSOCK(sp->st_mode)) {
		printf("stat.st_mode indicate a SOCKET file\n");
	}
}

static void lstat_test(void)
{
    /* char *path = "/dev/random"; */
    const char path[] = "/usr/share/hwdata/usb.ids";
    struct stat stat;
    int err = 0;

    err = lstat(path, &stat);
    if (!err) {
        stat_dump(&stat);
    }
}
