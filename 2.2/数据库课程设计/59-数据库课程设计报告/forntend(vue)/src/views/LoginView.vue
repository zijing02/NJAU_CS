<template>
    <div class="row justify-content-md-center">
        <div class="header">
            <h1>登录</h1>
        </div>
        <div class="col-8">
            <form @submit.prevent="login">
                <div class="mb-3">
                    <label for="username" class="form-label">用户名</label>
                    <input v-model="username" type="txet" class="form-control" id="username">
                </div>
                <div class="mb-3">
                    <label for="password" class="form-label">密码</label>
                    <input v-model="password" type="password" class="form-control" id="password">
                </div>
                <div class="error-message">{{ error_message }}</div>
                <button type="submit" class="btn btn-primary">登录</button>
            </form>
        </div>
    </div>
    <!-- <div id="liveAlertPlaceholder"></div>
    <button type="button" class="btn btn-primary" id="liveAlertBtn">显示警告框（alert）</button> -->
</template>

<script>
import { useStore } from 'vuex';
import { ref } from 'vue';
import router from '@/router';

export default {
    name: 'LoginView',
    setup() {
        const store = useStore();
        let username = ref("");
        let password = ref("");
        let error_message = ref("");

        let login = () => {
            error_message.value = "";

            store.dispatch("login", {
                username: username.value,
                password: password.value,   
                success(userID) {
                    console.log("success");
                    console.log(userID);
                    router.push({
                        name: 'profile',
                        params: { userID: userID },
                    });
                    
                },
                error() {
                    error_message.value = "用户或密码错误！";
                    console.log("failed");
                },
            });
            console.log(username.value, password.value);
        };

        //显示警告框
        // const alertPlaceholder = document.getElementById('liveAlertPlaceholder');

        // const alert = (message, type) => {
        //     const wrapper = document.createElement('div')
        //     wrapper.innerHTML = [
        //         `<div class="alert alert-${type} alert-dismissible" role="alert">`,
        //         `   <div>${message}</div>`,
        //         '   <button type="button" class="btn-close" data-bs-dismiss="alert" aria-label="Close"></button>',
        //         '</div>'
        //     ].join('');

        //     alertPlaceholder.append(wrapper);
        // };

        // const alertTrigger = document.getElementById('liveAlertBtn');
        // if (alertTrigger) {
        //     alertTrigger.addEventListener('click', () => {
        //         alert('Nice, you triggered this alert message!', 'success')
        //     })
        // }

        return {
            username, password, error_message, login,
            //alertPlaceholder,alert,alertTrigger,
        };
    },
}
</script>

<style scoped>
button {
    width: 100%;
    margin: 40px 0;
}

.error-message {
    color: red;
}

.row {
    margin: 7% auto;
    width: 380px;
    border-radius: 25px;
    background-color: rgba(0, 0, 0, 0.1);
    /* 阴影部分 */
    box-shadow: 0 0 17px #333;
}

.header {
    text-align: center;
    padding-top: 50px;
}

.header h1 {
    margin-bottom: 40px;
}
</style>