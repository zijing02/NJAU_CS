<template>
    <nav class="navbar navbar-expand-lg bg-body-tertiary">
        <div class="container">
            <router-link class="navbar-brand" :to="{ name: 'home' }"><span>418-物流订单管理系统</span></router-link>
            <button class="navbar-toggler" type="button" data-bs-toggle="collapse" data-bs-target="#navbarText"
                aria-controls="navbarText" aria-expanded="false" aria-label="Toggle navigation">
                <span class="navbar-toggler-icon"></span>
            </button>
            <div class="collapse navbar-collapse" id="navbarText">
                <ul class="navbar-nav me-auto mb-2 mb-lg-0">
                    <li class="nav-item" v-if="$store.state.user.is_login">
                        <router-link class="nav-link" :to="{ name: 'shop' }"><span>购物中心</span></router-link>
                    </li>
                    <li class="nav-item dropdown" v-if="$store.state.user.is_login">
                        <router-link class="nav-link dropdown-toggle" role="button" :to="{ name: 'home'}"
                            data-bs-toggle="dropdown" aria-expanded="false">
                            <span>查询中心</span>
                        </router-link>
                        <ul class="dropdown-menu">
                            <li>
                                <router-link class="dropdown-item" :to="{ name: 'query_photo' }">图片查询</router-link>
                            </li>
                            <li>
                                <router-link class="dropdown-item" :to="{ name: 'query_order' }">订货量查询</router-link>
                            </li>
                            <li>
                                <router-link class="dropdown-item" :to="{ name: 'query_transport' }">地区运货量查询</router-link>
                            </li>
                            <li>
                                <router-link class="dropdown-item" :to="{ name: 'query_season' }">季节运货量查询</router-link>
                            </li>
                            <li>
                                <router-link class="dropdown-item" :to="{ name: 'query_money' }">产品总金额查询</router-link>
                            </li>
                            <li>
                                <router-link class="dropdown-item" :to="{ name: 'query_ordernum' }">承接订单量查询</router-link>
                            </li>
                            <li>
                                <router-link class="dropdown-item" :to="{ name: 'query_offer' }">供应量查询</router-link>
                            </li>
                        </ul>
                    </li>
                    <li class="nav-item" v-if="$store.state.user.is_login">
                        <router-link class="nav-link" :to="{ name: 'order' }"><span>订单中心</span></router-link>
                    </li>
                </ul>
                <ul class="navbar-nav" v-if="!$store.state.user.is_login">
                    <li class="nav-item">
                        <router-link class="nav-link" :to="{ name: 'login' }"><span>登录</span></router-link>
                    </li>
                    <li class="nav-item">
                        <router-link class="nav-link" :to="{ name: 'register' }"><span>注册</span></router-link>
                    </li>
                </ul>
                <ul class="navbar-nav" v-else>
                    <li class="nav-item">
                        <router-link class="nav-link"
                            :to="{ name: 'profile', params: { userID: $store.state.user.cID } }">
                            <span>{{ $store.state.user.pname }}</span>
                        </router-link>
                    </li>
                    <li class="nav-item">
                        <router-link class="nav-link" :to="{ name: 'login' }"
                            @click="logout"><span>退出</span></router-link>
                    </li>
                </ul>
            </div>
        </div>
    </nav>
</template>

<script>
import store from '@/store';

export default {
    name: "NavBar",
    setup() {
        const logout = () => {
            store.commit("logout");
        };
        return {
            logout,
        }
    }
}
</script>

<style scoped>
.navbar {
    background-color: #333 !important;
    /* 使用!important可以确保该规则优先级最高 */
}

span {
    color: #F7F7F7;
}

span:hover {
    color: yellowgreen;
}
</style>