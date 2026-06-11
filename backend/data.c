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