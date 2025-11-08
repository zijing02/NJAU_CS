<template>
  <div id="app">
    <h1>CD 数据库管理系统</h1>

    <!-- 连接状态 -->
    <div class="status-section">
      <h2>系统状态</h2>
      <button @click="testConnection">测试连接</button>
      <div :class="['status', connectionStatus.includes('成功') ? 'success' : 'error']">
        {{ connectionStatus }}
      </div>
    </div>

    <!-- 分类管理 -->
    <div class="section">
      <h2>分类管理</h2>
      <button @click="fetchCategories">获取分类数据</button>

      <!-- 添加分类表单 -->
      <div class="form">
        <h3>添加新分类</h3>
        <input v-model="newCategory.categoryName" placeholder="分类名称" required>
        <input v-model="newCategory.instruction" placeholder="说明">
        <button @click="addCategory">添加分类</button>
      </div>

      <!-- 分类数据表格 -->
      <div v-if="categories.length > 0" class="data-table">
        <h3>分类列表 (共 {{ categories.length }} 条记录)</h3>
        <table>
          <thead>
            <tr>
              <th>分类名称</th>
              <th>说明</th>
              <th>操作</th>
            </tr>
          </thead>
          <tbody>
            <tr v-for="(category, index) in categories" :key="index">
              <td v-if="editingCategory !== index">{{ category.categoryName }}</td>
              <td v-else><input v-model="editCategoryData.categoryName" /></td>

              <td v-if="editingCategory !== index">{{ category.instruction }}</td>
              <td v-else><input v-model="editCategoryData.instruction" /></td>

              <td>
                <button
                  v-if="editingCategory !== index"
                  @click="startEditCategory(index, category)"
                  class="edit-btn"
                >
                  编辑
                </button>
                <button
                  v-else
                  @click="saveCategoryEdit(category.categoryName)"
                  class="save-btn"
                >
                  保存
                </button>
                <button
                  v-if="editingCategory !== index"
                  @click="deleteCategory(category.categoryName)"
                  class="delete-btn"
                >
                  删除
                </button>
                <button
                  v-else
                  @click="cancelEdit"
                  class="cancel-btn"
                >
                  取消
                </button>
              </td>
            </tr>
          </tbody>
        </table>
      </div>
    </div>

    <!-- 客户管理 -->
    <div class="section">
      <h2>客户管理</h2>
      <button @click="fetchCustomers">获取客户数据</button>

      <!-- 添加客户表单 -->
      <div class="form">
        <h3>添加新客户</h3>
        <input v-model="newCustomer.customerCode" placeholder="客户代码" required>
        <input v-model="newCustomer.companyName" placeholder="公司名称" required>
        <button @click="addCustomer">添加客户</button>
      </div>

      <!-- 客户数据表格 -->
      <div v-if="customers.length > 0" class="data-table">
        <h3>客户列表 (共 {{ customers.length }} 条记录)</h3>
        <table>
          <thead>
            <tr>
              <th>客户代码</th>
              <th>公司名称</th>
              <th>操作</th>
            </tr>
          </thead>
          <tbody>
            <tr v-for="(customer, index) in customers" :key="index">
              <td v-if="editingCustomer !== index">{{ customer.customerCode }}</td>
              <td v-else><input v-model="editCustomerData.customerCode" /></td>

              <td v-if="editingCustomer !== index">{{ customer.companyName }}</td>
              <td v-else><input v-model="editCustomerData.companyName" /></td>

              <td>
                <button
                  v-if="editingCustomer !== index"
                  @click="startEditCustomer(index, customer)"
                  class="edit-btn"
                >
                  编辑
                </button>
                <button
                  v-else
                  @click="saveCustomerEdit(customer.customerCode)"
                  class="save-btn"
                >
                  保存
                </button>
                <button
                  v-if="editingCustomer !== index"
                  @click="deleteCustomer(customer.customerCode)"
                  class="delete-btn"
                >
                  删除
                </button>
                <button
                  v-else
                  @click="cancelEdit"
                  class="cancel-btn"
                >
                  取消
                </button>
              </td>
            </tr>
          </tbody>
        </table>
      </div>
    </div>

    <!-- 错误信息 -->
    <div v-if="errorMessage" class="error-message">
      {{ errorMessage }}
    </div>
  </div>
</template>

<script lang="ts">
import { defineComponent } from 'vue';
import axios, { AxiosError } from 'axios';

const API_BASE = 'http://localhost:3001/api';

interface Category {
  categoryName: string;
  instruction: string;
}

interface Customer {
  customerCode: string;
  companyName: string;
}

interface NewCategory {
  categoryName: string;
  instruction: string;
}

interface NewCustomer {
  customerCode: string;
  companyName: string;
}

interface ApiErrorResponse {
  error?: string;
  details?: string;
}

// 错误处理工具函数
const handleApiError = (error: unknown): string => {
  if (axios.isAxiosError(error)) {
    const axiosError = error as AxiosError<ApiErrorResponse>;
    return axiosError.response?.data?.error || axiosError.response?.data?.details || axiosError.message;
  }

  if (error instanceof Error) {
    return error.message;
  }

  return '未知错误';
};

export default defineComponent({
  name: 'App',
  data() {
    return {
      connectionStatus: '点击测试连接按钮检查连接状态',
      categories: [] as Category[],
      customers: [] as Customer[],
      newCategory: {
        categoryName: '',
        instruction: ''
      } as NewCategory,
      newCustomer: {
        customerCode: '',
        companyName: ''
      } as NewCustomer,
      editingCategory: null as number | null,
      editingCustomer: null as number | null,
      editCategoryData: {
        categoryName: '',
        instruction: ''
      } as NewCategory,
      editCustomerData: {
        customerCode: '',
        companyName: ''
      } as NewCustomer,
      errorMessage: ''
    }
  },
  methods: {
    async testConnection(): Promise<void> {
      try {
        this.connectionStatus = '测试连接中...';
        const response = await axios.get(`${API_BASE}/test`);
        this.connectionStatus = `连接成功! 数据库: ${response.data.database}`;
        this.errorMessage = '';
      } catch (error) {
        this.connectionStatus = '连接失败';
        this.errorMessage = `错误: ${handleApiError(error)}`;
        console.error('连接测试失败:', error);
      }
    },

    async fetchCategories(): Promise<void> {
      try {
        const response = await axios.get(`${API_BASE}/categories`);
        this.categories = response.data.data;
        this.errorMessage = '';
        console.log('分类数据:', this.categories);
      } catch (error) {
        this.errorMessage = `获取分类失败: ${handleApiError(error)}`;
        console.error('获取分类失败:', error);
      }
    },

    async fetchCustomers(): Promise<void> {
      try {
        const response = await axios.get(`${API_BASE}/customers`);
        this.customers = response.data.data;
        this.errorMessage = '';
        console.log('客户数据:', this.customers);
      } catch (error) {
        this.errorMessage = `获取客户失败: ${handleApiError(error)}`;
        console.error('获取客户失败:', error);
      }
    },

    async addCategory(): Promise<void> {
      try {
        if (!this.newCategory.categoryName) {
          alert('请输入分类名称');
          return;
        }

        await axios.post(`${API_BASE}/categories`, this.newCategory);
        alert('分类添加成功!');
        this.newCategory = { categoryName: '', instruction: '' };
        this.fetchCategories();
      } catch (error) {
        this.errorMessage = `添加分类失败: ${handleApiError(error)}`;
        console.error('添加分类失败:', error);
      }
    },

    async addCustomer(): Promise<void> {
      try {
        if (!this.newCustomer.customerCode || !this.newCustomer.companyName) {
          alert('请填写客户代码和公司名称');
          return;
        }

        await axios.post(`${API_BASE}/customers`, this.newCustomer);
        alert('客户添加成功!');
        this.newCustomer = { customerCode: '', companyName: '' };
        this.fetchCustomers();
      } catch (error) {
        this.errorMessage = `添加客户失败: ${handleApiError(error)}`;
        console.error('添加客户失败:', error);
      }
    },

    startEditCategory(index: number, category: Category): void {
      this.editingCategory = index;
      this.editCategoryData = { ...category };
    },

    startEditCustomer(index: number, customer: Customer): void {
      this.editingCustomer = index;
      this.editCustomerData = { ...customer };
    },

    async saveCategoryEdit(oldCategoryName: string): Promise<void> {
      try {
        await axios.put(`${API_BASE}/categories/${oldCategoryName}`, {
          newCategoryName: this.editCategoryData.categoryName,
          instruction: this.editCategoryData.instruction
        });

        alert('分类更新成功!');
        this.cancelEdit();
        this.fetchCategories();
      } catch (error) {
        this.errorMessage = `更新分类失败: ${handleApiError(error)}`;
        console.error('更新分类失败:', error);
      }
    },

    async saveCustomerEdit(oldCustomerCode: string): Promise<void> {
      try {
        await axios.put(`${API_BASE}/customers/${oldCustomerCode}`, {
          newCustomerCode: this.editCustomerData.customerCode,
          companyName: this.editCustomerData.companyName
        });

        alert('客户更新成功!');
        this.cancelEdit();
        this.fetchCustomers();
      } catch (error) {
        this.errorMessage = `更新客户失败: ${handleApiError(error)}`;
        console.error('更新客户失败:', error);
      }
    },

    async deleteCategory(categoryName: string): Promise<void> {
      if (!confirm(`确定要删除分类 "${categoryName}" 吗？`)) {
        return;
      }

      try {
        await axios.delete(`${API_BASE}/categories/${categoryName}`);
        alert('分类删除成功!');
        this.fetchCategories();
      } catch (error) {
        this.errorMessage = `删除分类失败: ${handleApiError(error)}`;
        console.error('删除分类失败:', error);
      }
    },

    async deleteCustomer(customerCode: string): Promise<void> {
      if (!confirm(`确定要删除客户 "${customerCode}" 吗？`)) {
        return;
      }

      try {
        await axios.delete(`${API_BASE}/customers/${customerCode}`);
        alert('客户删除成功!');
        this.fetchCustomers();
      } catch (error) {
        this.errorMessage = `删除客户失败: ${handleApiError(error)}`;
        console.error('删除客户失败:', error);
      }
    },

    cancelEdit(): void {
      this.editingCategory = null;
      this.editingCustomer = null;
      this.editCategoryData = { categoryName: '', instruction: '' };
      this.editCustomerData = { customerCode: '', companyName: '' };
    }
  },
  mounted() {
    this.testConnection();
  }
});
</script>

<style>
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  max-width: 1200px;
  margin: 0 auto;
  padding: 20px;
}

.status-section, .section {
  margin: 20px 0;
  padding: 20px;
  border: 1px solid #ddd;
  border-radius: 8px;
}

.status {
  padding: 10px;
  margin: 10px 0;
  border-radius: 4px;
}

.status.success {
  background-color: #e8f5e8;
  color: #2e7d32;
  border: 1px solid #4caf50;
}

.status.error {
  background-color: #ffebee;
  color: #c62828;
  border: 1px solid #f44336;
}

.form {
  margin: 15px 0;
  padding: 15px;
  background-color: #f5f5f5;
  border-radius: 4px;
}

input {
  margin: 5px;
  padding: 8px;
  border: 1px solid #ccc;
  border-radius: 4px;
  width: 200px;
}

button {
  margin: 2px;
  padding: 6px 12px;
  border: none;
  border-radius: 4px;
  cursor: pointer;
  font-size: 14px;
}

button:hover {
  opacity: 0.8;
}

.edit-btn {
  background-color: #ffc107;
  color: black;
}

.save-btn {
  background-color: #28a745;
  color: white;
}

.delete-btn {
  background-color: #dc3545;
  color: white;
}

.cancel-btn {
  background-color: #6c757d;
  color: white;
}

.data-table {
  margin-top: 15px;
}

table {
  width: 100%;
  border-collapse: collapse;
  margin-top: 10px;
}

th, td {
  border: 1px solid #ddd;
  padding: 8px;
  text-align: left;
}

th {
  background-color: #f2f2f2;
}

.error-message {
  padding: 10px;
  background-color: #ffebee;
  color: #c62828;
  border: 1px solid #f44336;
  border-radius: 4px;
  margin: 10px 0;
}
</style>
