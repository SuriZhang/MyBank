
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "bank.h"

int main()
{
	int msgid1 = msgget(key1,0);
	if(msgid1 == -1)
	{
		perror("Accessing Message_Queue1 Failed.");
		printf("Starting Server Failed.\n");
		exit(-1);
	}
	int msgid2 = msgget(key2,0);
	if(msgid2 == -1)
	{
		perror("Accessing Message_Queue2 Failed.");
		printf("Starting Server Failed.\n");
		exit(-1);
	}
   //Begin receiving message
	while(1)
	{
		struct Msg msg;
		struct Account accMove,accResult;
		if(msgrcv(msgid1,&msg,sizeof(msg.acc),0,0) <= 0)
		{
			 continue;
		}
		if(msg.mtype == M_OPEN)
		{
		   int id = generator_id();
		   accMove = msg.acc;
		   accMove.id = id;
		   if(createUser(accMove) == -1)
		   {
			    printf("Account Open Failed.\n");
				  msg.mtype = M_FAILED;
		   }
		   else
		   {
			   printf("New account is ready...\n");
				 msg.mtype = M_SUCESS;
			}
			msgsnd(msgid2,&msg,sizeof(msg.acc),0);
		}
		else if(msg.mtype == M_DESTROY)
		{
          if(destoryUser(msg.acc) == -1)
			 {
				 printf("Cancelling Account Failed.\n");
				 msg.mtype = M_FAILED;
			 }
			 else
		    {
				 printf("Your account has been cancelled.\n");
				 msg.mtype = M_SUCESS;
			 }
			 msgsnd(msgid2,&msg,sizeof(msg.acc),0);
		}
		else if(msg.mtype == M_SAVE)
		{
         if(saveMoney(msg.acc, &accResult) == -1)
			{
				printf("Deposit Failed.\n");
				msg.mtype = M_FAILED;
			}
			else
			{
				printf("Deposit Succeeded.\n");
				msg.mtype = M_SUCESS;
			}
			//sending message indicating if deposit is successful
			msg.acc.balance = accResult.balance;
			msgsnd(msgid2,&msg,sizeof(msg.acc),0);
		}
		else if(msg.mtype == M_TAKE)
		{
			 if(getMoney(msg.acc,&accResult) == -1)
			 {
				  printf("Withdraw Failed.\n");
				  msg.mtype = M_FAILED;
			 }
			 else
			 {
				  printf("Withdraw Succeeded.\n");
				  msg.mtype = M_SUCESS;
			 }
			 //sending back messages back to client
			 msg.acc.balance = accResult.balance;
			 msgsnd(msgid2,&msg,sizeof(msg.acc),0);
		}
		else if(msg.mtype == M_QUERY)
		{
			 if(checkMoney(msg.acc,&accResult) == -1)
			 {
				  printf("Query Failed.\n");
				  msg.mtype = M_FAILED;
			 }
			 else
			 {
				 printf("Query Succeeded.\n");
				 msg.mtype = M_SUCESS;
			 }
			 //sending back the balance to client
			 msg.acc.balance = accResult.balance;
			 msgsnd(msgid2,&msg,sizeof(msg.acc),0);
		}
		else if(msg.mtype == M_TRANSF)
		{
			 //set up structure
			 struct Msg msgTemp;
			 msgrcv(msgid1,&msgTemp,sizeof(msgTemp.acc),0,0);
			 if(moveMoney(msg.acc,msgTemp.acc,&accResult) == -1)
			 {
				 printf("Transfer Failed.\n");
				 msg.mtype = M_FAILED;
			 }
			 else
			 {
				 printf("Transfer Succeeded.\n");
				 msg.mtype = M_SUCESS;
			 }
			 msg.acc.balance = accResult.balance;
			 printf("The New Balance is: %lf\n",msg.acc.balance);
			 //sending back the account balance to client.
			 msgsnd(msgid2,&msg,sizeof(msg.acc),0);
		}
	}
   return 0;
}
