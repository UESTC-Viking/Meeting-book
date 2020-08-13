#include <windows.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
    char input[20] = {0,};
    printf("+-------------------------------+\n");
    printf("| 010editor http verify patcherV2 |\n");
    printf("+-------------------------------+\n");
    printf("[Patch program?(Y/N): ]\n");
    printf("-> ");
    scanf("%s", input);

    if (toupper(input[0]) == 'Y')
    {
        FILE *fp = fopen("010Editor.exe", "rb+");
        if (fp == NULL)
        {
            printf("Can't find \'010Editor.exe\' !\n");
            goto END;
        }
        
        unsigned int offsets[] = {0x00308C51, 0x00308CC4, 0x00308D02, 0x00308D17};
        unsigned char opCodes[4][15] = {{0xEB,0x71,0x90,0x90,0x90}, {0xBB,0x00,0x00,0x00,0x00,0x90,0x90,0x90}, 
                                        {0xEB,0x13,0x90,0x90,0x90}, {0xB8,0x01,0x00,0x00,0x00,0x90}};
        int codeLen[] = {5, 8, 5, 6};

        for (size_t i = 0; i < 4; i++)
        {
            fseek(fp, offsets[i], SEEK_SET);
        
            int result = fwrite(&opCodes[i][0], 1, codeLen[i], fp);
            //printf("%d\n", result);
            if (result == codeLen[i])
                printf("patch%d succeed!...\n", (i+1));
            else 
            {
                printf("Something error(Try to run as administrator)...\n");
                break;
            }
               
        }

        fclose(fp);
    }
    
END:
    printf("\nPress any key to exit...");
    getchar();getchar();
    return 0;
}
