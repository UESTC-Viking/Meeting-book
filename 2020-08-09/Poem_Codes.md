## Decrypt-the-Message

题目描述：解密这段信息！

附件内容：

```
# 08d2187c2c0540e78e4d703a2ef3ff6f.txt
The life that I have
Is all that I have
And the life that I have
Is yours.

The love that I have
Of the life that I have
Is yours and yours and yours.

A sleep I shall have
A rest I shall have
Yet death will be but a pause.

For the peace of my years
In the long green grass
Will be yours and yours and yours.

decrypted message: emzcf sebt yuwi ytrr ortl rbon aluo konf ihye cyog rowh prhj feom ihos perp twnb tpak heoc yaui usoa irtd tnlu ntke onds goym hmpq
```

- `Poem Codes`

  - 加密原理：

    参考：http://wmbriggs.com/post/1001/

    ```python
    # 举例
    # poem: "for my purpose holds to sail beyond the sunset, and the baths of all the western stars until I die."
    # 需要加密的内容："We have run out of cigars, situation desperate."
    # 从poem里面选取单词作为秘钥（所选取的单词连接在一起后的总长度最好不超过26）并记录标号：0、5 13、16、19
      'for' 'sail' 'all' 'stars' 'die'
    # 给秘钥的每个字符标号(从a~z检索)
      f  o  r  s  a  i  l  a  l  l  s  t  a  r  s  d  i  e
      6  12 13 15 1  7  9  2  10 11 16 18 3  14 17 4  8  5
    # 给需要加密信息分组(多少位为一组取决于秘钥的长度)
      1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18
      w  e  h  a  v  e  r  u  n  o  u  t  o  f  c  i  g  a
      r  s  s  i  t  u  a  t  i  o  n  d  e  s  p  e  r  a
      t  e  a  b  c  d  e  f  g  h  i  j  k  l  m  n  o  p
    # 实施加密第一步：根据所取秘钥的标号取对应位置的字母(a~z)
      afnqt
    # 实施加密第二步：根据秘钥标号后进行，例如秘钥的第一个字母为f,对应编号为6,则先取第6列
      eud tdj oek cpm wrt rae nig ese ooh uni ien aap hsa fsl gro aib utf vtc
    # 最终加密所得密文内容为
      afnqt eud tdj oek cpm wrt rae nig ese ooh uni ien aap hsa fsl gro aib utf vtc
    ```

  - 解密原理

    ```python
    # 根据密文列表的第一个字符串得出组合成秘钥的最小序号组合
    code = []
    	for i in cwords.pop(0):
    		code.append(abc.index(i))
    # 根据最小序号组合生成所有可能的可能的秘钥组合(26k+i)---笛卡尔积
    xwords = [[] for x in range(len(code))]
    	for xcount, c in enumerate(code):
    		tlen = c
    		while(c<len(twords)):
    			xwords[xcount].append(twords[c].lower())
    			c+=26
    # itertools.product(*xwords)
    # for循环，用所有秘钥去对密文解密
      - 先生成秘钥的每个字符的标号
      - 再根据标号去还原明文的分组情况 
    count = 0
    while(count<plen):
    	for al in abc:
    		for pc, pl in enumerate(pwords):
    			if al!=pl: continue
    			pcode[count]=cwords[pc]
    			count+=1
    # 按还原的分组信息一行一行的输出结果
    wlen = len(pcode[0])
    	for c in range(0, wlen):
    		for word in pcode:
    			msg+=word[c]
    print msg
    ```

  - 加解密工具：https://github.com/abpolym/crypto-tools/tree/master/poemcode

- 解密工具的使用---只支持`python2`,选取有语意的。

  ```python
  python poemcode.py 诗歌文件名 待解密文件名
  
  # 这道题的用法
  python poemcode.py poem.txt cipher.txt 
  # poem.txt
  he life that I have
  Is all that I have
  And the life that I have
  Is yours
  
  The love that I have
  Of the life that I have
  Is yours and yours and yours
  
  A sleep I shall have
  A rest I shall have
  Yet death will be but a pause
  
  For the peace of my years
  In the long green grass
  Will be yours and yours and yours
  # cipher.txt
  emzcf sebt yuwi ytrr ortl rbon aluo konf ihye cyog rowh prhj feom ihos perp twnb tpak heoc yaui usoa irtd tnlu ntke onds goym hmpq
  ```

- 结果：`ifyouthinkcryptographyistheanswertoyourproblemthenyoudonotknowwhatyourproblemisabcdefghijklmnopqrstu`

- 加密

  ```python
  python python_encrypt.py 诗歌文件名 待加密文件名
  
  # poem内容
  To see a world in a grain of sand
  And a heaven in a wild flower
  Hold infinity in the palm of your hand
  And eternity in an hour
  # msg内容
  comeonletusjointhectfteamnamedvikingtogther
  # 加密操作
  python python_encrypt.py poem msg
  # 解密结果
  bdgl hgl eee oar con iki nij sdf mth eac tnk ctg uee jvg tmd nma oft etm lnb oih
  ```

  <img src="https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20200809192934.png" alt="image-20200809192934244" style="zoom:150%;" />

  <img src="https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20200809193039.png" alt="image-20200809193039369" style="zoom:150%;" />

