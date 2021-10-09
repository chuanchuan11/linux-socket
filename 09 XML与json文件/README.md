
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
