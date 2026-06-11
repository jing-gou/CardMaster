<template>
  <div class="wrap">
    <div class="header">学生系统</div>
    <div class="safari">
      <div class="item1 item" @click="Content(1)">账户信息</div>
      <div class="item2 item" @click="Content(2)">上机日志</div>
      <div class="item3 item" @click="Content(3)">账户充值</div>
      <div class="item4 item" @click="Content(4)">按钮4</div>
      <div class="item5 item" @click="Content(5)">按钮5</div>
    </div>
    <main class="information">
      <div v-if="active === 1">
        <p>卡号{{ userData.cardid }}</p>

        <p>姓名{{ userData.name }}</p>

        <p>余额{{ userData.balance }}</p>

        <p>上机时间{{ userData.onTime }}</p>

        <p>上机地点{{ userData.onAddress }}</p>
      </div>
      <div v-if="active === 2">内容区域 2：上机日志</div>
      <div v-if="active === 3">内容区域 3：账户充值</div>
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
  background-color: antiquewhite;
}
.header {
  grid-column: 1/-1;
  background-color: aqua;
  font-size: 25px;
}
.safari {
  grid-column: 1/2;
  grid-row: 2 / 3;
}
.item {
  width: 220px;
  height: 50px;
  line-height: 50px;
  text-align: center;
}
.information {
  grid-column: 2 / 3;
  grid-row: 2 / 3;
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
