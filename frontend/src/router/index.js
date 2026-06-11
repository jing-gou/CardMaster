import { createRouter, createWebHistory } from 'vue-router'

const router = createRouter({
  history: createWebHistory(import.meta.env.BASE_URL),
  routes: [
    {
      path: '/',
      name: 'login',
      component: () => import('../views/login.vue')
    },
    {
      path: '/student',
      name: 'student',
      component: () => import('../views/student.vue')
    },
    {
      path: '/admin',
      name: 'admin',
      component: () => import('../views/administrator.vue')
    }
  ]
})

export default router
