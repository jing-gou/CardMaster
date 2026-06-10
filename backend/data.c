#include "common.h"

void load_users(User users[], int *user_count){
    FILE *fp = fopen(DATA_FILE, "rb");
    if (fp == NULL) {
        printf("[WARN]无法打开数据文件\n");
        *user_count = 0;
        return;
    }
    *user_count = fread(users, sizeof(User), MAX_USERS, fp);
    fclose(fp);
}

void save_users(User users[], int user_count){
    FILE *fp = fopen(DATA_FILE, "wb");
    if (fp == NULL) {
        printf("[WARN]无法打开数据文件\n");
        return;
    }
    fwrite(users, sizeof(User), user_count, fp);
    fclose(fp);
}

void load_records(Record records[], int *record_count){
    
}


void save_records(Record records[], int record_count);