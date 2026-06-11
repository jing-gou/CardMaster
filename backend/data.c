#include "common.h"

void load_users(User users[], int *user_count){
    FILE *fp = fopen(DATA_FILE, "r");
    if (fp == NULL) {
        printf("[WARN]无法打开数据文件\n");
        *user_count = 0;
        return;
    }
    *user_count = 0;
    while (*user_count < MAX_USERS &&
           fscanf(fp, "%s %s %s %s %lf %lf %d %d",
                  users[*user_count].cardid,
                  users[*user_count].name,
                  users[*user_count].stuid,
                  users[*user_count].password,
                  &users[*user_count].balance,
                  &users[*user_count].chargeamount,
                  (int *)&users[*user_count].status,
                  &users[*user_count].role) == 8) {
        (*user_count)++;
    }
    fclose(fp);
}

void save_users(User users[], int user_count){
    FILE *fp = fopen(DATA_FILE, "w");
    if (fp == NULL) {
        printf("[WARN]无法打开数据文件\n");
        return;
    }
    int i;
    for(i = 0; i < user_count; i++){
        fprintf(fp, "%s %s %s %s %.2f %.2f %d %d\n",
                users[i].cardid,
                users[i].name,
                users[i].stuid,
                users[i].password,
                users[i].balance,
                users[i].chargeamount,
                (int)users[i].status,
                users[i].role);
    }
    fclose(fp);
}

void load_records(char *stuid, Record records[], int *record_count){
    char recordfilepath[100];
    snprintf(recordfilepath, sizeof(recordfilepath), "%s%s.txt", record_dir, stuid);
    FILE *fp = fopen(recordfilepath, "r");
    if(fp == NULL){
        *record_count = 0;
        return;
    }
    *record_count = 0;
    while (*record_count < MAX_RECORDS && 
           fscanf(fp, "%s %s %s %d %d %s", 
                  records[*record_count].cardid,
                  records[*record_count].date,
                  records[*record_count].time,
                  &records[*record_count].duration,
                  &records[*record_count].isOnline,
                  records[*record_count].address) == 6) {
        (*record_count)++;
    }
    fclose(fp);
}


void save_records(char *stuid, const Record *REC){
    char recordfilepath[100];
    snprintf(recordfilepath, sizeof(recordfilepath), "%s%s.txt", record_dir, stuid);
    FILE *fp = fopen(recordfilepath, "a");
    if(fp == NULL){
        printf("[WARN]无法打开数据文件\n");
        return;
    }
    fprintf(fp ,"%s %s %s %d %d %s\n" ,
        REC -> cardid,
        REC -> date,
        REC -> time,
        REC -> duration,
        REC -> isOnline,
        REC -> address
    );
    fclose(fp);
}