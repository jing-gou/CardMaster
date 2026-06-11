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
  background: linear-gradient(135deg, rgb(225, 225, 239) 50%, rgb(66, 75, 143));
}
.header {
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
}
.information {
  grid-column: 2 / 3;
  grid-row: 2 / 3;
}
.item:hover {
  width: 180px;
  border-radius: 20px;
  background-color: #ccc;
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
