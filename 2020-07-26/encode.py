# -*- coding: utf-8 -*-
'''from PIL import Image
import math

def encode(text):#加密函数
    str_len = len(text)#计算长度
    width = math.ceil(math.sqrt(str_len))#确定正方形图像的边长
    im = Image.new("RGB",(width,width),0x0)
    
    x,y = 0,0
    for i in text:
        index = ord(i)#得到读入文字的unicode编码
        rgb = (0,(index & 0xFF00) >> 8,index & 0xFF)
        im.putpixel((x,y),rgb)
        if x == width - 1:
            x = 0
            y += 1
        else:
            x += 1
        return im


with open("test.txt",encoding="utf_8") as f:
    text = f.read()
im = encode(text)
im.save("out.bmp")'''

from PIL import Image
import math

def encode(text):#加密函数
    str_len = len(text)#获得文本长度
    width = math.ceil(str_len**0.5)#确定正方形图片的边长
    im = Image.new("RGB", (width, width), 0x0)#创建空白图

    x, y = 0, 0
    for i in text:
        index = ord(i)
        rgb = ( 0,  (index & 0xFF00) >> 8,  index & 0xFF)
        im.putpixel( (x, y),  rgb )
        if x == width - 1:
            x = 0
            y += 1
        else:
            x += 1
    return im

def main(filename: str):
    with open(filename, encoding="utf-8") as f:
        all_text = f.read()
        
    im = encode(all_text)
    im.save("{}_layout.bmp".format('.'.join(filename.split('.')[:-1])))

if __name__ == '__main__':
    main('test.txt')

#(index & 0xFF0000) >> 16
#由于我自己写的代码有杂七杂八的小问题，所以用的原作者的代码演示