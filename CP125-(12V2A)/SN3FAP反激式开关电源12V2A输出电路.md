# SN3FAP反激式开关电源12V2A输出电路
<hr style=" border:solid; width:100px; height:1px;" color=#000000 size=1">

<font color=#999AAA >

@[TOC](文章目录)

</font>

<hr style=" border:solid; width:100px; height:1px;" color=#000000 size=1">

> # 前言
> <font color=#999AAA >前文<<智能家居无线组网遥控电子智能锁>>的CZ201C缺少一个12V/1A适配器，因此制作了一个12V/2A适配器，本文简单介绍下该适配器。
> </font>

<hr style=" border:solid; width:100px; height:1px;" color=#000000 size=1">

<font color=#999AAA >

# 一、概述
## 1.电路图
<font face="宋体" color=black size=4> 
&nbsp;&nbsp;下图是SN3FAP反激式开关电源12V2A输出电路
</font>

![在这里插入图片描述](https://img-blog.csdnimg.cn/ef54fc5872ad4d43be8aba4dd5dcb7b7.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_Q1NETiBAY2hlbmd6aWR1bg==,size_42,color_FFFFFF,t_70,g_se,x_16/resize,p_80,#pic_center)


## 2.原理简述
<font face="宋体" color=black size=4> 
&nbsp;&nbsp;市电从P1端子输入，经过保险丝和NTC电阻后进行全波整流，整流输出后进行滤波，以SN3FAP为核心的开关电源IC的开关频率工作在65KHz。SN3FAP以PWM方式驱动变压器T1初级产生震荡信号，耦合到变压器T1输出电压电流。通过可调稳压管U3和光耦U2反馈输出电压给SN3FAP的PIN3，根据反馈电压的大小来调节PWM的占空比大小，以此来稳定输出电压。
</font>

<font face="宋体" color=black size=4> 
&nbsp;&nbsp;通过改变R13和R14来达到预定输出电压, 根据可调稳压管TL431的公式可得出, Vout = (R13 / R14 + 1) * Vref, Vref固定在2.5V, 因此本电路的输出电压等于: Vout = (18K / 4.7K + 1) * 2.5V ≈ 12V
</font>

&ensp;

## 3.PCB图
### (1).布局
![在这里插入图片描述](https://img-blog.csdnimg.cn/98efa2966c5746c2a1c5f755ba98dd51.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_Q1NETiBAY2hlbmd6aWR1bg==,size_46,color_FFFFFF,t_70,g_se,x_16/resize,p_50,#pic_center)

&ensp;

### (2).印制板图
![在这里插入图片描述](https://img-blog.csdnimg.cn/e71a4005d82c425eb7bc29f05a4ec644.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_Q1NETiBAY2hlbmd6aWR1bg==,size_45,color_FFFFFF,t_70,g_se,x_16/resize,p_50,#pic_center)

&ensp;

# 二、技术参数要求
## 1.输入电源: 100~240VAC/50Hz
## 2.输出电源: 12VDC/2A

&ensp;

# 三、总结
<font face="宋体" color=black size=5>
&nbsp;&nbsp;该电路原理简单，电路简单且成本低，在实际使用中输出功率要裕量至少20%。
</font>

&ensp;

# 四、下载地址


&ensp;

<hr style=" border:solid; width:100px; height:1px;" color=#000000 size=1">

> <font face="宋体" color=red size=4>**免责申明: 作者难免技术水平有限，如有错误拒不承认，本文技术资料只适于学习和参考，不可商用，若要商用，不必告知我，跟我没一毛关系。**</font>


