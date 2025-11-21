-- MySQL dump 10.13  Distrib 8.0.44, for Win64 (x86_64)
--
-- Host: localhost    Database: cd
-- ------------------------------------------------------
-- Server version	8.0.44

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `category`
--

DROP TABLE IF EXISTS `category`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `category` (
  `categoryName` varchar(255) DEFAULT NULL,
  `instruction` varchar(255) DEFAULT NULL,
  `createTime` date DEFAULT NULL,
  `updateTime` date DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `category`
--

LOCK TABLES `category` WRITE;
/*!40000 ALTER TABLE `category` DISABLE KEYS */;
INSERT INTO `category` VALUES ('谷类/麦片','面包、饼干、生面团和谷物','2016-07-16','2016-08-01'),('调味品','香甜可口的果酱、调料、酱汁和调味品','2015-07-10','2015-08-16'),('点心','甜点、糖和甜面包','2023-07-12','3023-08-05');
/*!40000 ALTER TABLE `category` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `customer`
--

DROP TABLE IF EXISTS `customer`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `customer` (
  `customerCode` varchar(255) DEFAULT NULL,
  `companyName` varchar(255) DEFAULT NULL,
  `contactName` varchar(255) DEFAULT NULL,
  `contactPosition` varchar(255) DEFAULT NULL,
  `address` varchar(255) DEFAULT NULL,
  `city` varchar(255) DEFAULT NULL,
  `area` varchar(255) DEFAULT NULL,
  `postCode` int DEFAULT NULL,
  `country` varchar(255) DEFAULT NULL,
  `tel` varchar(255) DEFAULT NULL,
  `fax` varchar(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `customer`
--

LOCK TABLES `customer` WRITE;
/*!40000 ALTER TABLE `customer` DISABLE KEYS */;
INSERT INTO `customer` VALUES ('BLAUS','通恒机械','黄小姐','采购员','东园西甲 30 号','南京','华东',798089,'中国','(0921) 9123465','(0921) 55123467'),('BLONP','森通','王先生','销售代表','常保阁东 80 号','天津','华北',787045,'中国','(030) 30058460','(030)  33008924'),('BOTTM','国皓','黄雅玲','市场经理','广发北路 10 号','大连','东北',565479,'中国','(0671) 88601531','(0671) 88601532'),('BSBEV','广通','王先生','结算经理','平谷嘉石大街 38 号','重庆','西南',808059,'中国','(078) 95554729','(078) 95553745'),('CACTU','光明杂志','谢丽秋','销售代表','黄石路 50 号','深圳','华南',760908,'中国','(0571) 45551212',NULL),('CENTC','威航货运有限公司','刘先生','销售代理','经七纬二路 13 号','大连','东北',120412,'中国','(061) 11355555','(061) 11354892'),('CENTC','三捷实业','王先生','市场经理','英雄山路 84 号','大连','东北',130083,'中国','(061) 15553392','(061) 15557293');
/*!40000 ALTER TABLE `customer` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `order`
--

DROP TABLE IF EXISTS `order`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `order` (
  `orderID` int DEFAULT NULL,
  `orderDate` varchar(255) DEFAULT NULL,
  `supplyDate` varchar(255) DEFAULT NULL,
  `arrivalDate` varchar(255) DEFAULT NULL,
  `confirmDate` varchar(255) DEFAULT NULL,
  `shippingCost` varchar(255) DEFAULT NULL,
  `ownerName` varchar(255) DEFAULT NULL,
  `ownerAddress` varchar(255) DEFAULT NULL,
  `ownerCity` varchar(255) DEFAULT NULL,
  `ownerArea` varchar(255) DEFAULT NULL,
  `ownerPostCode` int DEFAULT NULL,
  `ownerCountry` varchar(255) DEFAULT NULL,
  `payWay` varchar(255) DEFAULT NULL,
  `insurance` int DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `order`
--

LOCK TABLES `order` WRITE;
/*!40000 ALTER TABLE `order` DISABLE KEYS */;
INSERT INTO `order` VALUES (10248,'1996-07-04','1996-07-16','1996-08-01',NULL,NULL,'余小姐','光明北路 124 号','北京','华北',111080,'中国','支付宝',1),(10249,'1996-07-05','1996-07-10','1996-08-16',NULL,NULL,'谢小姐','青年东路 543 号','济南','华东',440876,'中国','微信',1),(10250,'1996-07-08','1996-07-12','1996-08-05',NULL,NULL,'谢小姐','光化街 22 号','秦皇岛','华北',754546,'中国','银行卡',0),(10251,'2025-11-21','2025-11-28','2025-12-05',NULL,NULL,'admin','1','1',NULL,NULL,NULL,'支付宝',1);
/*!40000 ALTER TABLE `order` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `orderdetail`
--

DROP TABLE IF EXISTS `orderdetail`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `orderdetail` (
  `orderID` int DEFAULT NULL,
  `productID` int DEFAULT NULL,
  `num` int DEFAULT NULL,
  `ps` varchar(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `orderdetail`
--

LOCK TABLES `orderdetail` WRITE;
/*!40000 ALTER TABLE `orderdetail` DISABLE KEYS */;
INSERT INTO `orderdetail` VALUES (10248,17,12,NULL),(10248,42,10,NULL),(10248,72,5,NULL),(10249,14,9,NULL),(10249,51,40,NULL),(10250,41,10,NULL),(10250,51,35,NULL),(10250,65,15,NULL),(10251,1,1,NULL);
/*!40000 ALTER TABLE `orderdetail` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `product`
--

DROP TABLE IF EXISTS `product`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `product` (
  `productID` int DEFAULT NULL,
  `productName` varchar(255) DEFAULT NULL,
  `productNum` varchar(255) DEFAULT NULL,
  `price` double DEFAULT NULL,
  `inventoryNum` int DEFAULT NULL,
  `orderNum` int DEFAULT NULL,
  `twiceOrderNum` int DEFAULT NULL,
  `terminal` int DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `product`
--

LOCK TABLES `product` WRITE;
/*!40000 ALTER TABLE `product` DISABLE KEYS */;
INSERT INTO `product` VALUES (1,'苹果汁','每箱24瓶',18,38,0,10,1),(2,'牛奶','每箱24瓶',19,17,40,25,0),(3,'蕃茄酱','每箱12瓶',10,13,70,25,0),(4,'盐','每箱12瓶',22,53,0,0,0),(5,'麻油','每箱12瓶',21.35,19,0,0,1),(6,'酱油','每箱12瓶',25,120,0,25,0),(7,'海鲜粉','每箱30盒',30,15,0,10,0),(8,'胡椒粉','每箱30盒',40,17,0,0,0),(9,'鸡','每袋500克',97,29,0,0,1),(10,'蟹','每袋500克',31,31,0,0,0),(11,'大众奶酪','每袋6包',21,22,30,30,0),(12,'德国奶酪','每箱12瓶',38,86,0,0,0),(13,'龙虾','每袋500克',6,24,0,5,0),(14,'沙茶','每箱12瓶',23.25,35,0,0,0),(15,'味精','每箱30盒',15.5,39,0,5,0);
/*!40000 ALTER TABLE `product` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `productinfo`
--

DROP TABLE IF EXISTS `productinfo`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `productinfo` (
  `productBatchID` int DEFAULT NULL,
  `productID` int DEFAULT NULL,
  `productDate` date DEFAULT NULL,
  `shelfLife` date DEFAULT NULL,
  `f5` varchar(255) DEFAULT NULL,
  `f6` varchar(255) DEFAULT NULL,
  `f7` varchar(255) DEFAULT NULL,
  `f8` varchar(255) DEFAULT NULL,
  `f9` varchar(255) DEFAULT NULL,
  `f10` varchar(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `productinfo`
--

LOCK TABLES `productinfo` WRITE;
/*!40000 ALTER TABLE `productinfo` DISABLE KEYS */;
INSERT INTO `productinfo` VALUES (1,1,'1996-07-04','1996-07-16',NULL,NULL,NULL,NULL,NULL,NULL),(1,5,'1996-07-05','1996-07-10',NULL,NULL,NULL,NULL,NULL,NULL),(2,1,'1996-07-11','1996-07-14',NULL,NULL,NULL,NULL,NULL,NULL),(2,5,'1996-07-14','1996-07-16',NULL,NULL,NULL,NULL,NULL,NULL);
/*!40000 ALTER TABLE `productinfo` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `shipper`
--

DROP TABLE IF EXISTS `shipper`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `shipper` (
  `shipperID` int DEFAULT NULL,
  `companyName` varchar(255) DEFAULT NULL,
  `tel` varchar(255) DEFAULT NULL,
  `transportTool` varchar(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `shipper`
--

LOCK TABLES `shipper` WRITE;
/*!40000 ALTER TABLE `shipper` DISABLE KEYS */;
INSERT INTO `shipper` VALUES (1,'急速快递','(010) 65559831','飞机');
/*!40000 ALTER TABLE `shipper` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `supplier`
--

DROP TABLE IF EXISTS `supplier`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `supplier` (
  `supplierID` int DEFAULT NULL,
  `companyName` varchar(255) DEFAULT NULL,
  `contactName` varchar(255) DEFAULT NULL,
  `contactPosition` varchar(255) DEFAULT NULL,
  `address` varchar(255) DEFAULT NULL,
  `city` varchar(255) DEFAULT NULL,
  `area` varchar(255) DEFAULT NULL,
  `postCode` int DEFAULT NULL,
  `country` varchar(255) DEFAULT NULL,
  `tel` varchar(255) DEFAULT NULL,
  `fax` varchar(255) DEFAULT NULL,
  `homePage` varchar(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `supplier`
--

LOCK TABLES `supplier` WRITE;
/*!40000 ALTER TABLE `supplier` DISABLE KEYS */;
INSERT INTO `supplier` VALUES (1,'佳佳乐','陈小姐','采购经理','西直门大街 110 号','北京','华北',100023,'中国','(010) 65552222',NULL,NULL),(2,'康富食品','黄小姐','订购主管','幸福大街 290 号','北京','华北',170117,'中国','(010) 65554822',NULL,NULL),(3,'妙生','胡先生','销售代表','南京路 23 号','上海','华东',248104,'中国','(021) 85555735','(021) 85553349',NULL),(4,'为全','王先生','市场经理','永定路 342 号','北京','华北',100045,'中国','(020) 65555011',NULL,NULL),(5,'日正','李先生','出口主管','体育场东街 34 号','北京','华北',133007,'中国','(010) 65987654',NULL,NULL);
/*!40000 ALTER TABLE `supplier` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `users`
--

DROP TABLE IF EXISTS `users`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `users` (
  `id` int NOT NULL AUTO_INCREMENT,
  `username` varchar(50) NOT NULL,
  `password` varchar(255) NOT NULL,
  `userType` enum('admin','manager','user') DEFAULT 'user',
  `createdAt` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  UNIQUE KEY `username` (`username`)
) ENGINE=InnoDB AUTO_INCREMENT=10 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `users`
--

LOCK TABLES `users` WRITE;
/*!40000 ALTER TABLE `users` DISABLE KEYS */;
INSERT INTO `users` VALUES (7,'admin','$2b$10$XB/nGdTBgm.7RQpsCd4RZe4ioBFCnABlo2..Njn5zJM/utit4dKQC','admin','2025-11-21 02:42:33'),(8,'user1','$2b$10$ziCEbEGOtIVvjjtCCFV5euf05sDPxD.sHXKAt7ykYs2RLHNjx4KwS','user','2025-11-21 02:43:25'),(9,'user2','$2b$10$PGH6Oekd09KlrMMx5ySZ..YUPwLaf8xP1XdoUeLdt1RHRp4sMn/h6','user','2025-11-21 04:57:14');
/*!40000 ALTER TABLE `users` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2025-11-21 15:46:56
