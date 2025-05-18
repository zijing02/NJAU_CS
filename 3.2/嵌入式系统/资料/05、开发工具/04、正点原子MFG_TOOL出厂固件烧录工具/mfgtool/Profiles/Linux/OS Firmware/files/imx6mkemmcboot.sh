#!/bin/sh
#I.MX6 eMMC启动系统烧写脚本
#version v1.1
#Author QQ1252699831
#company 广州星翼电子科技有限公司
VERSION="1.1"

#version v1.0 2019.10.26
#version v1.1 2020.11.7 1.添加判断文件系统是否存在/lib/modules目录的功能，再解压模块

#打印用法
usage ()
{
  echo "

用法: `basename $1` [选项] <(必选)-device> <(可选)-ddrsize>
用法示例：
./imx6mkemmcboot.sh -device /dev/mmcblk1
./imx6mkemmcboot.sh -device /dev/mmcblk1 -ddrsize 512
命令选项:
  -device              eMMC块设备节点 (一般eMMC设备节点为/dev/mmcblk1)
  -ddrsize             请选择DDR大小 （512 | 256)
可选选项:
  --version             打印版本信息.
  --help                打印帮助信息.
"
  exit 1
}
#Uboot默认值
Uboot='u-boot-imx6ull-14x14-ddr512-emmc.imx'

#execute执行语句成功与否打印
execute ()
{
    $* >/dev/null
    if [ $? -ne 0 ]; then
        echo
        echo "错误: 执行 $*"
        echo
        exit 1
    fi
}

#打印版本信息
version ()
{
  echo
  echo "`basename $1` version $VERSION"
  echo "I.MX6 系统烧写脚本"
  echo

  exit 0
}

#等待mmc
while [ ! -e /dev/mmcblk1 ];do 
sleep 1
echo \"等待mmcblk1...\"
done

#判断参数个数
arg=$#
if [ $arg -ne 4 ];then
number=1
while [ $# -gt 0 ]; do
  case $1 in
    --help | -h)
      usage $0
      ;;
    -device) shift; device=$1; shift; ;;
    --version) version $0;;
    *) copy="$copy $1"; shift; ;;
  esac
done
#判断字符串是否为零
test -z $device && usage $0
echo ""
echo "根据下面的提示，补全缺省的参数-ddrsize"
read -p "请选择开发板参数，输入数字1|2，按Enter键确认
1.-ddrsize 512
2.-ddrsize 256
输入数字1~4(default 1): " number
if [ -z $number ];then
  echo "使用默开发板参数DDR大小为512MB"
else
  case $number in
    1)  echo '您已经选择开发板参数为：DDR大小为512MB'
        Uboot='u-boot-imx6ull-14x14-ddr512-emmc.imx'
    ;;
    2)  echo '您已经选择开发板参数为：DDR大小为256MB'
        Uboot='u-boot-imx6ull-14x14-ddr256-emmc.imx'
    ;;
    *)  echo '输入的参数有误，退出烧写eMMC';exit;
    ;;
esac
fi
else
#命令行处理，根据选项获得参数
while [ $# -gt 0 ]; do
  case $1 in
    --help | -h)
      usage $0
      ;;
    -device) shift; device=$1; shift; ;;
    -ddrsize) shift; ddrsize=$1; shift; ;;
    --version) version $0;;
    *) copy="$copy $1"; shift; ;;
  esac
done
  if [ $ddrsize = "512" ];then
    Uboot='u-boot-imx6ull-14x14-ddr512-emmc.imx'
    echo '您已经选择开发板参数为：DDR大小为512MB'
  elif [ $ddrsize = "256" ];then
    Uboot='u-boot-imx6ull-14x14-ddr256-emmc.imx'
    echo '您已经选择开发板参数为：DDR大小为256MB'
  else
    echo '参数有误!'
    usage $0
  fi
fi

#测试制卡包当前目录下是否缺失制卡所需要的文件
sdkdir=$PWD

if [ ! -d $sdkdir ]; then
   echo "错误: $sdkdir目录不存在"
   exit 1
fi

if [ ! -f $sdkdir/filesystem/*.tar.* ]; then
  echo "错误: $sdkdir/filesystem/下找不到文件系统压缩包"
  exit 1
fi

if [ ! -f $sdkdir/boot/zImage ]; then
  echo "错误: $sdkdir/boot/下找不到zImage"
  exit 1
fi

if [ ! -b $device ]; then
   echo "错误: $device 不是一个块设备文件"
   exit 1
fi
echo "即将进行制作eMMC系统启动卡，大约花费几分钟时间,请耐心等待!"
echo "************************************************************"
echo "*         注意：这将会清除$device所有的数据           *"
echo "          烧写eMMC前，请备份好重要的数据                   *"
echo "*             请按<Enter>确认继续                          *"
echo "************************************************************"
read enter

#分区前要卸载
for i in `ls -1 ${device}p?`; do
 echo "卸载 device '$i'"
 umount $i 2>/dev/null
done

#清空$device
execute "dd if=/dev/zero of=$device bs=1024 count=1024"

#第一个分区为32M用来存放设备树与内核镜像文件，因为设备树与内核都比较小，不需要太大的空间
#第二个分区为eMMC的总大小-32M，用来存放文件系统
cat << END | fdisk -H 255 -S 63 $device
n
p
1

+32M
n
p
2


t
1
c
a
1
w
END
sleep 3
#上面分区后系统会自动挂载，所以这里再一次卸载
for i in `ls -1 ${device}p?`; do
 echo "卸载 device '$i'"
 umount $i 2>/dev/null
done

#两个分区处理
PARTITION1=${device}1
if [ ! -b ${PARTITION1} ]; then
        PARTITION1=${device}p1
fi

PARTITION2=${device}2
if [ ! -b ${PARTITION2} ]; then
        PARTITION2=${device}p2
fi

#第一个分区创建为Fat32格式
echo "Formating ${device}p1 ..."
if [ -b ${PARTITION1} ]; then
	mkfs.vfat -F 32 -n "boot" ${PARTITION1}
else
	echo "错误: /dev下找不到 eMMC boot分区"
fi
#第二个分区创建为ext3格式
echo "格式化${device}p2 ..."
if [ -b ${PARITION2} ]; then
	mkfs.ext3 -F -L "rootfs" ${PARTITION2}
else
	echo "错误: /dev下找不到 eMMC rootfs分区"
fi

#烧写u-boot.imx到emmc boot0分区
echo "正在烧写${Uboot}到$device"
#烧写前，先使能mmcblk1boot0
echo 0 > /sys/block/mmcblk1boot0/force_ro
execute "dd if=$sdkdir/boot/$Uboot of=${device}boot0 bs=1024 seek=1 conv=fsync"
execute "dd if=$sdkdir/boot/$Uboot of=${device}boot0 bs=1024 seek=500 conv=fsync"
echo 1 >/sys/block/mmcblk1boot0/force_ro

#烧写内核与设备树到 emmc mmcblk1p1
echo "正在准备复制..."
echo "正在复制设备树与内核到${device}p1，请稍候..."
execute "mkdir -p /tmp/sdk/$$"
execute "mount ${device}p1 /tmp/sdk/$$"
execute "cp -r $sdkdir/boot/*emmc*.dtb /tmp/sdk/$$/"
execute "cp -r $sdkdir/boot/zImage /tmp/sdk/$$/"
sync
echo "复制设备树与内核到${device}p1完成！"

if [ "$copy" != "" ]; then
  echo "Copying additional file(s) on ${device}p1"
  execute "cp -r $copy /tmp/sdk/$$"
fi

echo "卸载${device}p1"
execute "umount /tmp/sdk/$$"

#挂载文件系统分区
execute "mkdir -p /tmp/sdk/$$"
execute "mount ${device}p2 /tmp/sdk/$$"

#解压文件系统到 emmc mmcblk1p2
echo "正在解压文件系统到${device}p2 ，请稍候..."
rootfs=`ls -1 filesystem/*.tar.*`
execute "tar jxfm $rootfs -C /tmp/sdk/$$"
sync
echo "解压文件系统到${device}p2完成！"

#判断是否存在这个目录，如果不存在就为文件系统创建一个modules目录
if [ ! -e "/tmp/sdk/$$/lib/modules/" ];then
mkdir -p /tmp/sdk/$$/lib/modules
fi

echo "正在解压模块到${device}p2/lib/modules/，请稍候..."
modules=`ls -1 modules/*.tar.*`
execute "tar jxfm $modules -C /tmp/sdk/$$/lib/modules/"
sync
echo "解压模块到${device}p2/lib/modules/完成！"

echo "卸载${device}p2"
execute "umount /tmp/sdk/$$"

execute "rm -rf /tmp/sdk/$$"
#使能启动分区
execute "mmc bootpart enable 1 1 /dev/mmcblk1"
sync
echo "eMMC启动系统烧写完成！"
