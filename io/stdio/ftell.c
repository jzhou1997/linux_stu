#include <stdio.h>
#include <stdlib.h>

int main(int argc,char **argv)
{
	FILE *fp;
	if(argc < 2)
	{
		fprintf(stderr,"Usage<file_name>\n");
		exit(1);
	}
	fp = fopen(argv[1],"r");
	if(fp == NULL)
	{
		perror("fopen()");
		exit(1);
	}
	fseek(fp,0,SEEK_END);
	printf("The total number of bytes is %ld\n",ftell(fp));
	
	fclose(fp);
	exit(0);
}
