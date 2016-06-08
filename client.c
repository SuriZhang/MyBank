
#include <stdio.h>
#include "bank.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>

static int msgid1;
static int msgid2;
void getId()
{
	msgid1 = msgget(key1,0);
	if(msgid1 == -1)
	{
		perror("Accessing Message_Queue1 Failed.");
		exit(-1);
	}
	msgid2 = msgget(key2,0);
	if(msgid2 == -1)
	{
		perror("Accessing Message_Queue2 Failed.");
		exit(-1);
	}
}


void createUser()
{
	struct Account acc;
	printf("Please enter your username: \n");
	scanf("%s",acc.name);
	printf("Please enter your password: \n");
	scanf("%s",acc.password);
	printf("Please enter your balance: \n");
	scanf("%lf",&acc.balance);
	struct Msg msg = {M_OPEN,acc};
	//access message queue
	getId();
	//send msg to message queue1
	msgsnd(msgid1,&msg,sizeof(msg.acc),0);
	//receive msg from message queue2
	msgrcv(msgid2,&msg,sizeof(msg.acc),0,0);
	if(msg.mtype == M_SUCESS)
	{
		printf("Your account has been opened.\n");
	}
	else
	{
		printf("Sorry, please try again...\n");
	}
}

void destroyUser()
{
	struct Account acc;
	printf("Please enter your userid: \n");
	scanf("%d",&acc.id);
	struct Msg msg = {M_DESTROY,acc};
	getId();
    msgsnd(msgid1,&msg,sizeof(msg.acc),0);
	msgrcv(msgid2,&msg,sizeof(msg.acc),0,0);
	if(msg.mtype == M_SUCESS)
	{
		printf("Your account has been cancelled.\n");
	}
	else
	{
		printf("Sorry, please try again.\n");
	}
}


void saveMoney()
{
	struct Account acc,accTemp;
	printf("Please enter your userid: \n");
	scanf("%d",&acc.id);
	printf("Please enter the amount: \n");
	scanf("%lf",&acc.balance);
	struct Msg msg = {M_SAVE,acc};
	getId();
	msgsnd(msgid1,&msg,sizeof(msg.acc),0);
	msgrcv(msgid2,&msg,sizeof(msg.acc),0,0);
	if(msg.mtype == M_SUCESS)
	{
		printf("Deposit Succeeded.\n");
		printf("Your Current Balance is : %lf\n",msg.acc.balance);
	}
	else
	{
		printf("Deposit Failed.\n");
	}
}

void getMoney()
{
	struct Account acc;
	printf("Please enter your userid: \n");
	scanf("%d",&acc.id);
	printf("Please enter the amount: \n");
   scanf("%lf",&acc.balance);
	struct Msg msg = {M_TAKE,acc};
	getId();
	msgsnd(msgid1,&msg,sizeof(msg.acc),0);
	msgrcv(msgid2,&msg,sizeof(msg.acc),0,0);
   if(msg.mtype == M_SUCESS)
	{
		printf("Withdraw Succeeded.\n");
		printf("Your Current Balance is :%lf\n",msg.acc.balance);
	}
	else
	{
		printf("There is not enough balance, please try again.\n");
	}
}

void checkMoney()
{
	struct Account acc;
	printf("Please enter your userid: \n");
	scanf("%d",&acc.id);
    struct Msg msg = {M_QUERY,acc};
	getId();
	msgsnd(msgid1,&msg,sizeof(msg.acc),0);
	msgrcv(msgid2,&msg,sizeof(msg.acc),0,0);
	if(msg.mtype == M_SUCESS)
	{
		printf("Your Current Balance is: %lf\n",msg.acc.balance);
	}
	else
	{
		printf("Query failed.\n");
	}
}

void moveMoney()
{
	struct Account acc;
	printf("Enter your userid: \n");
	scanf("%d",&acc.id);
	printf("Please enter the amount: \n");
	scanf("%lf",&acc.balance);
	struct Msg msg = {M_TRANSF,acc};
	getId();
	msgsnd(msgid1,&msg,sizeof(msg.acc),0);
	printf("Please enter the payee's userid: \n");
	scanf("%d",&acc.id);
	msg.acc = acc;
	msgsnd(msgid1,&msg,sizeof(msg.acc),0);
	msgrcv(msgid2,&msg,sizeof(msg.acc),0,0);
	if(msg.mtype == M_SUCESS)
	{
		printf("Transfer finished.\nYour Current Balance is: %lf\n",msg.acc.balance);
	}
	else
	{
		printf("Transfer Failed.\n");
	}
}
//create the commandline main page
void mainPage()
{
	while(1)
	{

	   printf("     Hello!\n");
	   printf("--------------------------\n");
	   printf("[1] Open Account");
	   printf("        [2] Cancel Account\n");
	   printf("[3] Deposit");
	   printf("             [4] Withdraw\n");
	   printf("[5] Query");
	   printf("               [6] Transfer\n");
	   printf("[0] Exit\n");
	   printf("-------------------------\n");
	   printf("Please enter the service code: \n");
	   int num = 0;
	   scanf("%d",&num);
	   switch(num)
	   {
			case 1:createUser();
		  		   break;
		   	case 2:destroyUser();
				    break;
	      	case 3:saveMoney();
				    break;
		   	case 4:getMoney();
				    break;
	     	case 5:checkMoney();
				    break;
		   	case 6:moveMoney();
				    break;
		   	case 0:printf("Thanks for using MyATM.\n");
				    return;
		   	default:printf("Service Code Invaild.\n");
 	   }
	}
}

int main()
{
	mainPage();
	return 0;
}
