
#ifndef BANK_H
#define BANK_H


extern const int key1;//from client to server
extern const int key2;//from server to client

//message type
#define M_OPEN 1 //open an account
#define M_DESTROY 2 //cancel an account
#define M_SAVE 3 //deposit
#define M_TAKE 4 //withdraw
#define M_QUERY 5 //query
#define M_TRANSF 6 //transer
#define M_SUCESS 7 
#define M_FAILED 8 


struct Account
{
	int id;//userid
	char name[10]; //username
	char password[10]; //password
	double balance; //balance
};


struct Msg
{
	int mtype;//message type
	struct Account acc;
};

#endif
