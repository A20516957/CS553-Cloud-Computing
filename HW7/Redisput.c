#include <stdio.h>
#include <stdlib.h>


int main(){
    system("ls");
    FILE* TestFile;
    char ch;
    TestFile = fopen("data2.in", "r");

    if (NULL == TestFile)
    {
        printf("file can't be opened \n");
    }

    printf("content of this file are \n");
    char key[10] = "";
    char value[90] = "";

    int a = 0;
    int b = 0;
    int c = 0;
    do
    {
        ch = fgetc(TestFile);
        if(i<10){
          key[i] = ch;
        i++;
        }
        if((c>11)&&(j<87))
        {
            value[j]=ch;
            j++;
        }

        c++;
        if(c>98)
        {
            printf("key: %s\n",key);
            printf("value: %s\n",value);
            a=0;
            b=0;
            c=0;
            system("jc.set(key,value)");
            
    } while (ch != EOF);

    fclose(TestFile);
    return 0;

}
