#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <cs50.h>

typedef uint8_t  BYTE;
BYTE buffer[512];
int BLOCK_SIZE = 512;
bool file000 = true;


int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("usage: ./recover DATA.raw\n");
        return 1;
    }
    else if (fopen(argv[1], "r") == NULL)
    {
        printf("cannot find file %s\n", argv[1]);
        return 1;
    }

    int i = 0;
    char file_name[10];
    sprintf(file_name,"%03i.jpg", i);
    FILE *c_file = fopen(file_name, "w");
    fclose(c_file);

    FILE *raw_file = fopen(argv[1], "r");
    while (fread(buffer, 1, BLOCK_SIZE, raw_file) == BLOCK_SIZE)
    {
        if(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if(file000 == false)
            {
                i++;
                sprintf(file_name,"%03i.jpg", i);
                c_file = fopen(file_name, "w");
                fwrite(buffer, 1, BLOCK_SIZE, c_file);
            }
            else
            {
                c_file = fopen(file_name, "w");
                file000 = false;
                fwrite(buffer, 1, BLOCK_SIZE, c_file);
            }
            fclose(c_file);
        }
        else if(file000 == false)
        {
            c_file = fopen(file_name, "a");
            fwrite(buffer, 1, BLOCK_SIZE, c_file);
            fclose(c_file);
        }
    }

    fclose(raw_file);
    return 0;
}