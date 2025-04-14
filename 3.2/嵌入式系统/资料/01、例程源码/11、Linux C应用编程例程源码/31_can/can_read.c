/***************************************************************
 Copyright © ALIENTEK Co., Ltd. 1998-2021. All rights reserved.
 文件名 : can_read.c
 作者 : 邓涛
 版本 : V1.0
 描述 : 一个简单地CAN数据读取示例代码
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
	int i;
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

	/* 设置过滤规则 */
	//setsockopt(sockfd, SOL_CAN_RAW, CAN_RAW_FILTER, NULL, 0);

	/* 接收数据 */
	for ( ; ; ) {
		if (0 > read(sockfd, &frame, sizeof(struct can_frame))) {
			perror("read error");
			break;
		}

		/* 校验是否接收到错误帧 */
		if (frame.can_id & CAN_ERR_FLAG) {
			printf("Error frame!\n");
			break;
		}

		/* 校验帧格式 */
		if (frame.can_id & CAN_EFF_FLAG)	//扩展帧
			printf("扩展帧 <0x%08x> ", frame.can_id & CAN_EFF_MASK);
		else		//标准帧
			printf("标准帧 <0x%03x> ", frame.can_id & CAN_SFF_MASK);

		/* 校验帧类型：数据帧还是远程帧 */
		if (frame.can_id & CAN_RTR_FLAG) {
			printf("remote request\n");
			continue;
		}

		/* 打印数据长度 */
		printf("[%d] ", frame.can_dlc);

		/* 打印数据 */
		for (i = 0; i < frame.can_dlc; i++)
			printf("%02x ", frame.data[i]);
		printf("\n");
	}

	/* 关闭套接字 */
	close(sockfd);
	exit(EXIT_SUCCESS);
}
