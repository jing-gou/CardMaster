#include "common.h"

/* 服务器监听端口 */
#define LISTEN_PORT "4012"

/* ev_handler: Mongoose 事件处理函数
 * 参数:
 *   c - 连接对象
 *   ev - 事件类型
 *   ev_data - 事件数据
 *   fn_data - 用户数据（未使用）
 */
static void ev_handler(struct mg_connection *c, int ev, void *ev_data){
    if(ev == MG_EV_HTTP_MSG){
        struct mg_http_message *hm = (struct mg_http_message *)ev_data;

        /* 路由：API 接口（交给 api.c 处理） */
        if(strncmp(hm->uri.buf, "/api/", 5) == 0){
            handle_api(c, hm);
        }
        /* 路由：静态文件（前端页面） */
        else {
            struct mg_http_serve_opts opts = {
                .root_dir = "../frontend"   /* 前端文件目录 */
            };
            mg_http_serve_dir(c, hm, &opts);
        }
    }
}

/* main: 程序入口
 * 返回:
 *   0 表示正常退出
 */
int main(void){
    struct mg_mgr mgr;          /* Mongoose 事件管理器 */
    struct mg_connection *c;    /* 连接对象 */

    /* 初始化事件管理器 */
    mg_mgr_init(&mgr);

    /* 创建 HTTP 监听 */
    c = mg_http_listen(&mgr, "http://0.0.0.0:" LISTEN_PORT, ev_handler, NULL);
    if(c == NULL){
        printf("[ERROR] 无法启动服务器，端口 %s 可能被占用\n", LISTEN_PORT);
        return 1;
    }

    printf("[INFO] 服务器已启动，监听端口 %s\n", LISTEN_PORT);
    printf("[INFO] 访问地址: http://localhost:%s\n", LISTEN_PORT);

    /* 事件循环 */
    while(1){
        mg_mgr_poll(&mgr, 1000);
    }

    /* 清理资源（实际不会执行到这里） */
    mg_mgr_free(&mgr);
    return 0;
}