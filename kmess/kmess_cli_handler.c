#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kmess_cli_handler.h"


int get_cli_params(int argc, char** args, cli_handler* result)
{
    memset(result, 0, sizeof(cli_handler));
    int flag_pos = 0;

    for(int i = 1; i < argc; i++)
    {
        if (STRSW(args[i], "--"))
        {
            if (STREQ("--help", args[i]))
            {
                result->help_option = i;
            }
        }
        else if (STRSW(args[i], "-"))
        {
            int flag_size = strlen(args[i]) - 1;
            int flag_count = 0;
            if (STRCT(args[i], "m"))
            {
                flag_count++;
                result->m_option = i;
            }
            if (STRCT(args[i], "e"))
            {
                flag_count++;
                result->e_option.index = i;
            }
            if (STRCT(args[i], "v"))
            {
                flag_count++;
                result->v_option.index = i;
            }
            if (STRCT(args[i], "i"))
            {
                flag_count++;
                result->i_option = i;
            }
            if (flag_count != flag_size)
            {
                printf("kmess: unrecognized option '%s'\n", args[i]);
                return -1;
            }
        }
    }

    if (result->e_option.index != 0 || result->v_option.index != 0)
    {
        if (result->e_option.index != argc - 1)
        {
            result->e_option.pattern = args[result->e_option.index + 1];
        }
        else
        {
            printf("kmess: no searching pattern provided\n");
            return -1;
        }
        if (result->v_option.index != argc - 1)
        {
            result->v_option.pattern = args[result->v_option.index + 1];
        }
        else
        {
            printf("kmess: no invert searching pattern provided\n");
            return -1;
        }
    }

    return 0;
}