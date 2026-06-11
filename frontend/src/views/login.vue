<template>
  <div class="containfather">
    <div class="loginBox">
      <h2 class="login-title">校园卡系统</h2>
      <div class="userName">
        <input type="text" id="username" placeholder="请输入卡号" />
      </div>
      <div class="userPassword">
        <input type="password" id="password" placeholder="请输入密码" />
      </div>
      <button class="login-button" id="login-button">立即登录</button>
    </div>
  </div>
</template>

<style scoped>
* {
  margin: 0;
  padding: 0;
  box-sizing: border-box;
  font-family: "Microsoft YaHei", sans-serif;
}

.containfather {
  width: 100%;
  height: 550px;
  margin-top: 100px;
  display: flex;
  justify-content: center;
  align-items: center;
}
.loginBox {
  width: 450px;
  height: 550px;
  /* background-color: rgb(17, 85, 145); */
  display: flex;
  flex-direction: column;
  border-radius: 20px;
  padding-top: 80px;
  box-shadow: 0 0 12px 3px rgba(0, 0, 0, 0.15);
  /* justify-content: center;
  align-items: center; */
  background: linear-gradient(45deg, #0779e3d3 50%, #dde1ea 50%);
  background-size: 200% 200%;
  /* 斜向流动动画 */
  animation: appleSlopeFlow 7s linear infinite;
  overflow: hidden;
}

/* 斜向流动关键帧 */
@keyframes appleSlopeFlow {
  0% {
    background-position: 0 0;
  }
  100% {
    background-position: 100% 100%;
  }
}
.login-title {
  height: 48px;
  display: flex;
  align-items: center;
  justify-content: center;
  margin-bottom: 40px;
}

.userName,
.userPassword {
  display: flex;
  justify-content: center;
  margin-bottom: 24px;
}
.userName input {
  width: 75%;
  height: 48px;
  border: 1px solid rgb(74, 74, 74);
  padding: 0 12px;
}
.userPassword input {
  width: 75%;
  height: 48px;
  border: 1px solid rgb(74, 74, 74);
  padding: 0 12px;
}
input {
  margin: 0 auto;
  width: 50px;
  border-radius: 5px;
  box-shadow: 0 0 12px 1px rgba(117, 116, 116, 0.15);
}
button {
  width: 120px;
  height: 40px;
  border: 1px solid rgb(193, 191, 191);
  border-radius: 20px;
  margin: 0 auto;
  background-color: rgb(214, 211, 211);
}
</style>
