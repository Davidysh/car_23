## 简单的摄像头循迹小车

开发板型号为：CH32V307 risc-v指令集

使用Mounriver ide管理此项目，双击src/MounRiver/car_23.wvproj打开项目

Mounriver安装软件下载[链接🔗](http://mounriver.com/)

> 1. xmake.lua文件仅用于为vscode生成complie_commands 
>
>    `xmake project -k compile_commands`
>
> 2. 因本项目lib使用gbk编码，为保持一致性，使用gbk编码文件

模板
/*==================================================================
* 函数名: OSG_Memcpy
* 功能: OSG库提供复制函数,效率会比系统自带的memcopy效率高
* 输入: dst:目标地址  src:源地址   lenth:复制长度
* 输出: 无
* 用法: OSG_Memcpy(str1,str2,100);
* 修改记录:
===================================================================*/