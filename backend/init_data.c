#include "common.h"
#include <direct.h>

/* main: 初始化测试数据
 * 创建管理员账号 njupt，密码 10241024
 */
int main(){
    User users[MAX_USERS];
    int user_count = 0;

    /* 确保数据目录存在 */
    _mkdir("data");
    _mkdir("data/records");

    /* 创建管理员用户 */
    User admin;
    memset(&admin, 0, sizeof(User));

    strncpy(admin.cardid, "njupt", sizeof(admin.cardid) - 1);
    strncpy(admin.name, "管理员", sizeof(admin.name) - 1);
    strncpy(admin.stuid, "admin", sizeof(admin.stuid) - 1);
    strncpy(admin.password, "10241024", sizeof(admin.password) - 1);

    admin.balance = 0.0;
    admin.chargeamount = 0.0;
    admin.status = card_active;
    admin.role = 1;  /* 管理员 */

    users[user_count] = admin;
    user_count++;

    /* 保存到文件 */
    save_users(users, user_count);

    printf("[INFO] 初始化完成\n");
    printf("[INFO] 管理员账号: njupt\n");
    printf("[INFO] 管理员密码: 10241024\n");

    return 0;
}
