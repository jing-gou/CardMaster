#include "common.h"

static int login_attempts[MAX_USERS] = {0};

/* user_login: 登录认证
 * 参数:
 *   cardid - 学号
 *   password - 密码
 *   role - 用户角色 0user 1admin
 */
LoginResult user_login(char *cardid, char *password, int *role){
    User users[MAX_USERS];     
    int user_count = 0;        
    int i;                     

    load_users(users, &user_count);

    for(i = 0; i < user_count; i++){
        if(strcmp(users[i].cardid, cardid) == 0){
            if(users[i].status == card_frozen){
                return LOGIN_FROZEN;
            }

            if(strcmp(users[i].password, password) == 0){
                login_attempts[i] = 0;
                *role = users[i].role; 
                return LOGIN_SUCCESS;
            } else {
                login_attempts[i]++;
                if(login_attempts[i] >= 3){
                    users[i].status = card_frozen;
                    save_users(users, user_count);
                }
                return LOGIN_WRONG_PWD;
            }
        }
    }


    return LOGIN_NOT_FOUND;
}

/* Token 存储（内存） */
static Token token_store[MAX_TOKENS];
static int token_count = 0;

/* generate_token: 生成 token
 * 参数:
 *   cardid - 卡号
 *   role - 角色
 * 返回:
 *   token 字符串指针
 */
char* generate_token(char *cardid, int role){
    int i;
    time_t now;
    time(&now);

    /* 清理过期 token */
    for(i = 0; i < token_count; i++){
        if(token_store[i].expire < now){
            /* 移除过期 token */
            for(int j = i; j < token_count - 1; j++){
                token_store[j] = token_store[j + 1];
            }
            token_count--;
            i--;
        }
    }

    /* 生成简单 token: cardid + 时间戳 */
    Token new_token;
    snprintf(new_token.token, sizeof(new_token.token), "%s_%lld_%d", cardid, (long long)now, rand() % 10000);
    strncpy(new_token.cardid, cardid, sizeof(new_token.cardid) - 1);
    new_token.role = role;
    new_token.expire = now + TOKEN_EXPIRE;

    /* 存储 token */
    if(token_count < MAX_TOKENS){
        token_store[token_count] = new_token;
        token_count++;
    }

    /* 返回静态缓冲区 */
    static char result[64];
    strncpy(result, new_token.token, sizeof(result) - 1);
    return result;
}

/* verify_token: 验证 token
 * 参数:
 *   token - token 字符串
 *   cardid - 输出参数，对应的卡号
 *   role - 输出参数，对应的角色
 * 返回:
 *   0 验证成功
 *  -1 token 无效或过期
 */
int verify_token(char *token, char *cardid, int *role){
    int i;
    time_t now;
    time(&now);

    for(i = 0; i < token_count; i++){
        if(strcmp(token_store[i].token, token) == 0){
            /* 检查是否过期 */
            if(token_store[i].expire < now){
                return -1;
            }
            /* 返回用户信息 */
            strncpy(cardid, token_store[i].cardid, 20);
            *role = token_store[i].role;
            return 0;
        }
    }
    return -1;
}