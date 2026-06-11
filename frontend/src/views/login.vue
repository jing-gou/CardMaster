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
      <button class="login-button" id="login-button">登录</button>
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
  height: 100vh;
  display: flex;
  justify-content: center;
  align-items: center;
  background: linear-gradient(45deg, #dde1ea 50%, #187bd6d3 50%);
}
.loginBox {
  margin-top: -100px;
  width: 450px;
  height: 550px;
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
  animation: appleSlopeFlow 15s linear infinite;
  overflow: hidden;
}

/* 斜向流动关键帧 */
@keyframes appleSlopeFlow {
  0% {
    background-position: 0 0;
  }
  50% {
    background-position: 100% 100%;
  }
  100% {
    background-position: 0 0;
  }
}
.login-title {
  height: 48px;
  display: flex;
  align-items: center;
  justify-content: center;
  margin-bottom: 40px;
}

h2 {
  font-size: 28px;
  color: rgb(56 50 85);
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
  background-color: rgb(237, 235, 235);
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
  transition: all 0.3s ease;
  cursor: pointer;
  font-size: 16px;
  margin-top: 20px;
  font-weight: 400;
}
button:hover {
  background-color: rgb(193, 191, 191);
  transform: scale(1.05);
}
</style>
