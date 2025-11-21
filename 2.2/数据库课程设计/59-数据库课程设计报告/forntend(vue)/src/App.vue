<template>
  <NavBar />
  <div id="app">
    <router-view :key="$route.fullPath" />
  </div>
</template>

<script>
import 'bootstrap/dist/css/bootstrap.css';
import 'bootstrap/dist/js/bootstrap';
import NavBar from './components/NavBar.vue';

export default {
  name: "App",
  components: {
    NavBar,
  }
}
</script>

<style scoped>
#app {
  display: flex;
  height: 100vh;
  width: 100%;
  flex-direction: column;
  height: 100vh;
  /* 确保元素占据整个视口高度 */
  margin: 0;
  /* 移除默认的margin */
  padding: 0;
  /* 移除默认的padding */
  box-sizing: border-box;
  /* 确保padding和border不会增加元素的总宽度和高度 */
  background-image: url("./img/wallpaper.jpg");
  /* 可选：设置背景图片不重复，并覆盖整个元素 */
  background-repeat: no-repeat;
  background-size: cover;
  background-position: center;
  /* 可选：设置背景图片居中 */
}
</style>
