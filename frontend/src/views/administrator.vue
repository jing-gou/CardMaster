<template>
  <div class="wrap">
    <div class="header">
      CardMaster 管理端
      <button class="logout-btn" @click="handleLogout">退出登录</button>
    </div>
    <div class="safari">
      <div class="item" :class="{ active: active === 1 }" @click="Content(1)">学生管理</div>
      <div class="item" :class="{ active: active === 2 }" @click="Content(2)">收入统计</div>
    </div>
    <main class="information">
      <!-- 学生管理 -->
      <div v-if="active === 1" class="section">
        <h3>学生管理</h3>
        <div class="toolbar">
          <input v-model="searchQuery" placeholder="搜索卡号或姓名..." class="search-input" />
          <button class="submit-btn" @click="openModal('create', null)">+ 创建学生</button>
        </div>
        <table class="data-table">
          <thead>
            <tr>
              <th>卡号</th>
              <th>姓名</th>
              <th>学号</th>
              <th>余额</th>
              <th>状态</th>
              <th>在线</th>
              <th>操作</th>
            </tr>
          </thead>
          <tbody>
            <tr v-for="s in paginatedStudents" :key="s.cardid">
              <td>{{ s.cardid }}</td>
              <td>{{ s.name }}</td>
              <td>{{ s.stuid }}</td>
              <td class="balance-cell">¥{{ s.balance.toFixed(2) }}</td>
              <td>
                <span :class="'badge status-' + s.status">{{ statusText(s.status) }}</span>
              </td>
              <td>
                <span :class="s.isOnline ? 'badge online-on' : 'badge online-off'">
                  {{ s.isOnline ? '在线' : '离线' }}
                </span>
              </td>
              <td class="action-cell">
                <button class="action-btn" @click="openModal('recharge', s)">充值</button>
                <button class="action-btn" @click="openModal('resetPwd', s)">重置密码</button>
                <button class="action-btn warn" @click="openModal('reportLost', s)">挂失</button>
                <button class="action-btn success" @click="openModal('releaseLost', s)">解除挂失</button>
                <button class="action-btn danger" @click="openModal('delete', s)">删除</button>
              </td>
            </tr>
            <tr v-if="paginatedStudents.length === 0">
              <td colspan="7" class="empty-tip">暂无学生数据</td>
            </tr>
          </tbody>
        </table>
        <div class="pagination" v-if="totalPages > 1">
          <button :disabled="currentPage <= 1" @click="currentPage--">上一页</button>
          <span>{{ currentPage }} / {{ totalPages }}</span>
          <button :disabled="currentPage >= totalPages" @click="currentPage++">下一页</button>
        </div>
      </div>

      <!-- 收入统计 -->
      <div v-if="active === 2" class="section">
        <h3>收入统计</h3>
        <div class="stat-cards">
          <div class="stat-card">
            <p class="stat-label">累计收入</p>
            <p class="stat-value">¥{{ incomeStats.total.toFixed(2) }}</p>
          </div>
          <div class="stat-card">
            <p class="stat-label">本月收入</p>
            <p class="stat-value">¥{{ incomeStats.monthly.toFixed(2) }}</p>
          </div>
          <div class="stat-card">
            <p class="stat-label">今日收入</p>
            <p class="stat-value">¥{{ incomeStats.daily.toFixed(2) }}</p>
          </div>
        </div>
        <div ref="chartRef" class="chart-container"></div>
        <h3 style="margin-top: 30px;">充值记录</h3>
        <table class="data-table">
          <thead>
            <tr>
              <th>卡号</th>
              <th>金额</th>
              <th>日期</th>
              <th>时间</th>
            </tr>
          </thead>
          <tbody>
            <tr v-for="(r, i) in rechargeRecords" :key="i">
              <td>{{ r.cardid }}</td>
              <td class="balance-cell">¥{{ r.amount.toFixed(2) }}</td>
              <td>{{ r.date }}</td>
              <td>{{ r.time }}</td>
            </tr>
            <tr v-if="rechargeRecords.length === 0">
              <td colspan="4" class="empty-tip">暂无充值记录</td>
            </tr>
          </tbody>
        </table>
      </div>
    </main>

    <!-- 弹窗 -->
    <div v-if="showModal" class="modal-overlay" @click.self="closeModal">
      <div class="modal-content">
        <h3 v-if="modalType === 'create'">创建学生</h3>
        <h3 v-else-if="modalType === 'recharge'">充值 - {{ selectedStudent?.name }}</h3>
        <h3 v-else-if="modalType === 'resetPwd'">重置密码 - {{ selectedStudent?.name }}</h3>
        <h3 v-else-if="modalType === 'reportLost'">挂失确认</h3>
        <h3 v-else-if="modalType === 'releaseLost'">解除挂失确认</h3>
        <h3 v-else-if="modalType === 'delete'">删除确认</h3>

        <div v-if="modalType === 'create'">
          <div class="form-group"><input v-model="newCardid" placeholder="卡号" /></div>
          <div class="form-group"><input v-model="newName" placeholder="姓名" /></div>
          <div class="form-group"><input v-model="newStuid" placeholder="学号" /></div>
          <div class="form-group"><input v-model="newPwd" type="password" placeholder="初始密码" /></div>
        </div>

        <div v-if="modalType === 'recharge'">
          <div class="form-group">
            <input v-model="modalAmount" placeholder="充值金额"
              oninput="value = value.replace(/^\D*(\d*(?:\.\d{0,2})?).*$/g, '$1')" />
          </div>
        </div>

        <div v-if="modalType === 'resetPwd'">
          <div class="form-group">
            <input v-model="modalNewPwd" type="password" placeholder="新密码" />
          </div>
        </div>

        <p v-if="modalType === 'reportLost'" class="confirm-text">
          确定要 {{ selectedStudent?.status === 1 ? '解除挂失' : '挂失' }} <strong>{{ selectedStudent?.name }}</strong> 的卡吗？
        </p>
        <p v-if="modalType === 'releaseLost'" class="confirm-text">
          确定要解除挂失 <strong>{{ selectedStudent?.name }}</strong> 的卡吗？
        </p>
        <p v-if="modalType === 'delete'" class="confirm-text">
          确定要删除 <strong>{{ selectedStudent?.name }}</strong> 吗？此操作不可恢复。
        </p>

        <p v-if="modalMsg" :class="modalMsg.includes('成功') ? 'msg-success' : 'msg-error'">{{ modalMsg }}</p>

        <div class="modal-actions">
          <button class="submit-btn" @click="handleModalSubmit">确认</button>
          <button class="submit-btn cancel" @click="closeModal">取消</button>
        </div>
      </div>
    </div>
  </div>
</template>

<script setup>
import { ref, computed, onMounted, nextTick } from "vue"
import { useRouter } from "vue-router"
import {
  isLoggedIn, clearToken, createStudent, recharge, reportLost, releaseLost,
  resetPassword, deleteStudent,
  getAllStudents, getRechargeRecords, getIncomeStats
} from "../api/login.js"
import * as echarts from "echarts"

const router = useRouter()
const active = ref(1)

/* ========== Tab 1: 学生管理 ========== */
const students = ref([])
const searchQuery = ref("")
const currentPage = ref(1)
const pageSize = 10

const filteredStudents = computed(() => {
  if (!searchQuery.value) return students.value
  const q = searchQuery.value.toLowerCase()
  return students.value.filter(s =>
    s.cardid.toLowerCase().includes(q) ||
    s.name.toLowerCase().includes(q)
  )
})

const paginatedStudents = computed(() => {
  const start = (currentPage.value - 1) * pageSize
  return filteredStudents.value.slice(start, start + pageSize)
})

const totalPages = computed(() => {
  return Math.ceil(filteredStudents.value.length / pageSize) || 1
})

const statusText = (status) => {
  const map = { 0: '正常', 1: '已挂失', 2: '已冻结' }
  return map[status] || '未知'
}

const loadStudents = async () => {
  const data = await getAllStudents()
  if (data.code === 0) {
    students.value = data.students
  }
}

/* ========== 弹窗 ========== */
const showModal = ref(false)
const modalType = ref("")
const selectedStudent = ref(null)
const modalAmount = ref("")
const modalNewPwd = ref("")
const modalMsg = ref("")
const newCardid = ref("")
const newName = ref("")
const newStuid = ref("")
const newPwd = ref("")

const openModal = (type, student) => {
  modalType.value = type
  selectedStudent.value = student
  modalAmount.value = ""
  modalNewPwd.value = ""
  modalMsg.value = ""
  showModal.value = true
}

const closeModal = () => {
  showModal.value = false
  modalMsg.value = ""
}

const handleModalSubmit = async () => {
  let result
  const s = selectedStudent.value

  switch (modalType.value) {
    case 'recharge':
      if (!modalAmount.value || Number(modalAmount.value) <= 0) {
        modalMsg.value = "请输入有效金额"
        return
      }
      result = await recharge(s.cardid, Number(modalAmount.value))
      break
    case 'resetPwd':
      if (!modalNewPwd.value) {
        modalMsg.value = "请输入新密码"
        return
      }
      result = await resetPassword(s.cardid, modalNewPwd.value)
      break
    case 'releaseLost':
      result = await releaseLost(s.cardid)
      break
    case 'reportLost':
      result = await reportLost(s.cardid)
      break
    case 'delete':
      result = await deleteStudent(s.cardid)
      break
    case 'create':
      if (!newCardid.value || !newName.value || !newStuid.value || !newPwd.value) {
        modalMsg.value = "请填写完整信息"
        return
      }
      result = await createStudent(newCardid.value, newName.value, newStuid.value, newPwd.value)
      break
  }

  if (result && result.code === 0) {
    modalMsg.value = result.message
    await loadStudents()
    if (modalType.value === 'create') {
      newCardid.value = ""; newName.value = ""; newStuid.value = ""; newPwd.value = ""
    }
    setTimeout(closeModal, 800)
  } else if (result) {
    modalMsg.value = result.message
  }
}

/* ========== Tab 2: 收入统计 ========== */
const incomeStats = ref({ total: 0, monthly: 0, daily: 0 })
const rechargeRecords = ref([])
const chartRef = ref(null)
let chartInstance = null

const loadIncomeData = async () => {
  const statsData = await getIncomeStats()
  if (statsData.code === 0) {
    incomeStats.value = statsData
  }
  const recordsData = await getRechargeRecords()
  if (recordsData.code === 0) {
    rechargeRecords.value = recordsData.records
  }
  await nextTick()
  renderChart()
}

const renderChart = () => {
  if (!chartRef.value) return
  if (chartInstance) chartInstance.dispose()
  chartInstance = echarts.init(chartRef.value)

  const dailyMap = {}
  rechargeRecords.value.forEach(r => {
    if (!dailyMap[r.date]) dailyMap[r.date] = 0
    dailyMap[r.date] += r.amount
  })
  const dates = Object.keys(dailyMap).sort()
  const amounts = dates.map(d => Math.round(dailyMap[d] * 100) / 100)

  chartInstance.setOption({
    title: { text: '收入趋势', left: 'center', textStyle: { fontSize: 16, color: '#28645a' } },
    tooltip: { trigger: 'axis', formatter: '{b}<br/>收入: ¥{c}' },
    grid: { left: 60, right: 30, top: 50, bottom: 30 },
    xAxis: { type: 'category', data: dates, boundaryGap: false },
    yAxis: { type: 'value', name: '金额 (元)' },
    series: [{
      data: amounts,
      type: 'line',
      smooth: true,
      symbol: 'circle',
      symbolSize: 8,
      lineStyle: { color: '#28645a', width: 3 },
      itemStyle: { color: '#28645a' },
      areaStyle: {
        color: new echarts.graphic.LinearGradient(0, 0, 0, 1, [
          { offset: 0, color: 'rgba(40,100,90,0.4)' },
          { offset: 1, color: 'rgba(40,100,90,0.05)' }
        ])
      }
    }]
  })
}

/* ========== Tab 切换 ========== */
const Content = async (num) => {
  active.value = num
  if (num === 1) {
    await loadStudents()
  } else if (num === 2) {
    await loadIncomeData()
  }
}

/* ========== 生命周期 ========== */
onMounted(async () => {
  if (!isLoggedIn()) {
    router.push("/")
    return
  }
  await loadStudents()
})

/* ========== 退出登录 ========== */
const handleLogout = () => {
  clearToken()
  router.push("/")
}
</script>

<style scoped>
.wrap {
  display: grid;
  grid-template-rows: auto 1fr;
  grid-template-columns: 220px 1fr;
  gap: 20px;
  padding: 0 20px 20px;
  box-sizing: border-box;
  width: 100%;
  height: 100vh;
  overflow: hidden;
  background: linear-gradient(135deg, rgb(218, 232, 226) 50%, rgb(40, 100, 90));
  font-family: "Microsoft YaHei", sans-serif;
}

.header {
  line-height: 80px;
  grid-column: 1/-1;
  background: linear-gradient(135deg, rgb(185, 218, 208) 70%, rgb(110, 165, 150) 50%);
  font-size: 28px;
  font-weight: 600;
  color: rgb(40, 100, 90);
  padding: 0 30px 0 40px;
  display: flex;
  align-items: center;
  justify-content: space-between;
}

.safari {
  background-color: rgb(185, 218, 208);
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
  color: rgb(40, 100, 90);
}

.item:hover {
  background-color: rgba(255, 255, 255, 0.6);
  transform: translateX(4px);
}

.item.active {
  background-color: rgb(110, 165, 150);
  color: white;
  font-weight: 600;
  box-shadow: 0 4px 12px rgba(110, 165, 150, 0.4);
}

.information {
  padding: 30px 40px;
  font-size: 16px;
  border-radius: 16px;
  box-shadow: 0 4px 16px rgba(67, 44, 44, 0.1), 0 -4px 16px rgb(255, 252, 252);
  background-color: rgb(185, 218, 208);
  grid-column: 2 / 3;
  grid-row: 2 / 3;
  overflow-y: auto;
}

h3 {
  font-size: 20px;
  color: rgb(40, 100, 90);
  margin-bottom: 20px;
  padding-bottom: 12px;
  border-bottom: 2px solid rgb(110, 165, 150);
  font-weight: 600;
}

.section {
  padding: 5px 0;
}

/* 工具栏 */
.toolbar {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 20px;
  gap: 15px;
}

.search-input {
  flex: 1;
  max-width: 350px;
  height: 42px;
  border-radius: 12px;
  border: 1px solid rgba(110, 165, 150, 0.5);
  padding: 0 18px;
  font-size: 14px;
  outline: none;
  transition: all 0.3s ease;
  background-color: rgba(255, 255, 255, 0.7);
}

.search-input:focus {
  border-color: rgb(40, 100, 90);
  background-color: white;
  box-shadow: 0 0 0 3px rgba(40, 100, 90, 0.1);
}

/* 表格 */
.data-table {
  width: 100%;
  border-collapse: collapse;
  border-radius: 12px;
  overflow: hidden;
}

.data-table th {
  background-color: rgb(110, 165, 150);
  color: white;
  padding: 12px 14px;
  text-align: left;
  font-weight: 500;
  font-size: 13px;
  white-space: nowrap;
}

.data-table td {
  padding: 11px 14px;
  border-bottom: 1px solid rgba(110, 165, 150, 0.2);
  font-size: 13px;
}

.data-table tbody tr {
  transition: background-color 0.2s;
}

.data-table tbody tr:hover {
  background-color: rgba(255, 255, 255, 0.4);
}

.balance-cell {
  font-weight: 600;
  color: rgb(231, 76, 60);
}

.empty-tip {
  text-align: center;
  color: rgb(150, 150, 150);
  padding: 30px 0 !important;
}

/* 徽章 */
.badge {
  display: inline-block;
  padding: 3px 10px;
  border-radius: 20px;
  font-size: 12px;
  font-weight: 500;
}

.status-0 {
  background-color: rgba(46, 204, 113, 0.15);
  color: rgb(39, 174, 96);
}

.status-1 {
  background-color: rgba(243, 156, 18, 0.15);
  color: rgb(211, 132, 0);
}

.status-2 {
  background-color: rgba(231, 76, 60, 0.15);
  color: rgb(192, 57, 43);
}

.online-on {
  background-color: rgba(46, 204, 113, 0.15);
  color: rgb(39, 174, 96);
}

.online-off {
  background-color: rgba(150, 150, 150, 0.15);
  color: rgb(120, 120, 120);
}

/* 操作按钮 */
.action-cell {
  display: flex;
  gap: 6px;
  flex-wrap: wrap;
}

.action-btn {
  padding: 5px 10px;
  border: none;
  border-radius: 8px;
  background-color: rgb(110, 165, 150);
  color: white;
  font-size: 12px;
  cursor: pointer;
  transition: all 0.2s;
  white-space: nowrap;
}

.action-btn:hover {
  background-color: rgb(40, 100, 90);
  transform: translateY(-1px);
}

.action-btn.warn {
  background-color: rgb(243, 156, 18);
}

.action-btn.warn:hover {
  background-color: rgb(211, 132, 0);
}

.action-btn.danger {
  background-color: rgb(231, 76, 60);
}

.action-btn.danger:hover {
  background-color: rgb(192, 57, 43);
}

/* 分页 */
.pagination {
  display: flex;
  justify-content: center;
  align-items: center;
  gap: 15px;
  margin-top: 20px;
}

.pagination button {
  padding: 8px 16px;
  border: none;
  border-radius: 8px;
  background-color: rgb(110, 165, 150);
  color: white;
  font-size: 13px;
  cursor: pointer;
  transition: all 0.2s;
}

.pagination button:hover:not(:disabled) {
  background-color: rgb(40, 100, 90);
}

.pagination button:disabled {
  opacity: 0.5;
  cursor: not-allowed;
}

.pagination span {
  font-size: 14px;
  color: rgb(40, 100, 90);
  font-weight: 500;
}

/* 统计卡片 */
.stat-cards {
  display: flex;
  gap: 20px;
  margin-bottom: 25px;
}

.stat-card {
  flex: 1;
  background: linear-gradient(135deg, rgb(40, 100, 90) 0%, rgb(110, 165, 150) 100%);
  color: white;
  padding: 24px;
  border-radius: 16px;
  box-shadow: 0 4px 12px rgba(40, 100, 90, 0.3);
}

.stat-label {
  font-size: 13px;
  opacity: 0.85;
  margin-bottom: 8px;
}

.stat-value {
  font-size: 28px;
  font-weight: 700;
}

/* 图表 */
.chart-container {
  width: 100%;
  height: 300px;
  background-color: rgba(255, 255, 255, 0.5);
  border-radius: 16px;
  margin-bottom: 10px;
}

/* 弹窗 */
.modal-overlay {
  position: fixed;
  top: 0;
  left: 0;
  width: 100%;
  height: 100%;
  background-color: rgba(0, 0, 0, 0.5);
  display: flex;
  justify-content: center;
  align-items: center;
  z-index: 1000;
}

.modal-content {
  background-color: white;
  border-radius: 20px;
  padding: 30px 35px;
  min-width: 380px;
  max-width: 450px;
  box-shadow: 0 20px 60px rgba(0, 0, 0, 0.3);
}

.modal-content h3 {
  font-size: 20px;
  color: rgb(40, 100, 90);
  margin-bottom: 20px;
  padding-bottom: 12px;
  border-bottom: 2px solid rgb(110, 165, 150);
}

.form-group {
  margin-bottom: 16px;
}

.form-group input {
  width: 100%;
  height: 44px;
  border-radius: 12px;
  border: 1px solid rgba(110, 165, 150, 0.5);
  padding: 0 16px;
  font-size: 14px;
  outline: none;
  transition: all 0.3s ease;
  background-color: rgba(245, 245, 250, 0.8);
  box-sizing: border-box;
}

.form-group input:focus {
  border-color: rgb(40, 100, 90);
  background-color: white;
  box-shadow: 0 0 0 3px rgba(40, 100, 90, 0.1);
}

.confirm-text {
  font-size: 15px;
  color: rgb(80, 80, 80);
  line-height: 1.6;
  margin-bottom: 10px;
}

.msg-success {
  color: rgb(39, 174, 96);
  margin-top: 12px;
  font-weight: 500;
  font-size: 14px;
}

.msg-error {
  color: rgb(231, 76, 60);
  margin-top: 12px;
  font-weight: 500;
  font-size: 14px;
}

.modal-actions {
  display: flex;
  gap: 12px;
  margin-top: 20px;
}

.submit-btn {
  flex: 1;
  height: 44px;
  border: none;
  border-radius: 12px;
  background-color: rgb(110, 165, 150);
  color: white;
  font-size: 14px;
  font-weight: 500;
  cursor: pointer;
  transition: all 0.3s ease;
}

.submit-btn:hover {
  background-color: rgb(40, 100, 90);
  transform: translateY(-1px);
}

.cancel {
  background-color: rgb(200, 200, 200);
}

.cancel:hover {
  background-color: rgb(170, 170, 170);
}

/* 退出按钮 */
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

/* 滚动条 */
.information::-webkit-scrollbar {
  width: 6px;
}

.information::-webkit-scrollbar-track {
  background: transparent;
}

.information::-webkit-scrollbar-thumb {
  background-color: rgba(110, 165, 150, 0.5);
  border-radius: 3px;
}

.information::-webkit-scrollbar-thumb:hover {
  background-color: rgba(110, 165, 150, 0.7);
}
</style>
