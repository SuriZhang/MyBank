
#ifndef DAO_H
#define DAO_H
#include "bank.h"


int generator_id();

int createUser(struct Account acc);

int destroyUser(struct Account acc);

int saveMoney(struct Account acc,struct Account *p);

int getMoney(struct Account acc,struct Account *p);

int checkMoney(struct Account acc,struct Account *p);

int moveMoney(struct Account acc1,struct Account acc2,struct Account *p);

#endif
