#include "LocalConfig.h"

SolConfig Load_Config()
{
    SolConfig config = {
        .volume = 1.0f,
        .opacity = 1.0f,
    };

    FILE *fptr;
    fptr = fopen("Sol_Config.txt", "rb");
    if (fptr == NULL)
    {
        printf("Error opening Sol_Config.txt, loading defaults");
        return config;
    }

    fread(&config, sizeof(SolConfig), 1, fptr);
    fclose(fptr);
    return config;
}

void Save_Config(SolConfig *config)
{
    FILE *fptr;
    fptr = fopen("Sol_Config.txt", "wb+");
    if (fptr == NULL)
    {
        printf("Error opening Sol_Config.txt, loading defaults");
        return;
    }
    int flag = 0;
    flag = fwrite(config, sizeof(SolConfig), 1, fptr);
    if (flag)
    {
        printf("Data successfully written in file "
               "Sol_Config.txt\n");
    }
    else
        printf("Error writing to file");

    fclose(fptr);
}
