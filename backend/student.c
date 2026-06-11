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
