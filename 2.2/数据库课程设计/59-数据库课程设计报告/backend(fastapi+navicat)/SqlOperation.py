import json
import models 
import base64
import pymssql
import datetime 
from decimal import Decimal  

connect = pymssql.connect(
    server = 'ZIJING-PC',
    user = 'sa',
    password = '19222126',
    database = 'test',
    as_dict = True
)

cursor = connect.cursor()

def Convert(o):  
    if isinstance(o, datetime.datetime):  
        return o.__str__() 
    elif isinstance(o, Decimal):  
        return str(o)  # 或者 float(o)
    elif isinstance(o, bytes):  
        return base64.b64encode(o).decode('utf-8')  
    # raise TypeError(f'Object of type {o.__class__.__name__} is not JSON serializable')  

def GetInfo(info:models.tableInfo):
    if(info.tablename == 'category'):
        cursor.execute('select * from category where ID = %d',info.id)
        for row in cursor:
            return json.dumps((row,),default = Convert)
    elif(info.tablename == 'customers'):
        if(info.id1==0):
            cursor.execute('select * from customers where cID = %s',(info.id))
        else:
            cursor.execute('select * from customers where cID = %s and password = %d',(info.id,info.id1))    
        for row in cursor:
            return json.dumps((row,),default = Convert)
    elif(info.tablename == 'orderdetail'):
        cursor.execute('select * from orderdetail where orderID = %d and productID = %d',(info.id,info.id1))    
        for row in cursor:
            return json.dumps((row,),default = Convert)
    elif(info.tablename == 'orders'):
        cursor.execute('select * from orders where ID = %d',info.id)    
        for row in cursor:
            return json.dumps((row,),default = Convert)
    elif(info.tablename == 'pici'):
        cursor.execute('select * from pici where ID = %d',info.id)    
        for row in cursor:
            return json.dumps((row,),default = Convert)
    elif(info.tablename == 'products'):
        cursor.execute('select * from products where ID = %d',info.id)    
        for row in cursor:
            return json.dumps((row,),default = Convert)
    elif(info.tablename == 'proinfo'):
        cursor.execute('select * from proinfo where piciID = %d and productID= %d',(info.id,info.id1))    
        for row in cursor:
            return json.dumps((row,),default = Convert)
    elif(info.tablename == 'rules'):
        cursor.execute('select * from rules where ruleID = %d',info.id)    
        for row in cursor:
            return json.dumps((row,))
    elif(info.tablename == 'shippers'):
        cursor.execute('select * from shippers where ID = %d',info.id)    
        for row in cursor:
            return json.dumps((row,),default = Convert)
    elif(info.tablename == 'suppliers'):
        cursor.execute('select * from suppliers where ID = %d',info.id)    
        for row in cursor:
            return json.dumps((row,),default = Convert)

def RegisterCustomer(info:models.register_customer):
    cursor.execute('insert into customers values(%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%d)',
                   (info.id,'s','s','s','s','s','s','s','s','s','s',info.password))
    connect.commit()
    return {"result": 'success'}

def UpdateCustomer(info:models.customers1):
    cursor.execute('''UPDATE  customers  SET cname = %s, pname = %s, pjob = %s, caddress = %s,city = %s, 
                   area = %s,postcode = %s, country = %s,phone = %s, fax = %s
                   WHERE cid = %s;
                   ''',(info.cname,info.pname,info.pjob,info.caddress,info.city,info.area,info.postcode,info.country,info.phone,info.fax,info.cID))
    connect.commit()

def CreateOrder(info:models.orders):
    cursor.execute('select max(id) as max_id from orders')
    max_id = cursor.fetchone()['max_id']
    cursor.execute('insert into orders values(%d,%s,%d,%s,%s,%s,%s,%d,%s,%s,%s,%s,%s,%s,%s,%d)',
                   (max_id + 1,info.customerID,info.employeeID,info.orderdate,info.starttime,info.arrivaltime,'',0,
                    info.name,info.addr,info.city,info.area,info.postcode,info.country,info.paymethod,info.insurance))
    connect.commit()

def CreateOrderDetail(info:models.orderdetail):
    cursor.execute('select max(id) as max_id from orders')
    max_id = cursor.fetchone()['max_id']
    cursor.execute('insert into orderdetail values(%d,%d,%d,%s)',max_id,info.productid,info.num,' ')
    connect.commit()

def CreateOrderAndDetail(info:models.orders,info1:models.orderdetail):
    cursor.execute('select max(id) as max_id from orders')
    max_id = cursor.fetchone()['max_id']            #找到当前最大的订单号码

    cursor.execute('insert into orders values(%d,%s,%d,%s,%s,%s,%s,%d,%s,%s,%s,%s,%s,%s,%s,%d)',
                   (max_id + 1,info.customerID,info.employeeID,info.orderdate,info.starttime,info.arrivaltime,info.orderdate,0,
                    info.name,info.addr,info.city,info.area,info.postcode,info.country,info.paymethod,info.insurance))
    connect.commit()

    cursor.execute('insert into orderdetail values(%d,%d,%d,%s)',(max_id + 1,info1.productid,info1.num,' '))
    connect.commit()

    cursor.execute('update products set inventory = inventory - %d where id = %d',(info1.num,info1.productid))
    cursor.execute('''
        UPDATE orders  
        SET orders.delivercost = ( 
            SELECT tq.total_quantity * rules.criterion    
            FROM (    
                SELECT od.orderID AS orders_ID, SUM(od.num) as total_quantity    
                FROM orderdetail od      
                GROUP BY od.orderID    
            ) tq    
            JOIN rules ON tq.total_quantity BETWEEN rules.weight AND rules.weight1    
            WHERE tq.orders_ID = orders.ID   
            --LIMIT 1 -- 假设每个订单只匹配一个criterion,但这通常不是一个好的做法  
        )   WHERE EXISTS (    
            SELECT 1    
            FROM orderdetail od    
            WHERE od.orderID = orders.ID  and orders.id = %d 
            );
        ''',max_id + 1)
    connect.commit()
    
def GetProductsOfCategory(info:int):
    cursor.execute('''
        SELECT products.ID AS product_id,products.cname AS product_name,suppliers.sname As supplier_name, products.inventory,products.Price,products.num, category.cname
            FROM  products   
            INNER JOIN  category  ON products.cID = category.ID
            JOIN suppliers ON products.sid = suppliers.ID
            WHERE category.id = %d and products.supplystate = 0;
    ''',info)
    rows=()
    for row in cursor:
        rows+=(row,)
    return json.dumps(rows,default = Convert)

def GetAllProductsOfCategory():
    cursor.execute('''
        SELECT products.ID AS product_id,products.cname AS product_name,suppliers.sname As supplier_name, products.inventory,products.Price,products.num
            FROM  products   
            INNER JOIN  category  ON products.cID = category.ID
            JOIN suppliers ON products.sid = suppliers.ID
            WHERE  products.supplystate = 0
			order by category.ID ASC
    ''')
    rows=()
    for row in cursor:
        rows+=(row,)
    return json.dumps(rows,default = Convert)

def GetPicturesofProducts(info:int):
    cursor.execute('''
        SELECT products.id AS product_id,products.cname AS products_name,category.cname as category_name, products.picture_base64 AS base64_img
            FROM  products   
            INNER JOIN  category  ON products.cID = category.ID  
            WHERE category.id = %d and products.supplystate = 0;
    ''',info)
    rows=()
    for row in cursor:
        # b = bytes(row['picture'], 'utf-8').decode('unicode_escape').encode('latin1')
        rows+=(row,)
    return json.dumps(rows,default = Convert)

def GetOrderDetails(info:int):
    cursor.execute('''
        SELECT  od.orderID, p.ID AS productID,  p.cname AS productName,  p.num,  od.num AS quantity,  p.price,  (p.price * od.num) AS subtotal,  o.orderdate, o.oname AS OwnerName,  o.addr AS address,  
            (SELECT SUM(op.price * odp.num) + o.delivercost 
            FROM products op  
            JOIN orderdetail odp ON op.ID = odp.productID  
            WHERE odp.orderID = od.orderID) AS totalOrderCost 
        FROM  products p  
        JOIN  orderdetail od ON p.ID = od.productID  
        JOIN  orders o ON o.ID = od.orderID  
        WHERE od.orderID = %d; 
        ''',info)
    rows=()
    for row in cursor:
        rows+=(row,)
    return json.dumps(rows,default = Convert)

def FuzzyQuery(info:int):
    target = info+'%'
    cursor.execute('select * from orders where ID like %s',target)
    rows=()
    for row in cursor:
        rows+=(row,)
    return json.dumps(rows,default = Convert)

def CountOrderNumOfAreaAndCustomers1():
    cursor.execute('''
         SELECT orders.area, customers.cname, SUM(orderdetail.num) AS total_num  
            FROM orders  
            JOIN orderdetail ON orders.ID = orderdetail.orderID
			JOIN customers ON orders.cid=customers.cID
			GROUP BY customers.cname,orders.area
        ''')
    rows=()
    for row in cursor:
        rows+=(row,)
    return json.dumps(rows,default = Convert)

def CountOrderNumOfAreaAndCustomers():
    cursor.execute('''
         SELECT orders.area, SUM(orderdetail.num) AS total_num  
            FROM orders  
            JOIN orderdetail ON orders.ID = orderdetail.orderID
			JOIN customers ON orders.cid=customers.cID
			GROUP BY orders.area
        ''')
    rows=()
    for row in cursor:
        rows+=(row,)
    return json.dumps(rows,default = Convert)

def CountOrderNumOfArea():
    cursor.execute('''
         SELECT suppliers.area, SUM(orderdetail.num) AS total_num 
	        FROM suppliers   
	        JOIN products ON suppliers.ID = products.sid  
	        JOIN orderdetail ON products.ID = orderdetail.productID  
	        GROUP BY suppliers.area;
        ''')
    rows=()
    for row in cursor:
        rows+=(row,)
    return json.dumps(rows,default = Convert)

def CountOrderNumOfSeason():
    cursor.execute('''
        SELECT  
            CASE  
            WHEN DATEPART(MONTH, orders.orderdate) IN (12, 1, 2) THEN 'winter'  
            WHEN DATEPART(MONTH, orders.orderdate) IN (3, 4, 5) THEN 'spring'  
            WHEN DATEPART(MONTH, orders.orderdate) IN (6, 7, 8) THEN 'summer'  
            WHEN DATEPART(MONTH, orders.orderdate) IN (9, 10, 11) THEN 'autumn'  
            ELSE '未知季节'  
        END AS season,  
        SUM(orderdetail.num) AS total_quantity  
        FROM  orders   
        JOIN  orderdetail  ON orders.id = orderdetail.orderid  
        GROUP BY  
            CASE  
            WHEN DATEPART(MONTH, orders.orderdate) IN (12, 1, 2) THEN 'winter'  
            WHEN DATEPART(MONTH, orders.orderdate) IN (3, 4, 5) THEN 'spring'  
            WHEN DATEPART(MONTH, orders.orderdate) IN (6, 7, 8) THEN 'summer'  
            WHEN DATEPART(MONTH, orders.orderdate) IN (9, 10, 11) THEN 'autumn'  
            ELSE '未知季节'  
        END;
        ''')
    rows=()
    for row in cursor:
        rows+=(row,)
    print(rows)
    return json.dumps(rows,default = Convert)

def PriceOfProductsOfSuppliers():
    cursor.execute('''
        SELECT suppliers.ID AS supplier_id,  suppliers.sname,  products.ID AS product_id, products.cname, products.price AS product_price  
        FROM suppliers   
        JOIN products  ON suppliers.ID= products.sid  
        ORDER BY  suppliers.ID ASC, products.ID ASC;
    ''') 
    rows=()
    for row in cursor:
        rows+=(row,)
    return json.dumps(rows,default = Convert)

def TotalMoneyOfSuppliers():
    cursor.execute('''
        SELECT suppliers.ID, suppliers.sname, SUM(products.price * orderdetail.num) AS total_amount_per_supplier  
        FROM suppliers   
        JOIN  products  ON suppliers.ID = products.sid  
        JOIN  orderdetail ON products.ID = orderdetail.productID
        GROUP BY  suppliers.ID,suppliers.sname
        order by suppliers.ID ASC
    ''')
    rows=()
    for row in cursor:
        rows+=(row,)
    return json.dumps(rows,default = Convert)

def CountOrdersOfShippers():
    cursor.execute('''
        SELECT  shippers.ID,shippers.sname,COUNT(orders.id) AS total_orders 
        FROM shippers   
        LEFT JOIN  orders  ON shippers.id = orders.employeeid  
        GROUP BY  shippers.ID, shippers.sname  
        ORDER BY  total_orders DESC;
    ''')
    rows=()
    for row in cursor:
        rows+=(row,)
    return json.dumps(rows,default = Convert)

def CountOrdersOfSuppliers(info:int):
    cursor.execute('''
        SELECT category.ID, category.cname, suppliers.ID, suppliers.sname, SUM(orderdetail.num) AS total_quantity
        FROM  category   
        JOIN  products  ON category.ID = products.cID  
        JOIN  orderdetail  ON products.id = orderdetail.productid  
        JOIN  orders  ON orderdetail.orderid = orders.id 
        JOIN  suppliers  ON products.sid = suppliers.id  
            WHERE category.ID = %d
        GROUP BY category.ID, category.cname, suppliers.id, suppliers.sname 
        ORDER BY category.ID, suppliers.id;
    ''',info)
    rows=()
    for row in cursor:
        rows+=(row,)
    return json.dumps(rows,default = Convert)



# cursor.close()    
# connect.close()     



