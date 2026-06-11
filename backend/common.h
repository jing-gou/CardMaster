#ifndef common_h
#define common_h

#include "mongoose.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// 结构体定义
typedef enum {card_active = 0, card_lost = 1, card_frozen = 2} cardstatus;
typedef enum {
    LOGIN_SUCCESS = 0,      // 登录成功
    LOGIN_WRONG_PWD = 1,    // 密码错误
    LOGIN_NOT_FOUND = 2,    // 用户不存在
    LOGIN_FROZEN = 3        // 账户已冻结
} LoginResult;


 typedef struct{
    char cardid[10];
    char name[15];
    char stuid[15];
    char password[20];
    double balance;
    double chargeamount;
    cardstatus status;
    int role;               
} User;

typedef struct{
    char cardid[10];
    char time[15];
    char date[20];
    int duration;
    int isOnline;
    char address[10];
} Record;

// Token 结构
typedef struct{
    char token[64];         /* token 字符串 */
    char cardid[20];        /* 对应的卡号 */
    int role;               /* 角色 */
    time_t expire;          /* 过期时间 */
} Token;

// 常量定义
#define MAX_USERS 100
#define MAX_RECORDS 1000
#define MAX_TOKENS 100
#define TOKEN_EXPIRE 3600   /* token 有效期（秒） */
#define record_dir "data/records/"
#define DATA_FILE "data/cards.txt"

// 函数声明

// data.c

void load_users(User users[], int *user_count);
void save_users(User users[], int user_count);
void load_records(char *stuid, Record records[], int *record_count);
void save_records(char *stuid, const Record *REC);
int delete_user(char *cardid);

// auth.c
LoginResult user_login(char *cardid, char *password, int *role);
char* generate_token(char *cardid, int role);
int verify_token(char *token, char *cardid, int *role);

// api.c

void handle_api(struct mg_connection *c, struct mg_http_message *hm);

// admin.c

int create_student(char *cardid, char *name, char *stuid, char *password);
int recharge(char *cardid, double amount);
int report_lost(char *cardid);
int reset_password(char *cardid, char *new_password);
int delete_student(char *cardid);
int search_student_info(char *cardid, User *user);
int edit_student_info(char *cardid, char *name, char *stuid);
double find_income(void);

// student.c
int get_student_info(char *cardid, User *user, Record *last_record);
int change_password(char *cardid, char *old_password, char *new_password);
int start_session(char *cardid, char *address);
int end_session(char *cardid);

#endif // common_h