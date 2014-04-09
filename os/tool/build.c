#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<linux/fs.h>
#include<unistd.h>
#include<fcntl.h>

#define AOUT_FORMAT_HEADER 32
#define SYS_SIZE 0x8000		//the longgest system length is (0x9000 - 0x1000) 
				//1 SIZE = 16 Btye

#define SECTOR 512
#define SETUP_SECTORS 4

void die (char * str)
{
	fprintf(stderr,"%s\n", str);
	exit(1);
}


int main(int argc, char **argv){
	
	int i,c,id;
	char buf[1024];

	/*printf("%d \n",argc);
	while(argc >= 0){
		printf("%s\n",argv[--argc]);
	}*/	

	if(argc != 4)
		die("There're three parameters should be input.");

	for (i = 0; i < sizeof(buf) ; i++) 
			buf[i] = 0;
	
	if( (id = open(argv[1] , O_RDONLY , 0)) < 0)
		die("Unable to open 'boot'");

	i = read( id, buf, sizeof(buf));
	
	if(i != 512)
		die("Boot Size Must Be Exactly 512 Bytes!!!");

	if((* (unsigned short *)(buf + 510) ) != 0xAA55)
		die("Boot Block doesn't have the boot flag (0xAA55)!!");

	i = write(1,buf,512);

	if(i != 512)
		die("Write failed!!!");

	close(id);


	if((id = open( argv[2], O_RDONLY ,0)) < 0)
		die("open setup failed!!!");

	for(i = 0 ; (c = read( id ,buf, sizeof(buf) )) > 0 ; i+=c)
		if(write(1 , buf ,c) != c) 
			die("Write failed!!!");

	close(id);

	if( i > SETUP_SECTORS * 512)
		die("SETUP IS TOO BIG !!!");


	for(c = 0 ; c < sizeof(buf) ; c++)
		buf[c] = '\0';
	while(i < SETUP_SECTORS * 512){
		
		c = SETUP_SECTORS * 512 - i;

		if(c > sizeof(buf))
			c = sizeof(buf);

		if(write(1 ,buf ,c ) !=  c)
			die("write failed!!!");
		
		 i += c;
	}

	if((id = open(argv[3] , O_RDONLY, 0)) < 0)
		die("OPEN THE SYSTEM_KERNEL FAILED!!!");
	if( read(id , buf, AOUT_FORMAT_HEADER) != AOUT_FORMAT_HEADER)
		die("read the system header failed!!");
	//if(((long *) buf)[5] != 0)
		//die("NON-AOUT HEADER OF 'SYSTEM'");
	
	for( i = 0; (c = read(id , buf, sizeof(buf))); i += c)
		if(write(1,buf,c) != c)
			die("WRITE FAILED");

	close(id);
	printf("system is %d Btyes\n",i);

	if(i > SYS_SIZE * 16){
		die("SYSTEM IS TOO BIG, IT GONNA BE FAILED WHEN LOADED TO THE MEMORY......");
	}
	
	return 0;
}
