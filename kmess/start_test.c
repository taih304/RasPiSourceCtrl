#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "kmess_cli_handler.h"
#include <ctype.h>
#include <string.h>

char params1[] = "kmess -eimv --help";

#define TOLOWER(str) for (char* p = str ; *p; ++p) *p = tolower(*p);

cli_handler my_cli_hanlder;

int main(int argc, char** args)
{
    char** double_ptr;
    double_ptr = malloc(sizeof(char*));
    int count = 0;
    char* temp = strtok(params1, " ");
    do
    {
        double_ptr[count++] = temp;
        temp = strtok(NULL, " ");
        if (temp != NULL)
        {
            double_ptr = realloc(double_ptr, sizeof(char*)*(count+1));
        }
    }
    while (temp != NULL);

    int result = get_cli_params(count, double_ptr, &my_cli_hanlder);
    if (result < 0)
    {
        printf("Fail to get cli params\n");
        return -1;
    }

    if (my_cli_hanlder.m_option != 0) printf("m option avail\n");
    if (my_cli_hanlder.help_option != 0) printf("help option avail\n");
    if (my_cli_hanlder.i_option != 0) printf("i option avail\n");
    if (my_cli_hanlder.e_option.index != 0)
    {
        printf("e option avail, pattern: %s\n", my_cli_hanlder.e_option.pattern);
    }
    if (my_cli_hanlder.v_option.index != 0)
    {
        printf("v option avail, pattern: %s\n", my_cli_hanlder.v_option.pattern);
    }

    free(double_ptr);

    return 0;
}