<template>
    <div class="container">
        <ul>
            <li class="col-5 left">
                <table class="table tb caption-top table-hover">
                    <thead class="table-primary">
                        <tr>
                            <th class="hh1"><span class="box">季节</span></th>
                            <th class="hh2"><span class="box">运货量</span></th>
                        </tr>
                    </thead>
                    <tbody class="t_height">
                        <tr v-for="items in data_list" :key="items.area">
                            <td><span class="box">{{ items.season }}</span></td>
                            <td><span class="box">{{ items.total_quantity }}</span></td>
                        </tr>
                    </tbody>
                </table>
            </li>
            <li class="col-5 right">
                <div id="myDiv1" class="line right"></div>  
            </li>
        </ul>
        <div class="column">
            <div id="myDiv2" class="bar"></div>  
        </div>
    </div>
</template>

<script>
import Plotly from 'plotly.js-dist'
import $ from 'jquery';
import { ref } from 'vue';
export default {
    name: 'QuerySeasonView',
    setup() {
        let data_list = ref([]);

        //CountOrderNumOfArea
        $.ajax({
            url: 'https://fb74-240e-478-810-9d7c-6574-8a60-376-aade.ngrok-free.app/CountOrderNumOfSeason',
            type: 'get',
            contentType: "application/json; charset=utf-8",
            dataType: "json",
            headers: { 
                'ngrok-skip-browser-warning': 'true' 
            },
            success(resp) {
                resp = resp.replace(/'/g, '"').replace(/None/g, "null");
                console.log("success");
                data_list.value = JSON.parse(resp);
                console.log(data_list.value);
                processGroupedData1();
                processGroupedData2();
                //createPlot();
            },
            error(){
                console.log("failed");
            }
        });
 
        const processGroupedData1 = () => {
            let season = data_list.value.map(item => item.season);
            let total_quantity = data_list.value.map(item => item.total_quantity);
            let data = [{
                x: season, // 地区数组  
                y: total_quantity, // 总数值数组  
                type: 'scatter',
                mode: 'markers+lines', // 添加标记和垂直线  
                name: '季节数据统计折线图'
            }];
            let layout = {
                title: '季节数据统计折线图'
            };
            Plotly.newPlot('myDiv1', data, layout);   
        };

        const processGroupedData2 = ()=>{ 
            let data = data_list.value.map(item => ({
                x: [item.season],
                y: [item.total_quantity],
                type: 'bar',
                name: item.season  
            }));
            let layout = {  
                title: '季节数据统计柱图'   
            };
            Plotly.newPlot('myDiv2', data, layout);
        }
        return {
            data_list,
            processGroupedData1,
            processGroupedData2,
        };
    },
}
</script>

<style scoped>
.tb {
    width:100%;
    table-layout: fixed;
    /* 使用固定表格布局 */
}

ol,ul{
	list-style-type: none;
}

ul {  
    height:350px;
    margin-top:20px;
}  

.container{
    width: 1280px;
	/* border: 1px solid #f00; */
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

.left{
    float:left;
}
.right {
    float: right;
}

.label1{
    margin:20px;
    font-size: 18px;
    font-weight: bold;
    /* border: 1px solid #000; */
}

#btn1{
    margin-left: 10px;
}

.label1 input{
    font-size: 16px;
}   

.curve{
    /* border: 1px solid #000; */
    height:100px;
}

.column{
    /* margin-top: 50px; */
    height:100px;
}

.line{
    width:600px;
    height:300px;
}

.bar{
    width:auto;
    height:350px;
}
</style>