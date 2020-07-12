# 结构化异常处理(SEH)

> **SEH是window操作系统默认的异常处理机制**
> **但是微软并未公开SEH机制的原理，只是在编译器方面提供了封装好的异常处理**
> **比如Visual C++中的__try{}/__finally{}和__try{}/__except{}结构本质上就是对Windows提供的SEH的封装**


## 1. SEH链的结构

>seh以链的形式存在

SEH链的结构在已知源码中是找不到的,可以通过0环调试器windbg等来查看结构
![dbg](https://p-blog.csdn.net/images/p_blog_csdn_net/smarttech/372278/o_st11.JPG)
![SEH结构](https://img-blog.csdn.net/20161016165459218?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![FS0](https://p-blog.csdn.net/images/p_blog_csdn_net/smarttech/372278/o_pietrek4.jpg)


## 2.windows异常处理过程

>seh是基于线程的, FS:[0]是链的头结点(NtTib.ExceptionList=FS:[0])

进程运行过程中若发生异常，OS会委托进程进行处理。若进程代码中存在具体的异常处理(如SEH异常处理器)代码，则能够顺利处理相关异常，程序继续运行，但如果进程内部没有具体实现SEH，那么相关异常就无法处理，OS就会启动默认的异常处理机制，终止进程运行.
即:
1. 程序的某一个线程发生异常
2. windows遍历该线程的SEH链
3. 若找到可处理该异常的异常处理函数则执行,否则5
4. 重新执行引起异常的语句
5. 执行默认的异常处理机制,终止进程

## 3.异常处理函数 _except_handler

```C++
EXCEPTION_DISPOSITION
__cdecl _except_handler( struct _EXCEPTION_RECORD *ExceptionRecord,
                        void * EstablisherFrame,
                        struct _CONTEXT *ContextRecord,
                        void * DispatcherContext);
```

第一个参数是一个指向EXCEPTION_RECORD结构的指针:
```C++
typedef struct _EXCEPTION_RECORD {
    DWORD ExceptionCode;
    DWORD ExceptionFlags;
    ......
} EXCEPTION_RECORD;
```
结构体的第一个成员变量ExceptionCode是赋予异常的代码。通过在WINNT.H中搜索以“STATUS_”开头的＃define定义，你可以得到一个异常代码列表。例如所有人都非常熟悉的STATUS_ACCESS_VIOLATION的代码是0xC0000005

第三个参数就是CONTEXT结构体,就是写调试器HOOK用到的CONTEXT,线程上下文,包含寄存器的值等一些信息

## 4. 安装异常处理函数

安装:
```C++
push @MyHandler  ;异常处理程序
push FS:[0]       ；SEH Linked List头
mov dword ptr fs:[0],esp  ;添加链表
```
删除:
```C++
POP DWORD PTR FS:[0] ;读取栈值并将其放入FS:[0],这里的栈值存放的下一个SEH的起始地址，执行该命令之后，就可以从栈中删除对应的SEH。
ADD ESP,4
```

## 5. 实例

参考文章:  [深入解析结构化异常处理(SEH)](https://blog.csdn.net/chenlycly/article/details/52575260)