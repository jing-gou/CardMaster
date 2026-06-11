<template>
  <div class="containfather">
    <div class="loginBox">
      <h2 class="login-title">校园卡系统</h2>
      <div class="userName">
        <input type="text" id="username" placeholder="请输入卡号" v-model="cardid" />
      </div>
      <div class="userPassword">
        <input type="password" id="password" placeholder="请输入密码" v-model="password" />
      </div>
      <button class="login-button" id="login-button" @click="handleLogin">立即登录</button>
      <p class="error-msg" v-if="errMsg">{{ errMsg }}</p>
    </div>
  </div>
</template>

<script setup>
import { ref } from 'vue'
import { useRouter } from 'vue-router'
import { login } from '../api/login.js'

const router = useRouter()
const cardid = ref('')
const password = ref('')
const errMsg = ref('')

/* handleLogin: 点击登录按钮 */
const handleLogin = async () => {
  errMsg.value = ''

  /* 前端校验 */
  if (!cardid.value) {
    errMsg.value = '请输入卡号'
    return
  }
  if (!password.value) {
    errMsg.value = '请输入密码'
    return
  }

  try {
    const data = await login(cardid.value, password.value)

    if (data.code === 0) {
      /* 登录成功，根据 role 跳转 */
      if (data.role === 0) {
        router.push('/student')
      } else if (data.role === 1) {
        router.push('/admin')
      }
    } else {
      errMsg.value = data.message
    }
  } catch (e) {
    console.error('登录错误:', e)
    errMsg.value = '请求失败: ' + e.message
  }
}
</script>

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
  background: linear-gradient(45deg, #0779e3d3 50%, #dde1ea 50%);
  background-size: 200% 200%;
  animation: appleSlopeFlow 7s linear infinite;
  overflow: hidden;
}

@keyframes appleSlopeFlow {
  0% { background-position: 0 0; }
  100% { background-position: 100% 100%; }
}

.login-title {
  height: 48px;
  display: flex;
  align-items: center;
  justify-content: center;
  margin-bottom: 40px;
}

.userName, .userPassword {
  display: flex;
  justify-content: center;
  margin-bottom: 24px;
}
.userName input, .userPassword input {
  width: 75%;
  height: 48px;
  border: 1px solid rgb(74, 74, 74);
  padding: 0 12px;
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
  cursor: pointer;
}
button:hover {
  background-color: rgb(190, 190, 190);
}

.error-msg {
  color: red;
  text-align: center;
  margin-top: 20px;
  font-size: 14px;
}
</style>
