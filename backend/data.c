#include "common.h"

/* load_users: 从文件加载所有用户信息
 * 参数:
 *   users - 用户数组
 *   user_count - 输出用户数量参数
 */
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

/* save_users: 将用户数组保存到文件
 * 参数:
 *   users - 用户数组
 *   user_count - 要保存的用户数量
 */
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

/* load_records: 从文件加载指定学生的上机记录
 * 参数:
 *   stuid - 学号，用于拼接文件名
 *   records - 记录数组，用于存放加载的数据
 *   record_count - 输出参数，实际加载的记录条数
 */
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

/* save_records: 将一条上机记录追加到学生文件
 * 参数:
 *   stuid - 学号，用于拼接文件名
 *   REC - 指向要保存的记录
 */
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

/* delete_user: 从数据文件中删除指定用户
 * 参数:
 *   cardid - 要删除的卡号
 * 返回:
 *   0 删除成功
 *  -1 用户不存在
 */
int delete_user(char *cardid){
    User users[MAX_USERS];
    int user_count = 0;
    int i, j;

    load_users(users, &user_count);

    for(i = 0; i < user_count; i++){
        if(strcmp(users[i].cardid, cardid) == 0){
            for(j = i; j < user_count - 1; j++){
                users[j] = users[j + 1];
            }
            user_count--;
            save_users(users, user_count);
            return 0;
        }
    }
    return -1;
}