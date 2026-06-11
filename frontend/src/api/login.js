/**
 * 登录接口调用逻辑
 * 后端负责维护此文件
 */

/* ========== Token 管理 ========== */

/**
 * 保存 token 到本地
 * @param {string} token - token 字符串
 * @param {string} cardid - 卡号
 * @param {number} role - 角色
 */
export function saveToken(token, cardid, role) {
  localStorage.setItem('token', token)
  localStorage.setItem('cardid', cardid)
  localStorage.setItem('role', role)
}

/**
 * 获取本地存储的 token
 * @returns {string|null}
 */
export function getToken() {
  return localStorage.getItem('token')
}

/**
 * 获取本地存储的卡号
 * @returns {string|null}
 */
export function getCardid() {
  return localStorage.getItem('cardid')
}

/**
 * 获取本地存储的角色
 * @returns {string|null}
 */
export function getRole() {
  return localStorage.getItem('role')
}

/**
 * 清除 token（退出登录）
 */
export function clearToken() {
  localStorage.removeItem('token')
  localStorage.removeItem('cardid')
  localStorage.removeItem('role')
}

/**
 * 检查是否已登录
 * @returns {boolean}
 */
export function isLoggedIn() {
  return !!localStorage.getItem('token')
}

/**
 * 验证 token 是否有效
 * @returns {Promise<{code: number, cardid?: string, role?: number}>}
 */
export async function verifyToken() {
  const token = getToken()
  if (!token) {
    return { code: -1, message: '未登录' }
  }

  const body = 'token=' + encodeURIComponent(token)
  const resp = await fetch('/api/verify_token', {
    method: 'POST',
    headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
    body: body
  })
  return await resp.json()
}

/* ========== 登录接口 ========== */

/**
 * 用户登录
 * @param {string} cardid - 卡号
 * @param {string} password - 密码
 * @param {string} [address] - 上机地点（可选，默认"机房"）
 * @returns {Promise<{code: number, role?: number, message?: string}>}
 */
export async function login(cardid, password, address) {
  let body = 'cardid=' + encodeURIComponent(cardid) +
             '&password=' + encodeURIComponent(password)
  if (address) {
    body += '&address=' + encodeURIComponent(address)
  }

  const resp = await fetch('/api/login', {
    method: 'POST',
    headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
    body: body
  })
  return await resp.json()
}

/**
 * 创建学生
 * @param {string} cardid - 卡号
 * @param {string} name - 姓名
 * @param {string} stuid - 学号
 * @param {string} password - 初始密码
 * @returns {Promise<{code: number, message: string}>}
 */
export async function createStudent(cardid, name, stuid, password) {
  const body = 'cardid=' + encodeURIComponent(cardid) +
               '&name=' + encodeURIComponent(name) +
               '&stuid=' + encodeURIComponent(stuid) +
               '&password=' + encodeURIComponent(password)

  const resp = await fetch('/api/admin/create_student', {
    method: 'POST',
    headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
    body: body
  })
  return await resp.json()
}

/**
 * 充值
 * @param {string} cardid - 卡号
 * @param {number} amount - 充值金额
 * @returns {Promise<{code: number, message: string}>}
 */
export async function recharge(cardid, amount) {
  const body = 'cardid=' + encodeURIComponent(cardid) +
               '&amount=' + encodeURIComponent(amount)

  const resp = await fetch('/api/admin/recharge', {
    method: 'POST',
    headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
    body: body
  })
  return await resp.json()
}

/**
 * 挂失
 * @param {string} cardid - 卡号
 * @returns {Promise<{code: number, message: string}>}
 */
export async function reportLost(cardid) {
  const body = 'cardid=' + encodeURIComponent(cardid)

  const resp = await fetch('/api/admin/report_lost', {
    method: 'POST',
    headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
    body: body
  })
  return await resp.json()
}

/**
 * 重置密码
 * @param {string} cardid - 卡号
 * @param {string} newPassword - 新密码
 * @returns {Promise<{code: number, message: string}>}
 */
export async function resetPassword(cardid, newPassword) {
  const body = 'cardid=' + encodeURIComponent(cardid) +
               '&new_password=' + encodeURIComponent(newPassword)

  const resp = await fetch('/api/admin/reset_password', {
    method: 'POST',
    headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
    body: body
  })
  return await resp.json()
}

/**
 * 学生下机
 * @param {string} cardid - 卡号
 * @returns {Promise<{code: number, message: string}>}
 */
export async function endSession(cardid) {
  const body = 'cardid=' + encodeURIComponent(cardid)

  const resp = await fetch('/api/student/end_session', {
    method: 'POST',
    headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
    body: body
  })
  return await resp.json()
}

/**
 * 学生查询自己信息
 * @param {string} cardid - 卡号
 * @returns {Promise<{code: number, cardid?: string, name?: string, balance?: number}>}
 */
export async function getStudentInfo(cardid) {
  const body = 'cardid=' + encodeURIComponent(cardid)

  const resp = await fetch('/api/student/info', {
    method: 'POST',
    headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
    body: body
  })
  return await resp.json()
}

/**
 * 学生修改密码
 * @param {string} cardid - 卡号
 * @param {string} oldPassword - 旧密码
 * @param {string} newPassword - 新密码
 * @returns {Promise<{code: number, message: string}>}
 */
export async function changePassword(cardid, oldPassword, newPassword) {
  const body = 'cardid=' + encodeURIComponent(cardid) +
               '&old_password=' + encodeURIComponent(oldPassword) +
               '&new_password=' + encodeURIComponent(newPassword)

  const resp = await fetch('/api/student/change_password', {
    method: 'POST',
    headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
    body: body
  })
  return await resp.json()
}

/**
 * 查询上机记录
 * @param {string} cardid - 卡号
 * @returns {Promise<{code: number, records: Array}>}
 */
export async function getRecords(cardid) {
  const body = 'cardid=' + encodeURIComponent(cardid)

  const resp = await fetch('/api/student/records', {
    method: 'POST',
    headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
    body: body
  })
  return await resp.json()
}
