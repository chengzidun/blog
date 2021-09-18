# RS485接口的IAP固件升级工具
<hr style=" border:solid; width:100px; height:1px;" color=#000000 size=1">

<font color=#999AAA >

@[TOC](文章目录)

</font>

<hr style=" border:solid; width:100px; height:1px;" color=#000000 size=1">

> # 前言
> <font color=#999AAA >当我们采用MCU设计的电子产品在出售后、安装在野外、固件存在BUG时或客户需求有变动, 都需要对固件进行维护和升级。由于距离远且需频繁调试测试, 不可能每次都安装人手去现场升级固件, 势必会造成巨大的成本开销, 这是非常不利于企业发展的。因此, 需要对产品可进行远程升级固件, 而远程升级固件就要使用到IAP技术。
> </font>

<hr style=" border:solid; width:100px; height:1px;" color=#000000 size=1">

<font color=#999AAA >

# 一、概述
<font face="宋体" color=black size=4>		
&nbsp;&nbsp;在介绍IAP技术前, 先了解下ISP技术。ISP（In-System Programming）在系统可编程，指电路板上的空白器件可以编程写入最终用户代码， 而不需要从电路板上取下器件，已经编程的器件也可以用ISP方式擦除或再编程。简单而言, ISP就是厂家在MCU芯片内植入了一段引导码, 可通过外设接口直接烧录程序进芯片，如串口。
</font>

<font face="宋体" color=black size=4>		
&nbsp;&nbsp;IAP(In-Application-Programming)在程序中编程,  是一种对通过微控制器的对外接口如：USART，IIC，CAN，USB, 以太网接口甚至是无线射频通道等对正在运行程序的微控制器进行内部程序的更新的技术。不同于ISP通过设置MCU内部的BootLoader程序引导烧写或者是ICP通过SWD、JTAG在线仿真烧写。简单而言, IAP编程就是可以在程序运行时进行编程烧录的一种技术。本文以CM3内核MCU及RS485(即USART)接口作为IAP技术的应用。
</font>

&ensp;

# 二、IAP技术原理
<font face="宋体" color=black size=4>		
&nbsp;&nbsp;IAP技术的核心是一段预先烧写在MCU内部的一个程序, 这个程序一般叫做bootloader。这段程序主要负责与外部的上位机软件进行握手同步，然后将通过外设通信接口将来自于上位机软件的程序数据接收后写入单片机内部指定的闪存区域，然后再跳转执行新写入的程序，最终就达到了程序更新的目的。
</font>

<font face="宋体" color=black size=4>		
&nbsp;&nbsp;那IAP程序是如何实现的呢?这就要首先了解MCU内部闪存组织架构和其启动过程, STM32做为ARM-CM3的经典产品, 就以它做为参考来描述其中原理。下面是CM3储存器映射图, 给出各部件间的地址分配。
</font>

![在这里插入图片描述](https://img-blog.csdnimg.cn/81319394fcb2441cab368f62cb39f69c.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAY2hlbmd6aWR1bg==,size_20,color_FFFFFF,t_70,g_se,x_16/resize,p_50#pic_center)

&ensp;

<font face="宋体" color=black size=4>		
&nbsp;&nbsp;可知RAM是从0x20000000处开始, 各厂家未必会一致, STM32的ROM地址是以0x8000000处开始。STM32内置Flash的分配情况大致如下图:
</font>

![在这里插入图片描述](https://img-blog.csdnimg.cn/615a918d0f9749f1991d87444e7f91f3.png#pic_center)

&ensp;

<font face="宋体" color=black size=4>		
&nbsp;&nbsp;下图是单个程序的启动过程, STM32有一个向量表, 从0x8000000+4处开始, 0x8000000处是栈顶地址。上电复位后从0x8000000+4处获取到复位中断向量地址, 然后跳转到复位中断程序入口标号1所示）, 执行结束后跳转main函数。在执行main函数的过程中发生中断，则STM32强制将PC指针指回中断向量表处(标号3所示)，从中断向量表中找到相应的中断函数入口地址，跳转到相应的中断服务函数，执行完中断服务函数后再返回到main函数中来。
</font>

![在这里插入图片描述](https://img-blog.csdnimg.cn/b3e98849e8e64d1f8860273aec068922.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAY2hlbmd6aWR1bg==,size_18,color_FFFFFF,t_70,g_se,x_16/resize,p_80#pic_center)

&ensp;

<font face="宋体" color=black size=4>		
&nbsp;&nbsp;当使用IAP技术时, FLASH分配情况就会不一样, 如下图:
</font>

![在这里插入图片描述](https://img-blog.csdnimg.cn/02f24f7a4d274f628cc7a9ed2c61b90a.png#pic_center)


&ensp;

<font face="宋体" color=black size=4>		
&nbsp;&nbsp;使用了IAP技术的启动流程图如下图:
</font>

![在这里插入图片描述](https://img-blog.csdnimg.cn/2199b734b70f481b81a9754e704ce539.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAY2hlbmd6aWR1bg==,size_15,color_FFFFFF,t_70,g_se,x_16/resize,p_80#pic_center)


<font face="宋体" color=black size=4>		
&nbsp;&nbsp;执行流程如下:
</font>

<font face="宋体" color=black size=4>		
&nbsp;&nbsp;1、STM32复位后，从地址为0x8000004处取出复位中断向量的地址，并跳转执行复位中断服务程序，随后跳转至IAP程序的main函数。
</font>

<font face="宋体" color=black size=4>		
&nbsp;&nbsp;2、执行完IAP过程后（STM32内部多出了新写入的程序，地址0x8000004+N+M）跳转至新写入程序的复位向量表，取出新程序的复位中断向量的地址，并跳转执行新程序的复位中断服务程序，随后跳转至新程序的main函数，注意在STM32的内部存储空间在不同的位置上出现了2个中断向量表。
</font>

<font face="宋体" color=black size=4>		
&nbsp;&nbsp;3、在新程序main函数执行的过程中，一个中断请求来临，PC指针仍会回转至地址为0x8000004中断向量表处，由于中断向量表         地址强制偏移而跳转新程序的中断向量表。⑤⑥⑦过程(内部存在两个中断向量表)。
</font>

<font face="宋体" color=black size=4>		
&nbsp;&nbsp;4、根据中断源跳转至对应的中断服务，注意此时是跳转至了新程序的中断服务程序中。
</font>

<font face="宋体" color=black size=4>		
&nbsp;&nbsp;5、中断服务执行完毕后，返回main函数。
</font>

&ensp;

<font face="宋体" color=black size=4>		
&nbsp;&nbsp;关于新中断向量表的设置, 可以用void NVIC_SetVectorTable函数设置它在FLASH中的偏移量, 或者把它映射到RAM中。
</font>

&ensp;

# 三、IAP编程实现
<font face="宋体" color=black size=4>		
&nbsp;&nbsp;根据上面的原理只要编写boootloader和APP(用户实际运行的程序)程序, 再编写上位机, 定义自定义协议交互即可实现IAP技术的应用, 就可远程升级固件。
</font>

&ensp;

## 1.bootloader程序
![在这里插入图片描述](https://img-blog.csdnimg.cn/873c0ca9e98b4e92a394719d769e6999.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAY2hlbmd6aWR1bg==,size_20,color_FFFFFF,t_70,g_se,x_16/resize,p_60#pic_center)

<font face="宋体" color=black size=4>		
&nbsp;&nbsp;首先为bootloader程序分配ROM大小, 一般它的程序在几KB之内, 不宜过大, 否则会挤占APP程序的ROM, 具体需求而定。以及分配堆栈大小, 如下图:
</font>

![在这里插入图片描述](https://img-blog.csdnimg.cn/1ecbb67e43754c0f99e78bc668eff23f.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAY2hlbmd6aWR1bg==,size_19,color_FFFFFF,t_70,g_se,x_16/resize,p_60#pic_center)

&ensp;

```c
#define ApplicationAddress 0x08001C00 //APP Address
#define ApplicationRemainingSize 57 //应用程序剩余flash大小 ( K byte)

uint16_t DFU_state;
typedef void (*pFunction)(void);
pFunction Jump_To_Application;
uint32_t JumpAddress;
uint16_t modbus_slave_address = 0;

int main(void)
{
	DELAY_Init();
	uart1_init();
	RS485_RECV_EN();
	DFU_state = DFU_read_state();
	modbus_slave_address = modbus_slave_read_address();
	if(modbus_slave_address == 0xFFFF) 
	{
		modbus_slave_address = 1;
		Setting_modbus_slave_address();
	}
	if (DFU_state == 0xaaaa) 
	{
		__disable_irq();
		if (((*(__IO uint32_t *)ApplicationAddress) & 0x2FFE0000) == 0x20000000) 
		{
			// Jump to user application 
			JumpAddress = *(__IO uint32_t *)(ApplicationAddress + 4);
			Jump_To_Application = (pFunction)JumpAddress;	
			// Initialize user application's Stack Pointer 
			__set_MSP(*(__IO uint32_t *)ApplicationAddress);
			//Jump to APP
			Jump_To_Application();
		}
	}
	Tim3_UPCount_test(72 - 1, 100 - 1); // 100us定时  用于做UART空闲中断
	while (1)
	{
		DELAY_Ms(10);
		APP_Update();
	}
}
```

<font face="宋体" color=black size=4>		
&nbsp;&nbsp;上电复位程序从0x08000000取出栈顶地址, 再从0x08000000+4处获取复位中断向量地址, 进入复位服务程序, 结束后进行main函数。main函数中, 首先通过一个flag值判断是否有过烧录新程序, 若有关闭总中断, 必须关闭总中断否则会引起未知故障,。为什么0x08000000一定要先储存栈顶地址而后才有中断向量表, 因为一旦发生中断嵌套而没有栈会引发故障。之后, 判断APP的栈是否为0x20000000, 再从APP中断向量表中取出复位入口, 设置APP的栈顶地址，最后跳转的APP的程序。若无烧录程序则等待上位机的交互请求, APP_Update是自定义协议程序。
</font>

&ensp;

## 2.APP程序

![在这里插入图片描述](https://img-blog.csdnimg.cn/914f88eee4644ff79357fdb7304ea163.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAY2hlbmd6aWR1bg==,size_20,color_FFFFFF,t_70,g_se,x_16/resize,p_60#pic_center)

<font face="宋体" color=black size=4>		
&nbsp;&nbsp;APP的ROM程序起始地址要设置成跟bootloader程序ApplicationAddress的跳转地址一致, 大小要减去bootloader的ROM大小。RAM区设置成0x200000C0是为了中断向量表的映射。
</font>

```c
#define ApplicationAddress  0x08001C00          //APP addrress
#define VECTOR_SIZE  0xC0   //48*4=192 = 0xC0   中断向量表cope的大小
int main(void)
{
	memcpy((void*)0x20000000, (void*)ApplicationAddress, VECTOR_SIZE);  //把中断向量表映射到RAM里面跑
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	SYSCFG_MemoryRemapConfig(SYSCFG_MemoryRemap_SRAM);
	__enable_irq();
	while(1)
	{
		//user code
	}
}
```

<font face="宋体" color=black size=4>		
&nbsp;&nbsp;当bootloader程序跳到APP程序的mian函数后, main函数先把中断向量表映射到RAM, 即向量表要占RAM的192个字节, 所以上图中RAM的起始地址是0x200000C0，RAM大小自然是要减去0xC0, 而栈大小是0x400, 加上0x200000C0后栈顶地址就是0x200004C0, 注意栈是向下生长的。再打开总中断, 因为先前bootloader程序关闭了, 最后执行用户的程序。
</font>

<font face="宋体" color=black size=4>		
&nbsp;&nbsp;因为通过bootloader烧录程序只能是bin文件, 所以要把APP程序编译或转换成bin文件, 在MDK软件中可添加一条命令实现。如下图:
</font>

![在这里插入图片描述](https://img-blog.csdnimg.cn/609a3d87809744b882cc13847c457c23.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAY2hlbmd6aWR1bg==,size_20,color_FFFFFF,t_70,g_se,x_16/resize,p_50#pic_center)

&ensp;


# 四、上位机的编写
<font face="宋体" color=black size=4>		
&nbsp;&nbsp;需要把APP程序生成的bin文件烧录进MCU, 则通过上位机的串口与MCU的串口的连接, 再把bin文件数据根据与bootloader程序定义的协议传送给MCU, bootloader实现对MCU内部的FLASH擦写, 所以上位机主要的工作就是实现bin文件数据的传送。该上位机是在windows平台下实现, 采用微软的VS2017的IDE和C#语言开发。 
</font>

![在这里插入图片描述](https://img-blog.csdnimg.cn/8d1d4f3de83148c0a70336fd9946087b.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAY2hlbmd6aWR1bg==,size_20,color_FFFFFF,t_70,g_se,x_16/resize,p_80#pic_center)

<font face="宋体" color=black size=4>		
&nbsp;&nbsp;该上位机的工作主要就是实现与bootloader程序协议的编写及串口通信, 因此只要写一个通信协议类即可, 大概代码如下: 
</font>

```csharp
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO.Ports;
using System.Threading;
using xUtilities;
using System.IO;

namespace IAP_Console
{
    class BootloaderProtocol
    {
        private int retries;
        private int writetimeout;
        private int readtimeout;
        private SerialPort _serialPort;
        private static byte[] rbuffer = new byte[1024];
        private static int receive_size;
        private string bootversion;
        private ushort appprogramsize;  //KB
        private int update_state;   //0--没有更新,1--正在更新,2--停止更新，3--更新失败,4--更新完成

        private const uint APP_BASE_ADDRESS = 0x08001C00;       //APP基地址,以此地址进行更新程序
        private const int APP_WRITE_DATA_MAXSIZE_ONCE = 1024;    //单次发送允许最大字节
        private const int APP_WRITE_DATA_SIZE_ONCE = APP_WRITE_DATA_MAXSIZE_ONCE;        //单次发送字节, 不得超出允许最大字节

        public const int APP_FLASH_TOTAL_SIZE = 57;    //APP区的总共大小KB

        public const int UPDATE_STATE_NO = 0;
        public const int UPDATE_STATE_IN = 1;
        public const int UPDATE_STATE_STOP = 2;
        public const int UPDATE_STATE_FAIL = 3;
        public const int UPDATE_STATE_COMPLETE = 4;


        public int Retries
        {
            set { if (value == 0) retries = 1; retries = value; }
            get { return retries; }
        }

        public int WriteTimeout
        {
            set { writetimeout = value; }
            get { return writetimeout; }
        }

        public int ReadTimeout
        {
            set { readtimeout = value; }
            get { return readtimeout; }
        }

        public string BootVersion
        {
            get { return bootversion; }
        }

        public int UpdateState
        {
            set { update_state = value; }
            get { return update_state; }
        }

        public BootloaderProtocol(SerialPort serialport)
        {
            Retries = retries = 1;
            WriteTimeout = writetimeout = 100;
            ReadTimeout = readtimeout = 100;

            bootversion = "";
            appprogramsize = 0;

            update_state = UPDATE_STATE_NO;

            _serialPort = serialport;
            _serialPort.WriteBufferSize = 4096;
            _serialPort.ReadBufferSize = 4096;
            _serialPort.ReceivedBytesThreshold = 1;

            _serialPort.WriteTimeout = WriteTimeout;
            _serialPort.ReadTimeout = ReadTimeout;

            receive_size = 0;
            _serialPort.DataReceived += new SerialDataReceivedEventHandler(SerialPort_DataReceivedEventHandler);
        }

        private void Send_data(byte[] buffer, int size)
        {
            try
            {
                _serialPort.WriteTimeout = WriteTimeout;
                receive_size = 0;
                _serialPort.Write(buffer, 0, size);
            }
            catch (TimeoutException)
            {
                throw new TimeoutException("send timeout");
            }
            catch
            {
                throw;
            }
        }

        private byte[] Receive_data()
        {
            int timeoutcnt = 0;

            while (receive_size > 0 || timeoutcnt < ReadTimeout)
            {
                if (receive_size > 0)
                {
                    byte[] data = new byte[receive_size];
                    for (int i = 0; i < receive_size; i++)
                    {
                        data[i] = rbuffer[i];
                    }
                    receive_size = 0;
                    return data;
                }
                Thread.Sleep(10);
                timeoutcnt += 10;
                if (timeoutcnt > ReadTimeout)
                {
                    receive_size = 0;
                    throw new TimeoutException("receive timeout");
                }
            }

            return null;
        }

        private ushort Check_sum(byte[] data, ushort size)
        {
            int sum = 0;
            ushort i = 0;

            while (size > 0)
            {
                sum = sum + data[i];
                i++;
                size--;
            }

            while ((sum >> 16) > 0) //当和的高16位不为0，把高16位作为校验和的一部分求和，
                sum = (sum & 0xffff) + (sum >> 16);

            return (ushort)~sum;
        }

        private bool Check_data(byte[] data, ushort size)
        {
            bool flag_check = false;
            ushort tmp_checksum = 0;
            byte temp1, temp2;

            tmp_checksum = Check_sum(data, (ushort)(data[1] - 2));
            temp1 = (byte)(tmp_checksum & 0x00FF);
            temp2 = (byte)((tmp_checksum >> 8) & 0x00FF);

            if ((temp2 != data[data[1] - 2]) || (temp1 != data[data[1] - 1]))
            {
                flag_check = true;
            }

            return flag_check;
        }

        //查看BOOT版本：01 00 09 52 45 56 45 fe c3
        public string Get_boot_version(byte mod_address)
        {
            byte[] cmd = new byte[] { 0x01, 0x00, 0x09, 0x52, 0x45, 0x56, 0x45, 0xFE, 0xC3 };
            int retries_cnt = 0;

            bootversion = "";
            while (retries_cnt < Retries)
            {
                try
                {
                    cmd[0] = mod_address;
                    ushort checksum = Check_sum(cmd, (ushort)(cmd.Length - 2));
                    cmd[cmd.Length - 2] = (byte)((checksum >> 8) & 0x00FF);
                    cmd[cmd.Length - 1] = (byte)(checksum  & 0x00FF);

                    Send_data(cmd, cmd.Length);
                    byte[] data = Receive_data();
                    if (data != null)
                    {
                        if ((data.Length == cmd[2]) && (data[0] == cmd[0]) && (data[1] == (cmd[1]| 0xC0)) && (data[2] == cmd[2]))
                        {
                            byte[] version = new byte[4];
                            for (int i = 0; i < 4; i++)
                            {
                                version[i] = data[i + 3];
                            }
                            bootversion = "Ver" + ((version[0] >> 4) & 0x0F).ToString() +  (version[0] & 0x0F).ToString() + "." +
                                        ((version[1] >> 4) & 0x0F).ToString() + (version[1] & 0x0F).ToString();
                            appprogramsize = version[2];
                            return bootversion;
                        }
                    }
                    retries_cnt++;
                    if (retries_cnt >= Retries)
                        throw new Exception("retry the overflow");          
                }
                catch (TimeoutException)
                {
                    if (retries_cnt >= Retries)
                        throw new TimeoutException("request timeout");
                }
                catch
                {
                    if (retries_cnt >= Retries)
                        throw new Exception("retry the overflow");
                }
            }

            return bootversion;
        }

        //擦除BOOT标记并且重启(0xffff)：01 20 0E 42 4F 4F 54 52 45 53 45 54 FD 19 
        public Boolean Erase_boot_flag_and_reset(byte mod_address)
        {
            byte[] cmd = new byte[] { 0x01, 0x20, 0x0E, 0x42, 0x4F, 0x4F, 0x54, 0x52, 0x45, 0x53, 0x45, 0x54, 0xFD, 0x19 };
            int retries_cnt = 0;

            while (retries_cnt < Retries)
            {
                try
                {
                    cmd[0] = mod_address;
                    ushort checksum = Check_sum(cmd, (ushort)(cmd.Length - 2));
                    cmd[cmd.Length - 2] = (byte)((checksum >> 8) & 0x00FF);
                    cmd[cmd.Length - 1] = (byte)(checksum & 0x00FF);

                    Send_data(cmd, cmd.Length);
                    byte[] data = Receive_data();
                    if (data != null)
                    {
                        if ((data.Length == cmd[2]) && (data[0] == cmd[0]) && (data[1] == (cmd[1] | 0xC0)) && (data[2] == cmd[2]))
                        {
                            return true;
                        }
                    }
                    retries_cnt++;
                    if (retries_cnt >= Retries)
                        throw new Exception("retry the overflow");
                }
                catch (TimeoutException)
                {
                    if (retries_cnt >= Retries)
                        throw new TimeoutException("request timeout");
                }
                catch
                {
                    if (retries_cnt >= Retries)
                        throw new Exception("retry the overflow");
                }
            }

            return false;
        }

        //擦除APP：01 21 0C 05 44 45 4C 41 50 50 FE 16  
        public Boolean Erase_app_and_boot_flag(byte mod_address, ushort erase_kb_size)
        {
            if((erase_kb_size == 0) || (erase_kb_size > appprogramsize)) throw new Exception("illegal parameters");

            byte[] cmd = new byte[] { 0x01, 0x21, 0x0C, 0x05, 0x44, 0x45, 0x4C, 0x41, 0x50, 0x50, 0xFE, 0x16 };
            int retries_cnt = 0;

            while (retries_cnt < Retries)
            {
                try
                {
                    cmd[0] = mod_address;
                    cmd[3] = (byte)erase_kb_size;
                    ushort checksum = Check_sum(cmd, (ushort)(cmd.Length - 2));             
                    cmd[cmd.Length - 2] = (byte)((checksum >> 8) & 0x00FF);
                    cmd[cmd.Length - 1] = (byte)(checksum & 0x00FF);

                    Send_data(cmd, cmd.Length);
                    byte[] data = Receive_data();
                    if (data != null)
                    {
                        if ((data.Length == cmd[2]) && (data[0] == cmd[0]) && (data[1] == (cmd[1] | 0xC0)) && (data[2] == cmd[2]))
                        {
                            return true;
                        }
                    }
                    retries_cnt++;
                    if (retries_cnt >= Retries)
                        throw new Exception("retry the overflow");
                }
                catch (TimeoutException)
                {
                    if (retries_cnt >= Retries)
                        throw new TimeoutException("request timeout");
                }
                catch
                {
                    if (retries_cnt >= Retries)
                        throw new Exception("retry the overflow");
                }
            }

            return false;
        }

        //修改BOOT标记（AAAA重启）01 22 0B 52 45 42 4F 4F 54 FE 06
        public Boolean Set_boot_flag_and_reset(byte mod_address)
        {
            byte[] cmd = new byte[] { 0x01, 0x22, 0x0B, 0x52, 0x45, 0x42, 0x4F, 0x4F, 0x54, 0xFE, 0x06 };
            int retries_cnt = 0;

            while (retries_cnt < Retries)
            {
                try
                {
                    cmd[0] = mod_address;
                    ushort checksum = Check_sum(cmd, (ushort)(cmd.Length - 2));
                    cmd[cmd.Length - 2] = (byte)((checksum >> 8) & 0x00FF);
                    cmd[cmd.Length - 1] = (byte)(checksum & 0x00FF);

                    Send_data(cmd, cmd.Length);
                    byte[] data = Receive_data();
                    if (data != null)
                    {
                        if ((data.Length == cmd[2]) && (data[0] == cmd[0]) && (data[1] == (cmd[1] | 0xC0)) && (data[2] == cmd[2]))
                        {
                            return true;
                        }
                    }
                    retries_cnt++;
                    if (retries_cnt >= Retries)
                        throw new Exception("retry the overflow");
                }
                catch (TimeoutException)
                {
                    if (retries_cnt >= Retries)
                        throw new TimeoutException("request timeout");
                }
                catch
                {
                    if (retries_cnt >= Retries)
                        throw new Exception("retry the overflow");
                }
            }

            return false;
        }

        //写数据到APP的扇区 01 23 48 (00 1C 00 08) (xxxx xxxx) (SUM SUM) 
        public Boolean Write_app_data(byte mod_address, byte[] wdata, uint address, ushort size)
        {
            if ((size == 0) || (size > APP_WRITE_DATA_MAXSIZE_ONCE) || ((size % 2) != 0)) throw new Exception("illegal parameters");

            byte[] cmd = new byte[size + 10];
            int retries_cnt = 0;

            cmd[1] = 0x23;
            cmd[2] = (byte)((size + 10) & 0x00FF);
            cmd[3] = (byte)(((size + 10) >> 8) & 0x00FF);
            cmd[4] = (byte)(address & 0x000000FF);
            cmd[5] = (byte)((address >> 8) & 0x000000FF);
            cmd[6] = (byte)((address >> 16) & 0x000000FF);
            cmd[7] = (byte)((address >> 24) & 0x000000FF);

            for (int i = 0; i < size; i++)
            {
                cmd[8 + i] = wdata[i];
            }

            while (retries_cnt < Retries)
            {
                try
                {
                    cmd[0] = mod_address;
                    ushort checksum = Check_sum(cmd, (ushort)(cmd.Length - 2));
                    cmd[cmd.Length - 2] = (byte)((checksum >> 8) & 0x00FF);
                    cmd[cmd.Length - 1] = (byte)(checksum & 0x00FF);

                    Send_data(cmd, cmd.Length);
                    byte[] data = Receive_data();
                    if (data != null)
                    {
                        if ((data.Length == 10) && (data[0] == cmd[0]) && (data[1] == (cmd[1] | 0xC0)) && (data[2] == 10))
                        {
                                return true;
                        }
                    }
                    retries_cnt++;
                    if (retries_cnt >= Retries)
                        throw new Exception("retry the overflow");
                }
                catch (TimeoutException)
                {
                    if (retries_cnt >= Retries)
                        throw new TimeoutException("request timeout");
                }
                catch
                {
                    if (retries_cnt >= Retries)
                        throw new Exception("retry the overflow");
                }
            }

            return false;
        }

        //修改BOOT标记（AAAA不重启）01 25 0B 42 4F 4F 54 53 45 FE 02 
        public Boolean Set_boot_flag(byte mod_address)
        {
            byte[] cmd = new byte[] { 0x01, 0x25, 0x0B, 0x42, 0x4F, 0x4F, 0x54, 0x53, 0x45, 0xFE, 0x02 };
            int retries_cnt = 0;

            while (retries_cnt < Retries)
            {
                try
                {
                    cmd[0] = mod_address;
                    ushort checksum = Check_sum(cmd, (ushort)(cmd.Length - 2));
                    cmd[cmd.Length - 2] = (byte)((checksum >> 8) & 0x00FF);
                    cmd[cmd.Length - 1] = (byte)(checksum & 0x00FF);

                    Send_data(cmd, cmd.Length);
                    byte[] data = Receive_data();
                    if (data != null)
                    {
                        if ((data.Length == cmd[2]) && (data[0] == cmd[0]) && (data[1] == (cmd[1] | 0xC0)) && (data[2] == cmd[2]))
                        {
                            return true;
                        }
                    }
                    retries_cnt++;
                    if (retries_cnt >= Retries)
                        throw new Exception("retry the overflow");
                }
                catch (TimeoutException)
                {
                    if (retries_cnt >= Retries)
                        throw new TimeoutException("request timeout");
                }
                catch
                {
                    if (retries_cnt >= Retries)
                        throw new Exception("retry the overflow");
                }
            }

            return false;
        }

        //bootloader初始化
        public Boolean Bootloader_init(byte mod_address)
        {
            //擦除标志复位后切换到boot模式(因为程序平时运行在APP模式下)
            UpdateState = UPDATE_STATE_NO;
            //获取boot版本
            try
            {
                if (Erase_boot_flag_and_reset(mod_address) == false)
                {
                    throw new Exception("Erase_boot_flag_and_reset fail");
                }
                Thread.Sleep(500);  //软复位MCU后等待它切换到BOOT程序

                if (Get_boot_version(mod_address).Equals("") == true)
                {
                    throw new Exception("Get_boot_version fail");
                }

                return true;
            }
            catch (TimeoutException ex)
            {
                UpdateState = UPDATE_STATE_FAIL;
                throw new TimeoutException("bootloader int timeout");
            }
            catch (Exception ex)
            {
                UpdateState = UPDATE_STATE_FAIL;
                throw new Exception("bootloader int fail");
            }
        }

        //bootloader更新APP文件
        public Boolean Bootloader_app_file_update(byte mod_address, FileStream fs)
        {
            try
            {
                if (fs.Length == 0)
                {
                    fs.Close();
                    throw new Exception("The file is empty");
                }

                ushort program_size_kb = (ushort)(fs.Length / 1024);
                if ((fs.Length % 1024) != 0) program_size_kb++;

                UpdateState = UPDATE_STATE_IN;
                if (Erase_app_and_boot_flag(mod_address, program_size_kb) == false)
                {
                    fs.Close();
                    throw new Exception("Erase_app_and_boot_flag fail");
                }
                Thread.Sleep(10);

                int package_size = BootloaderProtocol.APP_WRITE_DATA_SIZE_ONCE;
                int package_total = (ushort)(fs.Length / package_size);
                if ((fs.Length % package_size) != 0) package_total++;          
                uint address = BootloaderProtocol.APP_BASE_ADDRESS;
                byte[] data = new byte[package_size];

                SendMessage_UpdataProgressBar(package_total, 0);
                Thread.Sleep(10);

                for (int i = 0; i < package_total; i++)
                {
                    package_size = fs.Read(data, 0, (int)(package_size));

                    if (Write_app_data(mod_address, data, address, (ushort)package_size) == false)
                    {
                        fs.Close();
                        throw new Exception("Failed to write the app data");
                    }
                    address = (uint)(address + package_size);
                    if (UpdateState == UPDATE_STATE_STOP)
                    {
                        fs.Close();
                        SendMessage_UpdataProgressBar(package_total, 0);
                        throw new Exception("Stop updating the firmware");
                    }

                    SendMessage_UpdataProgressBar(package_total, i);
                }
                fs.Close();

                SendMessage_UpdataProgressBar(package_total, package_total);
                Thread.Sleep(20);

                if (Set_boot_flag_and_reset(mod_address) == false)
                {
                    throw new Exception("Failed to Set_boot_flag_and_reset");
                }
                UpdateState = UPDATE_STATE_COMPLETE;
            }
            catch (TimeoutException ex)
            {
                fs.Close();
                UpdateState = UPDATE_STATE_FAIL;
                throw new TimeoutException(ex.Message);
            }
            catch (Exception ex)
            {
                fs.Close();
                UpdateState = UPDATE_STATE_FAIL;
                throw new Exception("Failed to update the file");
            }

            return true;
        }
        private void SendMessage_UpdataProgressBar(int total, int current)
        {
            xUtilities.Message msg = new xUtilities.Message();
            msg.what = Appconstant.MSG_UPDATE_PROGRESS;
            msg.obj = (object)new int[] { total, current };
            MainForm.g_handler.SendMessage(msg);
        }

        private void SerialPort_DataReceivedEventHandler(object sender, SerialDataReceivedEventArgs e)
        {
            try
            {
                if (_serialPort.IsOpen)
                {
                    try
                    {
                        if (_serialPort.BytesToRead == 0)
                        {
                            return;
                        }

                        int byteNumber = 0;

                        //延时等待数据接收完毕。
                        while ((byteNumber < _serialPort.BytesToRead) && (_serialPort.BytesToRead < 4096))
                        {
                            byteNumber = _serialPort.BytesToRead;
                            Thread.Sleep(20);
                        }

                        _serialPort.Read(rbuffer, 0, byteNumber);
                        _serialPort.DiscardInBuffer();
                        receive_size = byteNumber;
                    }
                    catch (Exception ex)
                    {

                    }
                }
                else
                {
                    TimeSpan waitTime = new TimeSpan(0, 0, 0, 0, 50);
                    Thread.Sleep(waitTime);
                }
                Thread.Sleep(200);
            }
            catch (Exception ex)
            {

            }
        }
    }
}

```

<font face="宋体" color=black size=4>		
&nbsp;&nbsp;在BootloaderProtocol类的构造函数注册串口接收事件函数SerialPort_DataReceivedEventHandler, 主要是完成一帧数据接收。根据每条命令编写一个函数, 最后调用Bootloader_app_file_update便可实现整个协议交互序列， 只要提供RS485地址和bin文件名即可。烧录速度和稳定性跟波特率, 单次数据量和实际环境线路有关, 经过烧录一个将近20KB的bin文件测试, 大概需要4, 5秒, 还算可以。
</font>

&ensp;

# 五、总结
<font face="宋体" color=black size=4>
&nbsp;&nbsp;可见, IAP技术的应用很方便解决了远程固件升级问题, 有效节省的不必要的人力成本。当然我们不会局限于只用串口升级, 在万物互联的时代, 电子产品连网成了必要的功能, 采用网络升级更加发挥IAP技术的应用, 真可谓是把利剑。该文章IAP技术原理等部份内容摘录网友的, 如有侵权请告知。
</font>

&ensp;

<hr style=" border:solid; width:100px; height:1px;" color=#000000 size=4">

> <font face="宋体" color=red size=4>**免责申明: 作者难免技术水平有限，如有错误拒不承认，本文技术资料只适于学习和参考，不可商用，若要商用，不必告知我，跟我没一毛关系。**</font>
