#include "common.h"

/* send_json: 发送 JSON 响应
 * 参数:
 *   c - 连接对象
 *   code - HTTP 状态码
 *   json - JSON 字符串
 */
static void send_json(struct mg_connection *c, int code, const char *json){
    mg_http_reply(c, code, "Content-Type: application/json\r\n", "%s", json);
}

/* handle_login: 处理登录请求
 * 参数:
 *   c - 连接对象
 *   hm - HTTP 请求消息
 */
static void handle_login(struct mg_connection *c, struct mg_http_message *hm){
    char cardid[20] = {0};      /* 卡号 */
    char password[30] = {0};    /* 密码 */
    int role = 0;               /* 角色 */
    LoginResult result;         /* 登录结果 */

    /* 从 POST 请求体解析参数 */
    mg_http_get_var(&hm->body, "cardid", cardid, sizeof(cardid));
    mg_http_get_var(&hm->body, "password", password, sizeof(password));

    /* 调用登录验证 */
    result = user_login(cardid, password, &role);

    /* 根据结果返回 JSON */
    switch(result){
        case LOGIN_SUCCESS:
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

/* handle_api: API 路由总入口
 * 参数:
 *   c - 连接对象
 *   hm - HTTP 请求消息
 */
void handle_api(struct mg_connection *c, struct mg_http_message *hm){
    /* 登录接口 */
    if(mg_match(hm->uri, mg_str("/api/login"), NULL)){
        handle_login(c, hm);
    }
    /* 管理员：创建学生接口 */
    else if(mg_match(hm->uri, mg_str("/api/admin/create_student"), NULL)){
        handle_create_student(c, hm);
    }
    /* 未匹配的接口 */
    else {
        send_json(c, 404, "{\"code\":404,\"message\":\"接口不存在\"}");
    }
}
