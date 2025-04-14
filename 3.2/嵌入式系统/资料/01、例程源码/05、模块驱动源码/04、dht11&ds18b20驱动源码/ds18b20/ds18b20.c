/*
 * Ds18b20 driver
 * Copyright (C) 2020-5 Guangzhou ALIENTEK Electronic Technology Co., Ltd.
 *
 * Author: DengZhiMao <1252699831.com>
 * date: 2020/05/19
 * Maintainers: http://www.openedv.com/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <linux/delay.h>
#include <linux/ide.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/of_gpio.h>
#include "ds18b20.h"

unsigned char data[2];

struct ds18b20_dev {
	struct miscdevice mdev;
	int gpio;
};

struct ds18b20_dev ds18b20_device;
static long ds18b20_ioctl(struct file *, unsigned int, unsigned long);

static void ds18b20_set_output(int high)
{
	if (high)
		gpio_direction_output(ds18b20_device.gpio, 1);
	else
		gpio_direction_output(ds18b20_device.gpio, 0);
}

static void ds18b20_set_input(int high)
{
	gpio_direction_input(ds18b20_device.gpio);
	if (high)
		gpio_set_value(ds18b20_device.gpio, 1);
	else
		gpio_set_value(ds18b20_device.gpio, 0);
}

static unsigned char ds18b20_get_io(void)
{
	unsigned char ret = 0;
	ret = gpio_get_value(ds18b20_device.gpio);
	return ret;
}

static unsigned char ds18b20_write_byte(unsigned char byte)
{
	int i;
	for (i = 0; i < 8; i++) {
		if (byte & 0x01) {
			ds18b20_set_output(LOW);
			udelay(8);
			ds18b20_set_output(HIGH);
			udelay(55);
		} else {
			ds18b20_set_output(LOW);
			udelay(55);
			ds18b20_set_output(HIGH);
			udelay(8);
		}
		byte >>= 1;
	}
	
	return byte;
}

static unsigned char ds18b20_read_byte(void)
{
	unsigned char i = 0, byte = 0;
	for (i = 0; i< 8; i++) {
		byte >>= 1;
		ds18b20_set_output(LOW);
		udelay(1);
		
		ds18b20_set_output(HIGH);
		udelay(1);
		
		ds18b20_set_input(HIGH);
		
		if (ds18b20_get_io())
		byte |= 0x80;
		udelay(60);
	}

	return byte;
}

/*
 * ds18b20 reset, 0 for success, other for failure
 */
static int ds18b20_reset(void)
{
	int ret = -1;
	ds18b20_set_output(HIGH);
	ds18b20_set_output(LOW);
	udelay(500);
	
	ds18b20_set_output(HIGH);
	
	ds18b20_set_input(HIGH);
	
	/* wait 15-60us ds18b20 will respond */
	udelay(50);

	ret = ds18b20_get_io();

	udelay(800);
	
	return ret;
}

static int ds18b20_read_temp(void)
{
	int ret;
	
	ret = ds18b20_reset();

	/* skip ROM operation */
	ds18b20_write_byte(SKIP_ROM);
	
	/* start temperature convertion */
	ds18b20_write_byte(TEMP_CONVET); 
	mdelay(800);

	ds18b20_reset();
	
	/* skip ROM operation */
	ds18b20_write_byte(SKIP_ROM);
	
	/* start read temperature */
	ds18b20_write_byte(READ_TEMP); 

	data[0]=ds18b20_read_byte();
	data[1]=ds18b20_read_byte();

	ds18b20_set_output(HIGH);
	
	return ret;
}

static int ds18b20_init(void)
{
	ds18b20_reset();

	return 0;
}

static int ds18b20_request_gpio(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	int ret;

	ds18b20_device.gpio = of_get_named_gpio(dev->of_node, "ds18b20-gpio", 0);
	if (!gpio_is_valid(ds18b20_device.gpio)) {
		dev_err(dev, "Failed to get gpio");
		return -EINVAL;
	}

	ret = devm_gpio_request(dev, ds18b20_device.gpio, "DS18B20 Gpio");
	if (ret) {
		dev_err(dev, "Failed to request gpio");
		return ret;
	}

	ds18b20_init();

	return 0;
}

static int ds18b20_open(struct inode *inode, struct file *filp)
{
	filp->private_data = &ds18b20_device;
	return 0;
}

static ssize_t ds18b20_write(struct file *filp, const char __user *buf,
			size_t cnt, loff_t *offt)
{
	return 0;
}

static ssize_t ds18b20_read(struct file *filp, char __user *buf, size_t cnt, loff_t *offt) 
{
	ds18b20_read_temp();
	buf[0] = data[0];
	buf[1] = data[1];
	
	return 0;
}

static ssize_t show_rh_temp(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	u8 low_data,high_data;
	int temp, ret;
	
	ret = ds18b20_read_temp();
	
	low_data = data[0];
	high_data = data[1];
		
	if (high_data > 7) {
		high_data =~ high_data;
		low_data =~ low_data; 
		temp = 0; 
	} else
		temp = 1;
	
	temp = high_data;
	temp <<= 8;
	temp += low_data; 
	temp = temp * 625;

	if (ret != 0)
		return sprintf(buf, "check if the DS18B20 sensor is inserted, error data:%d\n", temp);
	
	if (temp)
		return sprintf(buf, "%d\n", temp);
	else 		
		return sprintf(buf, "-%d\n", temp);		
}

static DEVICE_ATTR(value, S_IRUGO, show_rh_temp, NULL);

static struct attribute *ds18b20_attrs[] = {
	&dev_attr_value.attr,
	NULL
};

static const struct attribute_group ds18b20_attrs_group = {
	.attrs = ds18b20_attrs,
};

static const struct attribute_group *ds18b20_attr_groups[] = {
	&ds18b20_attrs_group,
	NULL
};

static struct file_operations ds18b20_fops = {
	.owner	= THIS_MODULE,
	.open	= ds18b20_open,
	.write	= ds18b20_write,
	.read	= ds18b20_read,
	.unlocked_ioctl = ds18b20_ioctl,
};

static int ds18b20_probe(struct platform_device *pdev)
{
	struct miscdevice *mdev;
	int ret;

	dev_info(&pdev->dev, "ds18b20 device and driver matched successfully!\n");

	ds18b20_device.mdev.groups = ds18b20_attr_groups;
	ret = ds18b20_request_gpio(pdev);
	if (ret)
		return ret;

	mdev = &ds18b20_device.mdev;
	mdev->name	= "ds18b20";
	mdev->minor	= MISC_DYNAMIC_MINOR;
	mdev->fops	= &ds18b20_fops;
	

	return misc_register(mdev);
}

static int ds18b20_remove(struct platform_device *pdev)
{
	gpio_set_value(ds18b20_device.gpio, 0);

	misc_deregister(&ds18b20_device.mdev);

	dev_info(&pdev->dev, "DS18B20 driver has been removed!\n");
	
	return 0;
}

static long ds18b20_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	int ret = 0;
	
	switch (cmd) {
	case DS18B20_GET_VALUE:
	ret = ds18b20_read_temp();
		break;
	default:
		ret = -1;
		break;
	};

	if (copy_to_user((void *)arg, data, sizeof(data)))
		return -1;

	return ret;
}

static const struct of_device_id ds18b20_of_match[] = {
	{ .compatible = "alientek, ds18b20" },
	{ /* Sentinel */ }
};

static struct platform_driver ds18b20_driver = {
	.driver = {
		.name			= "ds18b20",
		.of_match_table	= ds18b20_of_match,
	},
	.probe		= ds18b20_probe,
	.remove		= ds18b20_remove,
};

module_platform_driver(ds18b20_driver);

MODULE_AUTHOR("DengZhiMao <1252699831@qq.com>");
MODULE_DESCRIPTION("DS18B20 Driver Based on Misc Driver Framework");
MODULE_LICENSE("GPL");
