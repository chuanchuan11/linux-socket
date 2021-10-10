
> XML基础语法和规范

- 基础语法

    xml文件主要以 “元素标签形式的树状结构” 进行编写，XML被设计用来结构化、存储以及传输信息

```cpp
  1. 必须有一个文件头 - 标准
      <?xml version="1.0" encoding="utf-8"?>
        ○ version不可以省略
        ○ encoding可以省略
    
  2. 正文内容
	○ 必须有一个根标签 -- (一个文件只能有一个)
	○ xml标签对大小写敏感
	○ 标签大多成对使用, 有开始, 有结束，且当标签中没有子标签时，中间则必须有值
	  <date>10-09</date>
	  <time>02:23</time>
        ○ 标签不成对使用时，需要以/结束
          <date year="2021 time=02:23"/>
      
  3. 标签中可以添加属性
    ○ <date year="2021">10-09</date>
    ○ <time during="pm">02:23</time>
    ○ 属性值必须加引号
    
  4. 标签注释
    ○ <!-- 这是注释 -->

```

- 常用开源库

    常常使用tinyxml, pugixml, minixml, libxml2等开源库对xml文件内容进行解析

    (1) minixml官网地址 
    
        ○  http://www.msweet.org/projects.php/Mini-XML
        ○ 包含头文件: mxml.h
        ○ 编译的时候需要添加动态库: libmxml.so
             § -lmxml
             § /usr/local/lib
        ○ minixml安装:
             §./configure --enable-threads=no && make
             § sudo make install

    (2) 开源库minixml的使用
    
        xml生成操作相关函数：

```cpp
	○ 跟标签节点的父亲节点是: 文件头节点
	○ 生成xml文件
		§  创建一个新的xml文件
		    mxml_node_t *mxmlNewXML(const char *version);
			□ 返回新创建的xml文件节点.
			□ 默认的文件的编码为utf8
		§ 删除节点的内存
		    void mxmlDelete(mxml_node_t *node);
		§ 添加一个新的节点
		    mxml_node_t *mxmlNewElement(
				mxml_node_t *parent,      // 父节点
				const char *name          //  新节点标签名
			);
		§ 设置节点的属性名和属性值 
			void mxmlElementSetAttr(
				mxml_node_t *node,       // 被设置属性的节点
				const char *name,        // 节点的属性名
				const char *value        // 属性值
			);
		§ 创建节点的文本内容
			mxml_node_t *mxmlNewText ( 
				mxml_node_t *parent,     // 节点地址
				int whitespace,          // 是否有空白  0
				const char *string       // 文本内容
			);
		§ 保存节点到xml文件
			int mxmlSaveFile(
				mxml_node_t *node,       // 根节点
				FILE *fp,                // 文件指针
				mxml_save_cb_t cb        // 默认MXML_NO_CALLBACK
			);
```

              解析xml文件相关函数
	      
```cpp
		§ 从文件加载xml到内存
			mxml_node_t *mxmlLoadFile(
				mxml_node_t *top,                // 一般为NULL
				FILE *fp,                        // 文件指针
				mxml_type_t (*cb)(mxml_node_t *) // 默认MXML_NO_CALLBACK
			);
		§ 获取节点的属性
			const char *mxmlElementGetAttr(
				mxml_node_t *node,      //  带属性的节点的地址
				const char *name        //  属性名
			);
		§ 获取指定节点的文本内容
			const char *mxmlGetText(
				mxml_node_t *node,      // 节点的地址
				int *whitespace         //  是否有空格
			);
		§ 跳转到下一个节点(注意：使用该函数，则原始xml文件内容需在同一行，如果在多行，则会出错)
			mxml_node_t *mxmlWalkNext(
				mxml_node_t *node,     // 当前节点
				mxml_node_t *top,      // 根节点
				int descend
			);
		□ descend：搜索的规则
			® MXML_NO_DESCEND：查看同层级
			® MXML_DESCEND_FIRST：查看下一层级的第一个
			® MXML_DESCEND：一直向下搜索
		§ 查找节点
			mxml_node_t *mxmlFindElement(
				mxml_node_t *node,        // 当前节点
				mxml_node_t *top,         //  根节点
				const char *name,         //  查找的标签名，没有属性传NULL
				const char *attr,         //  查找的标签的属性, 没有属性传NULL
				const char *value,        //  查找的标签的属性值, 没有属性传NULL
				int descend               //  同上
			);
```

- 示例

    (1)生成xml文件
    (2)解析xml文件
    



参考：

（1）https://www.runoob.com/xml/xml-tutorial.html

> json文件格式

    json文件常用三种表示方法：数组，对象和数组对象组合

```cpp
	- json数组
                ○ []表示法
		○ 中括号中为各种数据，如：[整形, 字符串, 布尔类型, json数组]，不同元素之间用逗号分隔
		○ 示例： [123, 123.2, true, false, [12, 34, 56, "hello, world"]]
		○ 最外层只能有一个中括号
		
	- json对象
		○ {}表示法
		○ 花括号中是一些键值对，如
			{
			    "name":"zhang3", 
			    "name2":"li4"
			}
		    § key值: 必须是字符串, 且不重复
		    § value值: json对象, json数组, 布尔, 整形, 字符串
		    § 最外层只能有一个花括号
		    
	- json数组+json对象
	        ○ 为数组和对象的组合，如：
	    {
		"name":"zhang3", 
		"name2":"li4",
		"张三":{
			"别名":"老王",
			"性别":"男",
			"年龄":34,
			"孩子":["小红", "小绿", "小黑"]
			}
	    }
```

> cjson开源库解析json文件

- 生成json文件

```cpp
	○ 创建一个json对象
		cJSON *cJSON_CreateObject(void);
	○ 往json对象中添加数据成员
		void cJSON_AddItemToObject(
			cJSON *object,              //  json对象
			const char *string,         //  key值
			cJSON *item                 //  value值（int，string，array，obj）
		);
	○ 创建一个整型值
		cJSON *cJSON_CreateNumber(double num);
	○ 创建一个字符串
		cJSON *cJSON_CreateString(const char *string);
	○ 创建一个json数组
		cJSON *cJSON_CreateArray(void); -- 空数组
	○ 创建默认有count个整形值的json数组
		cJSON *cJSON_CreateIntArray(const int *numbers,int count);
			§ int arry[] = {8,3,4,5,6};
			§ cJSON_CreateIntArray(arry, 5);
	○ 往json数组中添加数据成员
		void cJSON_AddItemToArray(cJSON *array, cJSON *item);
	○ 释放jSON结构指针
		void cJSON_Delete(cJSON *c)
	○ 将JSON结构转化为字符串
		char *cJSON_Print(cJSON *item);
			§ 返回值需要使用free释放
			§ FILE* fp = fopen();
			§ fwrite();
			§ fclose();
```

- 解析json文件

```cpp
	○ 将字符串解析为JSON结构
		cJSON *cJSON_Parse(const char *value);
			§ 返回值需要使用cJSON_Delete释放
	○ 根据键值查找json节点
		cJSON *cJSON_GetObjectItem(
			cJSON *object,           // 当前json对象
			const char *string       //  key值
		);
	○ 获取json数组中元素的个数
		int cJSON_GetArraySize(cJSON *array);
	○ 根据数组下标找到对应的数组元素
		cJSON *cJSON_GetArrayItem(cJSON *array, int index);
	○ 判断是否有可以值对应的键值对
		int cJSON_HasObjectItem(cJSON *object, const char *string)
```





