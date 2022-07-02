# GDB调试学习笔记 #
- [GDB调试学习笔记](#gdb调试学习笔记)
    - [基础](#基础)
        - [1.编译](#1编译)
        - [2.GDB启动方式](#2gdb启动方式)
            - [直接使用 gdb 指令启动 GDB 调试器](#直接使用-gdb-指令启动-gdb-调试器)
            - [调试尚未执行的程序](#调试尚未执行的程序)
            - [调试正在执行的程序](#调试正在执行的程序)
            - [调试执行异常崩溃的程序](#调试执行异常崩溃的程序)
        - [3.基础指令](#3基础指令)
## 基础 ##

### 1.编译 ###

编译选项：-g 表示可执行文件生成必要的调试信息，比如各行代码所在的行号、包含程序中所有变量名称的列表等，大小和不带-g的可执行文件也不同

### 2.GDB启动方式 ###

#### 直接使用 gdb 指令启动 GDB 调试器 ####

```gdb
gdb
file test
```

#### 调试尚未执行的程序 ####

```gdb
gdb test
```

#### 调试正在执行的程序 ####

```gdb
./可执行程序文件名   //执行程序
pidof 可执行程序文件名     //找到该程序运行所对应的进程号
gdb attach PID   //三选一
gdb 文件名 PID
gdb -p PID
```

#### 调试执行异常崩溃的程序 ####

```gdb
./可执行程序文件名   //执行程序
gdb 可执行程序文件名 core
```

### 3.基础指令 ###

```gdb
gdb --args main.exe a.txt //设置参数
(gdb) set args a.txt
(gdb) run a.txt
(gdb) start a.txt

1、(gdb) break location     // b location
2、(gdb) break ... if cond   // b .. if cond

对于监控 C、C++ 程序中某变量或表达式的值是否发生改变，watch 命令的语法非常简单，如下所示：
(gdb) watch cond

建立捕捉断点的方式很简单，就是使用 catch 命令，其基本格式为：
(gdb) catch event

next 是最常用来进行单步调试的命令，其最大的特点是当遇到包含调用函数的语句时，无论函数内部包含多少行代码，next 指令都会一步执行完。也就是说，对于调用的函数来说，next 命令只会将其视作一行代码。

next 命令可以缩写为 n 命令，使用方法也很简单，语法格式如下：
(gdb) next count

通常情况下，step 命令和 next 命令的功能相同，都是单步执行程序。不同之处在于，当 step 命令所执行的代码行中包含函数时，会进入该函数内部，并在函数第一行代码处停止执行。

step 命令可以缩写为 s 命令，用法和 next 命令相同，语法格式如下：
(gdb) step count

until 命令可以简写为 u 命令，有 2 种语法格式，如下所示：
1、(gdb) until
2、(gdb) until location

其中，参数 location 为某一行代码的行号。

不带参数的 until 命令，可以使 GDB 调试器快速运行完当前的循环体，并运行至循环体外停止。注意，until 命令并非任何情况下都会发挥这个作用，只有当执行至循环体尾部（最后一行代码）时，until 命令才会发生此作用；反之，until 命令和 next 命令的功能一样，只是单步执行程序。

examine 命令 (缩写形式 x) 可以查看变量在对应内存地址中的值，其命令格式为：x/[n][f][u] addr。

其中，可选参数 n 表示要打印的内存单元个数，默认值为1；f 表示上面介绍的各种格式控制符；u 表示要打印的内存单元长度;addr 表示变量的内存地址 (注意：不能是变量名)。

常用的内存单元包括：b (单字节)、h (双字节)、w (四字节)、g (八字节)。
```

### 4.多进程调试
1. 用 gdb 先调试父进程，等子进程fork出来后，使用 gdb attach 到子进程上去。当然，您需要重新开启一个 Shell 窗口用于调试
2. gdb 调试器提供一个选项叫 follow-fork ，通过 set follow-fork mode 来设置是当一个进程 fork 出新的子进程时，gdb 是继续调试父进程还是子进程（取值是 child），默认是父进程（取值是 parent）
### 5.多线程调试
info threads  ---> thread 线程编号 ----> bt查看栈帧 ---> frame 栈函数编号

set scheduler-locking on/step/off
[GDB指令大全](https://www.cnblogs.com/silentNight/p/5467134.html)

[多进程/多线程调试](https://cloud.tencent.com/developer/article/1662935)