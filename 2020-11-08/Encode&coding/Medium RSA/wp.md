openssl rsa -pubin -in pubkey.pem -text -modulus

>>> mm = '0xC2636AE5C3D8E43FFB97AB09028F1AAC6C0BF6CD3D70EBCA281BFFE97FBE30DD'
>>> m = int(mm, 16)

cp pubkey.pem rsatool/
python3 rsatool.py -out private.pem -e 65537 -p 275127860351348928173285174381581152299 -q 319576316814478949870590164193048041239
mv ut private.pem
mv private.pem ..
cd ..
openssl rsautl -decrypt -in flag.enc -inkey private.pem -out message.txt

--------
或者
```python
import gmpy2  
from Crypto.Util.number import long_to_bytes  
 
n = 87924348264132406875276140514499937145050893665602592992418171647042491658461  
p = 275127860351348928173285174381581152299  
q = 319576316814478949870590164193048041239  
e = 65537  
phi = (p-1)*(q-1)  
d = gmpy2.invert(e,phi)  
 
with open('./flag.enc') as f:  
    c = f.read().encode('hex')  
    c = int(c,16)  
m = pow(c,d,n)  
print long_to_bytes(m)
```

关于文件形式的密文c，一般都是16进制形式读取，接着再转为整型用于计算。
gmpy2和libnum的效果都是一样的，接下来题目就按照我爱用的gmpy2介绍吧。

-----
