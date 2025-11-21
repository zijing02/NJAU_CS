<template>
    <div class="row card position-relative">
        <div class="id">
            订单号: {{ orderID }}
        </div>  
        <hr>
        <table class="table tb caption-top table-hover">
            <!-- <caption>产品信息</caption> -->
            <thead class="table-primary ">
                <tr>
                    <th class="hh1"><span class="box">产品编号</span></th>
                    <th class="hh2"><span class="box">产品名称</span></th>
                    <th class="hh3"><span class="box">单位数量</span></th>
                    <th class="hh4"><span class="box">购买数量</span></th>
                    <th class="hh5"><span class="box">产品单价</span></th>
                    <th class="hh6"><span class="box">产品总价</span></th>
                </tr>
            </thead>
            <tbody class="t_height">
                <tr v-for="product in products" :key="product.productID">
                    <td><span class="box">{{ product.productID }}</span></td>
                    <td><span class="box">{{ product.productName }}</span></td>
                    <td><span class="box">{{ product.num }}</span></td>
                    <td><span class="box">{{ product.quantity }}</span></td>
                    <td><span class="box">{{ product.price }}</span></td>
                    <td><span class="box">{{ product.subtotal }}</span></td>
                </tr>
            </tbody>
        </table>
        <div class="user ">
            订单总金额:{{ totalOrderCost }}
        </div>
        <hr>
        <div class="user ">
            订单订购日期:{{ orderdate }}
        </div>
        <hr>
        <div class="user ">
            货主名称:{{ OwnerName }}
        </div>
        <hr>
        <div class="user ">
            货主地址:{{ address }}
        </div>
        <div id="btn1" class="bottom-button">
            <button type="button" class="btn btn-success" >打印订单</button>
        </div>
    </div>
</template>

<script>
import { useRoute } from 'vue-router';
import { ref } from 'vue';
import $ from 'jquery';
export default {
    name: 'OrderDetailedView',
    setup() {
        const route = useRoute();
        const orderID = route.params.orderID;//获取当前订单ID

        let products = ref([]);
        let totalOrderCost = ref();
        let orderdate = ref('');
        let OwnerName = ref('');
        let address = ref('');

        $.ajax({//获取用户列表,要改成获取产品列表
            url: 'https://fb74-240e-478-810-9d7c-6574-8a60-376-aade.ngrok-free.app/GetOrderDetails/?info='+parseInt(orderID),
            type: 'post',
            contentType: "application/json; charset=utf-8",
            dataType: "json",
            success(resp) {
                products.value = JSON.parse(resp);
                console.log(products.value);
                console.log(products.value[0].totalOrderCost);
                console.log("success");
                totalOrderCost.value=products.value[0].totalOrderCost;
                orderdate.value=products.value[0].orderdate;
                OwnerName.value=products.value[0].OwnerName;
                address.value=products.value[0].address;
            },
            error(){
                console.log("failed");
            }
        });

        return {
            route,orderID,products,totalOrderCost,orderdate,OwnerName,address,
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
    font-size:18px;
    margin-bottom: 5px;
    font-weight: bold;
    /* border: 1px solid #333; */
    /* text-align: center; */
}

.id{
    font-size:24px;
    font-weight: bold;
    text-align: center;
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
    flex-direction: row;
    justify-content: center;
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