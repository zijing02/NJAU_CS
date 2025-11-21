import models 
import SqlOperation
from typing import Any
from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware

#uvicorn PythonSqlBackend:app --reload

app = FastAPI()
origins = [ 
    "*",  # Allow all origins
    ]
app.add_middleware(
    CORSMiddleware,
    allow_origins=origins,
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],)

@app.post("/api/data")
async def GetData(user: models.User):    
    return {"message": f"Hello, {user.userName}!"}

@app.post("/GetInfo/")
async def GetInfo(info:models.tableInfo):
    return SqlOperation.GetInfo(info)

@app.post("/GetProductsOfCategory/")
async def GetProductsOfCategory(CategoryID:int):
    return SqlOperation.GetProductsOfCategory(CategoryID)

@app.get("/GetAllProductsOfCategory/")
async def GetAllProductsOfCategory():
    return SqlOperation.GetAllProductsOfCategory()

@app.post("/GetPicturesofProducts/")
async def  GetPicturesofProducts(CategoryID:int):
    return SqlOperation. GetPicturesofProducts(CategoryID)

@app.get("/CountOrderNumOfAreaAndCustomers")
async def CountOrderNumOfAreaAndCustomers():
    return SqlOperation.CountOrderNumOfAreaAndCustomers()

@app.get("/CountOrderNumOfAreaAndCustomers1")
async def CountOrderNumOfAreaAndCustomers1():
    return SqlOperation.CountOrderNumOfAreaAndCustomers1()

@app.get("/CountOrderNumOfArea/")
async def CountOrderNumOfArea():
    return SqlOperation.CountOrderNumOfArea()

@app.get("/CountOrderNumOfSeason/")
async def CountOrderNumOfSeason():
    return SqlOperation.CountOrderNumOfSeason()

@app.get("/PriceOfProductsOfSuppliers/")
async def PriceOfProductsOfSuppliers():
    return SqlOperation.PriceOfProductsOfSuppliers()\
    
@app.get("/TotalMoneyOfSuppliers/")
async def TotalMoneyOfSuppliers():
    return SqlOperation.TotalMoneyOfSuppliers()

@app.get("/CountOrdersOfShippers/")
async def CountOrdersOfShippers():
    return SqlOperation.CountOrdersOfShippers()

@app.post("/CountOrdersOfSuppliers/")
async def CountOrdersOfSuppliers(CategoryID:int):
    return SqlOperation.CountOrdersOfSuppliers(CategoryID)

@app.post("/RegisterCustomer/")
async def RegisterCustomer(info:models.register_customer):
    return SqlOperation.RegisterCustomer(info)
         
@app.post("/UpdateCustomer/")
async def UpdateCustomer(info:models.customers1):
    return SqlOperation.UpdateCustomer(info)

@app.post("/CreateOrder/")
async def CreateOrder(info:models.orders):
    return SqlOperation.CreateOrder(info)

@app.post("/CreateOrderDetail/")
async def CreateOrderDetail(info:models.orderdetail):
    return SqlOperation.CreateOrderDetail

@app.post("/CreateOrderAndDetail/")
async def CreateOrderAndDetail(info:models.orders,info1:models.orderdetail):
    return SqlOperation.CreateOrderAndDetail(info,info1)

@app.post("/GetOrderDetails/")
async def GetOrderDetails(info:int):
    return SqlOperation.GetOrderDetails(info)

@app.post("/FuzzyQuery/")
async def FuzzyQuery(info:str):
    return SqlOperation.FuzzyQuery(info)
    