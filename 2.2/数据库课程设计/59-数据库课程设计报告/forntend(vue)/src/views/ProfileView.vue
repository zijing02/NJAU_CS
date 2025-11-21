<template>
    <div class="row card">
        <div class="id user ">
            联系人姓名:
            <input class="modify1" v-model="pname" type="text" :placeholder="pname">
            <span id="id1">
            客户ID: {{ store.state.user.cID }}
            </span>
        </div>
        <div class="user ">
            联系人职务: 
            <input class="modify1" v-model="pjob" type="text" :placeholder="pjob">
        </div>
        <div class="user ">
            公司名称: 
            <input class="modify1" v-model="cname" type="text" :placeholder="cname">
        </div>
        <div class="user ">
            地址: 
            <input class="modify1" v-model="caddress" type="text" :placeholder="caddress">
        </div>
        <div class="user ">
            城市:
            <input class="modify1" v-model="city" type="text" :placeholder="city">
        </div>
        <div class="user ">
            地区: 
            <input class="modify1" v-model="area" type="text" :placeholder="area">
        </div>
        <div class="user ">
            邮政编码: 
            <input class="modify1" v-model="postcode" type="text" :placeholder="postcode">
        </div>
        <div class="user ">
            国家: 
            <input class="modify1" v-model="country" type="text" :placeholder="country">
        </div>
        <div class="user">
            电话: 
            <input class="modify1" v-model="phone" type="text" :placeholder="phone">
        </div>
        <div class="user ">
            传真:
            <input class="modify1" v-model="fax" type="text" :placeholder="fax">
        </div>    
        <div id="btn1">
            <button type="button" class="btn btn-primary" @click="update_profile">保存</button>
        </div>
    </div>
</template>

<script>
//import { reactive } from 'vue';
//import { useRoute } from 'vue-router';
import { useStore } from 'vuex';
import { ref } from 'vue';
//import { computed } from 'vue';
import $ from 'jquery';
//import store from '@/store';
export default {
    name: 'ProfileView',
    setup() {
        const store = useStore();
        let pname=ref(store.state.user.pname);
        let cID=ref(store.state.user.cID);
        let pjob=ref(store.state.user.pjob);
        let cname=ref(store.state.user.cname);
        let caddress=ref(store.state.user.caddress);
        let city=ref(store.state.user.city);
        let area=ref(store.state.user.area);
        let postcode=ref(store.state.user.postcode);
        let country=ref(store.state.user.country);
        let phone=ref(store.state.user.phone);
        let fax=ref(store.state.user.fax);

        const update_profile = ()=> {
            let datas={
                pname:pname.value,
                cID:cID.value,
                pjob:pjob.value,
                cname:cname.value,
                caddress:caddress.value,
                city:city.value,
                area:area.value,
                postcode:postcode.value,
                country:country.value,
                phone:phone.value,
                fax:fax.value,
            };
            $.ajax({
                url: 'https://fb74-240e-478-810-9d7c-6574-8a60-376-aade.ngrok-free.app/UpdateCustomer',
                type: 'post',
                contentType: "application/json; charset=utf-8",
                dataType: "json",
                data: JSON.stringify(datas),
                success() {
                    console.log("success");
                    store.commit("updateUser",{
                        pname:pname.value,
                        cID:cID.value,
                        pjob:pjob.value,
                        cname:cname.value,
                        caddress:caddress.value,
                        city:city.value,
                        area:area.value,
                        postcode:postcode.value,
                        country:country.value,
                        phone:phone.value,
                        fax:fax.value,
                        is_login:true,
                    });
                },
                error(){
                    console.log("failed");
                }
            });
 

        };
        return {
            store,pname,cID,pjob,cname,caddress,city,area,postcode,country,phone,fax,
            update_profile,
        };
    }
}
</script>

<style scoped>
img {
    border-radius: 50%;
}

.right{
    float:right;
}

/* 居中 */
.photo {
    display: flex;
    flex-direction: column;
    justify-content: center;
}

.row {
    margin: 7% auto;
    width: 760px;
    height:520px;
    border-radius: 25px;
    background-color:white; 
    /* rgba(0, 0, 0, 0.1); */
    /* 阴影部分 */
    box-shadow: 0 0 17px #333;
}

.user {
    font-size:24px;
    margin-left:60px;
    margin-bottom: 5px;
    font-weight: bold;
    /* text-align: center; */
}

.id{
    font-weight: bold;
}

.row span{
    margin:0px 10px;
}

#id1{
    float:right;
    margin-right:80px;
    color:rgb(72, 120, 122);
}

.modify1{
    height: 40px;
    width: 300px; /* 设置输入字段的宽度为 200 像素 */  
    /* padding: 5px; 设置内边距   */
    /* border-radius: 10px; */
    /* border-color:#FFFFFF; */
    /* outline: none; */
    
}

input:focus{
    /*  outline: none; 移除默认的聚焦轮廓   */
    /*  #4CAF50; 改变边框颜色   */
    box-shadow: 0 0 5px rgb(72, 120, 122); /* 添加阴影效果 */  
}

#btn1{
    display: flex;  
    justify-content: center; /* 水平居中 */  
    align-items: center; /* 垂直居中 */  
    width: 760px; /* 或者其他你需要的高度 */
}

#btn1 button{
    margin-top:10px;
    width:200px;
}
</style>