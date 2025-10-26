/*
SQLyog v10.2 
MySQL - 5.5.27 : Database - eshop
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
CREATE DATABASE /*!32312 IF NOT EXISTS*/`eshop` /*!40100 DEFAULT CHARACTER SET gbk */;

USE `eshop`;

/*Table structure for table `admin_info` */

DROP TABLE IF EXISTS `admin_info`;

CREATE TABLE `admin_info` (
  `id` int(4) NOT NULL AUTO_INCREMENT,
  `name` varchar(16) DEFAULT NULL,
  `pwd` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=gbk;

/*Data for the table `admin_info` */

insert  into `admin_info`(`id`,`name`,`pwd`) values (1,'admin','123456'),(2,'my','123456'),(3,'sj','123456'),(4,'lxf','123456');

/*Table structure for table `functions` */

DROP TABLE IF EXISTS `functions`;

CREATE TABLE `functions` (
  `id` int(4) NOT NULL AUTO_INCREMENT,
  `name` varchar(20) DEFAULT NULL COMMENT '功能菜单',
  `parentid` int(4) DEFAULT NULL,
  `url` varchar(50) DEFAULT NULL,
  `isleaf` bit(1) DEFAULT NULL,
  `nodeorder` int(4) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=12 DEFAULT CHARSET=gbk;

/*Data for the table `functions` */

insert  into `functions`(`id`,`name`,`parentid`,`url`,`isleaf`,`nodeorder`) values (1,'电子商城管理后台',0,NULL,'\0',0),(2,'商品管理',1,NULL,'\0',1),(3,'商品列表',2,NULL,'',1),(4,'商品类型列表',2,NULL,'',2),(5,'订单管理',1,NULL,'\0',2),(6,'查询订单',5,NULL,'',1),(7,'创建订单',5,NULL,'',2),(8,'客户管理',1,NULL,'\0',3),(9,'客户列表',8,NULL,'',1),(11,'退出系统',1,NULL,'',1);

/*Table structure for table `order_detail` */

DROP TABLE IF EXISTS `order_detail`;

CREATE TABLE `order_detail` (
  `id` int(4) NOT NULL AUTO_INCREMENT COMMENT '订单明细id',
  `oid` int(4) DEFAULT NULL COMMENT '订单id',
  `pid` int(4) DEFAULT NULL COMMENT '产品id',
  `num` int(4) DEFAULT NULL COMMENT '购买数量',
  PRIMARY KEY (`id`),
  KEY `pid` (`pid`),
  KEY `oid` (`oid`),
  CONSTRAINT `order_detail_ibfk_1` FOREIGN KEY (`oid`) REFERENCES `order_info` (`id`),
  CONSTRAINT `order_detail_ibfk_2` FOREIGN KEY (`pid`) REFERENCES `product_info` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8;

/*Data for the table `order_detail` */

insert  into `order_detail`(`id`,`oid`,`pid`,`num`) values (1,1,1,1),(2,1,2,1),(3,2,4,1),(4,2,5,1),(5,2,8,1);

/*Table structure for table `order_info` */

DROP TABLE IF EXISTS `order_info`;

CREATE TABLE `order_info` (
  `id` int(4) NOT NULL AUTO_INCREMENT,
  `uid` int(4) DEFAULT NULL,
  `status` varchar(16) DEFAULT NULL,
  `ordertime` datetime DEFAULT NULL,
  `orderprice` decimal(8,2) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `uid` (`uid`),
  CONSTRAINT `order_info_ibfk_1` FOREIGN KEY (`uid`) REFERENCES `user_info` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;

/*Data for the table `order_info` */

insert  into `order_info`(`id`,`uid`,`status`,`ordertime`,`orderprice`) values (1,1,'未付款','2018-05-12 00:00:00','10687.00'),(2,2,'已付款','2018-05-09 00:00:00','12997.00');

/*Table structure for table `powers` */

DROP TABLE IF EXISTS `powers`;

CREATE TABLE `powers` (
  `aid` int(4) NOT NULL,
  `fid` int(4) NOT NULL,
  PRIMARY KEY (`aid`,`fid`),
  KEY `fid` (`fid`),
  KEY `aid` (`aid`),
  CONSTRAINT `powers_ibfk_1` FOREIGN KEY (`aid`) REFERENCES `admin_info` (`id`),
  CONSTRAINT `powers_ibfk_2` FOREIGN KEY (`fid`) REFERENCES `functions` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=gbk;

/*Data for the table `powers` */

insert  into `powers`(`aid`,`fid`) values (1,1),(1,2),(1,3),(1,4),(1,5),(1,6),(1,7),(1,8),(1,9),(1,11);

/*Table structure for table `product_info` */

DROP TABLE IF EXISTS `product_info`;

CREATE TABLE `product_info` (
  `id` int(4) NOT NULL AUTO_INCREMENT,
  `code` varchar(16) DEFAULT NULL COMMENT '商品编号',
  `name` varchar(255) DEFAULT NULL COMMENT '商品名称',
  `tid` int(4) DEFAULT NULL COMMENT '商品类别',
  `brand` varchar(20) DEFAULT NULL COMMENT '商品品牌',
  `pic` varchar(255) DEFAULT NULL COMMENT '商品图片',
  `num` int(4) unsigned zerofill DEFAULT NULL COMMENT '商品库存',
  `price` decimal(10,0) unsigned zerofill DEFAULT NULL COMMENT '商品小图',
  `intro` longtext COMMENT '商品简介',
  `status` int(4) DEFAULT '1' COMMENT '商品状态',
  PRIMARY KEY (`id`),
  KEY `tid` (`tid`),
  CONSTRAINT `product_info_ibfk_1` FOREIGN KEY (`tid`) REFERENCES `type` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=13 DEFAULT CHARSET=utf8;

/*Data for the table `product_info` */

insert  into `product_info`(`id`,`code`,`name`,`tid`,`brand`,`pic`,`num`,`price`,`intro`,`status`) values (1,'1378538','AppleMJVE2CH/A',1,'APPLE','1378538.jpg',0100,'0000006488','Apple MacBook Air 13.3英寸笔记本电脑 银色(Core i5 处理器/4GB内存/128GB SSD闪存 MJVE2CH/A)',1),(2,'1309456','ThinkPadE450C(20EH0001CD)',1,'ThinkPad','1309456.jpg',0097,'0000004199','联想（ThinkPad） 轻薄系列E450C(20EH0001CD)14英寸笔记本电脑(i5-4210U 4G 500G 2G独显 Win8.1)',1),(3,'1999938','联想小新300经典版',1,'联想（Lenovo）','1999938.jpg',0099,'0000004399','联想（Lenovo）小新300经典版 14英寸超薄笔记本电脑（i7-6500U 4G 500G 2G独显 全高清屏 Win10）黑色',1),(4,'1466274','华硕FX50JX',1,'华硕（ASUS）','1466274.jpg',0100,'0000004799','华硕（ASUS）飞行堡垒FX50J 15.6英寸游戏笔记本电脑（i5-4200H 4G 7200转500G GTX950M 2G独显 全高清）',1),(5,'1981672','华硕FL5800',1,'华硕（ASUS）','1981672.jpg',0100,'0000004999','华硕（ASUS）FL5800 15.6英寸笔记本电脑 （i7-5500U 4G 128G SSD+1TB 2G独显 蓝牙 Win10 黑色）',1),(6,'1904696','联想G50-70M',1,'联想（Lenovo）','1904696.jpg',0012,'0000003499','联想（Lenovo）G50-70M 15.6英寸笔记本电脑（i5-4210U 4G 500G GT820M 2G独显 DVD刻录 Win8.1）金属黑',1),(7,'751624','美的BCD-206TM(E)',2,' 美的（Midea）','751624.jpg',0100,'0000001298','美的(Midea) BCD-206TM(E) 206升 三门冰箱 节能保鲜 闪白银',1),(8,'977433','美的BCD-516WKM(E)',2,' 美的（Midea）','977433.jpg',0100,'0000003199','美的(Midea) BCD-516WKM(E) 516升 对开门冰箱 风冷无霜 电脑控温 纤薄设计 节能静音 （泰坦银）',1),(9,'1143562','海尔BCD-216SDN',2,' 海尔（Haier）','1143562.jpg',0100,'0000001699','海尔（Haier）BCD-216SDN 216升 三门冰箱 电脑控温 中门 宽幅变温 大冷冻能力低能耗更省钱',1),(10,'1560207','海尔BCD-258WDPM',2,' 海尔（Haier）','1560207.jpg',0100,'0000002699','海尔（Haier）BCD-258WDPM 258升 风冷无霜三门冰箱 除菌 3D立体环绕风不风干 中门5℃~-18℃变温室',1),(11,'1721668','海信（Hisense）BCD-559WT/Q',2,' 海信（Hisense）','1721668.jpg',0100,'0000003499','海信（Hisense）BCD-559WT/Q 559升 金色电脑风冷节能对开门冰箱',1),(12,'823125','海信BCD-211TD/E',2,' 海信（Hisense）','823125.jpg',0100,'0000001699','海信（Hisense） BCD-211TD/E 211升 电脑三门冰箱 （亮金刚）',1);

/*Table structure for table `type` */

DROP TABLE IF EXISTS `type`;

CREATE TABLE `type` (
  `id` int(4) NOT NULL AUTO_INCREMENT,
  `name` varchar(20) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=gbk;

/*Data for the table `type` */

insert  into `type`(`id`,`name`) values (1,'电脑'),(2,'冰箱'),(3,'电视机'),(4,'洗衣机'),(5,'数码相机');

/*Table structure for table `user_info` */

DROP TABLE IF EXISTS `user_info`;

CREATE TABLE `user_info` (
  `id` int(4) NOT NULL AUTO_INCREMENT,
  `userName` varchar(16) DEFAULT NULL,
  `password` varchar(16) DEFAULT NULL,
  `realName` varchar(8) DEFAULT NULL,
  `sex` varchar(4) DEFAULT NULL,
  `address` varchar(255) DEFAULT NULL,
  `email` varchar(50) DEFAULT NULL,
  `regDate` date DEFAULT NULL,
  `status` int(4) DEFAULT '1',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8;

/*Data for the table `user_info` */

insert  into `user_info`(`id`,`userName`,`password`,`realName`,`sex`,`address`,`email`,`regDate`,`status`) values (1,'tom','123456','汤姆','女','江苏省苏州市吴中区','tom@123.com','2013-07-14',1),(2,'john','123456','约翰','女','江苏省南京市玄武区','wen@135.com','2013-07-14',1),(3,'my','123456','my','男','江苏省南京市玄武区','a@135.com','2015-09-16',1),(4,'sj','123456','sj','男','江苏省南京市玄武区','b@135.com','2015-09-16',1),(5,'lxf','123456','lxf','男','江苏省南京市玄武区','c@135.com','2015-09-16',1),(6,'lj','123456','lj','男','江苏省南京市玄武区','a@135.com','2015-09-20',1);

/* Procedure structure for procedure `sp_sale` */

/*!50003 DROP PROCEDURE IF EXISTS  `sp_sale` */;

DELIMITER $$

/*!50003 CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_sale`(c int)
BEGIN	
	declare stmt varchar(2000);
	set @sqlstr=concat("SELECT p.id AS id, p.name AS NAME,SUM(od.num) AS total ,SUM(od.num)*price AS money
	FROM order_detail od, product_info p 
	WHERE p.id=od.p_id 
	GROUP BY p.id,p.name,p.price ORDER BY total DESC LIMIT 1,",c);
     prepare stmt from @sqlstr;
     execute stmt;
    END */$$
DELIMITER ;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
