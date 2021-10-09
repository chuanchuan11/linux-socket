#include <stdio.h>
#include <mxml.h>

int main()
{
    // 创建xml文件头节点
    mxml_node_t *xml = mxmlNewXML("1.0");

    // 创建xml根节点 - china
    mxml_node_t* china = mxmlNewElement(xml, "China");

    // 创建城市节点:1
    mxml_node_t* city = mxmlNewElement(china, "City");
    // 添加子节点
    // name
    mxml_node_t* name = mxmlNewElement(city, "Name");
    // 设置标签值
    mxmlNewText(name, 0, "北京");
    mxmlElementSetAttr(name, "isbig", "true");
    // 面积
    mxml_node_t* area = mxmlNewElement(city, "Area");
    mxmlNewText(area, 0, "1.641万平方千米");
    // 人口
    mxml_node_t* popu = mxmlNewElement(city, "Population");
    mxmlNewText(popu, 0, "2200万");


    // 创建城市节点:2
    city = mxmlNewElement(china, "City");
    // 添加子节点
    // name
    name = mxmlNewElement(city, "Name");
    // 设置标签值
    mxmlNewText(name, 0, "石家庄");
    mxmlElementSetAttr(name, "isbig", "false");
    area = mxmlNewElement(city, "Area");
    mxmlNewText(area, 0, "15848平方千米");
    popu = mxmlNewElement(city, "Population");
    mxmlNewText(popu, 0, "107万");

    // 将xml内容保存到磁盘
    FILE* fp = fopen("china.xml", "w");
    mxmlSaveFile(xml, fp, MXML_NO_CALLBACK);
    fclose(fp);
    mxmlDelete(xml);
    
    return 0;
}
