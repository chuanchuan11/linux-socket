#include <stdio.h>
#include <string.h>
#include "cJSON.h"

int main(int argc, const char* argv[])
{
    if(argc < 2)
    {
        printf("./a.out jsonfile\n");
        return 0;
    }

    // 加载json文件 
    FILE* fp = fopen(argv[1], "r");
    char buf[1024] = {0};
    fread(buf, 1, sizeof(buf), fp);
    cJSON* root = cJSON_Parse(buf);

    cJSON* subobj = cJSON_GetObjectItem(root, "奔驰");
    // 判断对象是否存在
    if( subobj )
    {
        // 获取子对象
        cJSON* factory = cJSON_GetObjectItem(subobj, "factory");
        cJSON* last = cJSON_GetObjectItem(subobj, "last");
        cJSON* price = cJSON_GetObjectItem(subobj, "price");
        cJSON* sell = cJSON_GetObjectItem(subobj, "sell");
        cJSON* sum = cJSON_GetObjectItem(subobj, "sum");
        cJSON* other = cJSON_GetObjectItem(subobj, "other");

        // 打印value值
        printf("奔驰：\n");
        printf("    factory: %s\n", cJSON_Print(factory));
        printf("    last: %s\n", cJSON_Print(last));
        printf("    price: %s\n", cJSON_Print(price));
        printf("    sell: %s\n", cJSON_Print(sell));
        printf("    sum: %s\n", cJSON_Print(sum));

        // 打印数组内容
        printf("    other:\n");
        if(other->type == cJSON_Array)
        {
            for(int i=0; i<cJSON_GetArraySize(other); ++i)
            {
                cJSON* node = cJSON_GetArrayItem(other, i);
                // 判断数据类型
                if(node->type == cJSON_String)
                {
                    printf("        %s  \n", node->valuestring);
                }
                if(node->type == cJSON_Number)
                {
                    printf("        %d\n", node->valueint);
                }
                if(node->type == cJSON_True)
                {
                    printf("        %d\n", node->valueint);
                }
                if(node->type == cJSON_False)
                {
                    printf("        %d\n", node->valueint);
                }
            }
        }
    }

    cJSON_Delete(root);
    fclose(fp);


    return 0;
}
