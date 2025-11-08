const express = require('express');
const mysql = require('mysql2');
const cors = require('cors');

const app = express();
const port = 3001;

// 端口配置
app.use(cors({
  origin: ['http://localhost:5173', 'http://127.0.0.1:5173'],
  credentials: true,
  methods: ['GET', 'POST', 'PUT', 'DELETE']
}));

app.use(express.json());

// 数据库连接 - 连接到 cd 数据库
const db = mysql.createConnection({
  host: 'localhost',
  port: 3306,
  user: 'root',
  password: '123456',
  database: 'cd'
});

db.connect((err) => {
  if (err) {
    console.error('数据库连接失败:', err);
    return;
  }
  console.log('成功连接到 cd 数据库');
});

// 测试连接
app.get('/api/test', (req, res) => {
  res.json({
    message: '后端连接成功!',
    database: 'cd',
    tables: ['category', 'customer']
  });
});


// 获取所有分类数据
app.get('/api/categories', (req, res) => {
  const query = 'SELECT categoryName, instruction FROM category';

  db.query(query, (err, results) => {
    if (err) {
      console.error('查询分类错误:', err);
      res.status(500).json({ error: '分类查询失败', details: err.message });
      return;
    }
    res.json({
      message: '分类数据获取成功',
      data: results
    });
  });
});

// 添加新分类
app.post('/api/categories', (req, res) => {
  const { categoryName, instruction } = req.body;

  if (!categoryName) {
    return res.status(400).json({ error: '分类名称不能为空' });
  }

  const query = 'INSERT INTO category (categoryName, instruction) VALUES (?, ?)';

  db.query(query, [categoryName, instruction || ''], (err, results) => {
    if (err) {
      console.error('插入分类错误:', err);
      res.status(500).json({ error: '分类插入失败', details: err.message });
      return;
    }
    res.json({
      message: '分类添加成功'
    });
  });
});

// 更新分类
app.put('/api/categories/:categoryName', (req, res) => {
  const { categoryName } = req.params;
  const { instruction, newCategoryName } = req.body;

  if (!newCategoryName && !instruction) {
    return res.status(400).json({ error: '至少提供一个更新字段' });
  }

  let query, params;

  if (newCategoryName) {
    query = 'UPDATE category SET categoryName = ?, instruction = ? WHERE categoryName = ?';
    params = [newCategoryName, instruction || '', categoryName];
  } else {
    query = 'UPDATE category SET instruction = ? WHERE categoryName = ?';
    params = [instruction, categoryName];
  }

  db.query(query, params, (err, results) => {
    if (err) {
      console.error('更新分类错误:', err);
      res.status(500).json({ error: '分类更新失败', details: err.message });
      return;
    }

    if (results.affectedRows === 0) {
      return res.status(404).json({ error: '分类不存在' });
    }

    res.json({
      message: '分类更新成功'
    });
  });
});

// 删除分类
app.delete('/api/categories/:categoryName', (req, res) => {
  const { categoryName } = req.params;

  const query = 'DELETE FROM category WHERE categoryName = ?';

  db.query(query, [categoryName], (err, results) => {
    if (err) {
      console.error('删除分类错误:', err);
      res.status(500).json({ error: '分类删除失败', details: err.message });
      return;
    }

    if (results.affectedRows === 0) {
      return res.status(404).json({ error: '分类不存在' });
    }

    res.json({
      message: '分类删除成功'
    });
  });
});

// 获取所有客户数据
app.get('/api/customers', (req, res) => {
  const query = 'SELECT customerCode, companyName FROM customer';

  db.query(query, (err, results) => {
    if (err) {
      console.error('查询客户错误:', err);
      res.status(500).json({ error: '客户查询失败', details: err.message });
      return;
    }
    res.json({
      message: '客户数据获取成功',
      data: results
    });
  });
});

// 添加新客户
app.post('/api/customers', (req, res) => {
  const { customerCode, companyName } = req.body;

  if (!customerCode || !companyName) {
    return res.status(400).json({ error: '客户代码和公司名称不能为空' });
  }

  const query = 'INSERT INTO customer (customerCode, companyName) VALUES (?, ?)';

  db.query(query, [customerCode, companyName], (err, results) => {
    if (err) {
      console.error('插入客户错误:', err);
      res.status(500).json({ error: '客户插入失败', details: err.message });
      return;
    }
    res.json({
      message: '客户添加成功'
    });
  });
});

// 更新客户
app.put('/api/customers/:customerCode', (req, res) => {
  const { customerCode } = req.params;
  const { companyName, newCustomerCode } = req.body;

  if (!newCustomerCode && !companyName) {
    return res.status(400).json({ error: '至少提供一个更新字段' });
  }

  let query, params;

  if (newCustomerCode) {
    query = 'UPDATE customer SET customerCode = ?, companyName = ? WHERE customerCode = ?';
    params = [newCustomerCode, companyName || '', customerCode];
  } else {
    query = 'UPDATE customer SET companyName = ? WHERE customerCode = ?';
    params = [companyName, customerCode];
  }

  db.query(query, params, (err, results) => {
    if (err) {
      console.error('更新客户错误:', err);
      res.status(500).json({ error: '客户更新失败', details: err.message });
      return;
    }

    if (results.affectedRows === 0) {
      return res.status(404).json({ error: '客户不存在' });
    }

    res.json({
      message: '客户更新成功'
    });
  });
});

// 删除客户
app.delete('/api/customers/:customerCode', (req, res) => {
  const { customerCode } = req.params;

  const query = 'DELETE FROM customer WHERE customerCode = ?';

  db.query(query, [customerCode], (err, results) => {
    if (err) {
      console.error('删除客户错误:', err);
      res.status(500).json({ error: '客户删除失败', details: err.message });
      return;
    }

    if (results.affectedRows === 0) {
      return res.status(404).json({ error: '客户不存在' });
    }

    res.json({
      message: '客户删除成功'
    });
  });
});

// 启动服务器
app.listen(port, () => {
  console.log(`后端服务器运行在 http://localhost:${port}`);
  console.log('数据库: cd');
  console.log('可用表: category, customer');
});
