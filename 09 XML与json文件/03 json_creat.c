#include <stdio.h>
#include <string.h>
#include "cJSON.h"

int main(int argc, const char* argv[])
{
    // 创建json对象
    cJSON* obj = cJSON_CreateObject(); 

    // 创建子对象 - 品牌
    cJSON* brand = cJSON_CreateObject();
    // 添加键值对
    cJSON_AddItemToObject(brand, "factory", cJSON_CreateString("一汽大众"));
    cJSON_AddItemToObject(brand, "last", cJSON_CreateNumber(31));
    cJSON_AddItemToObject(brand, "price", cJSON_CreateNumber(83));
    cJSON_AddItemToObject(brand, "sell", cJSON_CreateNumber(49));
    cJSON_AddItemToObject(brand, "sum", cJSON_CreateNumber(80));

    // 创建json数组
    cJSON* array = cJSON_CreateArray();
    cJSON_AddItemToArray(array, cJSON_CreateNumber(124));
    cJSON_AddItemToArray(array, cJSON_CreateString("hello, world"));
    cJSON_AddItemToArray(array, cJSON_CreateBool(0));
    cJSON_AddItemToObject(brand, "other", array);

    cJSON_AddItemToObject(obj, "奔驰", brand);

    // 格式化json对象
    char* text = cJSON_Print(obj);
    FILE* fp = fopen("car.json", "w");
    fwrite(text, 1, strlen(text), fp);
    fclose(fp);

    return 0;
}
