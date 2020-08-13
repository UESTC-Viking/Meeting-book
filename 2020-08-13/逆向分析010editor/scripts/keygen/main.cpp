#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <time.h>
#include "main.h"

using namespace std;

struct stKey
{
    string name;
    string password;
    uint8_t hex[10];
} userInfo;

uint32_t getValue(int num1, int num2)
{
    uint32_t ret = 0;
    uint8_t j = 15 * num2, k = 0, m = 17 * num1;
    // toUpper
    //transform(userInfo.name.begin(), userInfo.name.end(), userInfo.name.begin(), ::toupper);

    // calc
    for (size_t i = 0; i < userInfo.name.size(); i++)
    {
        char ch = toupper(userInfo.name[i]);
        ret = datas[j] + datas[m] + datas[k] + datas[ch + 47] * ((ret + datas[ch]) ^ datas[ch + 13]);

        j += 13;
        m += 9;
        k += 19;
    }
    return ret;
}

int main(int argc, char const *argv[])
{
    int yearNum = 0, index1 = 0, index2 = 0;
    userInfo.name = "test";

    printf("+------------------+\n");
    printf("| 010editor keygen |\n");
    printf("+------------------+\n");

    cout << "Enter name: " << endl << "-> ";
    cin >> userInfo.name;
    cout << "Enter License valid years(since 2019-12-7): " << endl << "-> ";
    cin >> yearNum;

    srand(time(0));

    vector<int> num1, num2, num1Org, num2Org;
    for (int i = 1; i < 0x3E8*0xB; i++)
    {
        if (i % 0xB == 0)
            num2.push_back(i);
    }
    for (auto it = num2.begin(); it != num2.end(); it++)
    {
        num2Org.push_back((uint16_t)(((*it ^ 0x3421) - 0x4D30) ^ 0x7892));
    }
    for (int i = 0x473C*0x11; i < 0x00FFFFFF; i++) // 可修改范围 从2019-12-7开始
    {
        if (i % 0x11 == 0)
            num1.push_back(i);
    }
    for (auto it = num1.begin(); it != num1.end(); it++)
    {
        unsigned int tmp = ((*it ^ 0xFFE53167) + 0x2C175) ^ 0x5B8C27 ^ 0x22C078;
        num1Org.push_back(tmp & 0x00FFFFFF);
    }

    // setNumIndex
    index1 = yearNum*365 + rand()%150;
    if (index1 >= num1.size())
        index1 = 6*365 + rand()%30; // 6 years
    index2 = rand()%num2.size();

    uint32_t value = getValue(num1[index1]/0x11, num2[index2]/0xB);
    //printf("Value:%X\n", value);
    for (size_t i = 0; i < 4; i++) {
        userInfo.hex[4+i] = (uint8_t)(value >> 8*i);
    }
    userInfo.hex[3] = 0xAC;
    
    userInfo.hex[9] = (uint8_t)(num1Org[index1] >> 16) ^ userInfo.hex[5];
    userInfo.hex[8] = (uint8_t)(num1Org[index1] >> 8) ^ userInfo.hex[4];
    userInfo.hex[0] = (uint8_t)(num1Org[index1]) ^ userInfo.hex[6];

    userInfo.hex[1] = (uint8_t)(num2Org[index2] >> 8) ^ userInfo.hex[7];
    userInfo.hex[2] = (uint8_t)(num2Org[index2]) ^ userInfo.hex[5];

    // print name
    printf("----------------------------------------\n");
    printf("Name: %s\n", userInfo.name.c_str());

    // print password
    printf("PassWord: ");
    for (size_t i = 0; i < 10; i++)
    {
        if (i%2 == 0 && i != 0)
            printf("-");
        printf("%02X", userInfo.hex[i]);
    }
    printf("\n");    

    // print due time
    struct tm *dueTime;
    time_t timeValue = (unsigned int)(num1[index1]/0x11*86400);
    dueTime = localtime(&timeValue);
    printf("DueTime: %d-%d-%d\n", dueTime->tm_year+1900, dueTime->tm_mon+1, dueTime->tm_mday+1);
    printf("----------------------------------------\n");

    getchar();getchar();
    return 0;
}
