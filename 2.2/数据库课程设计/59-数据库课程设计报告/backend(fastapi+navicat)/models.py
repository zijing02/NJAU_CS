import datetime 
from typing import Optional
from pydantic import BaseModel

# Optional[Optional[str] = None] = None

class User(BaseModel):    
    userName: str    
    Id: int

class tableInfo(BaseModel):
    tablename: Optional[str] = None
    id: Optional[int|str] = None
    id1: Optional[int|str] = None

class category(BaseModel):
    id: Optional[str] = None
    cname: Optional[str] = None
    explain: Optional[str] = None
    setup: Optional[datetime.date] = None
    update: Optional[datetime.date] = None 
    picture: Optional[int] = None

class customers(BaseModel):
    cID: Optional[str] = None
    cname: Optional[str] = None
    pname: Optional[str] = None
    pjob: Optional[str] = None
    caddress: Optional[str] = None
    city: Optional[str] = None
    area: Optional[str] = None
    postcode: Optional[str] = None
    country: Optional[str] = None
    phone: Optional[str] = None
    fax: Optional[str] = None
    id: Optional[int] = None
    password: Optional[int] = None

class customers1(BaseModel):
    cID: str
    cname: Optional[str] = None
    pname: Optional[str] = None
    pjob: Optional[str] = None
    caddress: Optional[str] = None
    city: Optional[str] = None
    area: Optional[str] = None
    postcode: Optional[str] = None
    country: Optional[str] = None
    phone: Optional[str] = None
    fax: Optional[str] = None

class register_customer(BaseModel):
    id: str
    password: int

class orderdetail(BaseModel):
    # orderid: Optional[int] = None 
    productid: Optional[int] = None
    num: Optional[int] = None
    # remark: Optional[str] = None

class orders(BaseModel):
    # ID: Optional[int] = None
    customerID: Optional[str] = None
    employeeID: Optional[int] = None
    orderdate: Optional[datetime.date] = None
    starttime: Optional[datetime.date] = None
    arrivaltime: Optional[datetime.date] = None
    # confirmtime: Optional[datetime.date] = None
    # deliver_cost: Optional[int] = None
    name: Optional[str] = None
    addr: Optional[str] = None
    city: Optional[str] = None
    area: Optional[str] = None
    postcode: Optional[str] = None
    country: Optional[str] = None
    paymethod: Optional[str] = None
    insurance: Optional[int] = None

class pici(BaseModel):
    ID: Optional[int] = None

class products(BaseModel):
    ID: Optional[int] = None
    name: Optional[str] = None
    num: Optional[str] = None
    price: Optional[str] = None
    inventory: Optional[int] = None
    ordernum: Optional[int] = None
    reordernum: Optional[int] = None
    supplystate: bool

class proinfo(BaseModel):
    piciID: Optional[int] = None
    productID: Optional[int] = None 
    prodate: Optional[datetime.date] = None
    expirationdate: Optional[datetime.date] = None

class rule(BaseModel):
    id: Optional[int] = None
    weight: Optional[int] = None
    cost: Optional[str] = None
    cri: Optional[str] = None

class shippers(BaseModel):
    sID: Optional[int] = None
    sname: Optional[str] = None
    phone: Optional[str] = None
    tool: Optional[str] = None

class suppliers(BaseModel):
    ID: Optional[int] = None
    name: Optional[str] = None
    pname: Optional[str] = None
    pjob: Optional[str] = None
    address: Optional[str] = None
    city: Optional[str] = None
    area: Optional[str] = None
    postcode: Optional[str] = None
    country: Optional[str] = None
    phone: Optional[str] = None
    fax: Optional[str] = None
    homepage: Optional[str] = None

class insertInfo(BaseModel):
    tableName: Optional[str] = None
    categoryInput: category   #嵌套定义类，将外面的类引用到类内部，在内部也可以嵌套，但其他的函数引用内部类时麻烦
    customersInput: customers
    orderdetailInput: orderdetail
    ordersInput: orders
    piciInput: pici
    productsInput: products
    proinfoInput: proinfo
    ruleInput: rule
    shippersInput: shippers
    suppliersInput: suppliers



