import $ from 'jquery';
//import { jwtDecode } from 'jwt-decode';
const ModuleUser = {
    state: {//存储所有数据
        cID: "",
        cname: "",
        pname: "",
        pjob: "",
        caddress: "",
        city: "",
        area: "",
        postcode: "",
        country: "",
        phone: "",
        fax: "",
        is_login: false,
    },
    getters: {//读取数据信息，对数据进行一些调整，不能修改
    },
    mutations: {//进行所有修改操作，不能进行异步操作
        updateUser(state, user) {
            state.cID = user.cID;
            state.cname = user.cname;
            state.pname = user.pname;
            state.pjob = user.pjob;
            state.caddress = user.caddress;
            state.city = user.city;
            state.area = user.area;
            state.postcode = user.postcode;
            state.country = user.country;
            state.phone = user.phone;
            state.fax = user.fax;
            state.is_login = user.is_login;
        },
        logout(state) {
            state.cID = "";
            state.cname = "";
            state.pname = "";
            state.pjob = "";
            state.caddress = "";
            state.city = "";
            state.area = "";
            state.postcode = "";
            state.country = "";
            state.phone = "";
            state.fax = "";
            state.is_login = false;
        },
    },
    actions: {//定义对state(数据)的各种操作，不能修改//增删改查
        login(context, data) {
            let datas = {
                tablename: "customers",
                id: data.username,
                id1: data.password,
            };
            $.ajax({//获取jwt
                url: "https://fb74-240e-478-810-9d7c-6574-8a60-376-aade.ngrok-free.app/GetInfo",
                type: "post",
                data: JSON.stringify(datas),
                contentType: "application/json; charset=utf-8",
                dataType: "json",
                success(resp) {
                    resp = resp.replace(/'/g, '"').replace(/None/g, "null");
                    let responseJson = JSON.parse(resp)[0];
                    context.commit("updateUser", {
                        ...responseJson,//解构resp
                        is_login: true,
                    });
                    console.log(responseJson);
                    console.log(responseJson.cID);
                    data.success(responseJson.cID);//调用LoginView文件里面的success
                },
                error() {
                    data.error();
                },
            });
            return {
                datas,
            }
        }
    },
    modules: {//对类进行分割
    },
};

export default ModuleUser;