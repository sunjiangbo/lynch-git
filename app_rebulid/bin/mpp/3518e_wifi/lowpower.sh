#!/bin/sh

#关闭USB PHY的总电源：0x20050080 [12]bit配置为0
#himm 0x20050080 0x000121a8

#关闭USB PHY的模拟电源：0x20050084 [22]bit配置为1
#himm 0x20050084 0x005d2188

#关闭NANDC：0x200300D0 [1:0]配置为2'b01
himm 0x200300D0 0x5

#NANDC管脚复用成gpio
himm 0x200f00c8  0x1
himm 0x200f00cc  0x1
himm 0x200f00d0  0x1
himm 0x200f00d4  0x1
himm 0x200f00d8  0x1
himm 0x200f00dc  0x1
himm 0x200f00e0  0x1
himm 0x200f00e4  0x1
himm 0x200f00e8  0x1
himm 0x200f00ec  0x1
himm 0x200f00f4  0x1
himm 0x200f00f8  0x1

#关闭SAR ADC 时钟
himm 0x20030080 0x1

#关闭SAR ADC
himm 0x200b0008 0x1

#打开PWM
himm 0x20030038 0x2

#关闭IR
himm 0x20070000 0x0

#IR 管脚复用成gpio
himm 0x200f00c4 0x1

#UART2不使能：0x200A0000 [9][8][0]bit都配置为0
himm 0x200A0030 0x0

#UART2管脚复用成gpio
himm 0x200f0108 0x0
himm 0x200f010c 0x0

#关闭SPI0和SPI1
himm 0x200C0004 0x7F00
himm 0x200E0004 0x7F00

#spi0 管脚复用成gpio
himm 0x200f000c 0x0
himm 0x200f0010 0x0
himm 0x200f0014 0x0

#spi1 管脚复用成gpio
himm 0x200f0110 0x0
himm 0x200f0114 0x0
himm 0x200f0118 0x0
himm 0x200f011c 0x0

#AUDIO CODEC LINE IN 关闭左声道
#himm 0x20050068 0xa8022c2c
#himm 0x2005006c 0xf5035a4a

#关闭TDE
himm 0x20030058 0x0

#关闭VOU
himm 0x20030034 0x50

#关闭USB
#himm 0x200300b8 0xff
#himm 0x200300b8 0x48

#关闭SDIO
#himm 0x200300c4 0x4

#SDOI管脚复用成GPIO
#himm 0x200f0004 0x0
#himm 0x200f0080 0x0
#himm 0x200f0084 0x0
#himm 0x200f0088 0x0
#himm 0x200f008c 0x0
#himm 0x200f0090 0x0
#himm 0x200f0094 0x2
#himm 0x200f0098 0x0
#himm 0x200f009c 0x0

#3518E低功耗
himm 0x200f00f0 0x2
himm 0x200f00fc 0x2
