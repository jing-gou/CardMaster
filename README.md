# Card Master
一个基于 Clang 的学生卡管理系统。

## 项目成员
- 杨帅 `组长`
- 杨晨 `前端`
- 肖戍果 `后端`

## 项目框架
```
CampusCard_Web/
|
├── frontend/                 # Web 前端目录
│   ├── index.html            # 登录页面
│   ├── admin.html            # 管理员操作面板
│   ├── student.html          # 学生查询/上机面板
│   ├── css/                  # 样式文件
│   └── js/                   # 前端逻辑 
|
├── backend/                  # C 语言后端目录
│   ├── main.c                # 入口 
│   ├── api.c                 # 路由分发
│   ├── auth.c                # 认证模块
│   ├── admin.c               # 管理员业务
│   ├── student.c             # 学生业务
│   ├── data.c                # 数据层
│   └── common.h              # 结构体定义与函数声明
|
└── data/                     # 数据存储目录 
    ├── cards.dat             # 全局账户信息
    └── records/              # 上机记录
```
