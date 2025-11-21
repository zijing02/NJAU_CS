import { createStore } from 'vuex'
import ModuleUser from './user'
export default createStore({
  state: {//存储所有数据
  },
  getters: {//读取数据信息，对数据进行一些调整，不能修改
  },
  mutations: {//进行所有修改操作，不能进行异步操作
  },
  actions: {//定义对state(数据)的各种操作，不能修改
    //增删改查
  },
  modules: {//对类进行分割
    user: ModuleUser,
  }
})
