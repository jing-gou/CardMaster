<template>
  <div class="wrap">
    <div class="header">管理员系统</div>
    <div class="safari">
      <div class="item1 item" @click="Content(1)">充值</div>
      <div class="item2 item" @click="Content(2)">挂失</div>
      <div class="item3 item" @click="Content(3)">重置学生密码</div>
    </div>
    <main class="information">
      <div v-if="active === 1">
        <p>充值功能</p>
      </div>
      <div v-if="active === 2">
        <p>挂失功能</p>
      </div>
      <div v-if="active === 3">
        <p>重置学生密码</p>
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
</style>

<script setup>
import { ref } from "vue";
const active = ref(1);
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
</script>
