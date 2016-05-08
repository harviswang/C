//#define __S_IFIFO
//#define __S_IFLINK
#include <sys/stat.h>
#include <stdio.h>

static void stat_dump(struct stat *sp);
int main(int argc, char **argv)
{
	char *path = "/dev/random";
	struct stat stat;
	int err = 0;
	
	err = lstat(path, &stat);
	if (!err) {
		stat_dump(&stat);
	}
	
	return 0;
}

static void stat_dump(struct stat *sp)
{
	printf("st_dev = %d\n", sp->st_dev);
	printf("st_ino = %d\n", sp->st_ino);
	printf("st_mode = %d\n", sp->st_mode);
	printf("st_nlink = %d\n", sp->st_nlink);
	printf("st_uid = %d\n", sp->st_uid);
	printf("st_gid = %d\n", sp->st_gid);
	printf("st_rdev = %d\n", sp->st_rdev);
	printf("st_size = %ld\n", sp->st_size);
	printf("st_blksize = %d\n", sp->st_blksize);
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
