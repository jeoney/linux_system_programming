#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>

#include <ls.h>

int dbg2 = 0;

int  main(int argc, char *argv[])
{
	int flag;
	flag = mygetopt(argc, argv);
	if(dbg2) printf("current flag:%d\n", flag);

	if(flag != 0 )
	{
		ls(argc-2, &argv[1], flag);
	}
	else //이미 flag 없음
	{
		ls(argc-1, &argv[0], flag);
	}
	return 0;
}

int ls(int argc, char *argv[], int flag)
{
	if(dbg2) printf("argc: %d\n", argc);

	if(argc == 0) //인자 없이 들어오면
	{
		if(dbg2) printf("인자 없음 test dir start:\n");
		ls_dir(".", flag);
		return 0;
	}
	else //파일, 디렉토리명 지정해서 들어올 경우
	{
		int i;
		struct stat	buf;

		int max_st_nlink_len=0, max_pw_name_len=0, max_gr_name_len=0, max_st_size_len=0;
		unsigned long total_size;

		if(dbg2) printf("인자 있음 argc:%d test dir start:\n", argc);

		//check length
		for(i=1; i<=argc; i++)
		{
			check_list_len(argv[i], flag, &max_st_nlink_len, &max_pw_name_len, &max_gr_name_len, &max_st_size_len, &total_size);
		}

		//print
		for(i=1; i<=argc; i++)
		{
			memset(&buf, 1, sizeof buf);

			lstat(argv[i], &buf);
			
			if(dbg2) printf("current argv[i]:%s, %d\n", argv[i], S_ISDIR(buf.st_mode));

			if(S_ISDIR(buf.st_mode) || !(strcmp( argv[i] , "." ) || strcmp( argv[i] , "./" )) )//||//디렉토리, ., .. 는 디렉토리 고
				//strcmp( argv[i] , ".." )  || strcmp( argv[i] , "../" )) //상위 디렉토리 보기 미 지원;;
			{
				if(i>1)
					printf("\n");
				ls_dir(argv[i], flag);
			}
			else //file이면
			{
				ls_file(argv[i], flag, max_st_nlink_len, max_pw_name_len, max_gr_name_len, max_st_size_len);
				if(i==argc) //마지막이면 줄 내림
					printf("\n");
			}
		}

		return 0;
	}
}
