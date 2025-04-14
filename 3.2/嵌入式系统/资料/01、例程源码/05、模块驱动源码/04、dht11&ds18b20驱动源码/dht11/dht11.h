#ifndef __DHT11_H
#define __DHT11_H

#include <linux/ioctl.h>
#include <linux/types.h>

#define  HIGH                   1
#define  LOW                    0
#define  DHT11_IOCTL_BASE       'D'
#define  DHT11_GET_VALUE        _IOR(DHT11_IOCTL_BASE, 0, unsigned short)

#endif