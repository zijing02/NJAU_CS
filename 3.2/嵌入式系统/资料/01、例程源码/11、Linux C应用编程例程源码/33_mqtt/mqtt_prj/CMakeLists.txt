#*******************************************************************************
#  Copyright © ALIENTEK Co., Ltd. 1998-2021. All rights reserved.
#
#  顶层CMakeLists.txt
#  All rights reserved. This program and the accompanying materials
#  are made available under the terms of the Eclipse Public License v2.0
#  and Eclipse Distribution License v1.0 which accompany this distribution.
#*******************************************************************************/
cmake_minimum_required(VERSION 2.8.12)
project(MQTTClient C)
message(STATUS "CMake version: " ${CMAKE_VERSION})
message(STATUS "CMake system name: " ${CMAKE_SYSTEM_NAME})
message(STATUS "CMake system processor: " ${CMAKE_SYSTEM_PROCESSOR})

# 设置可执行文件输出路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

# 定义可执行文件目标
add_executable(mqttClient mqttClient.c)

# 指定MQTT客户端库头文件路径、库路径以及链接库
# ***大家需要根据MQTT的实际安装路径设置***
target_include_directories(mqttClient PRIVATE /home/dt/tools/paho.mqtt.c-1.3.8/install/include)#MQTT头文件搜索路径
target_link_directories(mqttClient PRIVATE /home/dt/tools/paho.mqtt.c-1.3.8/install/lib)	#MQTT库文件搜索路径
target_link_libraries(mqttClient PRIVATE paho-mqtt3c)		#MQTT链接库 libpaho-mqtt3c.so
