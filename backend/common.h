#ifndef common_h
#define common_h

#include "mongoose.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// 结构体定义
typedef enum {card_active = 0, card_lost = 1, card_frozen = 2} cardstatus;

 typedef struct{
    char cardid[10];
    char name[15];
    char stuid[15];
    char password[20];
    double balance;
    double chargeamount;
    cardstatus status;
} User;

typedef struct{
    char cardid[10];
    char time[15];
    char date[20];
    int duration;
    int ifOnline;
    char address[10];
} Record;

typedef struct{
    char adminid[10];
    char password[15];
} Admin;

// 常量定义
#define MAX_USERS 100
#define MAX_RECORDS 1000
#define record_dir "data/records/"
#define DATA_FILE "data/cards.dat"

// 函数声明

// data.c
void load_users(User users[], int *user_count);
void save_users(User users[], int user_count);
void load_records(Record records[], int *record_count);
void save_records(Record records[], int record_count);



#endif // common_h