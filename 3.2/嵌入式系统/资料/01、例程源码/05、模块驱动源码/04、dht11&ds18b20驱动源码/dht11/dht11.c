/*
 * Dht11 driver
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
#include "dht11.h"

struct dht11_dev {
	struct miscdevice mdev;
	int gpio;
};

struct dht11_dev dht11_device;
static long dht11_ioctl(struct file *, unsigned int, unsigned long);

static void dht11_set_output(int val)
{
	if(val)
		gpio_direction_output(dht11_device.gpio, 1);
	else
		gpio_direction_output(dht11_device.gpio, 0);
}

static void dht11_set_input(int val)
{
	gpio_direction_input(dht11_device.gpio);
/*	if(val)
		gpio_set_value(dht11_device.gpio, 1);
	else
		gpio_set_value(dht11_device.gpio, 0);*/
}

static unsigned char dht11_get_io(void)
{
	unsigned char ret = 0;
	ret = gpio_get_value(dht11_device.gpio);
	return ret;
}

static int dht11_request_gpio(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	int ret;

	dht11_device.gpio = of_get_named_gpio(dev->of_node, "dht11-gpio", 0);
	if (!gpio_is_valid(dht11_device.gpio)) {
		dev_err(dev, "Failed to get gpio");
		return -EINVAL;
	}

	ret = devm_gpio_request(dev, dht11_device.gpio, "DHT11 Gpio");
	if (ret) {
		dev_err(dev, "Failed to request gpio");
		return ret;
	}

	dht11_set_output(HIGH);

	return 0;
}

static int dht11_init(void)
{
	unsigned int time = 0;

	dht11_set_output(HIGH);

	dht11_set_output(HIGH);
	udelay(5);

	dht11_set_output(LOW);
	mdelay(20);

	dht11_set_output(HIGH);
	udelay(30);

	dht11_set_input(LOW);

	time = 0;
	while(dht11_get_io() != 0) {
		udelay(1);
		time++;
		if (time > 100)
		return -1;
	}

	time = 0;
	while(dht11_get_io() == 0) {
		udelay(1);
		time++;
		if (time > 100)
		return -2;
	}

	time = 0;
	while(dht11_get_io() == 1) {
		udelay(1);
		time++;
		if (time > 100)
		return -3;
	}

	return 0;
}

static int dht11_open(struct inode *inode, struct file *filp)
{
	filp->private_data = &dht11_device;
	return 0;
}

static ssize_t dht11_write(struct file *filp, const char __user *buf,
			size_t cnt, loff_t *offt)
{
	return 0;
}

static unsigned char Dht11_Read_Byte(void)
{
	unsigned char i, time = 0, data = 0;

	for(i = 0; i < 8; i++) {
		time = 0;

		while(dht11_get_io() == 0) {
			udelay(1);
			time++;
			if (time > 100) {
				return -EINVAL;
			}
		}
		udelay(45);
		if (dht11_get_io() == 1) {
			data |= 1<<(7 - i);
			time = 0;
			while(dht11_get_io() == 1) {
				udelay(1);
				time++;
				if (time > 100)
					return -EINVAL;
			}
		}
	}
	
	return data;
}

static ssize_t dht11_read(struct file *filp, char __user *buf, size_t cnt, loff_t *offt) 
{
	int ret = 0;
	unsigned char data[5] = {0, 0, 0, 0, 0};

	ret = dht11_init();
	dht11_set_input(HIGH);
	if (ret == 0){
		data[0] = Dht11_Read_Byte();
		data[1] = Dht11_Read_Byte();
		data[2] = Dht11_Read_Byte();
		data[3] = Dht11_Read_Byte();
		data[4] = Dht11_Read_Byte();

		ret = copy_to_user(buf, &data, sizeof(data)); 
	}else
		printk("error.number:%d,\r\n",ret);

	return ret;
}

static int dht11_read_rh_temp_data(char *data)
{
	int ret = 0;
	ret = dht11_init();
	dht11_set_input(HIGH);
	if (ret == 0) {
		data[0] = Dht11_Read_Byte();
		data[1] = Dht11_Read_Byte();
		data[2] = Dht11_Read_Byte();
		data[3] = Dht11_Read_Byte();
		data[4] = Dht11_Read_Byte();
	}
	
	return 0;
}

static ssize_t show_rh_temp(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	char data[5], time = 0;
	int ret = 0;
	
	if (dht11_read_rh_temp_data(data))
		return sprintf(buf, "%d\n", -1);
	else {
		while (data[4] != data[0] + data[1] + data[2] + data[3]){
			udelay(1);
			ret = dht11_read_rh_temp_data(data);
			time++;
			if (time > 50 || ret != 0)
			return sprintf(buf, "Get data timeout, check if the DHT11 sensor is inserted, error: %d\n", -1);
		}
	}

	return sprintf(buf, "%d.%d,%d.%d\n", data[0], data[1], data[2], data[3]);
}

static DEVICE_ATTR(value, S_IRUGO, show_rh_temp, NULL);

static struct attribute *dht11_attrs[] = {
	&dev_attr_value.attr,
	NULL
};

static const struct attribute_group dht11_attrs_group = {
	.attrs = dht11_attrs,
};

static const struct attribute_group *dht11_attr_groups[] = {
	&dht11_attrs_group,
	NULL
};

static struct file_operations dht11_fops = {
	.owner	= THIS_MODULE,
	.open	= dht11_open,
	.write	= dht11_write,
	.read   = dht11_read,
	.unlocked_ioctl = dht11_ioctl,
};

static int dht11_probe(struct platform_device *pdev)
{
	struct miscdevice *mdev;
	int ret;

	dev_info(&pdev->dev, "dht11 device and driver matched successfully!\n");

	dht11_device.mdev.groups = dht11_attr_groups;
	ret = dht11_request_gpio(pdev);
	if (ret)
		return ret;

	mdev = &dht11_device.mdev;
	mdev->name	= "dht11";
	mdev->minor	= MISC_DYNAMIC_MINOR;
	mdev->fops	= &dht11_fops;

	return misc_register(mdev);
}

static int dht11_remove(struct platform_device *pdev)
{
	gpio_set_value(dht11_device.gpio, 0);

	misc_deregister(&dht11_device.mdev);

	dev_info(&pdev->dev, "DHT11 driver has been removed!\n");
	return 0;
}

static long dht11_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	char data[5];
	int ret = 0;
	
	switch (cmd) {
	case DHT11_GET_VALUE:
	ret = dht11_read_rh_temp_data(data);
		break;
	default:
		ret = -1;
		break;
	};

	if (copy_to_user((void *)arg, data, sizeof(data)))
		return -1;

	return ret;
}

static const struct of_device_id dht11_of_match[] = {
	{ .compatible = "alientek, dht11" },
	{ /* Sentinel */ }
};

static struct platform_driver dht11_driver = {
	.driver = {
		.name			= "dht11",
		.of_match_table	= dht11_of_match,
	},
	.probe		= dht11_probe,
	.remove		= dht11_remove,
};

module_platform_driver(dht11_driver);

MODULE_AUTHOR("DengZhiMao <1252699831@qq.com>");
MODULE_DESCRIPTION("DHT11 Driver Based on Misc Driver Framework");
MODULE_LICENSE("GPL");
