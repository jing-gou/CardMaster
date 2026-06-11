#include "common.h"

/* get_student_info: 学生查询自己的信息
 * 参数:
 *   cardid - 卡号
 *   user - 输出参数，查到的用户信息
 * 返回:
 *   0 查询成功
 *  -1 用户不存在
 */
int get_student_info(char *cardid, User *user){
    User users[MAX_USERS];
    int user_count = 0;
    int i;

    load_users(users, &user_count);

    for(i = 0; i < user_count; i++){
        if(strcmp(users[i].cardid, cardid) == 0){
            *user = users[i];
            return 0;
        }
    }
    return -1;
}

/* change_password: 学生修改自己的密码
 * 参数:
 *   cardid - 卡号
 *   old_password - 旧密码
 *   new_password - 新密码
 * 返回:
 *   0 修改成功
 *  -1 用户不存在
 *  -2 旧密码错误
 */
int change_password(char *cardid, char *old_password, char *new_password){
    User users[MAX_USERS];
    int user_count = 0;
    int i;

    load_users(users, &user_count);

    for(i = 0; i < user_count; i++){
        if(strcmp(users[i].cardid, cardid) == 0){
            /* 验证旧密码 */
            if(strcmp(users[i].password, old_password) != 0){
                return -2;
            }
            /* 修改密码 */
            strncpy(users[i].password, new_password, sizeof(users[i].password) - 1);
            save_users(users, user_count);
            return 0;
        }
    }
    return -1;
}

/* start_session: 学生登录后自动记录上机
 * 参数:
 *   cardid - 卡号
 *   address - 上机地点
 * 返回:
 *   0 记录成功
 */
int start_session(char *cardid, char *address){
    Record rec;
    time_t now;
    struct tm *t;

    /* 获取当前时间 */
    time(&now);
    t = localtime(&now);

    /* 填充记录 */
    memset(&rec, 0, sizeof(Record));
    strncpy(rec.cardid, cardid, sizeof(rec.cardid) - 1);
    snprintf(rec.date, sizeof(rec.date), "%04d-%02d-%02d",
             t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);
    snprintf(rec.time, sizeof(rec.time), "%02d:%02d:%02d",
             t->tm_hour, t->tm_min, t->tm_sec);
    rec.duration = 0;
    rec.isOnline = 1;
    strncpy(rec.address, address, sizeof(rec.address) - 1);

    /* 保存记录 */
    save_records(cardid, &rec);
    return 0;
}

/* end_session: 学生下机，更新最近一条上机记录
 * 参数:
 *   cardid - 卡号
 * 返回:
 *   0 下机成功
 *  -1 没有正在上机的记录
 */
int end_session(char *cardid){
    Record records[MAX_RECORDS];
    int record_count = 0;
    int i;
    time_t now;
    struct tm *t;
    int end_hour, end_min, end_sec;
    int start_hour, start_min, start_sec;

    /* 加载该学生所有记录 */
    load_records(cardid, records, &record_count);

    /* 查找最后一条 isOnline=1 的记录 */
    for(i = record_count - 1; i >= 0; i--){
        if(records[i].isOnline == 1){
            /* 获取当前时间 */
            time(&now);
            t = localtime(&now);

            /* 计算上机时长（秒） */
            sscanf(records[i].time, "%d:%d:%d", &start_hour, &start_min, &start_sec);
            end_hour = t->tm_hour;
            end_min = t->tm_min;
            end_sec = t->tm_sec;

            int duration_sec = (end_hour * 3600 + end_min * 60 + end_sec)
                             - (start_hour * 3600 + start_min * 60 + start_sec);
            if(duration_sec < 0){
                duration_sec += 24 * 3600;  /* 跨天处理 */
            }

            /* 更新记录 */
            records[i].duration = duration_sec / 60;  /* 转为分钟 */
            records[i].isOnline = 0;

            /* 重写整个文件 */
            FILE *fp = fopen(
                (char[100]){0}, "w"
            );
            /* 需要用 save_records 重写，但它是追加模式 */
            /* 所以这里直接重写文件 */
            char filepath[100];
            snprintf(filepath, sizeof(filepath), "%s%s.txt", record_dir, cardid);
            fp = fopen(filepath, "w");
            if(fp == NULL){
                return -1;
            }
            for(int j = 0; j < record_count; j++){
                fprintf(fp, "%s %s %s %d %d %s\n",
                        records[j].cardid,
                        records[j].date,
                        records[j].time,
                        records[j].duration,
                        records[j].isOnline,
                        records[j].address);
            }
            fclose(fp);
            return 0;
        }
    }
    return -1;
}
