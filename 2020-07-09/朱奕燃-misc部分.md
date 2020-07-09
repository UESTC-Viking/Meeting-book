# Misc方向的探索

## zip普通加密(爆破)

工具:ARCHPR.exe

使用工具ARCHPR.exe应用进行爆破,直接得到密码

## zip伪加密

工具:010 Editor 或者 Winhex

zip伪加密是在文件头的加密标志位做修改，进而再打开文件时识被别为加密压缩包

一个 ZIP 文件由**三个部分**组成：

压缩源文件数据区+压缩源文件目录区+压缩源文件目录结束标志

- 压缩源文件数据区中每一个压缩的源文件或目录都是一条记录，其中

  -   `local file header` ：文件头用于标识该文件的开始，记录了该压缩文件的信息，这里的文件头标识由固定值 `50 4B 03 04` 开头，也是 `ZIP` 的文件头的重要标志
  -   `file data` ：文件数据记录了相应压缩文件的数据
  -   `data descriptor` ：数据描述符用于标识该文件压缩结束，该结构只有在相应的 `local file header` 中通用标记字段的第 `3 bit` 设为 `1` 时才会出现，紧接在压缩文件源数据后

- `Central directory` 核心目录

  - 记录了压缩文件的目录信息，在这个数据区中每一条纪录对应在压缩源文件数据区中的一条数据。

    | Offset | Bytes | Description                                          | 译                                      |
    | ------ | ----- | ---------------------------------------------------- | --------------------------------------- |
    | 0      | 4     | Central directory file header signature = 0x02014b50 | 核心目录文件 header标识 =（0x02014b50） |
    | 4      | 2     | Version made by                                      | 压缩所用的 pkware 版本                  |
    | 6      | 2     | Version needed to extract (minimum)                  | 解压所需 pkware 的最低版本              |
    | 8      | 2     | General purpose bit flag                             | 通用位标记伪加密                        |
    | 10     | 2     | Compression method                                   | 压缩方法                                |
    | 12     | 2     | File last modification time                          | 文件最后修改时间                        |
    | 14     | 2     | File last modification date                          | 文件最后修改日期                        |
    | 16     | 4     | CRC-32                                               | CRC-32 校验码                           |
    | 20     | 4     | Compressed size                                      | 压缩后的大小                            |
    | 24     | 4     | Uncompressed size                                    | 未压缩的大小                            |
    | 28     | 2     | File name length (n)                                 | 文件名长度                              |
    | 30     | 2     | Extra field length (m)                               | 扩展域长度                              |
    | 32     | 2     | File comment length (k)                              | 文件注释长度                            |
    | 34     | 2     | Disk number where file starts                        | 文件开始位置的磁盘编号                  |
    | 36     | 2     | Internal file attributes                             | 内部文件属性                            |
    | 38     | 4     | External file attributes                             | 外部文件属性                            |
    | 42     | 4     | relative offset of local header                      | 本地文件头的相对位移                    |
    | 46     | n     | File name                                            | 目录文件名                              |
    | 46+n   | m     | Extra field                                          | 扩展域                                  |
    | 46+n+m | k     | File comment                                         | 文件注释内容                            |

- `End of central directory record(EOCD)` 目录结束标识

  -   目录结束标识存在于整个归档包的结尾，用于标记压缩的目录数据的结束。每个压缩文件必须有且只有一个 `EOCD` 记录。
