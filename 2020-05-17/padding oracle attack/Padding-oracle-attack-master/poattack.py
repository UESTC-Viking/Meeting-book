# coding:utf-8
import requests
import base64
# b'\x97.\xda\xb8\xa5P\t\x95\xae\x9b\xf5\xbf\xe2\x8b.<'
CYPHERTEXT = base64.b64decode("ly7auKVQCZWum/W/4osuPA==")
# initialization vector
IV = "6666666666666666"
# PKCS7 16个字节为1组
N = 16
# intermediaryValue ^ IV = plainText
inermediaryValue = ""
plainText = ""
# 爆破时不断需要更改的iv
iv = ""
URL = "http://webdog.popscat.top/index.php?method=decrypt&source=1"
def xor(a, b):
    """
    用于输出两个字符串对位异或的结果
    """
    return "".join([chr(ord(a[i]) ^ ord(b[i])) for i in range(len(a))])
for step in range(1, N + 1):
    padding = chr(step) * (step - 1)
    end=","
    print(step,end)
    for i in range(0, 256):
        print(i)
        """
        iv由三部分组成：
            待爆破位置 chr(0)*(N-step) 
            正在爆破位置 chr(i) 
            使 iv[N-step+1:] ^ inermediaryValue = padding 的 xor(padding,inermediaryValue)
        """
        iv = chr(0)*(N-step)+chr(i)+xor(padding,inermediaryValue)
        data = {
            "data": "ly7auKVQCZWum/W/4osuPA==",
            "iv": iv
        }
        r = requests.post(URL,data = data)
        if r.text !="False":
            inermediaryValue = xor(chr(i),chr(step)) + inermediaryValue
            print(inermediaryValue)
            break
plainText = xor(inermediaryValue,IV)
print(plainText)