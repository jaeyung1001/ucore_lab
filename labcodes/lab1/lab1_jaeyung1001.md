#Lab1：系统软件启动过程
###学号：2012080061 班级：计23 姓名：李载隆 

# 练习 1：

## ucore.img 是如何一步一步生成的？
 
 ![result_practice_1](http://postfiles4.naver.net/20150319_83/jaeyung1001_142670504351038Hf1_PNG/Selection_001.png?type=w2)
 ![result_practice_2](http://postfiles10.naver.net/20150319_137/jaeyung1001_1426705043759anQy4_PNG/ucore_img.png?type=w2)
 
>gcc -Iboot/ -fno-builtin -Wall -ggdb -m32 -nostdinc -fno-stack-protector -Ilibs/ -Os -nostdinc -c boot/bootasm.S -o obj/boot/bootasm.o
 (编译 bootasm.S，生成 bootasm.o) \n

>gcc -Iboot/ -fno-builtin -Wall -ggdb -m32 -nostdinc -fno-stack-protector -Ilibs/ -Os -nostdinc -c boot/bootmain.c -o obj/boot/bootmain.o
 (编译 bootmain.c，生成 bootmain.o)

>gcc -Itools/ -g -Wall -O2 -c tools/sign.c -o obj/sign/tools/sign.o
 (编译 sign.c，生成 sign.o)

>gcc -g -Wall -O2 obj/sign/tools/sign.o -o bin/sign
 (编译 sign.o，生成 bin/sign)

>ld -m    elf_i386 -N -e start -Ttext 0x7C00 obj/boot/bootasm.o obj/boot/bootmain.o -o obj/bootblock.o
 (链接 bin/bootblock)

>dd if=/dev/zero of=bin/ucore.img count=10000
 (将空白写入 ucore.img)

>dd if=bin/bootblock of=bin/ucore.img conv=notrunc
 (将 bootblock 写入 ucore.img).
 
>-I<dir>		指定搜索系统头文件的目录，可以重复使用多个该选项指定多个目录

>-Wall		显示所有的警告消息

>-O2		优化（级别为2）

>-m32		指明目标代码32位

>-Os		对生成的二进制代码进行尺寸上的优化

>-ggdb		提供编译信息

>-nostdinc	只为头文件寻找-I选项指定的目录

>-fno-builtin	除非利用"__builtin_"进行引用，否则不识别所有内建函数

>-fno-stack-protector	不检测缓存溢出

>-c		完成编译或汇编工作，但是不链接，以目标文件（.o）形式输出

>-e		设置程序入口位置

>-Ttext	设置函数地址

>-m elf_i386	设定为i386平台ELF执行文件格式

>-S	源代码中混合有反汇编 (objdump)

>-S	去除掉源文件的符号信息和重分配信息 (objcopy)

>-O binary	生成二进制（binary）格式的输出文件

>if=file		输入文件名，缺省为标准输入

>of=file		输出文件名，缺省为标准输出

>count=blocks	仅拷贝 blocks 个块，块大小等于 ibs 指定的字节数

>conv=conversion[,conversion...]		用指定的参数转换文件

>notrunc	不截短输出文件

## 2. 一个被系统认为是符合规范的硬盘主引导扇区的特征是什么？

> 大小为512B，最后两个字节为0x55AA3

# 练习 2：
1. 屏幕显示：

![result_practice_3](http://postfiles9.naver.net/20150319_152/jaeyung1001_1426705043253kcrjr_PNG/practice2.png?type=w2)

2. 
> 敲入 break *0x7c00，定义断点，敲入 c，执行到指令 cli，断点触发。
3．屏幕显示：

![result_practice_4](http://blogfiles.naver.net/20150319_68/jaeyung1001_1426705320963sQERN_JPEG/num3.jpg)

4.
> 敲入 b *0x7c0e，再敲入 c，则执行到断点处停止，成功。

# 练习 3：
使用GDT，从实模式转换为保护模式
	
	'lgdt gdtdesc
	 movl %cr0, %eax
	 orl $CR0_PE_ON, %eax
	 movl %eax, %cr0
	 ljmp $PROT_MODE_CSEG, $protcseg'

# 练习 4：分析 bootloader 加载 ELF 格式的 OS

## 1．bootloader 如何读取硬盘扇区的？
>答：调用 readseg 函数，具体如下：
（1）旋转到扇区边界
（2）确定要读的扇区数
（3）读取相应的扇区数
（4）对每个扇区，先等待磁盘准备完毕，再设置读取信息，再等待磁盘准备完毕，即可开始读取.

## 2．bootloader 是如何加载 ELF 格式的 OS？
>答：在 main 函数中加载，具体如下：
（1）把磁盘的第一页加载到 ELFHDR 中
（2）判断是否为有效的 ELFHDR
（3）把 ELFHDR 中的每个程序段从磁盘加载到内存
（4）把 ELFHDR 的入口点地址转换为函数指针进行调用

# 练习 5：

![result_practice_7](http://postfiles11.naver.net/20150330_42/jaeyung1001_14277004623416ouSc_JPEG/687474703a2f2f626c6f6766696c65732e6e617665722e6e65742f32303135303331395f3130322f.jpg?type=w2)
实现过程是迭代寻找上一个 EBP，并根据当前 EBP 的值确定 EIP 和参数的值。
各个数值分别是 EBP、EIP，以及 4 个参数的值，调用函数的行号，当时调用的指令与现在指令的条数之差.

# 练习 6：
1．
>8个字节；0~15位和48~63位.

2.

![result_practice_8](http://postfiles8.naver.net/20150322_7/jaeyung1001_1427024294577BNf2G_PNG/prac6_2.png?type=w2)
![result_practice_9](http://postfiles5.naver.net/20150322_244/jaeyung1001_1427024294110SOrXa_PNG/prac6_1.png?type=w2)
>实现过程：对外部数组 __vector 赋值即可。注意系统调用要用GD_UTEXT 和 DPL_USER。最后要设置 IDTR 寄存器.

3.
>添加case IRQ_OFFSET + IRQ_TIMER 事件，累积 ticks 并触发调用 print_ticks() 即可.
 
