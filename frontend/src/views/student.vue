<template>
  <div class="wrap">
    <div class="header">
      CardMaster 学生端
      <button class="logout-btn" @click="handleLogout">下机</button>
    </div>
    <div class="safari">
      <div class="item" :class="{ active: active === 1 }" @click="Content(1)">账户信息</div>
      <div class="item" :class="{ active: active === 2 }" @click="Content(2)">上机日志</div>
      <div class="item" :class="{ active: active === 3 }" @click="Content(3)">修改密码</div>
    </div>
    <main class="information">
      <!-- 账户信息 -->
      <div v-if="active === 1" class="info-card">
        <h3>个人信息</h3>
        <div class="info-row">
          <span class="label">卡号</span>
          <span class="value">{{ userData.cardid }}</span>
        </div>
        <div class="info-row">
          <span class="label">姓名</span>
          <span class="value">{{ userData.name }}</span>
        </div>
        <div class="info-row">
          <span class="label">余额</span>
          <span class="value highlight">¥{{ userData.balance }}</span>
        </div>
        <div class="info-row">
          <span class="label">上机时间</span>
          <span class="value">{{ userData.onTime || '未上机' }}</span>
        </div>
        <div class="info-row">
          <span class="label">上机地点</span>
          <span class="value">{{ userData.onAddress || '未上机' }}</span>
        </div>
      </div>

      <!-- 上机日志 -->
      <div v-if="active === 2" class="log-section">
        <h3>上机日志</h3>
        <table v-if="records.length > 0" class="log-table">
          <thead>
            <tr>
              <th>日期</th>
              <th>时间</th>
              <th>时长(分钟)</th>
              <th>地点</th>
              <th>状态</th>
            </tr>
          </thead>
          <tbody>
            <tr v-for="(rec, index) in records" :key="index">
              <td>{{ rec.date }}</td>
              <td>{{ rec.time }}</td>
              <td>{{ rec.duration }}</td>
              <td>{{ rec.address }}</td>
              <td>
                <span :class="rec.isOnline ? 'status-on' : 'status-off'">
                  {{ rec.isOnline ? '上机中' : '已下机' }}
                </span>
              </td>
            </tr>
          </tbody>
        </table>
        <p v-else class="empty-tip">暂无上机记录</p>
      </div>

      <!-- 修改密码 -->
      <div v-if="active === 3" class="pwd-section">
        <h3>修改密码</h3>
        <div class="form-group">
          <input v-model="oldPwd" type="password" placeholder="请输入旧密码" />
        </div>
        <div class="form-group">
          <input v-model="newPwd" type="password" placeholder="请输入新密码" />
        </div>
        <button class="submit-btn" @click="handleChangePwd">确认修改</button>
        <p v-if="pwdMsg" :class="pwdMsg.includes('成功') ? 'msg-success' : 'msg-error'">{{ pwdMsg }}</p>
      </div>
    </main>
  </div>
</template>

<style scoped>
.wrap {
  display: grid;
  grid-template-rows: auto 1fr;
  grid-template-columns: 220px 1fr;
  gap: 20px;
  padding: 20px 20px 20px 20px;
  box-sizing: border-box;
  width: 100%;
  height: 100vh;
  overflow: hidden;
  background: linear-gradient(135deg, rgb(225, 225, 239) 50%, rgb(66, 75, 143));
  font-family: "Microsoft YaHei", sans-serif;
}

.header {
  line-height: 80px;
  grid-column: 1/-1;
  background: linear-gradient(135deg, rgb(198, 207, 246) 70%, rgb(138, 146, 206) 50%);
  font-size: 28px;
  font-weight: 600;
  color: rgb(66, 75, 143);
  padding: 0 30px 0 40px;
  display: flex;
  align-items: center;
  justify-content: space-between;
  border-radius: 16px;
}

.safari {
  background-color: rgb(198, 207, 246);
  border-radius: 16px;
  box-shadow: 0 4px 16px rgba(39, 39, 39, 0.1), 0 -4px 16px rgb(255, 252, 252);
  grid-column: 1/2;
  grid-row: 2 / 3;
  display: flex;
  flex-direction: column;
  align-items: center;
  padding: 30px 20px;
  gap: 10px;
}

.item {
  width: 100%;
  height: 48px;
  line-height: 48px;
  text-align: center;
  border-radius: 12px;
  transition: all 0.3s ease;
  cursor: pointer;
  font-size: 15px;
  color: rgb(66, 75, 143);
}

.item:hover {
  background-color: rgba(255, 255, 255, 0.6);
  transform: translateX(4px);
}

.item.active {
  background-color: rgb(138, 146, 206);
  color: white;
  font-weight: 600;
  box-shadow: 0 4px 12px rgba(138, 146, 206, 0.4);
}

.information {
  padding: 30px 40px;
  font-size: 16px;
  border-radius: 16px;
  box-shadow: 0 4px 16px rgba(67, 44, 44, 0.1), 0 -4px 16px rgb(255, 252, 252);
  background-color: rgb(198, 207, 246);
  grid-column: 2 / 3;
  grid-row: 2 / 3;
  overflow-y: auto;
}

h3 {
  font-size: 20px;
  color: rgb(66, 75, 143);
  margin-bottom: 20px;
  padding-bottom: 12px;
  border-bottom: 2px solid rgb(138, 146, 206);
  font-weight: 600;
}

/* 账户信息样式 */
.info-card {
  padding: 10px 0;
}

.info-row {
  display: flex;
  padding: 10px 10px;
  border-bottom: 1px solid rgba(138, 146, 206, 0.2);
  transition: background-color 0.2s;
}

.info-row:hover {
  background-color: rgba(255, 255, 255, 0.3);
  border-radius: 8px;
}

.label {
  width: 120px;
  color: rgb(100, 100, 100);
  font-weight: 500;
  font-size: 15px;
}

.value {
  flex: 1;
  color: rgb(50, 50, 50);
  font-size: 15px;
}

.value.highlight {
  color: rgb(231, 76, 60);
  font-weight: 700;
  font-size: 20px;
}

/* 上机日志样式 */
.log-section {
  padding: 10px 0;
}

.log-table {
  width: 100%;
  border-collapse: collapse;
  margin-top: 15px;
  border-radius: 12px;
  overflow: hidden;
}

.log-table th {
  background-color: rgb(138, 146, 206);
  color: white;
  padding: 14px 18px;
  text-align: left;
  font-weight: 500;
  font-size: 14px;
}

.log-table td {
  padding: 14px 18px;
  border-bottom: 1px solid rgba(138, 146, 206, 0.2);
  font-size: 14px;
}

.log-table tbody tr {
  transition: background-color 0.2s;
}

.log-table tbody tr:hover {
  background-color: rgba(255, 255, 255, 0.4);
}

.status-on {
  color: rgb(46, 204, 113);
  font-weight: 600;
  padding: 4px 12px;
  background-color: rgba(46, 204, 113, 0.1);
  border-radius: 20px;
  font-size: 13px;
}

.status-off {
  color: rgb(150, 150, 150);
  padding: 4px 12px;
  background-color: rgba(150, 150, 150, 0.1);
  border-radius: 20px;
  font-size: 13px;
}

.empty-tip {
  text-align: center;
  color: rgb(150, 150, 150);
  margin-top: 60px;
  font-size: 15px;
}

/* 修改密码样式 */
.pwd-section {
  padding: 10px 0;
}

.form-group {
  margin-bottom: 20px;
}

.form-group input {
  width: 320px;
  height: 48px;
  border-radius: 12px;
  border: 1px solid rgba(138, 146, 206, 0.5);
  padding: 0 18px;
  font-size: 15px;
  outline: none;
  transition: all 0.3s ease;
  background-color: rgba(255, 255, 255, 0.7);
}

.form-group input:focus {
  border-color: rgb(66, 75, 143);
  background-color: white;
  box-shadow: 0 0 0 3px rgba(66, 75, 143, 0.1);
}

.form-group input::placeholder {
  color: rgb(150, 150, 150);
}

.submit-btn {
  width: 130px;
  height: 48px;
  border: none;
  border-radius: 12px;
  background-color: rgb(138, 146, 206);
  color: white;
  font-size: 15px;
  font-weight: 500;
  cursor: pointer;
  transition: all 0.3s ease;
  box-shadow: 0 4px 12px rgba(138, 146, 206, 0.3);
}

.submit-btn:hover {
  background-color: rgb(66, 75, 143);
  transform: translateY(-2px);
  box-shadow: 0 6px 16px rgba(66, 75, 143, 0.4);
}

.msg-success {
  color: rgb(46, 204, 113);
  margin-top: 15px;
  font-weight: 500;
}

.msg-error {
  color: rgb(231, 76, 60);
  margin-top: 15px;
  font-weight: 500;
}

.logout-btn {
  padding: 10px 24px;
  border: none;
  border-radius: 50px;
  background-color: rgb(231, 76, 60);
  color: white;
  cursor: pointer;
  font-size: 14px;
  font-weight: 500;
  transition: all 0.3s ease;
  box-shadow: 0 4px 12px rgba(231, 76, 60, 0.3);
}

.logout-btn:hover {
  background-color: rgb(192, 57, 43);
  transform: translateY(-2px);
  box-shadow: 0 6px 16px rgba(192, 57, 43, 0.4);
}

/* 滚动条样式 */
.information::-webkit-scrollbar {
  width: 6px;
}

.information::-webkit-scrollbar-track {
  background: transparent;
}

.information::-webkit-scrollbar-thumb {
  background-color: rgba(138, 146, 206, 0.5);
  border-radius: 3px;
}

.information::-webkit-scrollbar-thumb:hover {
  background-color: rgba(138, 146, 206, 0.7);
}
</style>

<script setup>
import { ref, onMounted } from "vue";
import { useRouter } from "vue-router";
import { isLoggedIn, getCardid, getStudentInfo, endSession, changePassword, getRecords } from "../api/login.js";

const router = useRouter();
const active = ref(1);
const cardid = ref("");

/* 修改密码相关 */
const oldPwd = ref("");
const newPwd = ref("");
const pwdMsg = ref("");

/* 上机日志相关 */
const records = ref([]);

const userData = ref({
  cardid: "",
  name: "",
  balance: "",
  onTime: "",
  onAddress: "",
});

const Content = async (num) => {
  active.value = num;
  if (num === 2 && cardid.value) {
    const data = await getRecords(cardid.value);
    if (data.code === 0) {
      records.value = data.records;
    }
  }
};

/* 页面加载时检查登录状态 */
onMounted(async () => {
  if (!isLoggedIn()) {
    router.push("/");
    return;
  }
  cardid.value = getCardid();
  const data = await getStudentInfo(cardid.value);
  if (data.code === 0) {
    userData.value.cardid = data.cardid;
    userData.value.name = data.name;
    userData.value.balance = data.balance;
    userData.value.onTime = data.time;
    userData.value.onAddress = data.address;
  }
});

/* 修改密码 */
const handleChangePwd = async () => {
  if (!oldPwd.value || !newPwd.value) {
    pwdMsg.value = "请输入旧密码和新密码";
    return;
  }
  const data = await changePassword(cardid.value, oldPwd.value, newPwd.value);
  if (data.code === 0) {
    pwdMsg.value = "修改成功";
    oldPwd.value = "";
    newPwd.value = "";
  } else {
    pwdMsg.value = data.message;
  }
};

/* 退出登录 */
const handleLogout = async () => {
  await endSession(cardid.value);
  localStorage.clear();
  router.push("/");
};
</script>