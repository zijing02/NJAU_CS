<template>
    <div class="container">
        <div class="right label1">
            <input v-model="CategoryID" type="int" placeholder="请输入待查找的类别">
            <button type="button" class="btn btn-primary" id="btn1" @click="search_class">查询</button>
        </div>
        <table class="table tb caption-top table-hover">
            <!-- <caption>产品信息</caption> -->
            <thead class="table-primary ">
                <tr>
                    <th class="hh1"><span class="box">产品</span></th>
                    <th class="hh2"><span class="box">商家</span></th>
                    <th class="hh3"><span class="box">单位数量</span></th>
                    <th class="hh4"><span class="box">库存量</span></th>
                    <th class="hh5"><span class="box">单价</span></th>
                    <th class="hh6"><span class="box">操作</span></th>
                </tr>
            </thead>
            <tbody>
                <tr v-for="product in pagedProducts" :key="product.product_id">
                    <td><span class="box">{{ product.product_name }}</span></td>
                    <td><span class="box">{{ product.supplier_name }}</span></td>
                    <td><span class="box">{{ product.num }}</span></td>
                    <td><span class="box">{{ product.inventory }}</span></td>
                    <td><span class="box">{{ product.Price }}</span></td>
                    <td><span class="box">
                            <button type="button" :id="product.product_id" class="btn btn-primary"
                            @click="OpenNewOrder(product)">购买</button>
                        </span>
                    </td>
                </tr>
            </tbody>
        </table>
        <!-- 分页控件 -->
        <nav aria-label="Page navigation">
            <ul class="pagination right">
                <!-- <li class="page-item" :class="{ active: currentPage == 1 }">
                        <a class="page-link" href="#" @click.prevent="currentPage = 1">1</a>
                    </li> -->
                <!-- 可以根据需要添加更多的页码链接 -->
                <li class="page-item" :class="{ active: currentPage == page }" v-for="page in totalPages" :key="page">
                    <a class="page-link" href="#" @click="goToPage(page)">{{ page }}</a>
                </li>
            </ul>
        </nav>
    </div>
</template>

<script>
import $ from 'jquery';
import { computed, ref } from 'vue';
import router from '@/router';
export default {
    name: 'ShopView',
    setup() {
        let products = ref([]);
        let currentPage = ref(1);
        let pageSize = 10;
        let numbers = ref(0);//购买数量
        let CategoryID = ref();

        const pagedProducts = computed(() => {//定位当前页的产品下标[start,end]
            const start = (currentPage.value - 1) * pageSize;
            const end = start + pageSize;
            console.log(start, end);
            return products.value.slice(start, end);
        });
        const totalPages = computed(() => {//计算总页数
            return Math.ceil(products.value.length / pageSize);
        });
        const goToPage = (page) => {//跳转到某一页
            if (page >= 1 && page <= totalPages.value) {
                currentPage.value = page;
            }
            console.log(currentPage.value);
        };
        const OpenNewOrder = (product) => {
            console.log(product.product_id);
            router.push({
                name: 'new_order',
                params: { productID: product.product_id },
            });
        };
        // const OpenNewOrder = (product) => {  
        //     console.log(product.product_id);  
            
        //     // 构建完整的 URL  
        //     const baseURL = window.location.origin; // 获取当前应用的基础URL（例如：https://yourapp.com）  
        //     const path = router.resolve({  
        //         name: 'new_order',  
        //         params: { orderID: product.product_id },  
        //     }).href; // 使用 router.resolve 获取路由的完整路径  
        //     const fullURL = `${baseURL}${path}`; // 拼接完整的 URL  
            
        //     // 在新标签页中打开页面  
        //     window.open(fullURL, '_blank');  
        // };

        const search_class = () => {
            let ID = parseInt(CategoryID.value);
            console.log(ID);   
            console.log(ID);    
            $.ajax({//获取用户列表,要改成获取产品列表
                url: 'https://fb74-240e-478-810-9d7c-6574-8a60-376-aade.ngrok-free.app/GetProductsOfCategory/?CategoryID='+ID,
                type: 'post',
                contentType: "application/json; charset=utf-8",
                dataType: "json",
                success(resp) {
                    products.value = JSON.parse(resp);
                    console.log(products.value);
                    console.log("success");
                    currentPage.value = 1;
                },
                error(){
                    console.log("failed");
                }
            });
        };

        return {
            products,
            currentPage, // 当前页码  
            pageSize, // 每页显示的产品数量  
            pagedProducts,
            totalPages,
            numbers,
            CategoryID,
            goToPage,
            OpenNewOrder,
            search_class,
        };
    },
}
</script>

<style scoped>
.tb {
    margin-top: 5px;
    table-layout: fixed;
    /* 使用固定表格布局 */
}

.hh1 {
    width: 15%;
}

.hh2 {
    width: 15%;
}

.hh3 {
    width: 15%;
}

.hh4 {
    width: 15%;
}

.hh5 {
    width: 15%;
}

.hh6 {
    width: 25%;
}

button {
    font-size: 14px;
    margin: 2px;
}

button:hover {
    box-shadow: 2px 2px 10px lightgray;
    transition: 500ms;
}

.box {
    display: flex;
    justify-content: center;
    /* 水平居中 */
    align-items: center;
    /* 垂直居中 */
    font-size: 18px;
}

.right {
    float: right;
}

.label1{
    margin-top:20px;
    font-size: 18px;
    font-weight: bold;
}

#btn1{
    margin-left: 10px;
}

.label1 input{
    font-size: 16px;
}
</style>