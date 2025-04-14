#ifndef __DS18B20_H
#define __DS18B20_H

#include <linux/ioctl.h>
#include <linux/types.h>

#define  DS18B20_IOCTL_BASE                     'D'
#define  DS18B20_GET_VALUE                      _IOR(DS18B20_IOCTL_BASE, 0, unsigned short)
#define HIGH                                    1
#define LOW                                     0
#define SKIP_ROM                                0xCC    /* skip rom operation */
#define TEMP_CONVET                             0x44    /* start temperature convertion */
#define READ_TEMP                               0xBE    /* start read temperature */

#endif
