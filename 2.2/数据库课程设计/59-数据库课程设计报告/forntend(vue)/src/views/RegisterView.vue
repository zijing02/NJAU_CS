<template>
    <div class="row justify-content-md-center">
        <div class="header">
            <h1>注册</h1>
        </div>
        <div class="col-8">
            <form @submit.prevent="register">
                <div class="mb-3">
                    <label for="username" class="form-label">用户名</label>
                    <input v-model="username" type="txet" class="form-control" id="username">
                </div>
                <div class="mb-3">
                    <label for="password" class="form-label">密码</label>
                    <input v-model="password" type="password" class="form-control" id="password">
                </div>
                <!-- <div class="mb-3">
                    <label for="password_confirm" class="form-label">确认密码</label>
                    <input v-model="password_confirm" type="password" class="form-control" id="password_confirm">
                </div> -->
                <div class="error-message">{{ error_message }}</div>
                <button type="submit" class="btn btn-primary">登录</button>
            </form>
        </div>
    </div>
</template>

<script>
import { useStore } from 'vuex';
import { ref } from 'vue';
import router from '@/router';
import $ from 'jquery';
export default {
    name: 'RegisterView',
    setup() {
        const store = useStore();
        let username = ref("");
        let password = ref();
        //let password_confirm = ref("");
        let error_message = ref("");

        let register = () => {
            error_message.value = "";
            let datas = {
                id: username.value,
                password: parseInt(password.value),
            };
            $.ajax({
                url: "https://fb74-240e-478-810-9d7c-6574-8a60-376-aade.ngrok-free.app/RegisterCustomer",
                type: "post",
                contentType: "application/json; charset=utf-8",
                dataType: "json",
                data: JSON.stringify(datas),
                // headers: { 
                //     'ngrok-skip-browser-warning': 'true' 
                // },
                success(resp) {
                    //resp = resp.replace(/'/g, '"').replace(/None/g, "null");
                    console.log(resp);
                    if (resp.result === "success") {
                        store.dispatch("login", {
                            username: username.value,
                            password: password.value,
                            success(userID) {
                                console.log("success");
                                router.push({
                                    name: 'profile',
                                    params: { userID: userID },
                                });
                            },
                            error() {
                                error_message.value = "系统异常，请稍后重试";
                            }
                        });
                    }
                    else {
                        error_message.value = resp.result;
                    }
                }
            });
            console.log(username.value, password.value);
        };

        return {
            username, password, error_message, register, //password_confirm,
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