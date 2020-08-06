# Python简单爬虫（jpg爬取）

## 什么是爬虫？

**百度百科**：网络爬虫（又称为网页蜘蛛，网络机器人，在[FOAF](https://baike.baidu.com/item/FOAF)社区中间，更经常的称为网页追逐者），是一种按照一定的规则，自动地抓取[万维网](https://baike.baidu.com/item/万维网/215515)信息的程序或者脚本。另外一些不常使用的名字还有蚂蚁、自动索引、模拟程序或者蠕虫。

## robots协议

是一个放在网站目录下的txt文件，用于告诉爬虫可爬取的范围 (或者是搜索引擎访问的范围）。

**示例：**

```
# robots.txt generated at http://tool.chinaz.com/robots/ 
User-agent: *
Disallow: 
Disallow: /bin/
Sitemap: http://domain.com/sitemap.xml
```

## requests库

requests 是一个易用的http库，在爬虫中有广泛的应用

### requests.get（url）

用get请求传入的url

```python
page = requests.get(url)
print(page)
```

运行上面的python代码可得

![image-20200805135500299](image-20200805135500299.png)

这里得到的是一个Response 200为状态码

#### requests.get（url）.text

我们要爬取网页图片，就需要定位到图片。

**requests.get(url).text**可以使我们得到网站的**html**代码

我们知道，jpg图片以**src=http(s)://example.jpg**的模式出现在html中，故我们可以用正则表达式来匹配图片的位置
