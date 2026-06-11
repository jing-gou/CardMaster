/**
 * 登录接口调用逻辑
 * 后端负责维护此文件
 */

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
