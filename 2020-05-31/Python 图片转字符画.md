# Python 图片转字符画

***

## 1. 原理

***

什么是字符画？  也就是一系列字符组成的一副可识别的图画

> 首先，每一张彩色的图片，之所以能够呈现出colorful或者dim的画面，只因为每一张图都由很多个像素点组成，而每一个像素点都有不同的颜色，而这个颜色就由三个值（rgb）对应三原色的百分比构成，所以每一个像素点都有三个值（rgb，范围0-255），那么我们就可以将每一个像素点转换成一个字符这样就能形成一个字符图画了

问题来了，总共256^3 种颜色，哪来这么多字符？

> + 灰度值
>
> 1. 指黑白图像中点的颜色深度，范围一般从0到255，白色为255，黑色为0，故黑白图片也称灰度图像
>
> 2. rgb转灰度值计算公式：gray=int(r\*0.299+g\*0.587+b*0.114)

##  2.简单介绍pillow库

###  1.基本概念：

> + 通道（bands）：每张图片由一个或多个数据通道构成，例如常见的rgb图像，每张图片都由三个数据通道构成，即R、G和B通道，而上面说的灰度图像只有一个通道。获取图片的通道方法是：getbands()，以元组数据类型返回。
>
> + 滤波器：将多个输入像素映射为一个输出像素的几何操作。
>
> + 模式（mode）：图像的模式定义了图像的类型和像素的位宽，有一些常见模式：
>
>   > 1：1位像素，表示黑和白，但是存储的时候每个像素存储为8bit。
>   >
>   > L：8位像素，表示黑和白。
>   >
>   > P：8位像素，使用调色板映射到其他模式。
>   >
>   > RGB：3x8位像素，为真彩色。
>   >
>   > RGBA：4x8位像素，有透明通道的真彩色。
>   >
>   > CMYK：4x8位像素，颜色分离。
>   >
>   > YCbCr：3x8位像素，彩色视频格式。
>   >
>   > I：32位整型像素。
>   >
>   > F：32位浮点型像素。

###  2.Image 模块的常用方法

>+ Image.save("save.gif"，"GIF")  #保存为GIF格式，至于前一个参数是保存路径，如果不写绝对路径自动保存在当前文件夹下
>+ Image.new(mode，size，color)  #创建新图片
>  注意保存时是new创建图片对象，再用对象的save方法保存
>+ Image.conver("RGBA")  #图像类型转换
>+ Image.size  #返回的是一个二元组，即该图片的大小，size[0]为水平长，size[1]为竖直高
>+ Image.getpixel((w,h))  #获取某个点的像素值（即rgb值），注意其参数是必须（wedith，height）不能换顺序

## 3.代码

```Python
from PIL import Image
import os
str = "$@B%WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!Il;:,\"^`'"
count = len(str)

def transform(image_file):
    StrPic = ''
    bands = list(image_file.getbands())
    for h in range(0,image_file.size[1]):
        for w in range(0,image_file.size[0]):
            if bands[-1] == "A":
                r,g,b,a = image_file.getpixel((w,h))
            else:
                r,g,b = image_file.getpixel((w,h))
            gray = int(r*0.299+g*0.587+b*0.114)
            StrPic += str[int((count*gray)//256)]
        StrPic += '\t\n'
    return StrPic

fp = open("D:\CODE\k.png",'rb')
image_file = Image.open(fp)
str0 = transform(image_file)
os.system('cls')
for i in str0:
    print(i,end='')
fp.close()
```