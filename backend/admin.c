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

/* recharge: 管理员给学生充值
 * 参数:
 *   cardid - 卡号
 *   amount - 充值金额
 * 返回:
 *   0 充值成功
 *  -1 用户不存在
 *  -2 用户已挂失或冻结
 *  -3 金额无效
 */
int recharge(char *cardid, double amount){
    User users[MAX_USERS];      /* 用户数组 */
    int user_count = 0;         /* 用户数量 */
    int i;                      /* 循环变量 */

    /* 检查金额是否有效 */
    if(amount <= 0){
        return -3;
    }

    /* 加载所有用户 */
    load_users(users, &user_count);

    /* 遍历查找目标用户 */
    for(i = 0; i < user_count; i++){
        if(strcmp(users[i].cardid, cardid) == 0){
            /* 找到用户，检查状态 */
            if(users[i].status != card_active){
                return -2;  /* 用户已挂失或冻结 */
            }

            /* 充值 */
            users[i].balance += amount;
            users[i].chargeamount += amount;

            /* 保存到文件 */
            save_users(users, user_count);
            return 0;  /* 充值成功 */
        }
    }

    return -1;  /* 用户不存在 */
}

/* report_lost: 管理员挂失学生卡
 * 参数:
 *   cardid - 卡号
 * 返回:
 *   0 挂失成功
 *  -1 用户不存在
 *  -2 用户已挂失
 *  -3 用户已冻结
 */
int report_lost(char *cardid){
    User users[MAX_USERS];      /* 用户数组 */
    int user_count = 0;         /* 用户数量 */
    int i;                      /* 循环变量 */

    /* 加载所有用户 */
    load_users(users, &user_count);

    /* 遍历查找目标用户 */
    for(i = 0; i < user_count; i++){
        if(strcmp(users[i].cardid, cardid) == 0){
            /* 找到用户，检查状态 */
            if(users[i].status == card_lost){
                return -2;  /* 用户已挂失 */
            }
            if(users[i].status == card_frozen){
                return -3;  /* 用户已冻结 */
            }

            /* 挂失 */
            users[i].status = card_lost;

            /* 保存到文件 */
            save_users(users, user_count);
            return 0;  /* 挂失成功 */
        }
    }

    return -1;  /* 用户不存在 */
}

/* reset_password: 管理员重置学生密码
 * 参数:
 *   cardid - 卡号
 *   new_password - 新密码
 * 返回:
 *   0 重置成功
 *  -1 用户不存在
 */
int reset_password(char *cardid, char *new_password){
    User users[MAX_USERS];      /* 用户数组 */
    int user_count = 0;         /* 用户数量 */
    int i;                      /* 循环变量 */

    /* 加载所有用户 */
    load_users(users, &user_count);

    /* 遍历查找目标用户 */
    for(i = 0; i < user_count; i++){
        if(strcmp(users[i].cardid, cardid) == 0){
            /* 找到用户，重置密码 */
            strncpy(users[i].password, new_password, sizeof(users[i].password) - 1);

            /* 如果用户被冻结，同时解除冻结 */
            if(users[i].status == card_frozen){
                users[i].status = card_active;
            }

            /* 保存到文件 */
            save_users(users, user_count);
            return 0;  /* 重置成功 */
        }
    }

    return -1;  /* 用户不存在 */
}

/* delete_student: 管理员删除学生
 * 参数:
 *   cardid - 卡号
 * 返回:
 *   0 删除成功
 *  -1 用户不存在
 */
int delete_student(char *cardid){
    return delete_user(cardid);
}

/* search_student_info: 管理员查询学生信息
 * 参数:
 *   cardid - 卡号
 *   user - 输出参数，查到的用户信息
 * 返回:
 *   0 查询成功
 *  -1 用户不存在
 */
int search_student_info(char *cardid, User *user){
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

/* edit_student_info: 管理员修改学生信息
 * 参数:
 *   cardid - 卡号
 *   name - 新姓名
 *   stuid - 新学号
 * 返回:
 *   0 修改成功
 *  -1 用户不存在
 */
int edit_student_info(char *cardid, char *name, char *stuid){
    User users[MAX_USERS];
    int user_count = 0;
    int i;

    load_users(users, &user_count);

    for(i = 0; i < user_count; i++){
        if(strcmp(users[i].cardid, cardid) == 0){
            strncpy(users[i].name, name, sizeof(users[i].name) - 1);
            strncpy(users[i].stuid, stuid, sizeof(users[i].stuid) - 1);
            save_users(users, user_count);
            return 0;
        }
    }
    return -1;
}

/* find_income: 管理员统计所有学生累计充值总额
 * 返回:
 *   所有学生的 chargeamount 之和
 */
double find_income(void){
    User users[MAX_USERS];
    int user_count = 0;
    double total = 0.0;
    int i;

    load_users(users, &user_count);

    for(i = 0; i < user_count; i++){
        if(users[i].role == 0){
            total += users[i].chargeamount;
        }
    }
    return total;
}
