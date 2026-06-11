<template>
  <div class="wrap">
    <div class="header">
      学生系统
      <button class="logout-btn" @click="handleLogout">退出登录</button>
    </div>
    <div class="safari">
      <div class="item1 item" @click="Content(1)">账户信息</div>
      <div class="item2 item" @click="Content(2)">上机日志</div>
      <div class="item3 item" @click="Content(3)">账户充值</div>
      <div class="item4 item" @click="Content(4)">按钮4</div>
      <div class="item5 item" @click="Content(5)">按钮5</div>
    </div>
    <main class="information">
      <div v-if="active === 1">
        <p>卡号ID{{ userData.cardid }}</p>

        <p>姓名Name{{ userData.name }}</p>

        <p>余额Balance{{ userData.balance }}</p>

        <p>上机时间loginTime{{ userData.onTime }}</p>

        <p>上机地点loginAddress{{ userData.onAddress }}</p>
      </div>
      <div v-if="active === 2">内容区域 2：上机日志</div>
      <div v-if="active === 3">
        <div class="charge">
          <input
            type="text"
            placeholder="请输入充值金额"
            oninput="value = value.replace(/^\D*(\d*(?:\.\d{0,2})?).*$/g, '$1')"
          />
          <button class="charge-btn">充值</button>
        </div>
      </div>
      <div v-if="active === 4">内容区域 4：系统设置</div>
      <div v-if="active === 5">内容区域 5：系统设置</div>
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
  background: linear-gradient(135deg, rgb(225, 225, 239) 50%, rgb(66, 75, 143));
}
.header {
  line-height: 80px;
  grid-column: 1/-1;
  background: linear-gradient(
    135deg,
    rgb(198, 207, 246) 70%,
    rgb(138, 146, 206) 50%
  );
  font-size: 35px;
  font-weight: 400;
  padding-left: 75px;
}
.safari {
  margin-top: 90px;
  height: 600px;
  background-color: rgb(198, 207, 246);
  border-radius: 20px;
  box-shadow:
    0 4px 12px rgba(39, 39, 39, 0.15),
    0 -4px 12px rgb(255, 252, 252);
  grid-column: 1/2;
  grid-row: 2 / 3;
  display: flex;
  flex-direction: column;
  align-items: center;
}
.item {
  width: 220px;
  height: 50px;
  line-height: 50px;
  text-align: center;
  margin-top: 20px;
  transition: all 0.3s ease;
}
.item:hover {
  width: 180px;
  border-radius: 20px;
  background-color: #ccc;
  cursor: pointer;
  font-size: 18px;
}
.information {
  padding-left: 40px;
  font-size: 16px;
  line-height: 80px;
  border-radius: 20px;
  box-shadow:
    0 4px 12px rgba(67, 44, 44, 0.15),
    0 -4px 12px rgb(255, 252, 252);
  background-color: rgb(198, 207, 246);
  width: 1000px;
  height: 600px;
  margin-top: 90px;
  margin-left: 100px;
  grid-column: 2 / 3;
  grid-row: 2 / 3;
}
.item:hover {
  width: 180px;
  border-radius: 20px;
  background-color: #ccc;
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
input {
  width: 200px;
  height: 40px;
  border-radius: 20px;
  border: none;
  padding-left: 15px;
}
</style>

<script setup>
import { ref, onMounted } from "vue";
import { useRouter } from "vue-router";
import {
  isLoggedIn,
  getCardid,
  getStudentInfo,
  endSession,
} from "../api/login.js";

const isdebug = true; // 开发阶段允许直接访问学生界面，生产环境请设置为 false
const router = useRouter();
const active = ref(1);
const cardid = ref("");

const Content = (num) => {
  active.value = num;
};

const userData = ref({
  cardid: "",
  name: "",
  balance: "",
  onTime: "",
  onAddress: "",
});

/* 页面加载时检查登录状态 */
onMounted(async () => {
  if (!isLoggedIn() && !isdebug()) {
    router.push("/");
    return;
  }
  cardid.value = getCardid();
  /* 加载学生信息 */
  const data = await getStudentInfo(cardid.value);
  if (data.code === 0) {
    userData.value.cardid = data.cardid;
    userData.value.name = data.name;
    userData.value.balance = data.balance;
  }
});

/* 下机 */
const handleLogout = async () => {
  await endSession(cardid.value);
  localStorage.clear();
  router.push("/");
};
</script>
