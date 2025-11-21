import { createRouter, createWebHistory } from 'vue-router'
import HomeView from '../views/HomeView.vue'
import LoginView from '../views/LoginView.vue'
import RegisterView from '../views/RegisterView.vue'
import ProfileView from '../views/ProfileView.vue'
import NotFoundView from '../views/NotFoundView.vue'
import ShopView from '../views/ShopView.vue'
import QueryPhotoView from '../views/QueryPhotoView.vue'
import QueryOrderView from '../views/QueryOrderView.vue'
import QueryTransportView from '../views/QueryTransportView.vue'
import QueryMoneyView from '../views/QueryMoneyView.vue'
import QueryOrderNumView from '../views/QueryOrderNumView.vue'
import QueryOfferView from '../views/QueryOfferView.vue'
import QuerySeasonView from '../views/QuerySeasonView.vue'
import NewOrderView from '../views/NewOrderView.vue'
import OrderView from '../views/OrderView.vue'
import OrderDetailedView from '../views/OrderDetailedView.vue'

const routes = [
  {
    path: '/',
    name: 'home',
    component: HomeView
  },
  {
    path: '/shop/',
    name: 'shop',
    component: ShopView
  },
  {
    path: '/login/',
    name: 'login',
    component: LoginView
  },
  {
    path: '/register/',
    name: 'register',
    component: RegisterView
  },
  {
    path: '/profile/:userID/',
    name: 'profile',
    component: ProfileView
  },
  {
    path: '/404/',
    name: '404',
    component: NotFoundView
  },
  {
    path: '/query_photo/',
    name: 'query_photo',
    component: QueryPhotoView
  },
  {
    path: '/query_order/',
    name: 'query_order',
    component: QueryOrderView
  },
  {
    path: '/query_transport/',
    name: 'query_transport',
    component: QueryTransportView
  },
  {
    path: '/query_season/',
    name: 'query_season',
    component: QuerySeasonView
  },
  {
    path: '/query_money/',
    name: 'query_money',
    component: QueryMoneyView
  },
  {
    path: '/query_ordernum/',
    name: 'query_ordernum',
    component: QueryOrderNumView
  },
  {
    path: '/query_offer/',
    name: 'query_offer',
    component: QueryOfferView
  },
  {
    path: '/new_order/:productID/',
    name: 'new_order',
    component: NewOrderView
  },
  {
    path: '/order/',
    name: 'order',
    component: OrderView
  },
  {
    path: '/order_detail/:orderID/',
    name: 'order_detail',
    component: OrderDetailedView
  },
  {
    path: '/:catchAll(.*)',
    redirect: '/404/',
  },
]

const router = createRouter({
  history: createWebHistory(),
  routes
})

export default router
