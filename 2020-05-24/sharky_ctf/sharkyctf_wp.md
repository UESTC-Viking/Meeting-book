# sharkyctf

## miss_direction

TLS回调滥用> :(

### 使用的工具：

x32 / 64dbg

### 演练

#### 1）在入口点1（TLSCallback0）和0x4013C0处中断并继续

TLSCallback在函数的入口点之前执行

0x4013C0是第一个反混淆功能，我们将从此处检查二进制文件

#### 2）在0x4013E6处中断

我们需要跳转到0x4013E8，因为位于0x4013E6的JNE将使我们退出程序

```css
004013E6 | 75 02                    | jne miss_direction.4013EA               |
004013E8 | 74 0F                    | je miss_direction.4013F9                | <<---
```

将新的EIP来源设置为0x4013F9，也就是我们跳了

#### 3）成功通过第一项检查后，我们返回TLSCallback0

#### 4）在0x401104处中断并继续

```css
00401104 | E8 F2020000              | call miss_direction.4013FB              |
```

这是第二个反模糊处理功能，它将对真实检查进行模糊处理。在0x4013Fb处添加断点并继续

#### 5）在0x40140F：

```css
0040140A | B8 40144000              | mov eax,miss_direction.401440           | //loads the address 401440 into eax, this is the location of payload
0040140F | B9 7B020000              | mov ecx,27B                             | //loads integer 27B into ecx. This is the size of payload to deobfuscate
```

#### 6）然后从0x401414到0x40143E进行while循环。退出条件是：

```css
00401439 | 83F9 00                  | cmp ecx,0                               |
0040143C | 74 02                    | je miss_direction.401440                |
```

当ECX = 0时，将执行出口跳转。我们可以在0x40143C处设置一个条件断点，但是现在，当您在0x40143C处设置正常断点时，请观看魔术

#### 6）当您点击继续时，观看魔术。在0x401440，字节开始被填充

#### 7）当ECX为0时，退出跳转到0x401440。

```css
00401440 | EA 8BEC81EC 5801         | jmp far 158:EC81EC8B                    |
```

但是，这看起来无效，因此我们将新的EIP来源设置为0x401447

```css
00401447 | 0000                     | add byte ptr ds:[eax],al                |
```

#### 8）现在注意，从0x401449开始有大量的MOV指令，这是与输入进行比较的关键

```css
00401449 | C645 E0 8B               | mov byte ptr ss:[ebp-20],8B             |
0040144D | C645 E1 83               | mov byte ptr ss:[ebp-1F],83             |
00401451 | C645 E2 8A               | mov byte ptr ss:[ebp-1E],8A             |
00401455 | C645 E3 F6               | mov byte ptr ss:[ebp-1D],F6             |
00401459 | C645 E4 FA               | mov byte ptr ss:[ebp-1C],FA             |
0040145D | C645 E5 CB               | mov byte ptr ss:[ebp-1B],CB             |
00401461 | C645 E6 83               | mov byte ptr ss:[ebp-1A],83             |
00401465 | C645 E7 83               | mov byte ptr ss:[ebp-19],83             |
00401469 | C645 E8 F5               | mov byte ptr ss:[ebp-18],F5             |
0040146D | C645 E9 F4               | mov byte ptr ss:[ebp-17],F4             |
00401471 | C645 EA FA               | mov byte ptr ss:[ebp-16],FA             |
00401475 | C645 EB 82               | mov byte ptr ss:[ebp-15],82             |
00401479 | C645 EC 8A               | mov byte ptr ss:[ebp-14],8A             |
0040147D | C645 ED F6               | mov byte ptr ss:[ebp-13],F6             |
00401481 | C645 EE 8E               | mov byte ptr ss:[ebp-12],8E             |
00401485 | C645 EF 86               | mov byte ptr ss:[ebp-11],86             |
00401489 | C645 F0 81               | mov byte ptr ss:[ebp-10],81             |
0040148D | C645 F1 8B               | mov byte ptr ss:[ebp-F],8B              |
00401491 | C645 F2 F6               | mov byte ptr ss:[ebp-E],F6              |
00401495 | C645 F3 F9               | mov byte ptr ss:[ebp-D],F9              |
00401499 | C645 F4 C7               | mov byte ptr ss:[ebp-C],C7              |
0040149D | C645 F5 C7               | mov byte ptr ss:[ebp-B],C7              |
004014A1 | C645 F6 83               | mov byte ptr ss:[ebp-A],83              |
```

但是，它需要一些按摩，因为它看起来还不像有效的ASCII。

#### 9）在0x4014B4处，我们的输入逐字节加载以进行比较，退出条件为0x4014BA

```css
004014BA | 74 16                    | je miss_direction.4014D2                |
```

在0x4014D2处放置一个断点，然后继续

#### 10）在0x4014D2，在这里检查我们的输入是否等于23个字节（0x17）的长度，如果不是，则退出

```css
004014D2 | 837D C8 17               | cmp dword ptr ss:[ebp-38],17            |
004014D6 | 74 05                    | je miss_direction.4014DD                |
```

如果我们的输入正常，则跳转到0x4014DD。在0x4014E4处也放置一个断点。这是将使我们进入密码检查功能的过程

```css
004014E4 | EB 09                    | jmp miss_direction.4014EF               |
```

#### 11）密码检查功能是从0x4014E6到0x40151E的While循环。有趣的是下面的说明：

```css
00401500 | 83C1 55                  | add ecx,55                              |
00401503 | 83F1 42                  | xor ecx,42                              |
```

看起来我们的ECX寄存器加了55，然后异或了42，以获得密码。

#### 12）从8）中提取字节，然后对每个字节乘以42，然后减去55

#### 13）获得密码！正常启动二进制文件并输入密码:)

tls_c4llbacks_wont_f00l

## 安全的数据库

### 描述

我的一个朋友告诉我，他的服务器上某处有一个加密的泄漏数据库。他一直愿意给我一个程序来检索它，但是他没有给我提供用于解密数据库的密码。你能找到吗？

他还告诉我他的程序非常安全，并且我将无法在其上使用我的工具。

### 分析

在Ghidra中打开二进制文件，然后找到接受输入的位置。

```
undefined4 FUN_0804870a(void)
{
  size_t sVar1;
  undefined4 uVar2;
  int *in_ECX;
  int in_GS_OFFSET;
  bool bVar3;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  int local_20;
  undefined4 uStack24;
  int *local_14;
  
  uStack24 = 0x8048716;
  local_20 = *(int *)(in_GS_OFFSET + 0x14);
  local_14 = in_ECX;
  if (*in_ECX != 2) {
    __printf_chk();
                    /* WARNING: Subroutine does not return */
    exit(1);
  }
  _DAT_0804d0a4 = FUN_08048b70();
  local_30 = 0;
  local_2c = 0;
  local_28 = 0;
  local_24 = 0;
  __printf_chk();
  fgets(&DAT_0804d0c0,0x50,*(FILE **)PTR_stdin_0804cffc);
  sVar1 = strcspn(&DAT_0804d0c0,"\n");
  (&DAT_0804d0c0)[sVar1] = 0;
  strncpy((char *)&local_30,&DAT_0804d0c0,0x10);
  bVar3 = false;
  FUN_08048a90();
  if (bVar3) {
    puts("ok");
    if (((((DAT_0804d0c0 == *PTR_DAT_0804d068) && (DAT_0804d0c1 == PTR_DAT_0804d068[1])) &&
         (DAT_0804d0c2 == PTR_DAT_0804d068[2])) &&
        (((DAT_0804d0c3 == PTR_DAT_0804d068[3] && (DAT_0804d0c4 == PTR_DAT_0804d068[4])) &&
         ((DAT_0804d0c5 == PTR_DAT_0804d068[5] &&
          ((DAT_0804d0c6 == PTR_DAT_0804d068[6] && (DAT_0804d0c7 == PTR_DAT_0804d068[7])))))))) &&
       ((DAT_0804d0c8 == PTR_DAT_0804d068[8] &&
        (((((DAT_0804d0c9 == PTR_DAT_0804d068[9] && (DAT_0804d0ca == PTR_DAT_0804d068[10])) &&
           (DAT_0804d0cb == PTR_DAT_0804d068[0xb])) &&
          ((DAT_0804d0cc == PTR_DAT_0804d068[0xc] && (DAT_0804d0cd == PTR_DAT_0804d068[0xd])))) &&
         (DAT_0804d0ce == PTR_DAT_0804d068[0xe])))))) {
      puts("The password is valid.");
      FUN_08048c60();
      puts("Received and *hopefully* sucessfuly decrypted the database with the given password.");
      uVar2 = 0;
    }
    else {
      puts("Wrong password sorry, exiting.");
      uVar2 = 1;
    }
    if (local_20 != *(int *)(in_GS_OFFSET + 0x14)) {
      FUN_0804a460();
      __libc_start_main();
      do {
                    /* WARNING: Do nothing block with infinite loop */
      } while( true );
    }
    return uVar2;
  }
  func_0x8b927754();
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}
```

那只是一个难看的字符串比较。里面有什么`PTR_DAT_0804d068`？

```
                             PTR_DAT_0804d068                                XREF[1]:     FUN_0804870a:080487ec(R)  
        0804d068 e4 ac 04 08     addr       DAT_0804ace4                                     = 4Eh
        0804d06c f5 ac 04 08     addr       s_You_can't_debug_me_:*_0804acf5                 = "You can't debug me :*"

What's in DAT_0804ace4?

                             DAT_0804ace4                                    XREF[2]:     FUN_0804870a:080487f5(R), 
                                                                                          0804d068(*)  
        0804ace4 4e              undefined1 4Eh
                             DAT_0804ace5                                    XREF[1]:     FUN_0804870a:08048804(R)  
        0804ace5 33              undefined1 33h
                             DAT_0804ace6                                    XREF[1]:     FUN_0804870a:08048814(R)  
        0804ace6 6b              undefined1 6Bh
                             DAT_0804ace7                                    XREF[1]:     FUN_0804870a:08048824(R)  
        0804ace7 76              undefined1 76h
                             DAT_0804ace8                                    XREF[1]:     FUN_0804870a:08048834(R)  
        0804ace8 69              undefined1 69h
                             DAT_0804ace9                                    XREF[1]:     FUN_0804870a:08048844(R)  
        0804ace9 58              undefined1 58h
                             DAT_0804acea                                    XREF[1]:     FUN_0804870a:08048854(R)  
        0804acea 37              undefined1 37h
                             DAT_0804aceb                                    XREF[1]:     FUN_0804870a:08048864(R)  
        0804aceb 2d              undefined1 2Dh
                             DAT_0804acec                                    XREF[1]:     FUN_0804870a:08048874(R)  
        0804acec 76              undefined1 76h
                             DAT_0804aced                                    XREF[1]:     FUN_0804870a:08048884(R)  
        0804aced 58              undefined1 58h
                             DAT_0804acee                                    XREF[1]:     FUN_0804870a:08048890(R)  
        0804acee 45              undefined1 45h
                             DAT_0804acef                                    XREF[1]:     FUN_0804870a:0804889c(R)  
        0804acef 71              undefined1 71h
                             DAT_0804acf0                                    XREF[1]:     FUN_0804870a:080488a8(R)  
        0804acf0 76              undefined1 76h
                             DAT_0804acf1                                    XREF[1]:     FUN_0804870a:080488b4(R)  
        0804acf1 6c              undefined1 6Ch
                             DAT_0804acf2                                    XREF[1]:     FUN_0804870a:080488c0(R)  
        0804acf2 70              undefined1 70h
        0804acf3 00              ??         00h
        0804acf4 00              ??         00h
```

以ASCII表示`N3kviX7-vXEqvlp`。

```
kali@kali:~/Downloads/secure_db$ perl -e 'print "\x4e\x33\x6B\x76\x69\x58\x37\x2d\x76\x58\x45\x71\x76\x6c\x70\n"'
N3kviX7-vXEqvlp
```

那是密码吗？

```
kali@kali:~/Downloads/secure_db$ ./secure_db foo
Hi Doge ! 
░░░░░░░░░▄░░░░░░░░░░░░░░▄░░░░
░░░░░░░░▌▒█░░░░░░░░░░░▄▀▒▌░░░
░░░░░░░░▌▒▒█░░░░░░░░▄▀▒▒▒▐░░░
░░░░░░░▐▄▀▒▒▀▀▀▀▄▄▄▀▒▒▒▒▒▐░░░
░░░░░▄▄▀▒░▒▒▒▒▒▒▒▒▒█▒▒▄█▒▐░░░
░░░▄▀▒▒▒░░░▒▒▒░░░▒▒▒▀██▀▒▌░░░
░░▐▒▒▒▄▄▒▒▒▒░░░▒▒▒▒▒▒▒▀▄▒▒▌░░
░░▌░░▌█▀▒▒▒▒▒▄▀█▄▒▒▒▒▒▒▒█▒▐░░
░▐░░░▒▒▒▒▒▒▒▒▌██▀▒▒░░░▒▒▒▀▄▌░
░▌░▒▄██▄▒▒▒▒▒▒▒▒▒░░░░░░▒▒▒▒▌░
▀▒▀▐▄█▄█▌▄░▀▒▒░░░░░░░░░░▒▒▒▐░
▐▒▒▐▀▐▀▒░▄▄▒▄▒▒▒▒▒▒░▒░▒░▒▒▒▒▌
▐▒▒▒▀▀▄▄▒▒▒▄▒▒▒▒▒▒▒▒░▒░▒░▒▒▐░
░▌▒▒▒▒▒▒▀▀▀▒▒▒▒▒▒░▒░▒░▒░▒▒▒▌░
░▐▒▒▒▒▒▒▒▒▒▒▒▒▒▒░▒░▒░▒▒▄▒▒▐░░
░░▀▄▒▒▒▒▒▒▒▒▒▒▒░▒░▒░▒▄▒▒▒▒▌░░
░░░░▀▄▒▒▒▒▒▒▒▒▒▒▄▄▄▀▒▒▒▒▄▀░░░
░░░░░░▀▄▄▄▄▄▄▀▀▀▒▒▒▒▒▄▄▀░░░░░
░░░░░░░░░▒▒▒▒▒▒▒▒▒▒▀▀░░░░░░░░
Please input the password :
-> N3kviX7-vXEqvlp
ok
Wrong password sorry, exiting.
```

那没有用，所以必须进行一些转换。在字符串比较之前，我们在这里从STDIN获得输入：

```
  fgets(&DAT_0804d0c0,0x50,*(FILE **)PTR_stdin_0804cffc);
  sVar1 = strcspn(&DAT_0804d0c0,"\n");
  (&DAT_0804d0c0)[sVar1] = 0;
  strncpy((char *)&local_30,&DAT_0804d0c0,0x10);
  bVar3 = false;
  FUN_08048a90();
  if (bVar3) {
```

它采用字符串并将\ n替换为NULL终止符。缓冲区为16个字节（15个字符+'\ 0'）

怎么`FUN_08048a90()`办？反编译的C代码没有用：

```
void FUN_08048a90(void)
{
  return;
}
```

这是反汇编：

```
                             **************************************************************
                             *                          FUNCTION                          *
                             **************************************************************
                             undefined FUN_08048a90()
             undefined         AL:1           <RETURN>
             undefined4        Stack[-0x14]:4 local_14                                XREF[1]:     08048aa4(*)  
                             FUN_08048a90                                    XREF[1]:     FUN_0804870a:080487d4(c)  
        08048a90 55              PUSH       EBP
        08048a91 57              PUSH       EDI
        08048a92 e8 2e 05        CALL       __i686.get_pc_thunk.cx                           undefined __i686.get_pc_thunk.cx()
                 00 00
        08048a97 81 c1 69        ADD        ECX,0x4569
                 45 00 00
        08048a9d 56              PUSH       ESI
        08048a9e 53              PUSH       EBX
        08048a9f e8 00 00        CALL       LAB_08048aa4
                 00 00
                             LAB_08048aa4                                    XREF[1]:     08048a9f(j)  
        08048aa4 83 04 24 05     ADD        dword ptr [ESP]=>local_14,offset LAB_08048aa9
        08048aa8 c3              RET
                             LAB_08048aa9                                    XREF[1]:     FUN_08048a90:08048aa4(*)  
        08048aa9 8d b9 c0        LEA        EDI,[ECX + 0xc0]
                 00 00 00
        08048aaf 89 fe           MOV        ESI,EDI
        08048ab1 eb 0d           JMP        LAB_08048ac0
        08048ab3 90              ??         90h
        08048ab4 90              ??         90h
        08048ab5 90              ??         90h
        08048ab6 90              ??         90h
        08048ab7 90              ??         90h
        08048ab8 90              ??         90h
        08048ab9 90              ??         90h
        08048aba 90              ??         90h
        08048abb 90              ??         90h
        08048abc 90              ??         90h
        08048abd 90              ??         90h
        08048abe 90              ??         90h
        08048abf 90              ??         90h
                             LAB_08048ac0                                    XREF[2]:     08048ab1(j), 08048ad4(j)  
        08048ac0 8b 16           MOV        EDX,dword ptr [ESI]
        08048ac2 83 c6 04        ADD        ESI,0x4
        08048ac5 8d 82 ff        LEA        EAX,[EDX + 0xfefefeff]
                 fe fe fe
        08048acb f7 d2           NOT        EDX
        08048acd 21 d0           AND        EAX,EDX
        08048acf 25 80 80        AND        EAX,0x80808080
                 80 80
        08048ad4 74 ea           JZ         LAB_08048ac0
        08048ad6 89 c2           MOV        EDX,EAX
        08048ad8 c1 ea 10        SHR        EDX,0x10
        08048adb a9 80 80        TEST       EAX,0x8080
                 00 00
        08048ae0 0f 44 c2        CMOVZ      EAX,EDX
        08048ae3 8d 56 02        LEA        EDX,[ESI + 0x2]
        08048ae6 0f 44 f2        CMOVZ      ESI,EDX
        08048ae9 89 c2           MOV        EDX,EAX
        08048aeb 00 c2           ADD        DL,AL
        08048aed 83 de 03        SBB        ESI,0x3
        08048af0 29 fe           SUB        ESI,EDI
        08048af2 74 3c           JZ         LAB_08048b30
        08048af4 0f b6 a9        MOVZX      EBP,byte ptr [ECX + 0xa0]
                 a0 00 00 00
        08048afb 31 db           XOR        EBX,EBX
        08048afd 8d 76 00        LEA        ESI,[ESI]
                             LAB_08048b00                                    XREF[1]:     08048b25(j)  
        08048b00 89 d8           MOV        EAX,EBX
        08048b02 31 d2           XOR        EDX,EDX
        08048b04 89 d9           MOV        ECX,EBX
        08048b06 f7 f6           DIV        ESI
        08048b08 8b 44 24 14     MOV        EAX,dword ptr [ESP + 0x14]
        08048b0c 83 e1 03        AND        ECX,0x3
        08048b0f 83 c3 01        ADD        EBX,0x1
        08048b12 c1 e1 03        SHL        ECX,0x3
        08048b15 d3 f8           SAR        EAX,CL
        08048b17 89 c1           MOV        ECX,EAX
        08048b19 89 e8           MOV        EAX,EBP
        08048b1b 32 04 17        XOR        AL,byte ptr [EDI + EDX*0x1]
        08048b1e 31 c8           XOR        EAX,ECX
        08048b20 39 de           CMP        ESI,EBX
        08048b22 88 04 17        MOV        byte ptr [EDI + EDX*0x1],AL
        08048b25 75 d9           JNZ        LAB_08048b00
        08048b27 89 f6           MOV        ESI,ESI
        08048b29 8d bc 27        LEA        EDI,[EDI]
                 00 00 00 00
                             LAB_08048b30                                    XREF[1]:     08048af2(j)  
        08048b30 5b              POP        EBX
        08048b31 5e              POP        ESI
        08048b32 5f              POP        EDI
        08048b33 5d              POP        EBP
        08048b34 c3              RET
        08048b35 8d              ??         8Dh
        08048b36 74              ??         74h    t
        08048b37 26              ??         26h    &
        08048b38 00              ??         00h
        08048b39 8d              ??         8Dh
        08048b3a bc              ??         BCh
        08048b3b 27              ??         27h    '
        08048b3c 00              ??         00h
        08048b3d 00              ??         00h
        08048b3e 00              ??         00h
        08048b3f 00              ??         00h
        08048b40 53              ??         53h    S
        08048b41 e8              ??         E8h
        08048b42 7a              ??         7Ah    z
        08048b43 fe              ??         FEh
        08048b44 ff              ??         FFh
        08048b45 ff              ??         FFh
        08048b46 81              ??         81h
        08048b47 c3              ??         C3h
        08048b48 ba              ??         BAh
        08048b49 44              ??         44h    D
        08048b4a 00              ??         00h
        08048b4b 00              ??         00h
        08048b4c 83              ??         83h
        08048b4d ec              ??         ECh
        08048b4e 14              ??         14h
        08048b4f ff              ??         FFh
        08048b50 74              ??         74h    t
        08048b51 24              ??         24h    $
        08048b52 1c              ??         1Ch
        08048b53 e8              ??         E8h
        08048b54 f8              ??         F8h
        08048b55 fa              ??         FAh
        08048b56 ff              ??         FFh
        08048b57 ff              ??         FFh
        08048b58 83              ??         83h
        08048b59 f8              ??         F8h
        08048b5a 0f              ??         0Fh
        08048b5b 0f              ??         0Fh
        08048b5c 94              ??         94h
        08048b5d 83              ??         83h
        08048b5e 64              ??         64h    d
        08048b5f 00              ??         00h
        08048b60 00              ??         00h
        08048b61 00              ??         00h
        08048b62 83              ??         83h
        08048b63 c4              ??         C4h
        08048b64 18              ??         18h
        08048b65 5b              ??         5Bh    [
        08048b66 c3              ??         C3h
        08048b67 89              ??         89h
        08048b68 f6              ??         F6h
        08048b69 8d              ??         8Dh
        08048b6a bc              ??         BCh
        08048b6b 27              ??         27h    '
        08048b6c 00              ??         00h
        08048b6d 00              ??         00h
        08048b6e 00              ??         00h
        08048b6f 00              ??         00h
```

那是...我猜是这样。一次解决这个1个字符可能比了解WTF这样做要快得多。

让我们尝试`angr`解决这个问题。

## 解

```
import angr, time, claripy

BINARY='./secure_db'
OUTFILE='out.db'
t=time.time()
proj = angr.Project(BINARY, auto_load_libs=False)
print(proj.arch)
print(proj.filename)
print("Entry: 0x%x" % proj.entry)

FIND=0x080488d6 # puts("The password is valid.");
AVOID=0x0804890a # puts("Wrong password sorry, exiting.");
print("Find: 0x%x" % FIND)
print("Avoid: 0x%x" % AVOID)

password = claripy.BVS('password', 8 * 16)
state = proj.factory.entry_state(args=[BINARY, OUTFILE], stdin=password)
simgr = proj.factory.simulation_manager(state)
simgr.explore(find=FIND, avoid=AVOID)

print(simgr.found[0].posix.dumps(0))
print(time.time() - t, "seconds")

```

现在有了文件，我们可以看到它是一个SQLite数据库。它很小，因此运行`.dump`是查找标志位置的简便方法。

```
kali@kali:~/Downloads/secure_db$ file out.db 
out.db: SQLite 3.x database, last written using SQLite version 3022000
kali@kali:~/Downloads/secure_db$ sqlite3 out.db
SQLite version 3.31.0 2019-12-29 00:52:41
Enter ".help" for usage hints.
sqlite> .dump
...
sqlite> select * from flag;
shkCTF{p4ssw0rd_pr0t3ct3d_db_6a773d0fcb5d742603167d2958547914}
```

