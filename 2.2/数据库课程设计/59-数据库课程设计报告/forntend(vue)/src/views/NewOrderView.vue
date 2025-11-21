<template>
    <div class="row card position-relative">
        <div class="id user ">
            联系人姓名: {{ pname }}
            <span id="id1">
            客户ID: {{ store.state.user.cID }}
            </span>
        </div>
        <div class="user ">
            产品名称:{{ product_name }}
        </div>
        <div class="user ">
            产品单价:{{ Price }}
        </div>
        <div class="user ">
            购买数量:
            <input class="modify1" v-model="number" type="number" :placeholder="number">
        </div>
        <div class="user ">
            支付方式: 
            <input class="modify1" v-model="pay_way" type="text" placeholder="填'支付宝'或'微信'">
        </div>
        <div class="user ">
            保险信息: 
            <input class="modify1" v-model="baoxian" type="number" placeholder="填'0'或'1'">
        </div> 
        <div class="user ">
            雇员ID: 
            <input class="modify1" v-model="transportID" type="number" placeholder="填入选定的雇员的ID">
        </div> 
        <div id="btn1" class="bottom-button">
            <button type="button" class="btn btn-danger" @click="return_shop">取消</button>
            <button type="button" class="btn btn-success" @click="submit" >提交订单</button>
        </div>
    </div>
</template>

<script>
//import { reactive } from 'vue';
import { useRoute } from 'vue-router';
import { useStore } from 'vuex';
import { ref } from 'vue';
import router from '@/router';
//import { computed } from 'vue';
import $ from 'jquery';
//import store from '@/store';
export default {
    name: 'NewOrderView',
    setup() {
        const store = useStore();
        const route = useRoute();
        const productID = route.params.productID;//获取当前订单ID

        let cID=ref(store.state.user.cID);//客户ID
        let pname=ref(store.state.user.pname);//货主名称
        let caddress=ref(store.state.user.caddress);//货主地址
        let city=ref(store.state.user.city);//货主城市
        let area=ref(store.state.user.area);//货主地区
        let postcode=ref(store.state.user.postcode);//货主邮政编码
        let country=ref(store.state.user.country);//货主国家

        let number=ref(0);//购买数量
        let pay_way=ref('');//支付方式
        let baoxian=ref();//保险信息
        let product_name=ref('');//产品名称
        let Price=ref();//产品单价
        let transportID=ref();//雇员ID
        // 创建一个Date对象，代表当前日期和时间  
        const now = new Date();          
        // 获取年份  
        let year = now.getFullYear();  
        // 获取月份（注意，JavaScript的月份是从0开始的，所以需要加1）  
        let month = (now.getMonth() + 1).toString().padStart(2, '0');  
        // 获取日期  
        let date = now.getDate().toString().padStart(2, '0');       
        // 组合成年月日的格式  
        const formattedDate = `${year}-${month}-${date}`;  
        now.setDate(now.getDate() + 2); 
        year = now.getFullYear();  
        month = (now.getMonth() + 1).toString().padStart(2, '0'); 
        date = now.getDate().toString().padStart(2, '0');  
        const StartDate = `${year}-${month}-${date}`; 

        now.setDate(now.getDate() + 8); 
        year = now.getFullYear();  
        month = (now.getMonth() + 1).toString().padStart(2, '0'); 
        date = now.getDate().toString().padStart(2, '0');  
        const EndDate = `${year}-${month}-${date}`; 


        const return_shop = () =>{
            router.push({
                name: 'shop',
            });
        };

        let datas={
            tablename: 'products',
            id: parseInt(productID),
            id1: 0,
        }
        $.ajax({//获取用户列表,要改成获取产品列表
            url: 'https://fb74-240e-478-810-9d7c-6574-8a60-376-aade.ngrok-free.app/GetInfo/',
            type: 'post',
            contentType: "application/json; charset=utf-8",
            dataType: "json",
            data: JSON.stringify(datas),
            success(resp) {
                resp = JSON.parse(resp);
                console.log("success1");
                product_name.value = resp[0].cname;
                Price.value = resp[0].price;
            },
            error(){
                console.log("failed");
            }
        });

        const submit = () =>{
            let info={
                customerID: cID.value,
                employeeID: parseInt(transportID.value),
                orderdate: formattedDate,
                starttime: StartDate,
                arrivaltime: EndDate,
                name: pname.value,
                addr: caddress.value,
                city: city.value,
                area: area.value,
                postcode: postcode.value,
                country: country.value,
                paymethod: pay_way.value,
                insurance: parseInt(baoxian.value),
            };
            let info1={
                productid:parseInt(productID),
                num:parseInt(number.value),
            };
            let datas={
                info:info,
                info1:info1,
            };
            $.ajax({//获取用户列表,要改成获取产品列表
                url: 'https://fb74-240e-478-810-9d7c-6574-8a60-376-aade.ngrok-free.app/CreateOrderAndDetail/',
                type: 'post',
                contentType: "application/json; charset=utf-8",
                dataType: "json",
                data: JSON.stringify(datas),
                success() {
                    console.log("success2");
                    return_shop();
                },
                error(){
                    console.log("failed");
                }
            });
        };


        return {
            store,cID,pname,caddress,city,area,postcode,country,
            productID,return_shop,now,year,month,date,formattedDate,
            number,pay_way,baoxian,product_name,Price,transportID,
            StartDate,EndDate,submit,
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
    width: 760px; /* 或者其他你需要的高度 */
    display: flex;  
    /* border:1px solid #333; */
}

#btn1 button{
    margin:10px 100px 0px;
    /* border:1px solid #333; */
    width:200px;
    
}

.position-relative {  
    position: relative;  
}

.bottom-button {  
    position: absolute;  
    bottom: 20px;  
    width: 100%; /* 如果需要按钮宽度与容器相同 */  
    left: 0; /* 确保按钮从左边开始 */  
}  
</style>