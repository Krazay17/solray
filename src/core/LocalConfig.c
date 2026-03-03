#include "LocalConfig.h"

FILE *fptr;

SolConfig Load_Config()
{
    const char *test = "TestData";
    SolConfig config = {
        .volume = 1.0f,
        .opacity = 1.0f,
    };
    fptr = fopen("Sol_Config.txt", "rb");
    if (fptr == NULL)
    {
        printf("Error opening Sol_Config.txt, loading defaults");
    }
    else
    {
        fread(&config, sizeof(SolConfig), 1, fptr);
    }
    fclose(fptr);
    return config;
}

void Save_Config(SolConfig *config)
{
    fptr = fopen("Sol_Config.txt", "wb+");
    if (fptr == NULL)
    {
        printf("Error opening Sol_Config.txt, loading defaults");
    }
    else
    {
        int flag = 0;
        flag = fwrite(config, sizeof(SolConfig), 1, fptr);
        if (flag)
        {
            printf("Data successfully written in file "
                   "Sol_Config.txt\n");
        }
        else
            printf("Error writing to file");
    }
    fclose(fptr);
}
