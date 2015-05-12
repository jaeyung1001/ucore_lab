#lab2 report
##2012080061 计23 李载隆

##练习1：实现 first-fit 连续物理内存分配算法（需要编程）

default_init: 这个函数你可以把free_list初始化并能够nr_free设0

default_init_memmap: 通过eclipse结构，可以看出运行过程是kern_init-> pmm_init-> page_init->init_memmap-> pmm_manager-> init_memmap. 这个函数主要做的是：
1） 可以把一个空间初始化 页码，地址等.
2） 首先你应该把空间力各个页初始化, 包括: p->flags 应该设成PG_property, PG_reserved开始在                                           	p->flags中.
	如果这页是空的但不是第一页空间时, p->property应设为0.
	如果这页是空的且是第一页的空间时, p->property应设为总各数之和的数值.
3)  p->ref应设为0, 因为现在p是空的且不是参照.
4) 我们可以使用p->page_link链接把当页到free_list. For example: list_add_before(&free_list,&(p-.page_link))
5) 最后, 我们应该把free空间总和起来, nr_free+=n
 
![default_init_memmap](http://postfiles5.naver.net/20150329_212/jaeyung1001_1427624109150WAXG0_PNG/default_init_memmap.png?type=w2)
 
default_alloc_pages: 在free_list中查找第一个空格兵把重设free_block, 返回地址
	查找free_list是用 list_entry_t le = &free_list;
					 while((le=list_next(le))!=&free_list)
	在while循环中, 取得相应页, 检查是否 p->property>=n.
	如果找的满足的p, 说明我们找到了一个空格, 并且初页获得memory.
	一些因数在此页设成: PG_reserved=1, PG_property=0.
	如果p->property>n, 我们应该重新算出剩下的空格和 nr_free. 算法是 le2page(le,page_link))->property = p->property – n;) re-caluclate nr_free (number of the the rest of all free block) return p
 	如果找不到空格, 返回NULL

![default_alloc_pages](http://postfiles8.naver.net/20150329_167/jaeyung1001_1427624108286cRgoO_PNG/default_alloc_pages.png?type=w2)

default_free_pages: 把页重新连接到空格, 小的空格放到更大的空格处
	根据已经排除的基地址, 寻找free_list, 找到正确的位置, 从低到高地址, 然后插入页. 使用list_next, le2page, list_add_before.
	重设页面, 比如p->ref, p->flags(PageProperty)
	合并低到高地址格, 需要把一些页的p->property改正

![default_free_pages1](http://postfiles14.naver.net/20150329_205/jaeyung1001_1427624108571oMqjj_PNG/default_free_pages1.png?type=w2)
![default_free_pages2](http://postfiles14.naver.net/20150329_237/jaeyung1001_1427624108847FMUl8_PNG/default_free_pages2.png?type=w2)
 
##练习2：实现寻找虚拟地址对应的页表项（需要编程）

```
通过设置页表和对应的页表项，可建立虚拟内存地址和物理内存地址的对应关系。其中的get_pte函数是设置页表项环节中的一个重要步骤。此
函数找 到一个虚地址对应的二级页表项的内核虚地址，如果此二级页表项不存在，则分配一个包含此项的二级页表。本练习需要补全get_pte
函数 in kern/mm/pmm.c，实现其功能。请仔细查看和理解get_pte函数中的注释。PDX(la) = 虚拟地址初步入口.
```

KADDR(pa) : 物理地址换成虚拟地址的函数.

set_page_ref(page,1) : 参照此页一次.

page2pa(page):取得物理地址的容量.

struct Page * alloc_page() : 存储页面.

memset(void *s, char c, size_t n) : 把指向s的第一个N bytes空间到特定的值c
pte_get函数结构图:
![pte_get](https://github.com/jaeyung1001/ucore_docs/blob/master/lab2_figs/image001.png?raw=true)

###1)请描述页目录项（Pag Director Entry）和页表（Page Table Entry）中每个组成部分的含义和以及对ucore而言的潜在用处.
 
> 1.用于指明表项对地址转换是否有效

> 2.可以使页面可以被读,写或执行.

> 3.可以把线性地址和物理地址转换.

> 4.PTE存线性地址.

###2)如果ucore执行过程中访问内存，出现了页访问异常，请问硬件要做哪些事情？

> 首先要找到对应着那一页在外存当中存在什么地方, 找到之后此页读到内存里, 存在空的物理页面. 然后用页面置换法来找不常用的页面替换出去. 然后检查是否修改过, 如果
修改过那么不能直接扔掉的, 处理之前写到外存中. 然后重新执行产生访问异常的一条指令.
 
###练习3：释放某虚地址所在的页并取消对应二级页表项的映射（需要编程）

```
当释放一个包含某虚地址的物理内存页时,需要让对应此物理内存页的管理数据结构Page做相关的清除处理,使得此物理内存页成为空闲;
另外还需把表 示虚地址与物理地址对应关系的二级页表项清除.请仔细查看和理解page_remove_pte函数中的注释.
为此,需要补全在 kern/mm
/pmm.c中的page_remove_pte函数。page_remove_pte函数的调用关系图如下所示：
```

![practice_3](https://github.com/jaeyung1001/ucore_docs/blob/master/lab2_figs/image002.png?raw=true)

请在实验报告中简要说明你的设计实现过程。请回答如下问题：
•	数据结构Page的全局变量（其实是一个数组）的每一项与页表中的页目录项和页表项有无对应关系？如果有，其对应关系是啥？

#实验结果

执行 make qemu:

 ![result](http://postfiles11.naver.net/20150329_170/jaeyung1001_1427628618225jeSBi_PNG/lab2_make_qemu.png?type=w2)
 
执行 make V=:
```
转到保护状态过程
```
 ![protector](http://postfiles4.naver.net/20150329_275/jaeyung1001_1427629194764isIXv_PNG/protector.png?type=w2)
