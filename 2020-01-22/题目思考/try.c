#include<stdio.h>
#include<string.h>
int main (){ 
    char b[24]={0x78, 0x49 ,0x72 ,0x43 ,0x6A ,0x7E ,0x3C ,0x72 ,0x7C ,0x32 ,0x74 ,
            0x57 ,0x73 ,0x76 ,0x33 ,0x50 ,0x74 ,0x49 ,0x7F ,0x7A ,0x6E,0x64, 0x6B ,0x61 };
    int c = 23;
    for(c=0;c<24;c++){
        b[c] = b[c]^6-1;
    }
    printf("%s",b);
    printf("%d",'N');
    printf("%d",0x49^6);
    
}