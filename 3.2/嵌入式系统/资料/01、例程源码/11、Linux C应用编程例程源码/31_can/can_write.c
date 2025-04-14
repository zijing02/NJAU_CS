/***************************************************************
 Copyright © ALIENTEK Co., Ltd. 1998-2021. All rights reserved.
 文件名 : can_write.c
 作者 : 邓涛
 版本 : V1.0
 描述 : 一个简单地CAN数据发送示例代码
 其他 : 无
 论坛 : www.openedv.com
 日志 : 初版 V1.0 2021/7/20 邓涛创建
 ***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>

int main(void)
{
	struct ifreq ifr = {0};
	struct sockaddr_can can_addr = {0};
	struct can_frame frame = {0};
	int sockfd = -1;
	int ret;

	/* 打开套接字 */
	sockfd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
	if(0 > sockfd) {
		perror("socket error");
		exit(EXIT_FAILURE);
	}

	/* 指定can0设备 */
	strcpy(ifr.ifr_name, "can0");
	ioctl(sockfd, SIOCGIFINDEX, &ifr);
	can_addr.can_family = AF_CAN;
	can_addr.can_ifindex = ifr.ifr_ifindex;

	/* 将can0与套接字进行绑定 */
	ret = bind(sockfd, (struct sockaddr *)&can_addr, sizeof(can_addr));
	if (0 > ret) {
		perror("bind error");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	/* 设置过滤规则：不接受任何报文、仅发送数据 */
	setsockopt(sockfd, SOL_CAN_RAW, CAN_RAW_FILTER, NULL, 0);

	/* 发送数据 */
	frame.data[0] = 0xA0;
	frame.data[1] = 0xB0;
	frame.data[2] = 0xC0;
	frame.data[3] = 0xD0;
	frame.data[4] = 0xE0;
	frame.data[5] = 0xF0;
	frame.can_dlc = 6;	//一次发送6个字节数据
	frame.can_id = 0x123;//帧ID为0x123,标准帧

	for ( ; ; ) {

		ret = write(sockfd, &frame, sizeof(frame)); //发送数据
		if(sizeof(frame) != ret) { //如果ret不等于帧长度，就说明发送失败
			perror("write error");
			goto out;
		}

		sleep(1);		//一秒钟发送一次
	}

out:
	/* 关闭套接字 */
	close(sockfd);
	exit(EXIT_SUCCESS);
}
