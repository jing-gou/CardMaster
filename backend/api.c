#include "common.h"
#include <stdarg.h>

/* send_json: 发送 JSON 响应（支持格式化）
 * 参数:
 *   c - 连接对象
 *   code - HTTP 状态码
 *   fmt - 格式化字符串
 *   ... - 可变参数
 */
static void send_json(struct mg_connection *c, int code, const char *fmt, ...){
    char buf[512];              /* 响应缓冲区 */
    va_list ap;                 /* 可变参数列表 */

    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);

    mg_http_reply(c, code, "Content-Type: application/json\r\n", "%s", buf);
}

/* handle_login: 处理登录请求
 * 参数:
 *   c - 连接对象
 *   hm - HTTP 请求消息
 */
static void handle_login(struct mg_connection *c, struct mg_http_message *hm){
    char cardid[20] = {0};      /* 卡号 */
    char password[30] = {0};    /* 密码 */
    char address[20] = {0};     /* 上机地点 */
    int role = 0;               /* 角色 */
    LoginResult result;         /* 登录结果 */

    /* 从 POST 请求体解析参数 */
    mg_http_get_var(&hm->body, "cardid", cardid, sizeof(cardid));
    mg_http_get_var(&hm->body, "password", password, sizeof(password));
    mg_http_get_var(&hm->body, "address", address, sizeof(address));

    /* 调用登录验证 */
    result = user_login(cardid, password, &role);

    /* 根据结果返回 JSON */
    switch(result){
        case LOGIN_SUCCESS:
            /* 学生登录成功，自动记录上机 */
            if(role == 0){
                if(strlen(address) == 0){
                    strncpy(address, "机房", sizeof(address) - 1);
                }
                start_session(cardid, address);
            }
            send_json(c, 200, "{\"code\":0,\"role\":%d}", role);
            break;
        case LOGIN_WRONG_PWD:
            send_json(c, 200, "{\"code\":1,\"message\":\"密码错误\"}");
            break;
        case LOGIN_NOT_FOUND:
            send_json(c, 200, "{\"code\":2,\"message\":\"用户不存在\"}");
            break;
        case LOGIN_FROZEN:
            send_json(c, 200, "{\"code\":3,\"message\":\"账户已冻结\"}");
            break;
    }
}

/* handle_create_student: 处理创建学生请求
 * 参数:
 *   c - 连接对象
 *   hm - HTTP 请求消息
 */
static void handle_create_student(struct mg_connection *c, struct mg_http_message *hm){
    char cardid[20] = {0};      /* 卡号 */
    char name[20] = {0};        /* 姓名 */
    char stuid[20] = {0};       /* 学号 */
    char password[30] = {0};    /* 初始密码 */
    int result;                 /* 创建结果 */

    /* 从 POST 请求体解析参数 */
    mg_http_get_var(&hm->body, "cardid", cardid, sizeof(cardid));
    mg_http_get_var(&hm->body, "name", name, sizeof(name));
    mg_http_get_var(&hm->body, "stuid", stuid, sizeof(stuid));
    mg_http_get_var(&hm->body, "password", password, sizeof(password));

    /* 调用创建学生函数 */
    result = create_student(cardid, name, stuid, password);

    /* 根据结果返回 JSON */
    switch(result){
        case 0:
            send_json(c, 200, "{\"code\":0,\"message\":\"创建成功\"}");
            break;
        case -1:
            send_json(c, 200, "{\"code\":-1,\"message\":\"卡号已存在\"}");
            break;
        case -2:
            send_json(c, 200, "{\"code\":-2,\"message\":\"用户数量已达上限\"}");
            break;
    }
}

/* handle_recharge: 处理充值请求
 * 参数:
 *   c - 连接对象
 *   hm - HTTP 请求消息
 */
static void handle_recharge(struct mg_connection *c, struct mg_http_message *hm){
    char cardid[20] = {0};      /* 卡号 */
    char amount_str[20] = {0};  /* 金额字符串 */
    double amount;              /* 金额 */
    int result;                 /* 充值结果 */

    /* 从 POST 请求体解析参数 */
    mg_http_get_var(&hm->body, "cardid", cardid, sizeof(cardid));
    mg_http_get_var(&hm->body, "amount", amount_str, sizeof(amount_str));
    amount = atof(amount_str);

    /* 调用充值函数 */
    result = recharge(cardid, amount);

    /* 根据结果返回 JSON */
    switch(result){
        case 0:
            send_json(c, 200, "{\"code\":0,\"message\":\"充值成功\"}");
            break;
        case -1:
            send_json(c, 200, "{\"code\":-1,\"message\":\"用户不存在\"}");
            break;
        case -2:
            send_json(c, 200, "{\"code\":-2,\"message\":\"用户已挂失或冻结\"}");
            break;
        case -3:
            send_json(c, 200, "{\"code\":-3,\"message\":\"金额无效\"}");
            break;
    }
}

/* handle_report_lost: 处理挂失请求
 * 参数:
 *   c - 连接对象
 *   hm - HTTP 请求消息
 */
static void handle_report_lost(struct mg_connection *c, struct mg_http_message *hm){
    char cardid[20] = {0};      /* 卡号 */
    int result;                 /* 挂失结果 */

    /* 从 POST 请求体解析参数 */
    mg_http_get_var(&hm->body, "cardid", cardid, sizeof(cardid));

    /* 调用挂失函数 */
    result = report_lost(cardid);

    /* 根据结果返回 JSON */
    switch(result){
        case 0:
            send_json(c, 200, "{\"code\":0,\"message\":\"挂失成功\"}");
            break;
        case -1:
            send_json(c, 200, "{\"code\":-1,\"message\":\"用户不存在\"}");
            break;
        case -2:
            send_json(c, 200, "{\"code\":-2,\"message\":\"用户已挂失\"}");
            break;
        case -3:
            send_json(c, 200, "{\"code\":-3,\"message\":\"用户已冻结\"}");
            break;
    }
}

/* handle_reset_password: 处理重置密码请求
 * 参数:
 *   c - 连接对象
 *   hm - HTTP 请求消息
 */
static void handle_reset_password(struct mg_connection *c, struct mg_http_message *hm){
    char cardid[20] = {0};          /* 卡号 */
    char new_password[30] = {0};    /* 新密码 */
    int result;                     /* 重置结果 */

    /* 从 POST 请求体解析参数 */
    mg_http_get_var(&hm->body, "cardid", cardid, sizeof(cardid));
    mg_http_get_var(&hm->body, "new_password", new_password, sizeof(new_password));

    /* 调用重置密码函数 */
    result = reset_password(cardid, new_password);

    /* 根据结果返回 JSON */
    switch(result){
        case 0:
            send_json(c, 200, "{\"code\":0,\"message\":\"重置成功\"}");
            break;
        case -1:
            send_json(c, 200, "{\"code\":-1,\"message\":\"用户不存在\"}");
            break;
    }
}

/* handle_delete_student: 处理删除学生请求 */
static void handle_delete_student(struct mg_connection *c, struct mg_http_message *hm){
    char cardid[20] = {0};
    mg_http_get_var(&hm->body, "cardid", cardid, sizeof(cardid));

    int result = delete_student(cardid);
    if(result == 0){
        send_json(c, 200, "{\"code\":0,\"message\":\"删除成功\"}");
    } else {
        send_json(c, 200, "{\"code\":-1,\"message\":\"用户不存在\"}");
    }
}

/* handle_search_student: 处理查询学生信息请求 */
static void handle_search_student(struct mg_connection *c, struct mg_http_message *hm){
    char cardid[20] = {0};
    User user;
    mg_http_get_var(&hm->body, "cardid", cardid, sizeof(cardid));

    int result = search_student_info(cardid, &user);
    if(result == 0){
        send_json(c, 200, "{\"code\":0,\"cardid\":\"%s\",\"name\":\"%s\",\"stuid\":\"%s\",\"balance\":%.2f,\"status\":%d}",
                  user.cardid, user.name, user.stuid, user.balance, (int)user.status);
    } else {
        send_json(c, 200, "{\"code\":-1,\"message\":\"用户不存在\"}");
    }
}

/* handle_edit_student: 处理修改学生信息请求 */
static void handle_edit_student(struct mg_connection *c, struct mg_http_message *hm){
    char cardid[20] = {0};
    char name[20] = {0};
    char stuid[20] = {0};
    mg_http_get_var(&hm->body, "cardid", cardid, sizeof(cardid));
    mg_http_get_var(&hm->body, "name", name, sizeof(name));
    mg_http_get_var(&hm->body, "stuid", stuid, sizeof(stuid));

    int result = edit_student_info(cardid, name, stuid);
    if(result == 0){
        send_json(c, 200, "{\"code\":0,\"message\":\"修改成功\"}");
    } else {
        send_json(c, 200, "{\"code\":-1,\"message\":\"用户不存在\"}");
    }
}

/* handle_find_income: 处理统计收入请求 */
static void handle_find_income(struct mg_connection *c, struct mg_http_message *hm){
    double total = find_income();
    send_json(c, 200, "{\"code\":0,\"total\":%.2f}", total);
}

/* handle_get_student_info: 处理学生查询自己信息请求 */
static void handle_get_student_info(struct mg_connection *c, struct mg_http_message *hm){
    char cardid[20] = {0};
    User user;
    mg_http_get_var(&hm->body, "cardid", cardid, sizeof(cardid));

    int result = get_student_info(cardid, &user);
    if(result == 0){
        send_json(c, 200, "{\"code\":0,\"cardid\":\"%s\",\"name\":\"%s\",\"stuid\":\"%s\",\"balance\":%.2f,\"status\":%d}",
                  user.cardid, user.name, user.stuid, user.balance, (int)user.status);
    } else {
        send_json(c, 200, "{\"code\":-1,\"message\":\"用户不存在\"}");
    }
}

/* handle_change_password: 处理学生修改密码请求 */
static void handle_change_password(struct mg_connection *c, struct mg_http_message *hm){
    char cardid[20] = {0};
    char old_password[30] = {0};
    char new_password[30] = {0};
    mg_http_get_var(&hm->body, "cardid", cardid, sizeof(cardid));
    mg_http_get_var(&hm->body, "old_password", old_password, sizeof(old_password));
    mg_http_get_var(&hm->body, "new_password", new_password, sizeof(new_password));

    int result = change_password(cardid, old_password, new_password);
    if(result == 0){
        send_json(c, 200, "{\"code\":0,\"message\":\"修改成功\"}");
    } else if(result == -2){
        send_json(c, 200, "{\"code\":-2,\"message\":\"旧密码错误\"}");
    } else {
        send_json(c, 200, "{\"code\":-1,\"message\":\"用户不存在\"}");
    }
}

/* handle_end_session: 处理学生下机请求 */
static void handle_end_session(struct mg_connection *c, struct mg_http_message *hm){
    char cardid[20] = {0};
    mg_http_get_var(&hm->body, "cardid", cardid, sizeof(cardid));

    int result = end_session(cardid);
    if(result == 0){
        send_json(c, 200, "{\"code\":0,\"message\":\"下机成功\"}");
    } else {
        send_json(c, 200, "{\"code\":-1,\"message\":\"没有正在上机的记录\"}");
    }
}

/* handle_api: API 路由总入口
 * 参数:
 *   c - 连接对象
 *   hm - HTTP 请求消息
 */
void handle_api(struct mg_connection *c, struct mg_http_message *hm){
    /* 登录接口 */
    if(strncmp(hm->uri.buf, "/api/login", 10) == 0){
        handle_login(c, hm);
    }
    /* 管理员：创建学生接口 */
    else if(strncmp(hm->uri.buf, "/api/admin/create_student", 25) == 0){
        handle_create_student(c, hm);
    }
    /* 管理员：充值接口 */
    else if(strncmp(hm->uri.buf, "/api/admin/recharge", 19) == 0){
        handle_recharge(c, hm);
    }
    /* 管理员：挂失接口 */
    else if(strncmp(hm->uri.buf, "/api/admin/report_lost", 22) == 0){
        handle_report_lost(c, hm);
    }
    /* 管理员：重置密码接口 */
    else if(strncmp(hm->uri.buf, "/api/admin/reset_password", 25) == 0){
        handle_reset_password(c, hm);
    }
    /* 管理员：删除学生接口 */
    else if(strncmp(hm->uri.buf, "/api/admin/delete_student", 25) == 0){
        handle_delete_student(c, hm);
    }
    /* 管理员：查询学生信息接口 */
    else if(strncmp(hm->uri.buf, "/api/admin/search_student", 25) == 0){
        handle_search_student(c, hm);
    }
    /* 管理员：修改学生信息接口 */
    else if(strncmp(hm->uri.buf, "/api/admin/edit_student", 23) == 0){
        handle_edit_student(c, hm);
    }
    /* 管理员：统计收入接口 */
    else if(strncmp(hm->uri.buf, "/api/admin/find_income", 22) == 0){
        handle_find_income(c, hm);
    }
    /* 学生：查询自己信息接口 */
    else if(strncmp(hm->uri.buf, "/api/student/info", 17) == 0){
        handle_get_student_info(c, hm);
    }
    /* 学生：修改密码接口 */
    else if(strncmp(hm->uri.buf, "/api/student/change_password", 28) == 0){
        handle_change_password(c, hm);
    }
    /* 学生：下机接口 */
    else if(strncmp(hm->uri.buf, "/api/student/end_session", 24) == 0){
        handle_end_session(c, hm);
    }
    /* 未匹配的接口 */
    else {
        send_json(c, 404, "{\"code\":404,\"message\":\"接口不存在\"}");
    }
}
