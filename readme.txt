244181 전은엽

-----------------------------------------
구현내용
-----------------------------------------
구현된 요구사항 
0. 디렉토리 내 전체 list보이기
1. 기존 ls명령의 -a , -l, -R , -i 옵션을 똑같이 지원해야 한다.
2. -R 옵션은 재귀 호출을 이용하여 구현한다.
3. permission은 특수 퍼미션( s, s, t ) 까지 포함하여 출력 해야 한다.
4. 시간 출력은 localtime 함수를 이용하여 포메팅한 후 기존 명령과 동일하게 출력한다.
5. 모든 라인에 필드 값은 align 하여 출력 한다. => 일부 미 구현 (아래 미구현 1번)
6. 심볼릭 링크 파일인 경우 linkname -> targetname 형태로 출력 한다.
7. 디바이스 특수 파일인 경우 size의 자리에 major, minor번호 형태로 출력 한다.
9. 기존 ls의 명령과 같이 total 파일 크기를 출력 해야 한다. (ls 메뉴얼 참조)
8. ls 뒤에 디렉토리 명을 주었을 경우 해당 디렉토리 내용을 출력한다.
10. 기능별로 파일로 구분하여 구현하고 make를 이용하여 빌드 시스템을 구축한다.

미구현 기능
1. -l 옵션 없이 i 옵션을 줬을 때 정렬기능 미 구현
2. 요구사항에 없어서 구현하지 않음 이름 순 정렬 (linked list등을 써서 list관리를 해야 할듯 합니다.)
3. 상위 디렉토리 (../) 보기

-----------------------------------------
함수 기능 설명
-----------------------------------------
//main에서 호출되는 ls기본 함수
int ls(int argc, char *argv[], int flag);

//옵션 check후 return해 주는 함수
int mygetopt( int argc, char **argv );

//directory용 ls함수
void ls_dir( char *d_name, int flag);

//-l 용 list의 정보를 뽑아서 print해 주는 함수
int listview(char *file_name, int flag, int max_st_nlink_len, int max_pw_name_len, int max_gr_name_len, int max_st_size_len);

//숫자를 자리수로 return해 주는 함수 (align용)
int get_num_len(unsigned long num);

//file용 ls함수
void ls_file(char *f_name, int flag, int max_st_nlink_len, int max_pw_name_len, int max_gr_name_len, int max_st_size_len);

//file ist를 따로 관리하지 않고 디렉토리 내 또는 argument로 받은 file list의 최대 nlink자리수, pw_name길이, gr_name길이, st_size자리수를 return하는 함수 (align용)
void check_list_len(char *file_name, int flag, int *max_st_nlink_len, int *max_pw_name_len, int *max_gr_name_len, int *max_st_size_len, unsigned long *total_size);

//-i option용 flag체크 후 inode 정보 print해 주는 함수
void check_and_print_inode(char *f_name, int flag);


-----------------------------------------
과제 구현시 느낀 점
-----------------------------------------
평소에 아무생각없이 사용하던 ls였는데 man 페이지에 가니 엄청 많은 기능이 있다는것에 놀랐습니다.
실제로 구현해 보니 제대로된 초기 디자인을 하지 않으면 중구난방식으로 코드가 구현됨을 느끼게 되었습니다. (...)
linux file system관련하여 다양한 api와 정보들을 공부할 수 있게되어 좋았습니다.
생각보다 결과를 출력해 주는 것과 관련된 (align, 줄바꿈 등)이 까다롭게 느껴졌습니다. (시간도 오래 걸리고 test도 많이 해야 함)
감사합니다.

