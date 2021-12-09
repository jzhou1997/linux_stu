# 标准I/O库

不仅是UNIX，很多其他操作系统都实现了标准I/O库，这个库由ISO C标准说明，便于用户使用。

I/O操作是一切实现的基础

系统I/O  sysio （文件IO）

标准I/O  stdio （可移植性好）

## 1.FILE对象

stdio:FILE类型贯穿始终。对于标准I/O库，它们的操作是围绕FILE流进行的，当用标准I/O库打开或创建一个文件时，使一个流与一个文件相关联。当打开一个流时，标准I/O函数fopen返回一个指向FILE对象的指针。该对象通常是一个结构体，包含了标准I/O库为管理该流需要的所有信息，包括用于实际I/O的文件描述符、指向用于该流缓冲区的指针、缓冲区的长度、当前在缓冲区中的字符数以及出错标志等。

一个进程预定义了3个流，这3个流可以自动被进程使用，即标准输入、标准输出、标准错误，它们分别通过预定义文件指针stdin、stdout、stderr加以引用，定义在头文件<stdio.h>中。

## 2.打开和关闭流

打开一个标准I/O流主要有两个函数实现，fopen()和freopen（）。

FILE *fopen（const char *path, const char *mode）;

FILE *freopen（const char *path, const char *mode，FILE *fp）；

fopen函数打开路径名为path的一个指定的文件，freopen函数把一个新的文件名path与给定的打开的流 fp关联，同时关闭流中的旧文件。freopen函数一般用于将一个指定的文件打开为一个预定义的流，如标准输入、标准输出或标准错误。

如果文件成功打开，则函数返回一个FILE指针，指向用于标识流的对象。否则，返回空指针并记录错误errno。打印errno对应的错误信息可以以下函数实现：

perror（const char *s） 打印一个系统错误信息

strerror（int num） 参数需要errno，输出对应的错误信息，头文件<string.h>

函数中第二个参数mode为打开一个流的方式，其中r,r+文件不存在时不会创建文件，直接返回错误。w,w+,a,a+ 文件不存在时创建文件。几种方式的具体区别如下表

| 模式 | 描述                                                         |
| ---- | ------------------------------------------------------------ |
| r    | 打开一个用于读取的文件。文件必须存在                         |
| w    | 创建一个用于写入的空文件。文件若存在，删除已有文件的内容，文件被视为一个新的空文件 |
| a    | 追加到一个文件。写操作向文件末尾追加数据。如果文件不存在，则创建文件。 |
| r+   | 打开一个用于更新的文件，可读取也可写入。该文件必须存在。     |
| w+   | 创建一个用于读写的空文件。文件若存在，同w。                  |
| a+   | 打开一个用于读取和追加的文件。                               |

一般调用malloc函数申请的内存区就是在堆区，它会返回一个指针地址给你。FILE结构体指针也是类似的。（有互逆操作的一般都在堆区）

调用fclose关闭一个打开的流，函数原型为int fclose（FILE *fp）。

在该文件被关闭之前，冲洗缓冲区的输出数据，缓冲区的任何输入数据被丢弃，并释放此缓冲区。

一个线程最多打开文件数为1024，有三个流是默认打开的，stdin，stdout，strerr

## 3.读和写流

一旦打开了流，则可在3种不同类型的非格式化I/0中进行选择，对其进行读、写操作。

### 3.1每次一个字符的I/O

一次读或写一个字符，如果流是带缓冲的，则标准I/O函数处理所有缓冲。

可用以下函数一次读一个字符

int getc（FILE *fp）

int fgetc（FILE *fp）

int getchar(void)

getchar函数等同于getc（stdin）。getc和fgetc的区别在于getc可被实现为宏，在调用fgetc所需的时间很可能比调用getc要长，因为调用函数所需的时间通常长于调用宏。

宏不占用调用时间，只占用编译时间

函数相反，占用调用时间，不占用编译时间

与之对应的，上述每一个输入函数都有一个输出函数

int putc（int c，FILE *fp）

int fputc（int c，FILE *fp）

int putchar(int c)

与输入函数一样，putchar等同于putc（c,stdout）,putc可被实现为宏，而fputc不能实现为宏。

### 3.2每次一行的I/O

使用fgets和fputs可以实现一次读或写一行，每行都以一个换行符终止，当调用fgets时，应说明能处理的最大行长。

gets(char *s) 将缓冲区的数据一次全写入地址中，不知道地址的空间大小，不安全，可能造成缓冲区溢出，写到缓冲区之后的存储空间中，是一个不推荐使用的函数。

fgets(char *s, int size, FILE *stream)确定了地址能接收的数据大小。此函数一直读到下一个换行符为止，但是不超过n-1个字符，读入的字符被送入缓冲区。该缓冲区以null字节结尾，如果该行包括最后一个换行符的字符数超过了n-1，则fgets只返回一个不完整的行，对fgets的下一次调用会继续该行。具体例子如下：

当size = 5，char buf[size] = “abcd”，使用fgets(buf,size,stream)读流时，则将进行两次读操作，第一次读取 a b c d‘\0’，第二次读取‘\n’‘\0’

与之对应的，上述每一个输入函数都有一个输出函数

int fputs(const char *s, FILE *stream)

int puts（const char *s）

fputs将一个以null字节终止的字符串写到指定的流，尾端的终止符null不泄出，注意并以一定是每次输出一行。puts将一个以bull字节终止的字符串写到标准输出。

### 3.3直接I/O

在进行二进制I/O操作时，我们希望每次I/O操作读或写一个完整的结构。即读写某种数量的对象，而每个对象具有指定的长度。fread和fwrite可以用来执行二进制I/O操作。

size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)

size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)

返回值均为成功读/写的对象的数量，两个函数最好只用来操作工工整整的数据块。

例子：fread(buf,size,nmemb,fp)

考虑两种情况，1->数据量足够   2->只有5个字节

在fread（buf,1,10,fp）操作下，1-> 返回值为10，读取了10个字节 2-> 返回值为5，读取了5个字节

在fread（buf,10,1,fp）操作下，1->返回值为1，读取了10个字节  2->返回值为0，读取了？？？  无法确定读取了多少字节

## 4.格式化I/O

### 4.1格式化输出

int printf（const char *format，...）;

int fprintf（FILE *stream,const char *format，...）;

int sprintf（char *buf, const char *format，...）;

若成功，返回存入数组的字符数，若编码出错，返回负值。

int snprintf（char *buf, size_t size, const char *format，...）;

若缓冲区足够大，返回将要存入数组的字符数，若编码出错，返回负值。

  printf将格式化数据写到标准输出，fprintf写至指定的流，sprintf将格式化的字符送入数组buf中，在数组的尾端自动加一个null字节，但该字符不包括在返回值中。

### 4.2格式化输入

int scanf（const char *format，...）;

int fscanf（FILE *fp,const char *format，...）;

int sscanf（const char *buf,const char *format，...）;

3个函数返回值均为赋值的输入项数，若输入出错或在任意转换前已达到文件尾端，返回EOF。

## 5.定位流

fseek函数针对文件位置指针进行操作，重新定位一个流。

int fseek(FILE *fp, long offset, int whence),第一个参数为进行操作的流，第二个参数为偏移量的大小，第三个参数是偏移的相对位置（SEEK_SET,SEEK_CUR,SEEK_END，文件首，文件当前位置，文件尾）。操作成功返回0，失败返回-1，并设置errno值。

常用于空洞文件（下载工具）

ftell函数返回给定流 stream 的当前文件位置，原型为int ftell（FILE *fp）；

rewind函数将文件内部的位置指针重新指向起始位置，相当于

（void） fseek（stream，0L，SEEK_SET）

ftell和fseek函数，它们都假定文件的位置可以存放在一个长整型long中，而ftello和fssko函数使文件偏移量可以不必一定使用长整型，它们使用off_t数据类型代替了长整型。

off_t ftello(FILE *fp);

若成功，返回当前文件位置；若出错，返回off_t – 1

int fseeko(FILE *fp, off_t offset, int whence);

若成功，返回0；若出错，返回-1；

fseek和ftell受long类型大小限制，某些情况下可用fseeko和ftello替换，但是不支持C89，C99

## 6.缓冲

fflush(FILE *fp);刷新一个指定流，此函数将该流所有未写的数据都传送至内核。若fp是NULL，则将导致所有输出流都被冲洗。

int main()

{
   printf(“Before while（）”)；

  while(1);

  printf(“After while（）”)；

  exit(0);

}

在该例子中，Before信息并不会打印出来，因为标准输出是行缓存模式，碰到换行或者缓冲满了才会输出。可在每句输出语句中加换行，或者使用fflush()强制刷新。

缓冲，标准I/O库提供缓冲的目的是尽可能减少read和write调用的次数

缓冲区的作用：大多数情况下是好事，合并系统调用

行缓存：当在输入和输出中遇到换行符，标准I/O库执行I/O操作。

换行时候刷新，满了的时候刷新，强制刷新 （终端设备，如标准输出）

全缓冲：填满标准I/O缓冲区后才进行实际I/O操作，对于驻留在磁盘上的文件通常是全缓冲的。调用malloc获取需使用的缓冲区

填满缓冲区的时候刷新；强制刷新

无缓冲：如stderr，需要立即输出的内容

 缓冲的特征

  当且仅当标准输入和标准输出并不指向交互式设备时，它们才是全缓冲的。

  标准错误是不带缓冲的。

  若是指向终端设备的流，则是行缓冲的，否则是全缓冲的。

## 7.临时文件

关心的两个问题：1.各个进程间的临时文件如何不冲突；2.如何确保即使销毁临时文件。ISO C标准I/O库提供了两个函数创建临时文件。

  char *tmpnam（char *s）;不安全，产生冲突

tmpnam返回一个指向包含着有效文件名的字符串的指针。系统自己创建一个文件，文件名系统自己给定。有效文件名也就是在同一个目录下，在调用tmpnam时，没有同名的文件，这样程序员就可以认为这个文件名可以作为他的临时文件名。但是tmpname是不安全的，。tmpnam在申请一个临时文件时，并没有实际创建该文件，而是在调用该文件时生成该文件，这两个事件中间的事件差会产生一些安全漏洞（例如：另一个进程创建了一个同名的链接）。

  FILE *tmpfile（void）；创建一个匿名二进制文件，不会冲突，可直接操作FILE*，这个文件会在它关闭时，或者程序终止时，自动被释放。