

> 前面框架部分转载修改优秀的参考文章：https://www.cnblogs.com/thoupin/p/4018455.html

## 数据类型

- 所有MIPS指令都是32位长的
- 各单位：1字节=8位，半字长=2个字节，1字长=4个字节
- 一个字符空间=1个字节
- 一个整型=一个字长=4个字节
- 单个字符用单引号，例如：'b'
- 字符串用双引号，例如："A string"



### 寄存器

- MIPS下一共有32个通用寄存器
- 在汇编中，寄存器标志由$符开头
- 寄存器表示可以有两种方式
- 直接使用该寄存器对应的编号，例如：从$0到$31
使用对应的寄存器名称，例如：\$t1, $sp(详细含义，下文有表格
- 对于乘法和除法分别有对应的两个寄存器\$lo, \$hi
  对于以上二者，不存在直接寻址；必须要通过mfhi("move from hi")以及mflo("move from lo")分别来进行访问对应的内容
- 栈的走向是从高地址到低地址

| *Register* *Number*寄存器编号 | *Alternative*  *Name*寄存器名 | *Description*寄存器用途                                      |
| ----------------------------- | ----------------------------- | ------------------------------------------------------------ |
| *0*                           | *zero*                        | *the value 0*永远返回零                                      |
| *1*                           | *$at*                         | *(**a**ssembler **t**emporary) reserved by the assembler*汇编保留寄存器（不可做其他用途） |
| *2-3*                         | *\$v0 - ​\$v1*                 | *(**v**alues) from expression evaluation and function results*（**V**alue简写）存储表达式或者是函数的返回值 |
| *4-7*                         | *\$a0 - ​\$a3*                 | *(**a**rguments) First four parameters for subroutine.* *Not preserved across procedure calls*（**A**rgument简写）存储子程序的前4个参数，在子程序调用过程中释放 |
| *8-15*                        | *\$t0 - ​\$t7*                 | *(**t**emporaries) Caller saved if needed. Subroutines can use w/out saving.* *Not preserved across procedure calls*（**T**emp简写）临时变量，同上调用时不保存 |
| *16-23*                       | *\$s0 - ​\$s7*                 | *(**s**aved values) - Callee saved.*  *A subroutine using one of these must save original and restore it before exiting.* *Preserved across procedure calls*（**S**aved or **S**tatic简写？）静态变量？调用时保存 |
| *24-25*                       | *\$t8 - ​\$t9*                 | *(**t**emporaries) Caller saved if needed. Subroutines can use w/out saving.* *These are in addition to $t0 - $t7 above.* *Not preserved across procedure calls.*（**T**emp简写）算是前面$0~$7的一个继续，属性同$t0~$t7 |
| *26-27*                       | *\$k0 - ​\$k1*                 | *reserved for use by the interrupt/trap handler*（brea**K** off简写？）中断函数返回值，不可做其他用途 |
| *28*                          | *$gp*                         | global pointer.  **Points to the middle of the 64K block of memory in the static data segment.**（**G**lobal **P**ointer简写）指向64k(2^16)大小的静态数据块的中间地址（字面上好像就是这个意思，块的中间）* |
| *29*                          | *$sp*                         | ***s**tack **p**ointer*  *Points to last location on the stack.**(***S**tack **P**ointer简写）栈指针，指向的是栈顶 |
| *30*                          | *\$s8/​\$fp*                   | ***s**aved value / **f**rame **p**ointer* *Preserved across procedure calls*(**S**aved/**F**rame **P**ointer简写)帧指针 |
| *31*                          | *$ra*                         | ***r**eturn **a**ddress*返回地址，目测也是不可做其他用途     |



## 程序结构

- 本质其实就只是数据声明+普通文本+程序编码（文件后缀为.s，或者.asm也行）
- 数据声明在代码段之后（其实在其之前也没啥问题，也更符合高级程序设计的习惯）



### 数据声明

- 数据段以 **.data**为开始标志
- 声明变量后，即在主存中分配空间。



### 代码

- 代码段以 **.text**为开始标志
- 其实就是各项指令操作
- 程序入口为**main：**标志（这个都一样啦）
- 程序结束标志（详见下文）



### 注释

- 同C系语言

- - - MIPS程序的基本模板如下：

      ```
      # Comment giving name of program and description of function
      # 说明下程序的目的和作用（其实和高级语言都差不多了）
      # Template.s
      #Bare-bones outline of MIPS assembly language program
      
                 .data       # variable declarations follow this line　　　　                # 数据变量声明
                             # ...
      														
                 .text       # instructions follow this line	
      		       		  # 代码段部分															
      main:                  # indicates start of code (first instruction to execute)               # 主程序
                             # ...
      									
      ```







## 数据声明

> format for declarations:
>
> 声明的格式：
>
> ```
> name:	                storage_type	value(s)	
> 变量名：（冒号别少了）     数据类型         变量值     
> ```

- - create storage for variable of specified type with given name and specified value
  - value(s) usually gives initial value(s); for storage type .space, gives number of spaces to be allocated
  - 通常给变量赋一个初始值；对于**.space**,需要指明需要多少大小空间（bytes)

> Note: labels always followed by colon ( : )
>
> ```
> example
> 	
> var1:		.word	3	
> #create a single integer variable with initial value 3
> #声明一个 word 类型的变量 var1, 同时给其赋值为 3
> 
> array1:		.byte	'a','b'	
> #create a 2-element character array with elements initialized
> #to  a  and  b　　　　　　　　　　　　　　　　　　 
> #声明一个存储2个字符的数组 array1，并赋值 'a', 'b'
> 
> array2:	.space	40	
> #allocate 40 consecutive bytes, with storage uninitialized
> #could be used as a 40-element character array, or a
> #10-element integer array; a comment should indicate which!
> #为变量 array2 分配 40字节（bytes)未使用的连续空间，当然，对于这个变量到底要存放什么类型的值， 最好事先声明注释下！
> ```





### 加载/保存(也许这里写成读取/写入 可能更易理解一点) 指令集

- 如果要访问内存，不好意思，你只能用 **load** 或者 **store** 指令
- 其他的只能都一律是寄存器操作

**load:**

>```
		lw	register_destination, RAM_source
>```


>\#copy word (4 bytes) at source RAM location to destination register.
>
>从内存中 复制 RAM_source 的内容到 对应的寄存器中
>
>（1lw中的'w'意为'word',即该数据大小为4个字节）

>```
>	lb	register_destination, RAM_source
> ```

>\#copy byte at source RAM location to low-order byte of destination register,
>\# and sign-e.g.tend to higher-order bytes
>
>同上， lb 意为 load byte
>
> store word:
>
> ```
> 	sw	register_source, RAM_destination
> ```

> \#store word in source register into RAM destination
>
> \#将指定寄存器中的数据 写入 到指定的内存中
>
> ```
> 	sb	register_source, RAM_destination
> ```

>\#store byte (low-order) in source register into RAM destination
>
> load immediate:
>
> ```
> 	li	register_destination, value
>  ```

> \#load immediate value into destination register
>
> 顾名思义，这里的 li 意为 load immediate

>  
>
>  ```
>  example:
>  	.data
>  var1:	.word	23		
>  # declare storage for var1; initial value is 23
>  # 先声明一个 word 型的变量 var1 = 3;
>  	.text
>  __start:
>  	lw	$t0, var1	
>  # load contents of RAM location into register $t0:  $t0 = var1
>  # 令寄存器 $t0 = var1 = 3;
>  
>  	li	$t1, 5		
>  # $t1 = 5   ("load immediate")　　　　 
>  # 令寄存器 $t1 = 5;
>  
>  
>  	sw	$t1, var1	
>  # store contents of register $t1 into RAM:  var1 = $t1　　　　 
>  # 将var1的值修改为$t1中的值： var1 = $t1 = 5;
>  	done
>  ```



### 立即与间接寻址

load address:

直接给地址

```
	la	$t0, var1
```

- copy RAM address of var1 (presumably a label defined in the program) into register $t0

indirect addressing:

地址是寄存器的内容（可以理解为指针）

```
	lw	$t2, ($t0)
```

- load word at RAM address contained in \$t0 into \$t2

```
	sw	$t2, ($t0)
```

- store word in register \$t2 into RAM at address contained in ​\$t0

based or indexed addressing：

+偏移量

```
	lw	$t2, 4($t0)
```

- load word at RAM address ($t0+4) into register $t2
- "4" gives offset from address in register $t0

```
	sw	$t2, -12($t0)
```

- store word in register $t2 into RAM at address ($t0 - 12)
- negative offsets are fine

Note: based addressing is especially useful for:

不必多说，要用到偏移量的寻址，基本上使用最多的场景无非两种：数组，栈。

- arrays; access elements as offset from base address
- stacks; easy to access elements at offset from stack pointer or frame pointer

 

```
example：栗子：

		.data
array1:		.space	12		
#  declare 12 bytes of storage to hold array of 3 integers
#  定义一个 12字节 长度的数组 array1, 容纳 3个整型


		.text
__start:	la	$t0, array1	 #  load base address of array into register $t0　
                              #  让 $t0 = 数组首地址

		   li	$t1, 5		#  $t1 = 5   ("load immediate")
		   sw $t1, ($t0)	 #  first array element set to 5; indirect         　　　　　　　          　　    # addressing 对于数组第一个元素赋值 array[0] = $1 = 5
		   li $t1, 13		 #   $t1 = 13
		   sw $t1, 4($t0)	 #  second array element set to 13　　　　　　　　　　　							# 对于 数组第二个元素赋值 array[1] = $1 = 13 　　　　　　　　　　　　　　　　 # (该数组中每个元素地址相距长度就是自身数据类型长度，
		   				    #即4字节， 所以对于array+4就是array[1])
		   li $t1, -7		 #   $t1 = -7
		   sw $t1, 8($t0)	 #  third array element set to -7　　　　　　　　
		   					# 同上， array+8 = （address[array[0])+4）+ 4 = 
		   					# address(array[1]) + 4 = address(array[2])
		   done
```



### 算术指令集

- 最多3个操作数
- 再说一遍，在这里，操作数只能是寄存器，绝对不允许出现地址
- 所有指令统一是32位 = 4 * 8 bit = 4bytes = 1 word

 add \$t0,\$t1,\$t2 

\#​ \$t0 = ​\$t1 + $t2; 

\# add as signed (2's complement) integers

```
		sub		$t2,$t3,$t4	#  $t2 = $t3 Ð $t4
		addi	$t2,$t3, 5	#  $t2 = $t3 + 5;   "add immediate" (no sub immediate)
		addu	$t1,$t6,$t7	#  $t1 = $t6 + $t7;   add as unsigned integers
		subu	$t1,$t6,$t7	#  $t1 = $t6 + $t7;   subtract as unsigned integers

		mult	$t3,$t4		
				#  multiply 32-bit quantities in $t3 and $t4, and store 64-bit
				#  result in special registers Lo and Hi: (Hi,Lo)= $t3*$t4
				#  运算结果存储在hi,lo（hi高位数据， lo地位数据）
		div	$t5,$t6	     #  Lo = $t5 / $t6   (integer quotient)
					    #  Hi = $t5 mod $t6  (remainder)　　　　　　　　　
					    #  商数存放在 lo, 余数存放在 hi
		mfhi	$t0		#  move quantity in special register Hi to $t0:   $t0 = Hi
        				#  不能直接获取hi或lo中的值，需要mfhi, mflo指令传值给寄存器
		mflo	$t1		#  move quantity in special register Lo to $t1:   $t1 = Lo
					    #  used to get at result of product or quotient
		move	$t2,$t3	 #  $t2 = $t3
```