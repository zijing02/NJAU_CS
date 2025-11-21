import express from 'express'
import mysql from 'mysql2'
import cors from 'cors'
import bcrypt from 'bcryptjs'
import jwt from 'jsonwebtoken'

const app = express()
const port = 3001
const JWT_SECRET = 'your-secret-key'

// 中间件配置
app.use(
  cors({
    origin: ['http://localhost:5173', 'http://127.0.0.1:5173'],
    credentials: true,
    methods: ['GET', 'POST', 'PUT', 'DELETE'],
  }),
)

app.use(express.json())

// 数据库连接
const db = mysql.createConnection({
  host: 'localhost',
  port: 3306,
  user: 'root',
  password: '123456',
  database: 'cd',
})

db.connect((err) => {
  if (err) {
    console.error('数据库连接失败:', err)
    return
  }
  console.log('成功连接到 cd 数据库')
})

// JWT 验证中间件
const authenticateToken = (req, res, next) => {
  const authHeader = req.headers['authorization']
  const token = authHeader && authHeader.split(' ')[1]

  if (!token) {
    return res.status(401).json({ error: '访问令牌不存在' })
  }

  jwt.verify(token, JWT_SECRET, (err, user) => {
    if (err) {
      return res.status(403).json({ error: '令牌无效' })
    }
    req.user = user
    next()
  })
}

// 权限检查中间件
const requirePermission = (allowedTypes) => {
  return (req, res, next) => {
    if (!allowedTypes.includes(req.user.userType)) {
      return res.status(403).json({ error: '权限不足' })
    }
    next()
  }
}

// 用户注册
app.post('/api/register', async (req, res) => {
  const { username, password, userType = 'user' } = req.body

  if (!username || !password) {
    return res.status(400).json({ error: '用户名和密码不能为空' })
  }

  try {
    const hashedPassword = await bcrypt.hash(password, 10)
    const query = 'INSERT INTO users (username, password, userType) VALUES (?, ?, ?)'

    db.query(query, [username, hashedPassword, userType], (err, results) => {
      if (err) {
        if (err.code === 'ER_DUP_ENTRY') {
          return res.status(400).json({ error: '用户名已存在' })
        }
        console.error('注册错误:', err)
        return res.status(500).json({ error: '注册失败' })
      }

      res.json({ message: '注册成功' })
    })
  } catch (error) {
    console.error('注册错误:', error)
    res.status(500).json({ error: '注册失败' })
  }
})

// 用户登录
app.post('/api/login', (req, res) => {
  const { username, password } = req.body

  if (!username || !password) {
    return res.status(400).json({ error: '用户名和密码不能为空' })
  }

  const query = 'SELECT * FROM users WHERE username = ?'

  db.query(query, [username], async (err, results) => {
    if (err) {
      console.error('登录查询错误:', err)
      return res.status(500).json({ error: '登录失败' })
    }

    if (results.length === 0) {
      return res.status(401).json({ error: '用户名或密码错误' })
    }

    const user = results[0]

    try {
      const passwordMatch = await bcrypt.compare(password, user.password)

      if (!passwordMatch) {
        return res.status(401).json({ error: '用户名或密码错误' })
      }

      const token = jwt.sign(
        {
          userId: user.id,
          username: user.username,
          userType: user.userType,
        },
        JWT_SECRET,
        { expiresIn: '24h' },
      )

      res.json({
        message: '登录成功',
        token,
        user: {
          id: user.id,
          username: user.username,
          userType: user.userType,
        },
      })
    } catch (error) {
      console.error('密码验证错误:', error)
      res.status(500).json({ error: '登录失败' })
    }
  })
})

// 获取所有用户 (仅管理员)
app.get('/api/users', authenticateToken, requirePermission(['admin']), (req, res) => {
  const query = 'SELECT id, username, userType, createdAt FROM users'

  db.query(query, (err, results) => {
    if (err) {
      console.error('查询用户错误:', err)
      res.status(500).json({ error: '用户查询失败', details: err.message })
      return
    }
    res.json({
      message: '用户数据获取成功',
      data: results,
    })
  })
})

// 修改用户密码 (仅管理员)
app.put(
  '/api/users/:id/password',
  authenticateToken,
  requirePermission(['admin']),
  async (req, res) => {
    const { id } = req.params
    const { newPassword } = req.body

    if (!newPassword) {
      return res.status(400).json({ error: '新密码不能为空' })
    }

    try {
      const hashedPassword = await bcrypt.hash(newPassword, 10)
      const query = 'UPDATE users SET password = ? WHERE id = ?'

      db.query(query, [hashedPassword, id], (err, results) => {
        if (err) {
          console.error('修改密码错误:', err)
          res.status(500).json({ error: '密码修改失败', details: err.message })
          return
        }

        if (results.affectedRows === 0) {
          return res.status(404).json({ error: '用户不存在' })
        }

        res.json({
          message: '密码修改成功',
        })
      })
    } catch (error) {
      console.error('密码加密错误:', error)
      res.status(500).json({ error: '密码修改失败' })
    }
  },
)

// 删除用户 (仅管理员)
app.delete('/api/users/:id', authenticateToken, requirePermission(['admin']), (req, res) => {
  const { id } = req.params

  const query = 'DELETE FROM users WHERE id = ?'

  db.query(query, [id], (err, results) => {
    if (err) {
      console.error('删除用户错误:', err)
      res.status(500).json({ error: '用户删除失败', details: err.message })
      return
    }

    if (results.affectedRows === 0) {
      return res.status(404).json({ error: '用户不存在' })
    }

    res.json({
      message: '用户删除成功',
    })
  })
})

// 获取所有产品
app.get('/api/products', authenticateToken, (req, res) => {
  const query = `
    SELECT productID, productName, productNum, price, inventoryNum, orderNum, twiceOrderNum, terminal
    FROM product
    WHERE inventoryNum > 0
  `

  db.query(query, (err, results) => {
    if (err) {
      console.error('查询产品错误:', err)
      res.status(500).json({ error: '产品查询失败', details: err.message })
      return
    }
    res.json({
      message: '产品数据获取成功',
      data: results,
    })
  })
})

// 获取用户订单
app.get('/api/orders', authenticateToken, (req, res) => {
  const userId = req.user.userId
  let query, params

  if (req.user.userType === 'admin') {
    // 管理员可以查看所有订单
    query = `
      SELECT o.orderID, o.orderDate, o.supplyDate, o.arrivalDate, o.ownerName,
             o.ownerAddress, o.ownerCity, o.payWay, o.insurance,
             SUM(od.num * p.price) as totalAmount
      FROM \`order\` o
      LEFT JOIN orderdetail od ON o.orderID = od.orderID
      LEFT JOIN product p ON od.productID = p.productID
      GROUP BY o.orderID, o.orderDate, o.supplyDate, o.arrivalDate, o.ownerName,
               o.ownerAddress, o.ownerCity, o.payWay, o.insurance
      ORDER BY o.orderDate DESC
    `
    params = []
  } else {
    // 普通用户只能查看自己的订单
    query = `
      SELECT o.orderID, o.orderDate, o.supplyDate, o.arrivalDate, o.ownerName,
             o.ownerAddress, o.ownerCity, o.payWay, o.insurance,
             SUM(od.num * p.price) as totalAmount
      FROM \`order\` o
      LEFT JOIN orderdetail od ON o.orderID = od.orderID
      LEFT JOIN product p ON od.productID = p.productID
      WHERE o.ownerName = ?
      GROUP BY o.orderID, o.orderDate, o.supplyDate, o.arrivalDate, o.ownerName,
               o.ownerAddress, o.ownerCity, o.payWay, o.insurance
      ORDER BY o.orderDate DESC
    `
    params = [req.user.username]
  }

  db.query(query, params, (err, results) => {
    if (err) {
      console.error('查询订单错误:', err)
      res.status(500).json({ error: '订单查询失败', details: err.message })
      return
    }
    res.json({
      message: '订单数据获取成功',
      data: results,
    })
  })
})
// 获取订单详情
app.get('/api/orders/:orderId', authenticateToken, (req, res) => {
  const { orderId } = req.params
  const userId = req.user.userId

  // 先检查订单权限
  const checkQuery =
    req.user.userType === 'admin'
      ? 'SELECT * FROM `order` WHERE orderID = ?'
      : 'SELECT * FROM `order` WHERE orderID = ? AND ownerName = ?'

  const checkParams = req.user.userType === 'admin' ? [orderId] : [orderId, req.user.username]

  db.query(checkQuery, checkParams, (err, orderResults) => {
    if (err) {
      console.error('检查订单权限错误:', err)
      res.status(500).json({ error: '订单查询失败', details: err.message })
      return
    }

    if (orderResults.length === 0) {
      return res.status(404).json({ error: '订单不存在或无权访问' })
    }

    // 获取订单详情
    const detailQuery = `
      SELECT od.*, p.productName, p.price, (od.num * p.price) as subtotal
      FROM orderdetail od
      JOIN product p ON od.productID = p.productID
      WHERE od.orderID = ?
    `

    db.query(detailQuery, [orderId], (err, detailResults) => {
      if (err) {
        console.error('查询订单详情错误:', err)
        res.status(500).json({ error: '订单详情查询失败', details: err.message })
        return
      }

      res.json({
        message: '订单详情获取成功',
        data: {
          order: orderResults[0],
          details: detailResults,
        },
      })
    })
  })
})

// 创建订单 - 直接创建订单和订单明细
app.post('/api/orders', authenticateToken, (req, res) => {
  const { orderDetails, ownerName, ownerAddress, ownerCity, payWay, insurance } = req.body
  const userId = req.user.userId

  if (!orderDetails || !Array.isArray(orderDetails) || orderDetails.length === 0) {
    return res.status(400).json({ error: '订单明细不能为空' })
  }

  if (!ownerName || !ownerAddress || !ownerCity || !payWay) {
    return res.status(400).json({ error: '收货信息不完整' })
  }

  // 开始事务
  db.beginTransaction((err) => {
    if (err) {
      console.error('开始事务错误:', err)
      return res.status(500).json({ error: '创建订单失败' })
    }

    // 1. 检查库存
    const checkStockQueries = orderDetails.map((detail) => {
      return new Promise((resolve, reject) => {
        const query = 'SELECT productID, productName, inventoryNum FROM product WHERE productID = ?'
        db.query(query, [detail.productID], (err, results) => {
          if (err) {
            reject(err)
          } else if (results.length === 0) {
            reject(new Error(`产品ID ${detail.productID} 不存在`))
          } else if (results[0].inventoryNum < detail.quantity) {
            reject(
              new Error(
                `产品 "${results[0].productName}" 库存不足，当前库存: ${results[0].inventoryNum}`,
              ),
            )
          } else {
            resolve()
          }
        })
      })
    })

    Promise.all(checkStockQueries)
      .then(() => {
        // 2. 生成订单ID
        const getMaxOrderIdQuery = 'SELECT MAX(orderID) as maxId FROM `order`'
        db.query(getMaxOrderIdQuery, (err, results) => {
          if (err) {
            return db.rollback(() => {
              res.status(500).json({ error: '生成订单ID失败', details: err.message })
            })
          }

          const newOrderId = (results[0].maxId || 10250) + 1
          const currentDate = new Date().toISOString().split('T')[0]
          const supplyDate = new Date(Date.now() + 7 * 24 * 60 * 60 * 1000)
            .toISOString()
            .split('T')[0]
          const arrivalDate = new Date(Date.now() + 14 * 24 * 60 * 60 * 1000)
            .toISOString()
            .split('T')[0]

          // 3. 插入订单主表
          const insertOrderQuery = `
            INSERT INTO \`order\` (orderID, orderDate, supplyDate, arrivalDate, ownerName, ownerAddress, ownerCity, payWay, insurance)
            VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)
          `

          db.query(
            insertOrderQuery,
            [
              newOrderId,
              currentDate,
              supplyDate,
              arrivalDate,
              ownerName,
              ownerAddress,
              ownerCity,
              payWay,
              insurance || 0,
            ],
            (err) => {
              if (err) {
                return db.rollback(() => {
                  res.status(500).json({ error: '创建订单失败', details: err.message })
                })
              }

              // 4. 插入订单详情并更新库存
              const insertDetailsQueries = orderDetails.map((detail) => {
                return new Promise((resolve, reject) => {
                  // 插入订单详情
                  const insertDetailQuery =
                    'INSERT INTO orderdetail (orderID, productID, num) VALUES (?, ?, ?)'
                  db.query(
                    insertDetailQuery,
                    [newOrderId, detail.productID, detail.quantity],
                    (err) => {
                      if (err) {
                        reject(err)
                        return
                      }

                      // 更新产品库存
                      const updateStockQuery =
                        'UPDATE product SET inventoryNum = inventoryNum - ? WHERE productID = ?'
                      db.query(updateStockQuery, [detail.quantity, detail.productID], (err) => {
                        if (err) {
                          reject(err)
                        } else {
                          resolve()
                        }
                      })
                    },
                  )
                })
              })

              Promise.all(insertDetailsQueries)
                .then(() => {
                  // 提交事务
                  db.commit((err) => {
                    if (err) {
                      return db.rollback(() => {
                        res.status(500).json({ error: '提交订单失败', details: err.message })
                      })
                    }
                    res.json({
                      message: '订单创建成功',
                      orderId: newOrderId,
                    })
                  })
                })
                .catch((error) => {
                  db.rollback(() => {
                    res.status(500).json({ error: '创建订单详情失败', details: error.message })
                  })
                })
            },
          )
        })
      })
      .catch((error) => {
        db.rollback(() => {
          res.status(400).json({ error: error.message })
        })
      })
  })
})

// 删除订单
app.delete('/api/orders/:orderId', authenticateToken, (req, res) => {
  const { orderId } = req.params
  const userId = req.user.userId

  // 开始事务
  db.beginTransaction((err) => {
    if (err) {
      console.error('开始事务错误:', err)
      return res.status(500).json({ error: '删除订单失败' })
    }

    // 1. 检查订单权限并获取订单详情
    const checkQuery =
      req.user.userType === 'admin'
        ? 'SELECT * FROM `order` WHERE orderID = ?'
        : 'SELECT * FROM `order` WHERE orderID = ? AND ownerName = ?'

    const checkParams = req.user.userType === 'admin' ? [orderId] : [orderId, req.user.username]

    db.query(checkQuery, checkParams, (err, orderResults) => {
      if (err) {
        return db.rollback(() => {
          res.status(500).json({ error: '检查订单权限失败', details: err.message })
        })
      }

      if (orderResults.length === 0) {
        return db.rollback(() => {
          res.status(404).json({ error: '订单不存在或无权访问' })
        })
      }

      // 2. 获取订单详情以恢复库存
      const getDetailsQuery = 'SELECT * FROM orderdetail WHERE orderID = ?'
      db.query(getDetailsQuery, [orderId], (err, detailResults) => {
        if (err) {
          return db.rollback(() => {
            res.status(500).json({ error: '获取订单详情失败', details: err.message })
          })
        }

        // 3. 恢复库存
        const restoreStockQueries = detailResults.map((detail) => {
          return new Promise((resolve, reject) => {
            const updateStockQuery =
              'UPDATE product SET inventoryNum = inventoryNum + ? WHERE productID = ?'
            db.query(updateStockQuery, [detail.num, detail.productID], (err) => {
              if (err) {
                reject(err)
              } else {
                resolve()
              }
            })
          })
        })

        Promise.all(restoreStockQueries)
          .then(() => {
            // 4. 删除订单详情
            const deleteDetailsQuery = 'DELETE FROM orderdetail WHERE orderID = ?'
            db.query(deleteDetailsQuery, [orderId], (err) => {
              if (err) {
                return db.rollback(() => {
                  res.status(500).json({ error: '删除订单详情失败', details: err.message })
                })
              }

              // 5. 删除订单
              const deleteOrderQuery = 'DELETE FROM `order` WHERE orderID = ?'
              db.query(deleteOrderQuery, [orderId], (err) => {
                if (err) {
                  return db.rollback(() => {
                    res.status(500).json({ error: '删除订单失败', details: err.message })
                  })
                }

                // 提交事务
                db.commit((err) => {
                  if (err) {
                    return db.rollback(() => {
                      res.status(500).json({ error: '提交删除操作失败', details: err.message })
                    })
                  }
                  res.json({
                    message: '订单删除成功',
                  })
                })
              })
            })
          })
          .catch((error) => {
            db.rollback(() => {
              res.status(500).json({ error: '恢复库存失败', details: error.message })
            })
          })
      })
    })
  })
})

// 测试连接
app.get('/api/test', authenticateToken, (req, res) => {
  res.json({
    message: '后端连接成功!',
    database: 'cd',
    user: req.user,
  })
})

// 分类管理路由
app.get('/api/categories', authenticateToken, (req, res) => {
  const query = 'SELECT categoryName, instruction FROM category'

  db.query(query, (err, results) => {
    if (err) {
      console.error('查询分类错误:', err)
      res.status(500).json({ error: '分类查询失败', details: err.message })
      return
    }
    res.json({
      message: '分类数据获取成功',
      data: results,
    })
  })
})

// 客户管理 API
// 获取所有客户
app.get('/api/customers', authenticateToken, requirePermission(['admin']), (req, res) => {
  const query = 'SELECT * FROM customer'
  db.query(query, (err, results) => {
    if (err) {
      console.error('查询客户错误:', err)
      res.status(500).json({ error: '客户查询失败', details: err.message })
      return
    }
    res.json({ message: '客户数据获取成功', data: results })
  })
})

// 添加客户
app.post('/api/customers', authenticateToken, requirePermission(['admin']), (req, res) => {
  const {
    customerCode,
    companyName,
    contactName,
    contactPosition,
    address,
    city,
    area,
    postCode,
    country,
    tel,
    fax,
  } = req.body

  if (!customerCode || !companyName) {
    return res.status(400).json({ error: '客户代码和公司名称不能为空' })
  }

  const query = `INSERT INTO customer (customerCode, companyName, contactName, contactPosition, address, city, area, postCode, country, tel, fax)
                 VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)`

  db.query(
    query,
    [
      customerCode,
      companyName,
      contactName,
      contactPosition,
      address,
      city,
      area,
      postCode,
      country,
      tel,
      fax,
    ],
    (err, results) => {
      if (err) {
        if (err.code === 'ER_DUP_ENTRY') {
          return res.status(400).json({ error: '客户代码已存在' })
        }
        console.error('添加客户错误:', err)
        res.status(500).json({ error: '添加客户失败', details: err.message })
        return
      }
      res.json({ message: '客户添加成功' })
    },
  )
})

// 更新客户
app.put(
  '/api/customers/:customerCode',
  authenticateToken,
  requirePermission(['admin']),
  (req, res) => {
    const { customerCode } = req.params
    const {
      companyName,
      contactName,
      contactPosition,
      address,
      city,
      area,
      postCode,
      country,
      tel,
      fax,
    } = req.body

    if (!companyName) {
      return res.status(400).json({ error: '公司名称不能为空' })
    }

    const query = `UPDATE customer SET companyName=?, contactName=?, contactPosition=?, address=?, city=?, area=?, postCode=?, country=?, tel=?, fax=?
                 WHERE customerCode=?`

    db.query(
      query,
      [
        companyName,
        contactName,
        contactPosition,
        address,
        city,
        area,
        postCode,
        country,
        tel,
        fax,
        customerCode,
      ],
      (err, results) => {
        if (err) {
          console.error('更新客户错误:', err)
          res.status(500).json({ error: '更新客户失败', details: err.message })
          return
        }

        if (results.affectedRows === 0) {
          return res.status(404).json({ error: '客户不存在' })
        }

        res.json({ message: '客户更新成功' })
      },
    )
  },
)

// 删除客户
app.delete(
  '/api/customers/:customerCode',
  authenticateToken,
  requirePermission(['admin']),
  (req, res) => {
    const { customerCode } = req.params

    const query = 'DELETE FROM customer WHERE customerCode = ?'

    db.query(query, [customerCode], (err, results) => {
      if (err) {
        console.error('删除客户错误:', err)
        res.status(500).json({ error: '删除客户失败', details: err.message })
        return
      }

      if (results.affectedRows === 0) {
        return res.status(404).json({ error: '客户不存在' })
      }

      res.json({ message: '客户删除成功' })
    })
  },
)

// 类似的为 shippers 和 suppliers 添加完整的 CRUD API
// 运货商管理 API
app.get('/api/shippers', authenticateToken, requirePermission(['admin']), (req, res) => {
  const query = 'SELECT * FROM shipper'
  db.query(query, (err, results) => {
    if (err) {
      console.error('查询运货商错误:', err)
      res.status(500).json({ error: '运货商查询失败', details: err.message })
      return
    }
    res.json({ message: '运货商数据获取成功', data: results })
  })
})

app.post('/api/shippers', authenticateToken, requirePermission(['admin']), (req, res) => {
  const { shipperID, companyName, tel, transportTool } = req.body

  if (!shipperID || !companyName) {
    return res.status(400).json({ error: '运货商ID和公司名称不能为空' })
  }

  const query =
    'INSERT INTO shipper (shipperID, companyName, tel, transportTool) VALUES (?, ?, ?, ?)'
  db.query(query, [shipperID, companyName, tel, transportTool], (err, results) => {
    if (err) {
      if (err.code === 'ER_DUP_ENTRY') {
        return res.status(400).json({ error: '运货商ID已存在' })
      }
      console.error('添加运货商错误:', err)
      res.status(500).json({ error: '添加运货商失败', details: err.message })
      return
    }
    res.json({ message: '运货商添加成功' })
  })
})

app.put('/api/shippers/:shipperID', authenticateToken, requirePermission(['admin']), (req, res) => {
  const { shipperID } = req.params
  const { companyName, tel, transportTool } = req.body

  const query = 'UPDATE shipper SET companyName=?, tel=?, transportTool=? WHERE shipperID=?'
  db.query(query, [companyName, tel, transportTool, shipperID], (err, results) => {
    if (err) {
      console.error('更新运货商错误:', err)
      res.status(500).json({ error: '更新运货商失败', details: err.message })
      return
    }

    if (results.affectedRows === 0) {
      return res.status(404).json({ error: '运货商不存在' })
    }

    res.json({ message: '运货商更新成功' })
  })
})

app.delete(
  '/api/shippers/:shipperID',
  authenticateToken,
  requirePermission(['admin']),
  (req, res) => {
    const { shipperID } = req.params

    const query = 'DELETE FROM shipper WHERE shipperID = ?'
    db.query(query, [shipperID], (err, results) => {
      if (err) {
        console.error('删除运货商错误:', err)
        res.status(500).json({ error: '删除运货商失败', details: err.message })
        return
      }

      if (results.affectedRows === 0) {
        return res.status(404).json({ error: '运货商不存在' })
      }

      res.json({ message: '运货商删除成功' })
    })
  },
)

// 供应商管理 API (类似上面，包含所有字段)
app.get('/api/suppliers', authenticateToken, requirePermission(['admin']), (req, res) => {
  const query = 'SELECT * FROM supplier'
  db.query(query, (err, results) => {
    if (err) {
      console.error('查询供应商错误:', err)
      res.status(500).json({ error: '供应商查询失败', details: err.message })
      return
    }
    res.json({ message: '供应商数据获取成功', data: results })
  })
})

app.post('/api/suppliers', authenticateToken, requirePermission(['admin']), (req, res) => {
  const {
    supplierID,
    companyName,
    contactName,
    contactPosition,
    address,
    city,
    area,
    postCode,
    country,
    tel,
    fax,
    homePage,
  } = req.body

  if (!supplierID || !companyName) {
    return res.status(400).json({ error: '供应商ID和公司名称不能为空' })
  }

  const query = `INSERT INTO supplier (supplierID, companyName, contactName, contactPosition, address, city, area, postCode, country, tel, fax, homePage)
                 VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)`

  db.query(
    query,
    [
      supplierID,
      companyName,
      contactName,
      contactPosition,
      address,
      city,
      area,
      postCode,
      country,
      tel,
      fax,
      homePage,
    ],
    (err, results) => {
      if (err) {
        if (err.code === 'ER_DUP_ENTRY') {
          return res.status(400).json({ error: '供应商ID已存在' })
        }
        console.error('添加供应商错误:', err)
        res.status(500).json({ error: '添加供应商失败', details: err.message })
        return
      }
      res.json({ message: '供应商添加成功' })
    },
  )
})

app.put(
  '/api/suppliers/:supplierID',
  authenticateToken,
  requirePermission(['admin']),
  (req, res) => {
    const { supplierID } = req.params
    const {
      companyName,
      contactName,
      contactPosition,
      address,
      city,
      area,
      postCode,
      country,
      tel,
      fax,
      homePage,
    } = req.body

    const query = `UPDATE supplier SET companyName=?, contactName=?, contactPosition=?, address=?, city=?, area=?, postCode=?, country=?, tel=?, fax=?, homePage=?
                 WHERE supplierID=?`

    db.query(
      query,
      [
        companyName,
        contactName,
        contactPosition,
        address,
        city,
        area,
        postCode,
        country,
        tel,
        fax,
        homePage,
        supplierID,
      ],
      (err, results) => {
        if (err) {
          console.error('更新供应商错误:', err)
          res.status(500).json({ error: '更新供应商失败', details: err.message })
          return
        }

        if (results.affectedRows === 0) {
          return res.status(404).json({ error: '供应商不存在' })
        }

        res.json({ message: '供应商更新成功' })
      },
    )
  },
)

app.delete(
  '/api/suppliers/:supplierID',
  authenticateToken,
  requirePermission(['admin']),
  (req, res) => {
    const { supplierID } = req.params

    const query = 'DELETE FROM supplier WHERE supplierID = ?'
    db.query(query, [supplierID], (err, results) => {
      if (err) {
        console.error('删除供应商错误:', err)
        res.status(500).json({ error: '删除供应商失败', details: err.message })
        return
      }

      if (results.affectedRows === 0) {
        return res.status(404).json({ error: '供应商不存在' })
      }

      res.json({ message: '供应商删除成功' })
    })
  },
)

// 获取当前用户信息
app.get('/api/user/profile', authenticateToken, (req, res) => {
  res.json({
    user: req.user,
  })
})

// 启动服务器
app.listen(port, () => {
  console.log(`后端服务器运行在 http://localhost:${port}`)
  console.log('数据库: cd')
})
