## Java中的容器介绍

#### Java容器类类库两个不同的概念:

### Collection:一个独立元素的序列,包括List,Set,Queue

#### List介绍:

ArrayList:在List的中间插入和移除元素时较慢

LinkedList:通过代价较低的在List中间进行的插入和删除操作,提供优化顺序访问

比较:LinkedList在随机访问方面相对较慢

#### Set介绍:

HashSet:最快获取元素的方式

TreeSet:存储顺序很重要,按照比较结果的升序保存对象

LinkedHashSet:按照被添加的顺序保存对象

### Map:一组成对的"键值对"对象

HashMap:最快的查找技术

TreeMap:按照比较结果的升序保存键

LinkedListMap:插入顺序保存键,保留HashMap的查询速度

参考代码:

    import java.util.*;
    public class PrintContainers 
    {
        static Collection fill(Collection<String> collection) 
        {
            collection.add("rat");
            collection.add("cat");
            collection.add("dog");
            collection.add("dog");
            return collection;
        }
        static Map fill(Map<String, String> map) 
        {
            map.put("rat", "米老鼠");
            map.put("cat", "喵瞄");
            map.put("dog", "拉拉");
            map.put("dog", "灰灰");
            return map;
        }
    public static void main(String[] args) {
            print(fill(new ArrayList<String>()));
            print(fill(new LinkedList<String>()));
            print(fill(new HashSet<String>()));
            print(fill(new TreeSet<String>()));
            print(fill(new LinkedHashSet<String>()));
            print(fill(new HashMap<String,String>()));
            print(fill(new TreeMap<String,String>()));
            print(fill(new LinkedHashMap<String,String>()));
         }
    }
#### 迭代器:

迭代器(Iterator)模式，又叫做游标模式，它的含义是，提供一种方法访问一个容器对象中各个元素，而又不需暴露该对象的内部细节。

代码:

	public class Test3 
	{
	    public static void main(String[] args) 
	    {
		    List<String>list=new ArrayList<>();
		    list.add("a");
		    list.add("b");
		    Iterator<String>it=list.iterator();//得到lits的迭代器
		    //调用迭代器的hasNext方法，判断是否有下一个元素
		    while (it.hasNext()) 
		    {
		       //将迭代器的下标移动一位，并得到当前位置的元素值
			   System.out.println(it.next());	
		    }	
		}
	}
#### for each语法:

for(初始化; 布尔表达式; 更新) {
    //代码语句
}

for(元素类型t 元素变量x : 遍历对象obj){ 
     引用了x的java语句; 
} 