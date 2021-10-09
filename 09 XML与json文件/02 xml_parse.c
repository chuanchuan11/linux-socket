#include <stdio.h>
#include <mxml.h>

int main(int argc, const char* argv[])
{
    // 从磁盘加载xml文件
    FILE* fp = fopen("china.xml", "r");
    if(fp == NULL)
    {
        printf("fopen error\n");
        return 0;
    }
    // root 节点指向xml文件头
    mxml_node_t* root = mxmlLoadFile(NULL, fp, MXML_NO_CALLBACK);

    // 遍历 - 取出各个节点的值
    // 找到第一个城市节点
    mxml_node_t* city = mxmlFindElement(root, root, "City", NULL, NULL, MXML_DESCEND);
    if(city == NULL)
    {
        printf("xml node not found\n");
        return 0;
    }
    while( city  )
    {
        printf("==================\n");
        // 向下走一个节点
        mxml_node_t* node = mxmlWalkNext(city, root, MXML_DESCEND_FIRST);
        printf("city:   \n");
        printf("    name = %s\n", mxmlGetText(node, NULL));
        // 
        node = mxmlWalkNext(node, root, MXML_NO_DESCEND);
        printf("    area = %s\n", mxmlGetText(node, NULL));
        //
        node = mxmlWalkNext(node, root, MXML_NO_DESCEND);
        printf("    population = %s\n", mxmlGetText(node, NULL));
        // 搜索下一个城市节点
        city = mxmlFindElement(city, root, "City", NULL, NULL, MXML_DESCEND);
    }

    fclose(fp);
    mxmlDelete(root);

    return 0;
}
