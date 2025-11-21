<template>
  <div id="app">
    <!-- 登录页面 -->
    <div v-if="!isLoggedIn" class="login-container">
      <div class="login-form">
        <h1>CD 订单管理系统</h1>

        <div class="form-group">
          <label>用户名:</label>
          <input v-model="loginForm.username" type="text" placeholder="请输入用户名" @keyup.enter="login">
        </div>

        <div class="form-group">
          <label>密码:</label>
          <input v-model="loginForm.password" type="password" placeholder="请输入密码" @keyup.enter="login">
        </div>

        <div class="form-actions">
          <button @click="login" class="login-btn">登录</button>
          <button @click="showRegister = true" class="register-btn">注册</button>
        </div>

        <div v-if="loginError" class="error-message">
          {{ loginError }}
        </div>
      </div>
    </div>

    <!-- 注册模态框 -->
    <div v-if="showRegister" class="modal-overlay">
      <div class="modal">
        <h2>用户注册</h2>

        <div class="form-group">
          <label>用户名:</label>
          <input v-model="registerForm.username" type="text" placeholder="请输入用户名">
        </div>

        <div class="form-group">
          <label>密码:</label>
          <input v-model="registerForm.password" type="password" placeholder="请输入密码">
        </div>

        <div class="modal-actions">
          <button @click="register" class="confirm-btn">注册</button>
          <button @click="showRegister = false" class="cancel-btn">取消</button>
        </div>

        <div v-if="registerError" class="error-message">
          {{ registerError }}
        </div>
      </div>
    </div>

    <!-- 主应用界面 -->
    <div v-if="isLoggedIn" class="main-app">
      <!-- 顶部导航栏 -->
      <header class="app-header">
        <div class="header-left">
          <h1>CD 订单管理系统</h1>
        </div>
        <div class="header-right">
          <span class="user-info">
            欢迎, {{ currentUser.username }} ({{ getUserTypeText(currentUser.userType) }})
          </span>
          <button @click="logout" class="logout-btn">退出登录</button>
        </div>
      </header>

      <!-- 侧边栏导航 -->
      <div class="app-container">
        <nav class="sidebar">
          <ul>
            <li>
              <button @click="currentPage = 'dashboard'" :class="{ active: currentPage === 'dashboard' }"
                class="nav-btn">
                系统概览
              </button>
            </li>
            <li>
              <button @click="currentPage = 'products'" :class="{ active: currentPage === 'products' }" class="nav-btn">
                产品浏览
              </button>
            </li>
            <li>
              <button @click="currentPage = 'orders'" :class="{ active: currentPage === 'orders' }" class="nav-btn">
                我的订单
              </button>
            </li>
            <li v-if="currentUser.userType === 'admin'">
              <button @click="currentPage = 'userManagement'" :class="{ active: currentPage === 'userManagement' }"
                class="nav-btn">
                用户管理
              </button>
            </li>
            <li v-if="currentUser.userType === 'admin'">
              <button @click="currentPage = 'allOrders'" :class="{ active: currentPage === 'allOrders' }"
                class="nav-btn">
                所有订单
              </button>
            </li>
            <li v-if="currentUser.userType === 'admin'">
              <button @click="currentPage = 'customerManagement'"
                :class="{ active: currentPage === 'customerManagement' }" class="nav-btn">
                客户管理
              </button>
            </li>
            <li v-if="currentUser.userType === 'admin'">
              <button @click="currentPage = 'shipperManagement'"
                :class="{ active: currentPage === 'shipperManagement' }" class="nav-btn">
                运货商管理
              </button>
            </li>
            <li v-if="currentUser.userType === 'admin'">
              <button @click="currentPage = 'supplierManagement'"
                :class="{ active: currentPage === 'supplierManagement' }" class="nav-btn">
                供应商管理
              </button>
            </li>
          </ul>
        </nav>

        <!-- 主内容区域 -->
        <main class="main-content">
          <!-- 系统概览页面 -->
          <div v-if="currentPage === 'dashboard'" class="page">
            <h2>系统概览</h2>
            <div class="status-section">
              <h3>系统状态</h3>
              <button @click="testConnection">测试连接</button>
              <div :class="['status', connectionStatus.includes('成功') ? 'success' : 'error']">
                {{ connectionStatus }}
              </div>
            </div>

            <div class="stats-cards">
              <div class="stat-card">
                <h4>可用产品</h4>
                <p class="stat-number">{{ availableProductsCount }}</p>
              </div>
              <div class="stat-card">
                <h4>我的订单</h4>
                <p class="stat-number">{{ myOrdersCount }}</p>
              </div>
              <div class="stat-card">
                <h4>用户权限</h4>
                <p class="stat-text">{{ getUserTypeText(currentUser.userType) }}</p>
              </div>
            </div>

            <div class="quick-actions">
              <h4>快速操作</h4>
              <div class="action-buttons">
                <button @click="currentPage = 'products'" class="action-btn">
                  浏览产品
                </button>
                <button @click="currentPage = 'orders'" class="action-btn">
                  查看订单
                </button>
                <button v-if="currentUser.userType === 'admin'" @click="currentPage = 'userManagement'"
                  class="action-btn">
                  用户管理
                </button>
              </div>
            </div>
          </div>

          <!-- 产品浏览页面 -->
          <div v-if="currentPage === 'products'" class="page">
            <h2>产品浏览</h2>
            <div class="search-section">
              <input v-model="productSearch" placeholder="搜索产品名称..." class="search-input">
              <button @click="fetchProducts" class="search-btn">搜索</button>
            </div>

            <div class="products-grid">
              <div v-for="product in filteredProducts" :key="product.productID" class="product-card"
                :class="{ 'low-stock': product.inventoryNum < 10 }">
                <div class="product-info">
                  <h3>{{ product.productName }}</h3>
                  <p class="product-spec">{{ product.productNum }}</p>
                  <p class="product-price">¥{{ product.price }}</p>
                  <p class="product-stock">库存: {{ product.inventoryNum }}</p>
                </div>

                <div class="product-actions">
                  <button @click="showCreateOrderModal(product)" class="order-btn"
                    :disabled="product.inventoryNum <= 0">
                    {{ product.inventoryNum > 0 ? '立即订购' : '缺货' }}
                  </button>
                </div>
              </div>
            </div>
          </div>

          <!-- 创建订单模态框 -->
          <div v-if="showCreateOrder" class="modal-overlay">
            <div class="modal checkout-modal">
              <h2>创建订单</h2>

              <div class="order-products">
                <h4>订购产品</h4>
                <div class="product-order-item">
                  <span class="product-name">{{ selectedProduct.productName }}</span>
                  <div class="quantity-controls">
                    <button @click="decreaseOrderQuantity" :disabled="orderQuantity <= 1" class="quantity-btn">
                      -
                    </button>
                    <span class="quantity-display">{{ orderQuantity }}</span>
                    <button @click="increaseOrderQuantity" :disabled="orderQuantity >= selectedProduct.inventoryNum"
                      class="quantity-btn">
                      +
                    </button>
                  </div>
                  <span class="product-subtotal">
                    ¥{{ (selectedProduct.price * orderQuantity).toFixed(2) }}
                  </span>
                </div>
              </div>

              <div class="form-group">
                <label>收货人姓名:</label>
                <input v-model="orderForm.ownerName" type="text" required readonly class="readonly-input">
                <small class="form-hint">自动填充为当前用户名</small>
              </div>

              <div class="form-group">
                <label>收货地址:</label>
                <input v-model="orderForm.ownerAddress" type="text" required>
              </div>

              <div class="form-group">
                <label>所在城市:</label>
                <input v-model="orderForm.ownerCity" type="text" required>
              </div>

              <div class="form-group">
                <label>支付方式:</label>
                <select v-model="orderForm.payWay" required>
                  <option value="支付宝">支付宝</option>
                  <option value="微信">微信</option>
                  <option value="银行卡">银行卡</option>
                </select>
              </div>

              <div class="form-group">
                <label>
                  <input type="checkbox" v-model="orderForm.insurance">
                  购买运输保险
                </label>
              </div>

              <div class="order-summary">
                <h4>订单摘要</h4>
                <div class="summary-item">
                  {{ selectedProduct.productName }} × {{ orderQuantity }} =
                  ¥{{ (selectedProduct.price * orderQuantity).toFixed(2) }}
                </div>
                <div class="summary-total">总计: ¥{{ (selectedProduct.price * orderQuantity).toFixed(2) }}</div>
              </div>

              <div class="modal-actions">
                <button @click="createOrder" class="confirm-btn">确认下单</button>
                <button @click="showCreateOrder = false" class="cancel-btn">取消</button>
              </div>
            </div>
          </div>

          <!-- 我的订单页面 -->
          <div v-if="currentPage === 'orders'" class="page">
            <h2>我的订单</h2>
            <button @click="fetchMyOrders" class="fetch-btn">刷新订单</button>

            <div v-if="myOrders.length > 0" class="orders-table">
              <table>
                <thead>
                  <tr>
                    <th>订单ID</th>
                    <th>下单时间</th>
                    <th>收货人</th>
                    <th>所在城市</th>
                    <th>支付方式</th>
                    <th>订单金额</th>
                    <th>操作</th>
                  </tr>
                </thead>
                <tbody>
                  <tr v-for="order in myOrders" :key="order.orderID">
                    <td>{{ order.orderID }}</td>
                    <td>{{ order.orderDate }}</td>
                    <td>{{ order.ownerName }}</td>
                    <td>{{ order.ownerCity }}</td>
                    <td>{{ order.payWay }}</td>
                    <td>¥{{ order.totalAmount ? order.totalAmount.toFixed(2) : '0.00' }}</td>
                    <td>
                      <button @click="viewOrderDetails(order.orderID)" class="view-btn">
                        查看详情
                      </button>
                      <button @click="deleteOrder(order.orderID)" class="delete-btn">
                        取消订单
                      </button>
                    </td>
                  </tr>
                </tbody>
              </table>
            </div>

            <div v-else class="no-data">
              暂无订单
            </div>
          </div>

          <!-- 订单详情模态框 -->
          <div v-if="showOrderDetails" class="modal-overlay">
            <div class="modal">
              <h2>订单详情 #{{ currentOrder.orderID }}</h2>

              <div class="order-details">
                <div class="detail-section">
                  <h4>订单信息</h4>
                  <p><strong>下单时间:</strong> {{ currentOrder.orderDate }}</p>
                  <p><strong>预计发货:</strong> {{ currentOrder.supplyDate }}</p>
                  <p><strong>预计送达:</strong> {{ currentOrder.arrivalDate }}</p>
                </div>

                <div class="detail-section">
                  <h4>收货信息</h4>
                  <p><strong>收货人:</strong> {{ currentOrder.ownerName }}</p>
                  <p><strong>地址:</strong> {{ currentOrder.ownerAddress }}</p>
                  <p><strong>城市:</strong> {{ currentOrder.ownerCity }}</p>
                  <p><strong>支付方式:</strong> {{ currentOrder.payWay }}</p>
                </div>

                <div class="detail-section">
                  <h4>商品清单</h4>
                  <table class="details-table">
                    <thead>
                      <tr>
                        <th>商品名称</th>
                        <th>单价</th>
                        <th>数量</th>
                        <th>小计</th>
                      </tr>
                    </thead>
                    <tbody>
                      <tr v-for="detail in orderDetails" :key="detail.productID">
                        <td>{{ detail.productName }}</td>
                        <td>¥{{ detail.price }}</td>
                        <td>{{ detail.num }}</td>
                        <td>¥{{ detail.subtotal }}</td>
                      </tr>
                    </tbody>
                    <tfoot>
                      <tr>
                        <td colspan="3"><strong>总计:</strong></td>
                        <td><strong>¥{{ orderDetailsTotal.toFixed(2) }}</strong></td>
                      </tr>
                    </tfoot>
                  </table>
                </div>
              </div>

              <div class="modal-actions">
                <button @click="showOrderDetails = false" class="cancel-btn">关闭</button>
              </div>
            </div>
          </div>

          <!-- 用户管理页面 (仅管理员) -->
          <div v-if="currentPage === 'userManagement' && currentUser.userType === 'admin'" class="page">
            <h2>用户管理</h2>
            <button @click="fetchUsers" class="fetch-btn">刷新用户列表</button>

            <div v-if="users.length > 0" class="users-table">
              <table>
                <thead>
                  <tr>
                    <th>用户ID</th>
                    <th>用户名</th>
                    <th>用户类型</th>
                    <th>注册时间</th>
                    <th>操作</th>
                  </tr>
                </thead>
                <tbody>
                  <tr v-for="user in users" :key="user.id">
                    <td>{{ user.id }}</td>
                    <td>{{ user.username }}</td>
                    <td>{{ getUserTypeText(user.userType) }}</td>
                    <td>{{ user.createdAt }}</td>
                    <td>
                      <button @click="showChangePassword(user)" class="edit-btn">
                        修改密码
                      </button>
                      <button @click="deleteUser(user.id)" class="delete-btn" :disabled="user.id === currentUser.id">
                        删除用户
                      </button>
                    </td>
                  </tr>
                </tbody>
              </table>
            </div>
          </div>

          <!-- 修改密码模态框 -->
          <div v-if="showChangePasswordModal" class="modal-overlay">
            <div class="modal">
              <h2>修改用户密码</h2>
              <p>用户: {{ selectedUser.username }}</p>

              <div class="form-group">
                <label>新密码:</label>
                <input v-model="newPassword" type="password" required>
              </div>

              <div class="modal-actions">
                <button @click="changeUserPassword" class="confirm-btn">确认修改</button>
                <button @click="showChangePasswordModal = false" class="cancel-btn">取消</button>
              </div>
            </div>
          </div>

          <!-- 所有订单页面 (仅管理员) -->
          <div v-if="currentPage === 'allOrders' && currentUser.userType === 'admin'" class="page">
            <h2>所有用户订单</h2>
            <button @click="fetchAllOrders" class="fetch-btn">刷新订单列表</button>

            <div v-if="allOrders.length > 0" class="orders-table">
              <table>
                <thead>
                  <tr>
                    <th>订单ID</th>
                    <th>下单时间</th>
                    <th>收货人</th>
                    <th>所在城市</th>
                    <th>支付方式</th>
                    <th>订单金额</th>
                    <th>操作</th>
                  </tr>
                </thead>
                <tbody>
                  <tr v-for="order in allOrders" :key="order.orderID">
                    <td>{{ order.orderID }}</td>
                    <td>{{ order.orderDate }}</td>
                    <td>{{ order.ownerName }}</td>
                    <td>{{ order.ownerCity }}</td>
                    <td>{{ order.payWay }}</td>
                    <td>¥{{ order.totalAmount ? order.totalAmount.toFixed(2) : '0.00' }}</td>
                    <td>
                      <button @click="viewOrderDetails(order.orderID)" class="view-btn">
                        查看详情
                      </button>
                      <button @click="deleteOrder(order.orderID)" class="delete-btn">
                        删除订单
                      </button>
                    </td>
                  </tr>
                </tbody>
              </table>
            </div>
          </div>

          <!-- 客户管理页面 -->
          <div v-if="currentPage === 'customerManagement' && currentUser.userType === 'admin'" class="page">
            <h2>客户管理</h2>

            <div class="management-actions">
              <div class="search-section">
                <input v-model="customerSearch" placeholder="搜索客户名称..." class="search-input">
                <button @click="fetchCustomers" class="search-btn">搜索</button>
                <button @click="showAddCustomerModal = true" class="add-btn">添加客户</button>
              </div>
              <button @click="fetchCustomers" class="fetch-btn">刷新</button>
            </div>

            <div v-if="customers.length > 0" class="management-table">
              <table>
                <thead>
                  <tr>
                    <th>客户代码</th>
                    <th>公司名称</th>
                    <th>联系人</th>
                    <th>职位</th>
                    <th>地址</th>
                    <th>城市</th>
                    <th>电话</th>
                    <th>操作</th>
                  </tr>
                </thead>
                <tbody>
                  <tr v-for="customer in filteredCustomers" :key="customer.customerCode">
                    <td>{{ customer.customerCode }}</td>
                    <td>{{ customer.companyName }}</td>
                    <td>{{ customer.contactName }}</td>
                    <td>{{ customer.contactPosition }}</td>
                    <td>{{ customer.address }}</td>
                    <td>{{ customer.city }}</td>
                    <td>{{ customer.tel }}</td>
                    <td>
                      <button @click="editCustomer(customer)" class="edit-btn">编辑</button>
                      <button @click="deleteCustomer(customer.customerCode)" class="delete-btn">删除</button>
                    </td>
                  </tr>
                </tbody>
              </table>
            </div>
            <div v-else class="no-data">
              暂无客户数据
            </div>
          </div>


          <!-- 添加/编辑客户模态框 -->
          <div v-if="showAddCustomerModal || showEditCustomerModal" class="modal-overlay">
            <div class="modal">
              <h2>{{ showEditCustomerModal ? '编辑客户' : '添加客户' }}</h2>

              <div class="form-group">
                <label>客户代码:</label>
                <input v-model="customerForm.customerCode" type="text" required :readonly="showEditCustomerModal">
              </div>

              <div class="form-group">
                <label>公司名称:</label>
                <input v-model="customerForm.companyName" type="text" required>
              </div>

              <div class="form-group">
                <label>联系人:</label>
                <input v-model="customerForm.contactName" type="text">
              </div>

              <div class="form-group">
                <label>职位:</label>
                <input v-model="customerForm.contactPosition" type="text">
              </div>

              <div class="form-group">
                <label>地址:</label>
                <input v-model="customerForm.address" type="text">
              </div>

              <div class="form-group">
                <label>城市:</label>
                <input v-model="customerForm.city" type="text">
              </div>

              <div class="form-group">
                <label>地区:</label>
                <input v-model="customerForm.area" type="text">
              </div>

              <div class="form-group">
                <label>邮编:</label>
                <input v-model="customerForm.postCode" type="text">
              </div>

              <div class="form-group">
                <label>国家:</label>
                <input v-model="customerForm.country" type="text">
              </div>

              <div class="form-group">
                <label>电话:</label>
                <input v-model="customerForm.tel" type="text">
              </div>

              <div class="form-group">
                <label>传真:</label>
                <input v-model="customerForm.fax" type="text">
              </div>

              <div class="modal-actions">
                <button @click="saveCustomer" class="confirm-btn">{{ showEditCustomerModal ? '更新' : '添加' }}</button>
                <button @click="closeCustomerModal" class="cancel-btn">取消</button>
              </div>
            </div>
          </div>

          <!-- 运货商管理页面 -->
          <div v-if="currentPage === 'shipperManagement' && currentUser.userType === 'admin'" class="page">
            <h2>运货商管理</h2>

            <div class="management-actions">
              <div class="search-section">
                <input v-model="shipperSearch" placeholder="搜索运货商名称..." class="search-input">
                <button @click="fetchShippers" class="search-btn">搜索</button>
                <button @click="showAddShipperModal = true" class="add-btn">添加运货商</button>
              </div>
              <button @click="fetchShippers" class="fetch-btn">刷新</button>
            </div>

            <div v-if="shippers.length > 0" class="management-table">
              <table>
                <thead>
                  <tr>
                    <th>运货商ID</th>
                    <th>公司名称</th>
                    <th>电话</th>
                    <th>运输工具</th>
                    <th>操作</th>
                  </tr>
                </thead>
                <tbody>
                  <tr v-for="shipper in filteredShippers" :key="shipper.shipperID">
                    <td>{{ shipper.shipperID }}</td>
                    <td>{{ shipper.companyName }}</td>
                    <td>{{ shipper.tel }}</td>
                    <td>{{ shipper.transportTool }}</td>
                    <td>
                      <button @click="editShipper(shipper)" class="edit-btn">编辑</button>
                      <button @click="deleteShipper(shipper.shipperID)" class="delete-btn">删除</button>
                    </td>
                  </tr>
                </tbody>
              </table>
            </div>
            <div v-else class="no-data">
              暂无运货商数据
            </div>
          </div>

          <!-- 添加/编辑运货商模态框 -->
          <div v-if="showAddShipperModal || showEditShipperModal" class="modal-overlay">
            <div class="modal">
              <h2>{{ showEditShipperModal ? '编辑运货商' : '添加运货商' }}</h2>

              <div class="form-group">
                <label>运货商ID:</label>
                <input v-model="shipperForm.shipperID" type="text" required :readonly="showEditShipperModal">
              </div>

              <div class="form-group">
                <label>公司名称:</label>
                <input v-model="shipperForm.companyName" type="text" required>
              </div>

              <div class="form-group">
                <label>电话:</label>
                <input v-model="shipperForm.tel" type="text">
              </div>

              <div class="form-group">
                <label>运输工具:</label>
                <input v-model="shipperForm.transportTool" type="text">
              </div>

              <div class="modal-actions">
                <button @click="saveShipper" class="confirm-btn">{{ showEditShipperModal ? '更新' : '添加' }}</button>
                <button @click="closeShipperModal" class="cancel-btn">取消</button>
              </div>
            </div>
          </div>

          <!-- 供应商管理页面 -->
          <div v-if="currentPage === 'supplierManagement' && currentUser.userType === 'admin'" class="page">
            <h2>供应商管理</h2>

            <div class="management-actions">
              <div class="search-section">
                <input v-model="supplierSearch" placeholder="搜索供应商名称..." class="search-input">
                <button @click="fetchSuppliers" class="search-btn">搜索</button>
                <button @click="showAddSupplierModal = true" class="add-btn">添加供应商</button>
              </div>
              <button @click="fetchSuppliers" class="fetch-btn">刷新</button>
            </div>

            <div v-if="suppliers.length > 0" class="management-table">
              <table>
                <thead>
                  <tr>
                    <th>供应商ID</th>
                    <th>公司名称</th>
                    <th>联系人</th>
                    <th>职位</th>
                    <th>地址</th>
                    <th>城市</th>
                    <th>电话</th>
                    <th>操作</th>
                  </tr>
                </thead>
                <tbody>
                  <tr v-for="supplier in filteredSuppliers" :key="supplier.supplierID">
                    <td>{{ supplier.supplierID }}</td>
                    <td>{{ supplier.companyName }}</td>
                    <td>{{ supplier.contactName }}</td>
                    <td>{{ supplier.contactPosition }}</td>
                    <td>{{ supplier.address }}</td>
                    <td>{{ supplier.city }}</td>
                    <td>{{ supplier.tel }}</td>
                    <td>
                      <button @click="editSupplier(supplier)" class="edit-btn">编辑</button>
                      <button @click="deleteSupplier(supplier.supplierID)" class="delete-btn">删除</button>
                    </td>
                  </tr>
                </tbody>
              </table>
            </div>
            <div v-else class="no-data">
              暂无供应商数据
            </div>
          </div>

          <!-- 添加/编辑供应商模态框 -->
          <div v-if="showAddSupplierModal || showEditSupplierModal" class="modal-overlay">
            <div class="modal">
              <h2>{{ showEditSupplierModal ? '编辑供应商' : '添加供应商' }}</h2>

              <div class="form-group">
                <label>供应商ID:</label>
                <input v-model="supplierForm.supplierID" type="text" required :readonly="showEditSupplierModal">
              </div>

              <div class="form-group">
                <label>公司名称:</label>
                <input v-model="supplierForm.companyName" type="text" required>
              </div>

              <div class="form-group">
                <label>联系人:</label>
                <input v-model="supplierForm.contactName" type="text">
              </div>

              <div class="form-group">
                <label>职位:</label>
                <input v-model="supplierForm.contactPosition" type="text">
              </div>

              <div class="form-group">
                <label>地址:</label>
                <input v-model="supplierForm.address" type="text">
              </div>

              <div class="form-group">
                <label>城市:</label>
                <input v-model="supplierForm.city" type="text">
              </div>

              <div class="form-group">
                <label>地区:</label>
                <input v-model="supplierForm.area" type="text">
              </div>

              <div class="form-group">
                <label>邮编:</label>
                <input v-model="supplierForm.postCode" type="text">
              </div>

              <div class="form-group">
                <label>国家:</label>
                <input v-model="supplierForm.country" type="text">
              </div>

              <div class="form-group">
                <label>电话:</label>
                <input v-model="supplierForm.tel" type="text">
              </div>

              <div class="form-group">
                <label>传真:</label>
                <input v-model="supplierForm.fax" type="text">
              </div>

              <div class="form-group">
                <label>主页:</label>
                <input v-model="supplierForm.homePage" type="text">
              </div>

              <div class="modal-actions">
                <button @click="saveSupplier" class="confirm-btn">{{ showEditSupplierModal ? '更新' : '添加' }}</button>
                <button @click="closeSupplierModal" class="cancel-btn">取消</button>
              </div>
            </div>
          </div>

        </main>
      </div>
    </div>

    <!-- 全局错误消息 -->
    <div v-if="errorMessage" class="global-error">
      {{ errorMessage }}
      <button @click="errorMessage = ''" class="close-error">×</button>
    </div>

    <!-- 加载遮罩 -->
    <div v-if="loading" class="loading-overlay">
      <div class="loading-spinner"></div>
      <p>加载中...</p>
    </div>
  </div>
</template>

<script lang="ts">
import { defineComponent } from 'vue';
import axios, { AxiosError } from 'axios';

const API_BASE = 'http://localhost:3001/api';

interface Product {
  productID: number;
  productName: string;
  productNum: string;
  price: number;
  inventoryNum: number;
  orderNum: number | null;
  twiceOrderNum: number | null;
  terminal: number | null;
}

interface Order {
  orderID: number;
  orderDate: string;
  supplyDate: string;
  arrivalDate: string;
  ownerName: string;
  ownerAddress: string;
  ownerCity: string;
  payWay: string;
  insurance: number;
  totalAmount?: number;
}

interface OrderDetail {
  productID: number;
  productName: string;
  price: number;
  num: number;
  subtotal: number;
}

interface User {
  id: number;
  username: string;
  userType: 'admin' | 'user';
  createdAt: string;
}

interface Customer {
  customerCode: string;
  companyName: string;
  contactName: string;
  contactPosition: string;
  address: string;
  city: string;
  area: string;
  postCode: string;
  country: string;
  tel: string;
  fax: string;
}

interface Shipper {
  shipperID: string;
  companyName: string;
  tel: string;
  transportTool: string;
}

interface Supplier {
  supplierID: string;
  companyName: string;
  contactName: string;
  contactPosition: string;
  address: string;
  city: string;
  area: string;
  postCode: string;
  country: string;
  tel: string;
  fax: string;
  homePage: string;
}

interface ApiErrorResponse {
  error?: string;
  details?: string;
}

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
      // 认证状态
      isLoggedIn: false,
      currentUser: {} as User,
      token: '',

      // 登录表单
      loginForm: {
        username: '',
        password: ''
      },
      loginError: '',

      // 注册表单
      showRegister: false,
      registerForm: {
        username: '',
        password: '',
        userType: 'user'
      },
      registerError: '',

      // 页面导航
      currentPage: 'dashboard',

      // 系统状态
      connectionStatus: '点击测试连接按钮检查连接状态',

      // 产品数据
      products: [] as Product[],
      productSearch: '',

      // 订单创建
      showCreateOrder: false,
      selectedProduct: {} as Product,
      orderQuantity: 1,
      orderForm: {
        ownerName: '',
        ownerAddress: '',
        ownerCity: '',
        payWay: '支付宝',
        insurance: false
      },

      // 订单数据
      myOrders: [] as Order[],
      allOrders: [] as Order[],
      showOrderDetails: false,
      currentOrder: {} as Order,
      orderDetails: [] as OrderDetail[],

      // 用户管理 (仅管理员)
      users: [] as User[],
      showChangePasswordModal: false,
      selectedUser: {} as User,
      newPassword: '',

      // 客户管理
      customers: [] as Customer[],
      customerSearch: '',
      showAddCustomerModal: false,
      showEditCustomerModal: false,
      customerForm: {
        customerCode: '',
        companyName: '',
        contactName: '',
        contactPosition: '',
        address: '',
        city: '',
        area: '',
        postCode: '',
        country: '',
        tel: '',
        fax: ''
      },

      // 运货商管理
      shippers: [] as Shipper[],
      shipperSearch: '',
      showAddShipperModal: false,
      showEditShipperModal: false,
      shipperForm: {
        shipperID: '',
        companyName: '',
        tel: '',
        transportTool: ''
      },

      // 供应商管理
      suppliers: [] as Supplier[],
      supplierSearch: '',
      showAddSupplierModal: false,
      showEditSupplierModal: false,
      supplierForm: {
        supplierID: '',
        companyName: '',
        contactName: '',
        contactPosition: '',
        address: '',
        city: '',
        area: '',
        postCode: '',
        country: '',
        tel: '',
        fax: '',
        homePage: ''
      },
      // 错误和加载状态
      errorMessage: '',
      loading: false
    }
  },
  computed: {
    // 过滤产品
    filteredProducts(): Product[] {
      if (!this.productSearch) {
        return this.products;
      }
      return this.products.filter(product =>
        product.productName.toLowerCase().includes(this.productSearch.toLowerCase())
      );
    },

    // 可用产品数量
    availableProductsCount(): number {
      return this.products.filter(p => p.inventoryNum > 0).length;
    },

    // 我的订单数量
    myOrdersCount(): number {
      return this.myOrders.length;
    },

    // 订单详情总价
    orderDetailsTotal(): number {
      return this.orderDetails.reduce((total, detail) => total + detail.subtotal, 0);
    },
    filteredCustomers(): Customer[] {
      if (!this.customerSearch) return this.customers;
      return this.customers.filter(customer =>
        customer.companyName.toLowerCase().includes(this.customerSearch.toLowerCase()) ||
        customer.contactName?.toLowerCase().includes(this.customerSearch.toLowerCase())
      );
    },

    filteredShippers(): Shipper[] {
      if (!this.shipperSearch) return this.shippers;
      return this.shippers.filter(shipper =>
        shipper.companyName.toLowerCase().includes(this.shipperSearch.toLowerCase())
      );
    },

    filteredSuppliers(): Supplier[] {
      if (!this.supplierSearch) return this.suppliers;
      return this.suppliers.filter(supplier =>
        supplier.companyName.toLowerCase().includes(this.supplierSearch.toLowerCase()) ||
        supplier.contactName?.toLowerCase().includes(this.supplierSearch.toLowerCase())
      );
    }

  },
  methods: {
    // 订单创建相关方法
    showCreateOrderModal(product: Product): void {
      this.selectedProduct = product;
      this.orderQuantity = 1;
      this.orderForm = {
        ownerName: this.currentUser.username, // 自动填充当前用户名
        ownerAddress: '',
        ownerCity: '',
        payWay: '支付宝',
        insurance: false
      };
      this.showCreateOrder = true;
    },

    increaseOrderQuantity(): void {
      if (this.orderQuantity < this.selectedProduct.inventoryNum) {
        this.orderQuantity++;
      }
    },

    decreaseOrderQuantity(): void {
      if (this.orderQuantity > 1) {
        this.orderQuantity--;
      }
    },


    async createOrder(): Promise<void> {
      try {
        // 确保收货人姓名是当前用户名（防止用户修改）
        const orderData = {
          orderDetails: [{
            productID: this.selectedProduct.productID,
            quantity: this.orderQuantity
          }],
          ownerName: this.currentUser.username, // 强制使用当前用户名
          ownerAddress: this.orderForm.ownerAddress,
          ownerCity: this.orderForm.ownerCity,
          payWay: this.orderForm.payWay,
          insurance: this.orderForm.insurance ? 1 : 0
        };

        // 验证必填字段
        if (!orderData.ownerAddress || !orderData.ownerCity) {
          alert('请填写完整的收货信息');
          return;
        }

        const response = await axios.post(`${API_BASE}/orders`, orderData);

        alert(`订单创建成功！订单号: ${response.data.orderId}`);

        // 关闭模态框
        this.showCreateOrder = false;

        // 添加短暂延迟确保数据库事务完成
        await new Promise(resolve => setTimeout(resolve, 500));

        // 刷新数据
        await this.fetchProducts();
        await this.fetchMyOrders();

      } catch (error) {
        this.errorMessage = `创建订单失败: ${handleApiError(error)}`;
        console.error('创建订单失败:', error);
      }
    },

    async login(): Promise<void> {
      this.loading = true;
      this.loginError = '';

      try {
        const response = await axios.post(`${API_BASE}/login`, this.loginForm);

        this.token = response.data.token;
        this.currentUser = response.data.user;
        this.isLoggedIn = true;

        localStorage.setItem('authToken', this.token);
        localStorage.setItem('userData', JSON.stringify(this.currentUser));

        axios.defaults.headers.common['Authorization'] = `Bearer ${this.token}`;

        this.loginForm = { username: '', password: '' };
        this.currentPage = 'dashboard';

        this.testConnection();
        this.fetchProducts();
        this.fetchMyOrders();

      } catch (error) {
        this.loginError = handleApiError(error);
      } finally {
        this.loading = false;
      }
    },

    async register(): Promise<void> {
      this.loading = true;
      this.registerError = '';

      try {
        await axios.post(`${API_BASE}/register`, this.registerForm);

        alert('注册成功！请使用新账户登录。');
        this.showRegister = false;
        this.registerForm = { username: '', password: '', userType: 'user' };

      } catch (error) {
        this.registerError = handleApiError(error);
      } finally {
        this.loading = false;
      }
    },

    logout(): void {
      this.isLoggedIn = false;
      this.currentUser = {} as User;
      this.token = '';

      localStorage.removeItem('authToken');
      localStorage.removeItem('userData');

      delete axios.defaults.headers.common['Authorization'];

      this.currentPage = 'dashboard';
      this.products = [];
      this.myOrders = [];
      this.allOrders = [];
    },

    checkAuth(): void {
      const token = localStorage.getItem('authToken');
      const userData = localStorage.getItem('userData');

      if (token && userData) {
        this.token = token;
        this.currentUser = JSON.parse(userData);
        this.isLoggedIn = true;

        axios.defaults.headers.common['Authorization'] = `Bearer ${this.token}`;

        this.testConnection();
        this.fetchProducts();
        this.fetchMyOrders();
      }
    },

    getUserTypeText(userType: string): string {
      const typeMap: { [key: string]: string } = {
        'admin': '管理员',
        'user': '普通用户'
      };
      return typeMap[userType] || '未知用户';
    },

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

    async fetchProducts(): Promise<void> {
      try {
        const response = await axios.get(`${API_BASE}/products`);
        this.products = response.data.data;
        this.errorMessage = '';
      } catch (error) {
        this.errorMessage = `获取产品失败: ${handleApiError(error)}`;
        console.error('获取产品失败:', error);
      }
    },

    async fetchMyOrders(): Promise<void> {
      try {
        const response = await axios.get(`${API_BASE}/orders`);
        this.myOrders = response.data.data;
        this.errorMessage = '';
      } catch (error) {
        this.errorMessage = `获取订单失败: ${handleApiError(error)}`;
        console.error('获取订单失败:', error);
      }
    },

    async fetchAllOrders(): Promise<void> {
      try {
        // 使用正确的 API 端点
        const response = await axios.get(`${API_BASE}/orders`);
        this.allOrders = response.data.data;
        this.errorMessage = '';
      } catch (error) {
        this.errorMessage = `获取所有订单失败: ${handleApiError(error)}`;
        console.error('获取所有订单失败:', error);
      }
    },

    async fetchUsers(): Promise<void> {
      try {
        const response = await axios.get(`${API_BASE}/users`);
        this.users = response.data.data;
        this.errorMessage = '';
      } catch (error) {
        this.errorMessage = `获取用户列表失败: ${handleApiError(error)}`;
        console.error('获取用户列表失败:', error);
      }
    },

    async viewOrderDetails(orderId: number): Promise<void> {
      try {
        const response = await axios.get(`${API_BASE}/orders/${orderId}`);
        this.currentOrder = response.data.data.order;
        this.orderDetails = response.data.data.details;
        this.showOrderDetails = true;
      } catch (error) {
        this.errorMessage = `获取订单详情失败: ${handleApiError(error)}`;
        console.error('获取订单详情失败:', error);
      }
    },

    async deleteOrder(orderId: number): Promise<void> {
      if (!confirm(`确定要删除订单 #${orderId} 吗？`)) {
        return;
      }

      try {
        await axios.delete(`${API_BASE}/orders/${orderId}`);
        alert('订单删除成功！');

        this.fetchProducts();
        this.fetchMyOrders();
        if (this.currentUser.userType === 'admin') {
          this.fetchAllOrders();
        }

      } catch (error) {
        this.errorMessage = `删除订单失败: ${handleApiError(error)}`;
        console.error('删除订单失败:', error);
      }
    },

    // 用户管理操作 (仅管理员)
    showChangePassword(user: User): void {
      this.selectedUser = user;
      this.newPassword = '';
      this.showChangePasswordModal = true;
    },

    async changeUserPassword(): Promise<void> {
      if (!this.newPassword) {
        alert('请输入新密码');
        return;
      }

      try {
        await axios.put(`${API_BASE}/users/${this.selectedUser.id}/password`, {
          newPassword: this.newPassword
        });

        alert('密码修改成功！');
        this.showChangePasswordModal = false;
        this.selectedUser = {} as User;
        this.newPassword = '';

      } catch (error) {
        this.errorMessage = `修改密码失败: ${handleApiError(error)}`;
        console.error('修改密码失败:', error);
      }
    },

    async deleteUser(userId: number): Promise<void> {
      if (!confirm('确定要删除这个用户吗？此操作不可恢复。')) {
        return;
      }

      try {
        await axios.delete(`${API_BASE}/users/${userId}`);
        alert('用户删除成功！');
        this.fetchUsers();
      } catch (error) {
        this.errorMessage = `删除用户失败: ${handleApiError(error)}`;
        console.error('删除用户失败:', error);
      }
    },
    // 客户管理方法
    async fetchCustomers(): Promise<void> {
      try {
        const response = await axios.get(`${API_BASE}/customers`);
        this.customers = response.data.data;
        this.errorMessage = '';
      } catch (error) {
        this.errorMessage = `获取客户列表失败: ${handleApiError(error)}`;
        console.error('获取客户列表失败:', error);
      }
    },

    editCustomer(customer: Customer): void {
      this.customerForm = { ...customer };
      this.showEditCustomerModal = true;
    },

    async saveCustomer(): Promise<void> {
      try {
        if (this.showEditCustomerModal) {
          await axios.put(`${API_BASE}/customers/${this.customerForm.customerCode}`, this.customerForm);
          alert('客户信息更新成功！');
        } else {
          await axios.post(`${API_BASE}/customers`, this.customerForm);
          alert('客户添加成功！');
        }
        this.closeCustomerModal();
        this.fetchCustomers();
      } catch (error) {
        this.errorMessage = `保存客户信息失败: ${handleApiError(error)}`;
      }
    },

    async deleteCustomer(customerCode: string): Promise<void> {
      if (!confirm('确定要删除这个客户吗？此操作不可恢复。')) {
        return;
      }

      try {
        await axios.delete(`${API_BASE}/customers/${customerCode}`);
        alert('客户删除成功！');
        this.fetchCustomers();
      } catch (error) {
        this.errorMessage = `删除客户失败: ${handleApiError(error)}`;
      }
    },

    closeCustomerModal(): void {
      this.showAddCustomerModal = false;
      this.showEditCustomerModal = false;
      this.customerForm = {
        customerCode: '',
        companyName: '',
        contactName: '',
        contactPosition: '',
        address: '',
        city: '',
        area: '',
        postCode: '',
        country: '',
        tel: '',
        fax: ''
      };
    },

    // 运货商管理方法 (类似客户管理)
    async fetchShippers(): Promise<void> {
      try {
        const response = await axios.get(`${API_BASE}/shippers`);
        this.shippers = response.data.data;
        this.errorMessage = '';
      } catch (error) {
        this.errorMessage = `获取运货商列表失败: ${handleApiError(error)}`;
        console.error('获取运货商列表失败:', error);
      }
    },

    editShipper(shipper: Shipper): void {
      this.shipperForm = { ...shipper };
      this.showEditShipperModal = true;
    },

    async saveShipper(): Promise<void> {
      try {
        if (this.showEditShipperModal) {
          await axios.put(`${API_BASE}/shippers/${this.shipperForm.shipperID}`, this.shipperForm);
          alert('运货商信息更新成功！');
        } else {
          await axios.post(`${API_BASE}/shippers`, this.shipperForm);
          alert('运货商添加成功！');
        }
        this.closeShipperModal();
        this.fetchShippers();
      } catch (error) {
        this.errorMessage = `保存运货商信息失败: ${handleApiError(error)}`;
      }
    },

    async deleteShipper(shipperID: string): Promise<void> {
      if (!confirm('确定要删除这个运货商吗？此操作不可恢复。')) {
        return;
      }

      try {
        await axios.delete(`${API_BASE}/shippers/${shipperID}`);
        alert('运货商删除成功！');
        this.fetchShippers();
      } catch (error) {
        this.errorMessage = `删除运货商失败: ${handleApiError(error)}`;
      }
    },

    closeShipperModal(): void {
      this.showAddShipperModal = false;
      this.showEditShipperModal = false;
      this.shipperForm = {
        shipperID: '',
        companyName: '',
        tel: '',
        transportTool: ''
      };
    },

    // 供应商管理方法 (类似客户管理)
    async fetchSuppliers(): Promise<void> {
      try {
        const response = await axios.get(`${API_BASE}/suppliers`);
        this.suppliers = response.data.data;
        this.errorMessage = '';
      } catch (error) {
        this.errorMessage = `获取供应商列表失败: ${handleApiError(error)}`;
        console.error('获取供应商列表失败:', error);
      }
    },

    editSupplier(supplier: Supplier): void {
      this.supplierForm = { ...supplier };
      this.showEditSupplierModal = true;
    },

    async saveSupplier(): Promise<void> {
      try {
        if (this.showEditSupplierModal) {
          await axios.put(`${API_BASE}/suppliers/${this.supplierForm.supplierID}`, this.supplierForm);
          alert('供应商信息更新成功！');
        } else {
          await axios.post(`${API_BASE}/suppliers`, this.supplierForm);
          alert('供应商添加成功！');
        }
        this.closeSupplierModal();
        this.fetchSuppliers();
      } catch (error) {
        this.errorMessage = `保存供应商信息失败: ${handleApiError(error)}`;
      }
    },

    async deleteSupplier(supplierID: string): Promise<void> {
      if (!confirm('确定要删除这个供应商吗？此操作不可恢复。')) {
        return;
      }

      try {
        await axios.delete(`${API_BASE}/suppliers/${supplierID}`);
        alert('供应商删除成功！');
        this.fetchSuppliers();
      } catch (error) {
        this.errorMessage = `删除供应商失败: ${handleApiError(error)}`;
      }
    },

    closeSupplierModal(): void {
      this.showAddSupplierModal = false;
      this.showEditSupplierModal = false;
      this.supplierForm = {
        supplierID: '',
        companyName: '',
        contactName: '',
        contactPosition: '',
        address: '',
        city: '',
        area: '',
        postCode: '',
        country: '',
        tel: '',
        fax: '',
        homePage: ''
      };
    }

  },
  mounted() {
    this.checkAuth();
  }
});
</script>

<style>
/* 基础重置 */
* {
  margin: 0;
  padding: 0;
  box-sizing: border-box;
}

body {
  font-family: 'Arial', sans-serif;
  background-color: #f5f5f5;
}

/* 登录容器 */
.login-container {
  display: flex;
  justify-content: center;
  align-items: center;
  min-height: 100vh;
  background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
}

.login-form {
  background: white;
  padding: 2rem;
  border-radius: 8px;
  box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
  width: 100%;
  max-width: 400px;
}

.form-group {
  margin-bottom: 1rem;
}

.form-group label {
  display: block;
  margin-bottom: 0.5rem;
  font-weight: bold;
}

.form-group input {
  width: 100%;
  padding: 0.5rem;
  border: 1px solid #ddd;
  border-radius: 4px;
}

.form-actions {
  display: flex;
  gap: 1rem;
  margin-top: 1rem;
}

.login-btn,
.register-btn {
  flex: 1;
  padding: 0.75rem;
  border: none;
  border-radius: 4px;
  cursor: pointer;
}

.login-btn {
  background-color: #007bff;
  color: white;
}

.register-btn {
  background-color: #28a745;
  color: white;
}

/* 模态框 */
.modal-overlay {
  position: fixed;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  background: rgba(0, 0, 0, 0.5);
  display: flex;
  justify-content: center;
  align-items: center;
  z-index: 1000;
}

.modal {
  background: white;
  padding: 2rem;
  border-radius: 8px;
  width: 100%;
  max-width: 500px;
  max-height: 90vh;
  overflow-y: auto;
}

/* 主应用布局 */
.main-app {
  display: flex;
  flex-direction: column;
  min-height: 100vh;
}

.app-header {
  background: #343a40;
  color: white;
  padding: 1rem;
  display: flex;
  justify-content: space-between;
  align-items: center;
}

.app-container {
  display: flex;
  flex: 1;
}

.sidebar {
  width: 250px;
  background: #f8f9fa;
  padding: 1rem;
}

.sidebar ul {
  list-style: none;
}

.nav-btn {
  width: 100%;
  padding: 0.75rem;
  border: none;
  background: none;
  text-align: left;
  cursor: pointer;
  border-radius: 4px;
  margin-bottom: 0.5rem;
}

.nav-btn.active {
  background: #007bff;
  color: white;
}

.main-content {
  flex: 1;
  padding: 2rem;
}

.page {
  background: white;
  padding: 1.5rem;
  border-radius: 8px;
  box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
}

/* 产品网格 */
.products-grid {
  display: grid;
  grid-template-columns: repeat(auto-fill, minmax(250px, 1fr));
  gap: 1rem;
  margin-top: 1rem;
}

.product-card {
  border: 1px solid #ddd;
  border-radius: 8px;
  padding: 1rem;
  background: white;
}

.product-card.low-stock {
  border-color: #ffc107;
  background-color: #fff3cd;
}

/* 按钮样式 */
button {
  cursor: pointer;
  padding: 0.5rem 1rem;
  border-radius: 4px;
  border: 1px solid transparent;
}

.confirm-btn {
  background-color: #28a745;
  color: white;
}

.cancel-btn {
  background-color: #6c757d;
  color: white;
}

.delete-btn {
  background-color: #dc3545;
  color: white;
}

.view-btn {
  background-color: #17a2b8;
  color: white;
}

.fetch-btn {
  background-color: #007bff;
  color: white;
  margin-bottom: 1rem;
}

/* 表格样式 */
table {
  width: 100%;
  border-collapse: collapse;
  margin-top: 1rem;
}

th,
td {
  padding: 0.75rem;
  text-align: left;
  border-bottom: 1px solid #ddd;
}

th {
  background-color: #f8f9fa;
  font-weight: bold;
}

/* 错误消息 */
.error-message {
  color: #dc3545;
  margin-top: 1rem;
  padding: 0.75rem;
  background-color: #f8d7da;
  border-radius: 4px;
}

/* 其他已有样式保留 */
.order-btn {
  background: #27ae60;
  color: white;
  padding: 10px 20px;
  border: none;
  border-radius: 4px;
  cursor: pointer;
  width: 100%;
}

.order-btn:disabled {
  background: #ccc;
  cursor: not-allowed;
}

.product-order-item {
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 15px;
  background: #f8f9fa;
  border-radius: 6px;
  margin: 10px 0;
}

.quantity-controls {
  display: flex;
  align-items: center;
  gap: 10px;
}

.quantity-btn {
  width: 30px;
  height: 30px;
  border: 1px solid #ddd;
  background: white;
  border-radius: 4px;
  display: flex;
  align-items: center;
  justify-content: center;
  cursor: pointer;
}

.quantity-btn:disabled {
  opacity: 0.5;
  cursor: not-allowed;
}

.quantity-display {
  min-width: 30px;
  text-align: center;
  font-weight: bold;
}

.order-products {
  margin-bottom: 20px;
}

.management-actions {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 1rem;
  gap: 1rem;
}

.search-section {
  display: flex;
  gap: 1rem;
  align-items: center;
  flex: 1;
}

.add-btn {
  background-color: #28a745;
  color: white;
  padding: 0.5rem 1rem;
  border: none;
  border-radius: 4px;
  cursor: pointer;
}

.edit-btn {
  background-color: #ffc107;
  color: black;
  padding: 0.25rem 0.5rem;
  border: none;
  border-radius: 4px;
  cursor: pointer;
  margin-right: 0.5rem;
}

.delete-btn {
  background-color: #dc3545;
  color: white;
  padding: 0.25rem 0.5rem;
  border: none;
  border-radius: 4px;
  cursor: pointer;
}
</style>
