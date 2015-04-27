##【练习1】##
实验思路：
	按照要求将proc的state/pid/runs/kstack/need_resched/parent/mm/context/tf/cr3/flags/name的字段设置为初始值（绝大多数都是将值置为0）。
	
请说明proc_struct中 struct context context 和 struct trapframe *tf 成员变量含义和在本实验中的作用是啥？（提示通过看代码和编程调试可以判断出来）
```
答：context保存了进程切换时proc的上下文，tf保存了中断信息和ds，es，fs，gs等信息
```
##【练习2】##
实验思路：
	1、调用alloc_proc（）函数分配proc的空间，若分配失败则跳至失败退出
	2、设置父进程id
	3、调用setup_kstack（）函数分配栈空间
	4、调用copy_mm（）函数复制内存控制信息
	5、调用copy_thread（）函数复制上下文
	6、将新进程添加到进程列表
	7、调用wakeup_proc（）函数唤醒进程并返回进程id
请说明ucore是否做到给每个新fork的线程一个唯一的id？请说明你的分析和理由。
```
答：在进程中是唯一的，但在整个操作系统运行过程中不是，而是循环使用的，在get_pid（）函数中多处发现
	if(last_pid >= MAX_PID)
	{
		last_pid = 1;
	}
	就是这样的情况
```
##【练习3】##
在本实验的执行过程中，创建且运行了几个内核线程？
```
答：2个。idleproc和init_main
```
语句 local_intr_save(intr_flag);....local_intr_restore(intr_flag); 在这里有何作用?请说明理由
```
答：在这个区域内关闭中断防止硬件中断对程序出现干扰
```

## 实验过程图

![lab4](http://postfiles6.naver.net/20150421_101/jaeyung1001_1429620567443M2jIj_PNG/make_V.png?type=w2)
![lab4](http://postfiles15.naver.net/20150421_30/jaeyung1001_142961989882235kYO_PNG/lab4_output.png?type=w2)
![lab4](http://postfiles3.naver.net/20150421_66/jaeyung1001_1429619899456cJkKv_PNG/lab4_output2.png?type=w2)
![lab4](http://postfiles3.naver.net/20150421_130/jaeyung1001_1429619899931WKO5s_PNG/lab4_output3.png?type=w2)
