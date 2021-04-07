#include <stdio.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>

#define A_FLAG  (1<<0)
#define L_FLAG  (1<<1)
#define R_FLAG  (1<<2)
#define I_FLAG  (1<<3)

int ls(int argc, char *argv[], int flag);
int mygetopt( int argc, char **argv );
void ls_dir( char *d_name, int flag);
int listview(char *file_name, int flag, int max_st_nlink_len, int max_pw_name_len, int max_gr_name_len, int max_st_size_len);
int get_num_len(unsigned long num);
void ls_file(char *f_name, int flag, int max_st_nlink_len, int max_pw_name_len, int max_gr_name_len, int max_st_size_len);
void check_list_len(char *file_name, int flag, int *max_st_nlink_len, int *max_pw_name_len, int *max_gr_name_len, int *max_st_size_len, unsigned long *total_size);
void check_and_print_inode(char *f_name, int flag);
