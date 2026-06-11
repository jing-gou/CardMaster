<template>
  <div class="wrap">
    <div class="header">管理员系统 <button class="logout-btn" @click="handleLogout">退出登录</button></div>
    <div class="safari">
      <div class="item1 item" @click="Content(1)">充值</div>
      <div class="item2 item" @click="Content(2)">挂失</div>
      <div class="item3 item" @click="Content(3)">重置学生密码</div>
    </div>
    <main class="information">
      <div v-if="active === 1">
        <p>充值功能</p>
        <input v-model="chargeCardid" placeholder="请输入卡号" />
        <input v-model="chargeAmount" placeholder="请输入金额" />
        <button @click="handleRecharge">充值</button>
        <p v-if="chargeMsg">{{ chargeMsg }}</p>
      </div>
      <div v-if="active === 2">
        <p>挂失功能</p>
        <input v-model="lostCardid" placeholder="请输入卡号" />
        <button @click="handleReportLost">挂失</button>
        <p v-if="lostMsg">{{ lostMsg }}</p>
      </div>
      <div v-if="active === 3">
        <p>重置学生密码</p>
        <input v-model="resetCardid" placeholder="请输入卡号" />
        <input v-model="resetNewPwd" placeholder="请输入新密码" />
        <button @click="handleResetPwd">重置</button>
        <p v-if="resetMsg">{{ resetMsg }}</p>
      </div>
    </main>
  </div>
</template>

<style scoped>
.wrap {
  display: grid;
  grid-template-rows: auto 1fr;
  grid-template-columns: 220px 1fr;
  width: 100vw;
  height: 100vh;
  background-color: rgb(247, 243, 243);
}
.header {
  line-height: 80px;
  color: rgb(27 24 42);
  grid-column: 1/-1;
  background: linear-gradient(
    135deg,
    rgb(249, 243, 243) 70%,
    rgb(199, 193, 193) 50%
  );
  font-size: 35px;
  font-weight: 400;
  padding-left: 75px;
}
.safari {
  margin-top: 150px;
  height: 400px;
  grid-column: 1/2;
  grid-row: 2 / 3;
  display: flex;
  flex-direction: column;
  align-items: center;
  background-color: rgb(245, 238, 238);
  border-radius: 20px;
  box-shadow:
    0 4px 12px rgba(39, 39, 39, 0.15),
    0 -4px 12px rgb(255, 252, 252);
}
.item {
  font-size: 18px;
  text-shadow: 0 0 1px rgba(0, 0, 0, 0.08);
  color: rgb(27 24 42);
  width: 220px;
  height: 50px;
  line-height: 50px;
  text-align: center;
  margin-top: 40px;
  transition: font-size 0.3s ease;
}
.information {
  width: 1000px;
  height: 600px;
  margin-top: 100px;
  margin-left: 100px;
  background-color: rgb(245, 238, 238);
  border-radius: 20px;
  box-shadow:
    0 4px 12px rgba(39, 39, 39, 0.15),
    0 -4px 12px rgb(255, 252, 252);
  grid-column: 2 / 3;
  grid-row: 2 / 3;
}
.item:hover {
  width: 180px;
  border-radius: 20px;
  background-color: #ccc;
  font-size: 20px;
  cursor: pointer;
}
.logout-btn {
  float: right;
  margin-right: 30px;
  padding: 8px 20px;
  border: none;
  border-radius: 5px;
  background-color: #e74c3c;
  color: white;
  cursor: pointer;
  font-size: 14px;
}
.logout-btn:hover {
  background-color: #c0392b;
}
</style>

<script setup>
import { ref, onMounted } from "vue";
import { useRouter } from "vue-router";
import { isLoggedIn, clearToken, recharge, reportLost, resetPassword } from "../api/login.js";

const isdebug = true;
const router = useRouter();
const active = ref(1);

/* 充值相关 */
const chargeCardid = ref("");
const chargeAmount = ref("");
const chargeMsg = ref("");

/* 挂失相关 */
const lostCardid = ref("");
const lostMsg = ref("");

/* 重置密码相关 */
const resetCardid = ref("");
const resetNewPwd = ref("");
const resetMsg = ref("");

const Content = (num) => {
  active.value = num;
};

/* 页面加载时检查登录状态 */
onMounted(() => {
  if (!isLoggedIn() && !isdebug()) {
    router.push("/");
  }
});

/* 充值 */
const handleRecharge = async () => {
  if (!chargeCardid.value || !chargeAmount.value) {
    chargeMsg.value = "请输入卡号和金额";
    return;
  }
  const data = await recharge(chargeCardid.value, Number(chargeAmount.value));
  chargeMsg.value = data.message;
};

/* 挂失 */
const handleReportLost = async () => {
  if (!lostCardid.value) {
    lostMsg.value = "请输入卡号";
    return;
  }
  const data = await reportLost(lostCardid.value);
  lostMsg.value = data.message;
};

/* 重置密码 */
const handleResetPwd = async () => {
  if (!resetCardid.value || !resetNewPwd.value) {
    resetMsg.value = "请输入卡号和新密码";
    return;
  }
  const data = await resetPassword(resetCardid.value, resetNewPwd.value);
  resetMsg.value = data.message;
};

/* 退出登录 */
const handleLogout = () => {
  clearToken();
  router.push("/");
};
</script>
