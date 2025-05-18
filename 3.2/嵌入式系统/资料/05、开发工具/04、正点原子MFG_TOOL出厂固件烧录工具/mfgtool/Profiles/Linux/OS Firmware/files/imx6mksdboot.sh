#! /bin/sh
#I.MX6 SD卡启动系统烧写脚本
#version v1.1
#Author QQ1252699831
#company 广州星翼电子科技有限公司

#version v1.0 2019.10.26
#version v1.1 2020.11.7 1.添加判断文件系统是否存在/lib/modules目录的功能，再解压模块
VERSION="1.1"
#打印用法
usage ()
{
  echo "

用法: `basename $1` [选项] <(必选)-device> <(可选)-flash> <(可选)-ddrsize>
用法示例：
sudo ./imx6mksdboot.sh -device /dev/sdd
sudo ./imx6mksdboot.sh -device /dev/sdd -flash emmc -ddrsize 512 
命令选项:
  -device              SD卡块设备节点 (例如/dev/sdx)
  -flash	       请选择开发板Flash类型（emmc | nand）
  -ddrsize	       请选择DDR大小 （512 | 256） 
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
  echo "I.MX6 SD卡制卡脚本"
  echo

  exit 0
}

#判断参数个数
arg=$#
if [ $arg -ne 6 ];then
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
echo "根据下面的提示，补全缺省的参数-flash -ddrsize"
read -p "请选择开发板参数，输入数字1~4，按Enter键确认
1.-flash emmc，-ddrsize 512
2.-flash emmc，-ddrsize 256
3.-flash nand，-ddrsize 512
4.-flash nand，-ddrsize 256 
输入数字1~4(default 1): " number
if [ -z $number ];then
  echo "使用默认参数:EMMC版本，DDR大小为512MB"
else
  case $number in
    1)  echo '您已经选择开发板参数为：EMMC版本，DDR大小为512MB'
	Uboot='u-boot-imx6ull-14x14-ddr512-emmc.imx'
    ;;
    2)  echo '您已经选择开发板参数为：EMMC版本，DDR大小为256MB'
	Uboot='u-boot-imx6ull-14x14-ddr256-emmc.imx'
    ;;
    3)  echo '您已经选择开发板参数为：NAND FLASH版本，DDR大小为512MB'
	Uboot='u-boot-imx6ull-14x14-ddr512-nand-sd.imx'
    ;;
    4)  echo '您已经选择开发板参数为：NAND FLASH版本，DDR大小为256MB'
	Uboot='u-boot-imx6ull-14x14-ddr256-nand-sd.imx'
    ;;
    *)  echo '输入的参数有误，退出制卡';exit;
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
    -flash) shift; flash=$1; shift; ;;
    -ddrsize) shift; ddrsize=$1; shift; ;;
    --version) version $0;;
    *) copy="$copy $1"; shift; ;;
  esac
done
  if [ $flash = "emmc" -a $ddrsize = "512" ];then
    Uboot='u-boot-imx6ull-14x14-ddr512-emmc.imx'
    echo '您已经选择开发板参数为：EMMC版本，DDR大小为512MB'

  elif [ $flash = "emmc" -a $ddrsize = "256" ];then
    Uboot='u-boot-imx6ull-14x14-ddr256-emmc.imx'
    echo '您已经选择开发板参数为：EMMC版本，DDR大小为256MB'

  elif [ $flash = "nand" -a $ddrsize = "512" ];then
    Uboot='u-boot-imx6ull-14x14-ddr512-nand-sd.imx'
    echo '您已经选择开发板参数为：NAND FLASH版本，DDR大小为512MB'

  elif [ $flash = "nand" -a $ddrsize = "256" ];then
    Uboot='u-boot-imx6ull-14x14-ddr256-nand-sd.imx'
    echo '您的开发板参数为：NAND FLASH版本，DDR大小为256MB'
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
#判断选择的块设备是否存在及是否是一个块设备
if [ ! -b $device ]; then
  echo "错误: $device 不是一个块设备文件"
  exit 1
fi
#这里防止选错设备，否则会影响Ubuntu系统的启动
if [ $device = '/dev/sda' ];then
  echo "请不要选择sda设备，/dev/sda通常是您的Ubuntu硬盘!
继续操作你的系统将会受到影响！脚本已自动退出"
  exit 1 
fi
echo "即将进行制作SD系统启动卡，大约花费几分钟时间,请耐心等待!"
echo "************************************************************"
echo "*         注意：这将会清除$device所有的数据               *"
echo "*         在脚本执行时请不要将$device拔出                 *"
echo "*             请按<Enter>确认继续                          *"
echo "************************************************************"
read enter

#格式化前要卸载
for i in `ls -1 $device?`; do
 echo "卸载 device '$i'"
 umount $i 2>/dev/null
done

#执行格式化$device
execute "dd if=/dev/zero of=$device bs=1024 count=1024"

#第一个分区为64M用来存放设备树与内核镜像文件，因为设备树与内核都比较小，不需要太大的空间
#第二个分区为SD卡的总大小-64M，用来存放文件系统
cat << END | fdisk -H 255 -S 63 $device
n
p
1

+64M
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

#两个分区处理
PARTITION1=${device}1
if [ ! -b ${PARTITION1} ]; then
        PARTITION1=${device}1
fi

PARTITION2=${device}2
if [ ! -b ${PARTITION2} ]; then
        PARTITION2=${device}2
fi

#第一个分区创建为Fat32格式
echo "格式化 ${device}1 ..."
if [ -b ${PARTITION1} ]; then
	mkfs.vfat -F 32 -n "boot" ${PARTITION1}
else
	echo "错误: /dev下找不到 SD卡 boot分区"
fi
#第二个分区创建为ext3格式
echo "格式化${device}2 ..."
if [ -b ${PARITION2} ]; then
	mkfs.ext3 -F -L "rootfs" ${PARTITION2}
else
	echo "错误: /dev下找不到 SD卡 rootfs分区"
fi

while [ ! -e $device ]
do
sleep 1
echo "wait for $device appear"
done

echo "正在烧写${Uboot}到${device}"
execute "dd if=$sdkdir/boot/$Uboot of=$device bs=1024 seek=1 conv=fsync"
sync
echo "烧写${Uboot}到${device}完成！"

echo "正在准备复制..."
echo "正在复制设备树与内核到${device}1，请稍候..."
execute "mkdir -p /tmp/sdk/$$"

while [ ! -e ${device}1 ]
do
sleep 1
echo "wait for ${device}1 appear"
done

execute "mount ${device}1 /tmp/sdk/$$"
execute "cp -r $sdkdir/boot/*${flash}*.dtb /tmp/sdk/$$/"
execute "cp -r $sdkdir/boot/zImage /tmp/sdk/$$/"
#execute "cp $sdkdir/boot/alientek.bmp /tmp/sdk/$$/"
sync
echo "复制设备树与内核到${device}1完成！"

if [ "$copy" != "" ]; then
  echo "Copying additional file(s) on ${device}p1"
  execute "cp -r $copy /tmp/sdk/$$"
fi

echo "卸载${device}1"
execute "umount /tmp/sdk/$$"
sleep 1
#解压文件系统到文件系统分区
#挂载文件系统分区
execute "mkdir -p /tmp/sdk/$$"
execute "mount ${device}2 /tmp/sdk/$$"

echo "正在解压文件系统到${device}2 ，请稍候..."
rootfs=`ls -1 filesystem/*.tar.*`
execute "tar jxfm $rootfs -C /tmp/sdk/$$"
sync
echo "解压文件系统到${device}2完成！"

#判断是否存在这个目录，如果不存在就为文件系统创建一个modules目录
if [ ! -e "/tmp/sdk//lib/modules/" ];then
mkdir -p /tmp/sdk/lib/modules/
fi

echo "正在解压模块到${device}2/lib/modules/ ，请稍候..."
modules=`ls -1 modules/*.tar.*`
execute "tar jxfm $modules -C /tmp/sdk/$$/lib/modules/"
sync
echo "解压模块到${device}2/lib/modules/完成！"

echo "卸载${device}2"
execute "umount /tmp/sdk/$$"

execute "rm -rf /tmp/sdk/$$"
sync
echo "SD卡启动系统烧写完成！"



