<template>
    <div class="container">
        <ul>
            <li class="col-5 left">
                <table class="table tb caption-top table-hover">
                    <thead class="table-primary">
                        <tr>
                            <th class="hh1"><span class="box">地区</span></th>
                            <th class="hh2"><span class="box">运货量</span></th>
                        </tr>
                    </thead>
                    <tbody class="t_height">
                        <tr v-for="items in data_list" :key="items.area">
                            <td><span class="box">{{ items.area }}</span></td>
                            <td><span class="box">{{ items.total_num }}</span></td>
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
//import Plotly from 'plotly.js';
import Plotly from 'plotly.js-dist'
import $ from 'jquery';
import { ref } from 'vue';
export default {
    name: 'QueryTransportView',
    setup() {
        let data_list = ref([]);
        // let grouped_data = ref([]);

        //CountOrderNumOfArea
        $.ajax({
            url: 'https://fb74-240e-478-810-9d7c-6574-8a60-376-aade.ngrok-free.app/CountOrderNumOfArea',
            type: 'get',
            contentType: "application/json; charset=utf-8",
            //async: true,
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
        // 创建柱状图的数据  
            let traces = [];  
            let uniqueCnames = new Set(); // 用来收集唯一的客户名称  
            data_list.value.forEach(item => {  
                uniqueCnames.add(item.cname);  
            });  
            
            uniqueCnames.forEach(cname => {  
                let xValues = data_list.value  
                    .filter(item => item.cname === cname)  
                    .map(item => item.area);  
                let yValues = data_list.value  
                    .filter(item => item.cname === cname)  
                    .map(item => item.total_num);  
                
                // 如果某个地区没有该客户的数据，则添加0作为占位符  
                const allAreas = new Set(data_list.value.map(item => item.area));  
                xValues = Array.from(allAreas).map(area => {  
                    const index = xValues.indexOf(area);  
                    return index !== -1 ? xValues[index] : area;  
                });  
                yValues = Array.from(allAreas).map(area => {  
                    const index = xValues.indexOf(area);  
                    return index !== -1 ? yValues[index] : 0;  
                });  
                
                traces.push({  
                    x: xValues,  
                    y: yValues,  
                    type: 'line',  
                    name: cname  
                });  
            });  
            
            let layout = {  
                title: '不同地区运货量统计折线图',  
                xaxis: {  
                    tickangle: -45,  
                    tickfont: {  
                        size: 14,  
                        color: 'rgb(107, 107, 107)'  
                    }  
                },  
                yaxis: {  
                    title: '总运货量'  
                },  
                barmode: 'group' // 如果有多个系列在同个x轴位置需要堆叠，则使用'stack'  
            };  
            
            Plotly.newPlot('myDiv1', traces, layout);  
        };

        const processGroupedData2 = ()=>{
            // 创建柱状图的数据  
            let traces = [];  
            let uniqueCnames = new Set(); // 用来收集唯一的客户名称  
            data_list.value.forEach(item => {  
                uniqueCnames.add(item.cname);  
            });  
            
            uniqueCnames.forEach(cname => {  
                let xValues = data_list.value
                    .filter(item => item.cname === cname)  
                    .map(item => item.area);  
                let yValues = data_list.value
                    .filter(item => item.cname === cname)  
                    .map(item => item.total_num);  
                
                // 如果某个地区没有该客户的数据，则添加0作为占位符  
                const allAreas = new Set(data_list.value.map(item => item.area));  
                xValues = Array.from(allAreas).map(area => {  
                    const index = xValues.indexOf(area);  
                    return index !== -1 ? xValues[index] : area;  
                });  
                yValues = Array.from(allAreas).map(area => {  
                    const index = xValues.indexOf(area);  
                    return index !== -1 ? yValues[index] : 0;  
                });  
                
                traces.push({  
                    x: xValues,  
                    y: yValues,  
                    type: 'bar',  
                    name: cname  
                });  
            });  
            
            let layout = {  
                title: '不同地区运货量统计柱图',  
                xaxis: {  
                    tickangle: -45,  
                    tickfont: {  
                        size: 14,  
                        color: 'rgb(107, 107, 107)'  
                    }  
                },  
                yaxis: {  
                    title: '总运货量'  
                },  
                barmode: 'group' // 如果有多个系列在同个x轴位置需要堆叠，则使用'stack'  
            };  
            
            Plotly.newPlot('myDiv2', traces, layout);  
        }
        return {
            //traces,
            //uniqueCnames,
            data_list,
            processGroupedData1,
            processGroupedData2,
            //grouped_data,
            //layout,
            //createPlot,
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