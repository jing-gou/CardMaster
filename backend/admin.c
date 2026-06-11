#include "common.h"
#include <direct.h>

/* create_student: 管理员创建新学生
 * 参数:
 *   cardid - 卡号
 *   name - 姓名
 *   stuid - 学号
 *   password - 初始密码
 * 返回:
 *   0 创建成功
 *  -1 卡号已存在
 *  -2 用户数量已达上限
 */
int create_student(char *cardid, char *name, char *stuid, char *password){
    User users[MAX_USERS];      /* 用户数组 */
    int user_count = 0;         /* 用户数量 */
    int i;                      /* 循环变量 */

    /* 确保数据目录存在 */
    _mkdir("data");
    _mkdir("data/records");

    /* 加载现有用户 */
    load_users(users, &user_count);

    /* 检查用户数量是否已达上限 */
    if(user_count >= MAX_USERS){
        return -2;
    }

    /* 检查卡号是否已存在 */
    for(i = 0; i < user_count; i++){
        if(strcmp(users[i].cardid, cardid) == 0){
            return -1;  /* 卡号已存在 */
        }
    }

    /* 创建新用户 */
    User new_user;
    memset(&new_user, 0, sizeof(User));     /* 清零初始化 */

    strncpy(new_user.cardid, cardid, sizeof(new_user.cardid) - 1);
    strncpy(new_user.name, name, sizeof(new_user.name) - 1);
    strncpy(new_user.stuid, stuid, sizeof(new_user.stuid) - 1);
    strncpy(new_user.password, password, sizeof(new_user.password) - 1);

    new_user.balance = 0.0;             /* 初始余额为0 */
    new_user.chargeamount = 0.0;        /* 累计充值为0 */
    new_user.status = card_active;      /* 状态：正常 */
    new_user.role = 0;                  /* 角色：学生 */

    /* 添加到用户数组 */
    users[user_count] = new_user;
    user_count++;

    /* 保存到文件 */
    save_users(users, user_count);

    return 0;  /* 创建成功 */
}
