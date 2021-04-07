#include <unistd.h>
#include <ls.h>

int mygetopt( int argc, char **argv )
{
	int i;
	int ch;
	int flag = 0;

	while( (ch = getopt( argc, argv, "alRi")) != -1 )
	{
		switch(ch)
		{
			case 'a' : flag |= A_FLAG; break;  // 0001
			case 'l' : flag |= L_FLAG; break;  // 0010
			case 'R' : flag |= R_FLAG; break;  // 0100
			case 'i' : flag |= I_FLAG; break;  // 1000
					   // 0011
		}
	}

	return flag;
}
