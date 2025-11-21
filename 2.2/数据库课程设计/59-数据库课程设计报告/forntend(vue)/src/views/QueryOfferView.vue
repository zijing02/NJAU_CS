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
                    <th class="hh1"><span class="box">产品类别</span></th>
                    <th class="hh2"><span class="box">供应商</span></th>
                    <th class="hh3"><span class="box">产品数量</span></th>
                </tr>
            </thead>
            <tbody class="t_height">
                <tr v-for="product in pagedProducts" :key="product.ID">
                    <td><span class="box">{{ product.cname }}</span></td>
                    <td><span class="box">{{ product.sname }}</span></td>
                    <td><span class="box">{{ product.total_quantity }}</span></td>
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
export default {
    name: 'QueryOfferView',
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
        const logProductName = (product) => {
            console.log(product.username);
        };

        const search_class = () => {
            let ID = parseInt(CategoryID.value);
            console.log(ID);   
            console.log(ID);    
            $.ajax({//获取用户列表,要改成获取产品列表
                url: 'https://fb74-240e-478-810-9d7c-6574-8a60-376-aade.ngrok-free.app/CountOrdersOfSuppliers/?CategoryID='+ID,
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
            logProductName,
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
    width: 30%;
}

.hh2 {
    width: 30%;
}

.hh3 {
    width: 40%;
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
    height: 100%; /* 确保容器有高度 */ 
}

.txt{
    text-align: center;
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

.pics img{
    width: auto; /* 或使用其他固定宽度 */  
    height: 100px; /* 或使用其他固定高度 */  
    /* object-fit: cover; 图像覆盖整个容器，保持纵横比   */
}

.t_height{
    height:56px;
}
</style>