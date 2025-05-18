#! /bin/sh
#I.MX6 NandFlash启动系统烧写脚本
#version v1.1
#Author QQ1252699831
#company 广州星翼电子科技有限公司

#version v1.0 2019.10.26
#version v1.1 2020.11.7 1.添加判断文件系统是否存在/lib/modules目录的功能，再解压模块

VERSION="1.0"
#打印用法
usage ()
{
  echo "

用法: `basename $1` [选项] <(可选)-ddrsize>
用法示例：
./imx6mknandboot.sh
./imx6mknandboot.sh -ddrsize 512
命令选项:
  -ddrsize             请选择DDR大小 （512 | 256)
可选选项:
  --version             打印版本信息.
  --help                打印帮助信息.
"
  exit 1
}
#Uboot默认值
Uboot='u-boot-imx6ull-14x14-ddr512-nand.imx'
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

version ()
{
  echo
  echo "`basename $1` version $VERSION"
  echo "I.MX6 NandFlash启动系统烧写脚本"
  echo

  exit 0
}

#判断参数个数
arg=$#
#命令行处理，根据选项获得参数
while [ $# -gt 0 ]; do
  case $1 in
    --help | -h)
      usage $0
      ;;
    -ddrsize) shift; ddrsize=$1;shift; ;;
    --version) version $0;;
    *) copy="$copy $1"; shift; ;;
  esac
done

if [ $arg -ne 2 ];then
number=1
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
        Uboot='u-boot-imx6ull-14x14-ddr512-nand.imx'
    ;;
    2)  echo '您已经选择开发板参数为：DDR大小为256MB'
        Uboot='u-boot-imx6ull-14x14-ddr256-nand.imx'
    ;;
    *)  echo '输入的参数有误，退出烧写Nand Flash';exit;
    ;;
esac
fi
else
  if [ $ddrsize = "512" ];then
    Uboot='u-boot-imx6ull-14x14-ddr512-nand.imx'
    echo '您已经选择开发板参数为：DDR大小为512MB'
  elif [ $ddrsize = "256" ];then
    Uboot='u-boot-imx6ull-14x14-ddr256-nand.imx'
    echo '您已经选择开发板参数为：DDR大小为256MB'
  else
    echo '参数有误!'
    usage $0
  fi
fi

#测试制卡包当前目录下是否缺失制卡所需要的文件
#test -z $device && usage $0
sdkdir=$PWD

if [ ! -b /dev/mtdblock0 ]; then
   echo "错误: 你的设备不存在NandFlash"
   exit 1
fi

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

echo "即将进行固化NandFlash系统，大约花费几分钟时间,请耐心等待!"
echo "************************************************************"
echo "*         注意：这将会清除NandFlash所有的数据              *"
echo "*       在脚本执行时请勿断电或者中断烧写过程               *"
echo "*             请按<Enter>确认继续                          *"
echo "************************************************************"
read enter
#挂载debug，默认系统已经挂载
#execute "mount -t debugfs debugfs /sys/kernel/debug"
#烧写前，先使用指令flash_erase擦除各个分区
#NandFlash擦除是往设备里写'1'，写数据是写'0'

echo "正在擦除Uboot分区..."
flash_erase /dev/mtd0 0 0
echo "正在烧写U-boot..."
kobs-ng init -x -v --chip_0_device_path=/dev/mtd0 $sdkdir/boot/$Uboot >/dev/null 2>&1

flash_erase /dev/mtd1 0 0
flash_erase /dev/mtd2 0 0

echo "正在擦除设备树分区..."
flash_erase /dev/mtd3 0 0
echo "正在烧写设备树..."
nandwrite -p /dev/mtd3 $sdkdir/boot/imx6ull-14x14-nand-4.3-480x272-c.dtb >/dev/null 2>&1
nandwrite -s 0x20000 -p /dev/mtd3 $sdkdir/boot/imx6ull-14x14-nand-4.3-800x480-c.dtb >/dev/null 2>&1
nandwrite -s 0x40000 -p /dev/mtd3 $sdkdir/boot/imx6ull-14x14-nand-7-800x480-c.dtb >/dev/null 2>&1
nandwrite -s 0x60000 -p /dev/mtd3 $sdkdir/boot/imx6ull-14x14-nand-7-1024x600-c.dtb >/dev/null 2>&1
nandwrite -s 0x80000 -p /dev/mtd3 $sdkdir/boot/imx6ull-14x14-nand-10.1-1280x800-c.dtb >/dev/null 2>&1
nandwrite -s 0xa0000 -p /dev/mtd3 $sdkdir/boot/imx6ull-14x14-nand-hdmi.dtb >/dev/null 2>&1
nandwrite -s 0xc0000 -p /dev/mtd3 $sdkdir/boot/imx6ull-14x14-nand-vga.dtb >/dev/null 2>&1

echo "正在擦除内核分区..."
flash_erase /dev/mtd4 0 0
echo "正在烧写内核..."
nandwrite -p /dev/mtd4 $sdkdir/boot/zImage >/dev/null 2>&1

echo "正在擦除文件系统分区，请稍候..."
flash_erase /dev/mtd5 0 0
ubiformat /dev/mtd5
ubiattach /dev/ubi_ctrl -m 5
ubimkvol /dev/ubi0 -Nrootfs -m
mkdir -p /mnt/mtd5
mount -t ubifs ubi0:rootfs /mnt/mtd5
echo "正在解压文件系统到mtd5分区，请稍候..."
tar jxfm $sdkdir/filesystem/*.tar.* -C /mnt/mtd5

#判断是否存在这个目录，如果不存在就为文件系统创建一个modules目录
if [ ! -e "/mnt/mtd5/lib/modules" ];then
mkdir -p /mnt/mtd5/lib/modules
fi

echo "正在解压模块到mtd5分区，请稍候..."
tar jxfm $sdkdir/modules/*.tar.* -C /mnt/mtd5/lib/modules

sleep 1
sync
umount /mnt/mtd5
rm -rf /mnt/mtd5
ubidetach -p /dev/mtd5
sync

echo "NandFlash启动系统烧写完成！"
