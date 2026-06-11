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