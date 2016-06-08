#include "dao.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>

//create a file to store data 
const char* ID_FILE = "id.dat";

//automatically generate a new userid 
int generator_id()
{
	static int x = 100000;
	//check if ID_FILE exists, return -1 if nonexists
	if(access(ID_FILE,F_OK) == -1)
	{
		int fd = open(ID_FILE,O_WRONLY|O_CREAT|O_EXCL,0600);
		if(fd == -1)
		{
			 perror("File open failed.");
			 return -1;
		}
		write(fd,&x,sizeof(x));
		close(fd);
		return x;
	}

	int fd = open(ID_FILE,O_RDWR);
	if(fd == -1)
	{
		perror("File open failed.");
		return -1;
	}
	//read out the file and store the data to x
	read(fd,&x,sizeof(x));
    x++;
	//set pointer to the begining of the file
	lseek(fd,0,SEEK_SET);
	//write x to the file
	write(fd,&x,sizeof(x));
	close(fd);
	return x;
}

//save a new account to an individual file
int createUser(struct Account acc)
{
	char filename[20] = {0};
	sprintf(filename,"%d.dat",acc.id);
  int fd = open(filename,O_WRONLY|O_CREAT|O_EXCL,0600);
	if(fd == -1)
	{
		perror("Create account file failed.");
		return -1;
	}
	if(write(fd,&acc,sizeof(acc)) < 0)
	{
		return -1;
	}
	close(fd);
	printf("New user has been created.\n");
	return 0;
}
//find the user and destory the coresponding file
int destoryUser(struct Account acc)
{
	char filename[20] = {0};
	sprintf(filename,"%d.dat",acc.id);
	int fd = open(filename,O_RDONLY,0600);
   if(fd == -1)
   {
		perror("failed:");
		printf("The account does not exist.\n");
		return -1;
   }
	//delete file
	remove(filename);
	return 0;
}
//search the account and deposit money
int saveMoney(struct Account acc,struct Account *p)
{
	char filename[20] = {0};
	sprintf(filename,"%d.dat",acc.id);
   int fd = open(filename,O_RDWR,0600);	
	if(fd == -1)
	{
		perror("File open failed.");
		printf("The account does not exist, please open an account first.\n");
		return -1;
	}
	struct Account accTemp;
	read(fd,&accTemp,sizeof(accTemp));
	accTemp.balance += acc.balance;
	printf("Your New Balance is:%lf\n",accTemp.balance);
	*p = accTemp;
	lseek(fd,-sizeof(accTemp),SEEK_CUR);
	write(fd,&accTemp,sizeof(accTemp));
	return 0;
}

int getMoney(struct Account acc,struct Account *p)
{
	char filename[20] = {0};
	sprintf(filename,"%d.dat",acc.id);
	int fd = open(filename,O_RDWR,0600);
	if(fd == -1)
	{
		perror("File open failed.");
		return -1;
	}
	struct Account accTemp;
	read(fd,&accTemp,sizeof(accTemp));
	if(accTemp.balance < acc.balance)
	{
		return -1;
	}
   accTemp.balance -= acc.balance;
	printf("Your New Balance is:%lf\n",accTemp.balance);
	lseek(fd,-sizeof(accTemp),SEEK_CUR);
	write(fd,&accTemp,sizeof(accTemp));
	*p = accTemp;
	return 0;
}

int checkMoney(struct Account acc,struct Account *p)
{
	char filename[20] = {0};
	sprintf(filename,"%d.dat",acc.id);
	int fd = open(filename,O_RDONLY,0600);
	if(fd == -1)
	{
		perror("File open failed.");
		return -1;
	}
	struct Account accTemp;
	read(fd,&accTemp,sizeof(accTemp));
	*p = accTemp;
	return 0;
}

int moveMoney(struct Account acc1,struct Account acc2,struct Account *p)
{
   char filenameOut[20] = {0},filenameIn[20] = {0};
	//transfer out
	sprintf(filenameOut,"%d.dat",acc1.id);
	int fd = open(filenameOut,O_RDWR);
	if(fd == -1)
	{
		printf("The account does not exist.\n");
		return -1;
	}
	struct Account accTemp;
	read(fd,&accTemp,sizeof(accTemp));
	if(accTemp.balance < acc1.balance)
	{
		printf("There is not enought balance in the current account.\n");
		return -1;
	}
	accTemp.balance -= acc1.balance;
	lseek(fd,0,SEEK_SET);
	write(fd,&accTemp,sizeof(accTemp));
	*p = accTemp;
   //receive money
	sprintf(filenameIn,"%d.dat",acc2.id);
	int fd2 = open(filenameIn,O_RDWR);
	if(fd2 == -1)
	{
		perror("The account does not exist.");
		return -1;
	}
	read(fd2,&accTemp,sizeof(accTemp));
	accTemp.balance += acc1.balance;
	lseek(fd2,0,SEEK_SET);
	write(fd2,&accTemp,sizeof(accTemp));
   return 1;
}
