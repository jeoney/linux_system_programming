#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>

#include <ls.h>

int dbg = 0;

void ls_dir( char *d_name, int flag )
{
	DIR *dp;
	struct dirent *p;
	struct stat buf;
	struct passwd *pwd;
	struct group *grp;

	int max_st_nlink_len=0, max_pw_name_len=0, max_gr_name_len=0, max_st_size_len=0;
	unsigned long total_size = 0;
	int dir_start = 1;
	//int cur_st_nlink_len=0, cur_pw_name_len=0, cur_gr_name_len=0, cur_st_size_len=0;

	chdir( d_name );
	dp = opendir(".");


	if( strcmp( d_name , "." ) && strcmp( d_name , ".." ) && strcmp( d_name , "./" ) && strcmp( d_name , "../" ) )
		printf("./%s:\n", d_name);
	else
		printf("%s:\n", d_name);

	//start get max file size and name length
	while(p = readdir(dp))
	{
		check_list_len(p->d_name, flag, &max_st_nlink_len, &max_pw_name_len, &max_gr_name_len, &max_st_size_len, &total_size);
	}
	rewinddir(dp);
	//end get max file size and name length

	while( p = readdir(dp) )
	{
		if( (strcmp( p->d_name , "." ) &&  strcmp( p->d_name , ".." )) || // .이나 ..이 아닌 일반 file이거나
				(!(strcmp( p->d_name , "." ) &&  strcmp( p->d_name , ".." )) && (flag & A_FLAG) ) ) //.와 ..이지만 A flag가 있을 때 (-a)
		{
			if(flag & L_FLAG) //-l 옵션이 있으면 list 형태로 출력
			{
				if(dir_start)
				{
					//size 출력
					printf("total %lu\n", total_size/1000);
					dir_start = 0;
				}
				listview(p->d_name, flag, max_st_nlink_len, max_pw_name_len, max_gr_name_len, max_st_size_len);
			}
			else //아니면 걍 이름만 출력
			{
				check_and_print_inode(p->d_name, flag);
				printf("%s  ", p->d_name );
			}
		}
	}
	printf("\n");

	if(flag & R_FLAG)
	{
		rewinddir(dp);
		while( p = readdir(dp) )
		{
			lstat( p->d_name, &buf );
			if( S_ISDIR(buf.st_mode) )
			{
				if( strcmp( p->d_name , "." ) &&  strcmp( p->d_name , ".." ) )
				{
					ls_dir( p->d_name, flag );
					//chdir("..");
				}
			}
		}
		//printf("\n");
	}
	closedir(dp);
	chdir("..");
}

void check_and_print_inode(char *f_name, int flag)
{
	if(flag & I_FLAG)
	{
		struct stat buf;
		lstat(f_name, &buf);
		printf("%lu ", buf.st_ino);
	}
}

void ls_file( char *f_name, int flag, int max_st_nlink_len, int max_pw_name_len, int max_gr_name_len, int max_st_size_len )
{
	if(flag & L_FLAG) //-l 옵션이 있으면 list 형태로 출력
	{
		listview(f_name, flag, max_st_nlink_len, max_pw_name_len, max_gr_name_len, max_st_size_len);
	}
	else //아니면 걍 이름만 출력
	{
		//i option이 있으면 inode값 출력
		check_and_print_inode(f_name, flag);
		printf("%s  ", f_name );
	}
}

int listview(char *file_name, int flag, int max_st_nlink_len, int max_pw_name_len, int max_gr_name_len, int max_st_size_len)
{
	int	i;
	struct stat	buf;
	struct passwd *pwd;
	struct group *grp;
	struct tm *tmp;

	//상세 정보 보이기 -l option 있는 경우
	char perm[] = "----------";
	char rwx[] = "rwx";
	char sst[] = "sst";
	char	*ptr;

	if(dbg) printf("max_st_nlink_len:%d, max_pw_name_len:%d, max_gr_name_len:%d, max_st_size_len:%d\n", max_st_nlink_len, max_pw_name_len, max_gr_name_len, max_st_size_len);

	lstat(file_name, &buf);
	if (S_ISDIR(buf.st_mode))	perm[0] = 'd';
	else if (S_ISCHR(buf.st_mode)) 	perm[0] = 'c';
	else if (S_ISBLK(buf.st_mode))	perm[0] = 'b';
	else if (S_ISFIFO(buf.st_mode))	perm[0] = 'p';
	else if (S_ISLNK(buf.st_mode))	perm[0] = 'l';
	else if (S_ISSOCK(buf.st_mode))	perm[0] = 's';

	for( i=0; i<9; i++ )
	{
		if( (buf.st_mode >> (8-i)) & 0x1 )
		{
			perm[1+i] = rwx[i%3];
		}
	}
	for( i=0; i<3; i++ )
	{
		if( (buf.st_mode >> (11-i)) & 0x1 )
		{
			if( perm[(i+1)*3] == '-' )
				perm[(i+1)*3] = sst[i]-('a'-'A');
			else
				perm[(i+1)*3] = sst[i];
		}
	}

	//inode number with I flag
	if(flag & I_FLAG)
		printf("%lu ", buf.st_ino);

	printf("%s ", perm);
	printf("%*lu ", max_st_nlink_len, buf.st_nlink);
	pwd = getpwuid(buf.st_uid);
	printf("%*s ", max_pw_name_len, pwd->pw_name);
	grp = getgrgid(buf.st_gid);
	printf("%*s ", max_gr_name_len, grp->gr_name);
	if(perm[0] == 'c' || perm[0] == 'b' )
		printf("%lu, %lu ", (buf.st_rdev>>8)&0xff, buf.st_rdev&0xff ); //st_rdev의 상위 8 bit는 major ver. 하위 8 bit는 min. ver
	else
		printf("%*lu", max_st_size_len, buf.st_size);
	tmp = localtime(&buf.st_mtime);
	printf("%2d월 %2d %02d:%02d ", tmp->tm_mon+1, tmp->tm_mday, tmp->tm_hour, tmp->tm_min );
	//printf("perm[0]:%c\n", perm[0]);
	if(perm[0] == 'l') //symbolic link이면 
	{
		int ret;
		char buff[256];
		ret = readlink(file_name, buff, sizeof buff);
		buff[ret] = 0;
		printf("%s -> %s\n", file_name, buff);
	}
	else
		printf("%s\n", file_name);		
}


int get_num_len(unsigned long num)
{
	unsigned long ret=0;

	while(num>0)
	{
		num /= 10;
		ret++;
	}

	return ret;
}

void check_list_len(char *file_name, int flag, int *max_st_nlink_len, int *max_pw_name_len, int *max_gr_name_len, int *max_st_size_len, unsigned long *total_size)
{
	struct stat	buf;
	struct passwd *pwd;
	struct group *grp;

	int cur_st_nlink_len=0, cur_pw_name_len=0, cur_gr_name_len=0, cur_st_size_len=0;

	lstat(file_name, &buf);

	//일반 file이거나  또는  ., ..인데 A flag가 있을 때 
	if( (strcmp( file_name , "." ) &&  strcmp( file_name , ".." )) ||
			(!(strcmp( file_name , "." ) && strcmp( file_name , ".." )) && (flag & A_FLAG)) )
	{
		cur_st_nlink_len = get_num_len(buf.st_nlink);
		if(*max_st_nlink_len < cur_st_nlink_len )
			*max_st_nlink_len = cur_st_nlink_len;

		pwd = getpwuid(buf.st_uid);
		cur_pw_name_len = strlen(pwd->pw_name);
		if(*max_pw_name_len < cur_pw_name_len )
			*max_pw_name_len = cur_pw_name_len;

		grp = getgrgid(buf.st_gid);
		cur_gr_name_len = strlen(grp->gr_name);
		if(*max_gr_name_len < cur_gr_name_len )
			*max_gr_name_len = cur_gr_name_len;

		cur_st_size_len = get_num_len(buf.st_size);
		if(*max_st_size_len < cur_st_size_len )
			*max_st_size_len = cur_st_size_len;
		
		*total_size += buf.st_size;
	}
}
