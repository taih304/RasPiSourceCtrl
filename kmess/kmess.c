#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <ctype.h>
#include "kmess_cli_handler.h"

#define MAX_EVENTS 5
#define TOLOWER(str) for (char* p = str ; *p; ++p) *p = tolower(*p);

char buffer[1024];
int epollfd, fd;
bool isMultipleLine = false;
cli_handler my_cli_hanlder;

char kmess_help[]=
"Usage: kmess [OPTION]\n"\
"   , --help     Get help for the command\n"\
" -m             Some kernel message is multi-lines, use this to display full msg\n"\
"                1 line displayed is default\n"\
" -e             use pattern for matching, not currently supporting regexp\n"\
" -v             use pattern for non-matching, not currently supporting regexp\n"\
" -i             ignore case sensitive";

void signal_action_handler(int signal_number, siginfo_t *siginfo, void *ucontext){
    close(fd);
    close(epollfd);
    exit(0);
}

int main(int argc, char** args)
{

    int result = get_cli_params(argc, args, &my_cli_hanlder);
    if (result < 0)
    {
        printf("kmess: fail to get cli params\n");
        exit(1);
    }

    if (my_cli_hanlder.help_option != 0)
    {
        printf("%s\n", kmess_help);
        exit(0);
    }

    if (my_cli_hanlder.m_option != 0)
    {
        isMultipleLine = true;
    }

    struct sigaction sa;
    sa.sa_sigaction = &signal_action_handler;
    sa.sa_flags = SA_SIGINFO;
    sigaction(SIGINT, &sa, NULL);
    fd = open("/dev/kmsg", O_RDONLY|O_NONBLOCK);
    if (fd < 0)
    {
        printf("fail to open\n");
        return -1;
    }

    // need this to fetch the latest update
    lseek(fd, 0, SEEK_END);

    struct epoll_event ev, events[MAX_EVENTS];
    int nfds;

    epollfd = epoll_create1(0);
    if (epollfd == -1) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }

    ev.events = EPOLLIN;
    ev.data.fd = fd;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
        perror("Fail to init epoll\n");
        exit(EXIT_FAILURE);
    }

    char* analize_content_msg = NULL;
    for (;;) {
        bzero(buffer, 1024);
        nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            perror("epoll_wait");
            close(epollfd);
            exit(EXIT_FAILURE);
        }
        for (int n = 0; n < nfds; ++n) {
            if (events[n].data.fd == fd) {
                int sz = read(fd, buffer, 1024);
                // the format will be sth like this
                // 4,516,1452858886,-;<kernel message>
                // string process
                char* content_msg = strstr(buffer, ";") + 1;
                free(analize_content_msg);
                if (my_cli_hanlder.m_option != 0)
                {
                    content_msg[strlen(content_msg) - 1] = 0; // strip out lats \n
                }
                else
                {
                    content_msg = strtok(content_msg, "\n");
                }

                analize_content_msg = malloc(strlen(content_msg) + 1);
                strcpy(analize_content_msg, content_msg);
                if (my_cli_hanlder.i_option != 0)
                {
                    TOLOWER(analize_content_msg);
                }

                if (my_cli_hanlder.e_option.index != 0)
                {
                    char* match_pattern = malloc(strlen(my_cli_hanlder.e_option.pattern) + 1);
                    strcpy(match_pattern, my_cli_hanlder.e_option.pattern);
                    if (my_cli_hanlder.i_option != 0)
                    {
                        TOLOWER(match_pattern);
                    }
                    bool isMatch = STRCT(analize_content_msg, match_pattern);
                    free(match_pattern);
                    if (isMatch == false)
                    {
                        continue;
                    }
                }
                if (my_cli_hanlder.i_option != 0)
                {
                    TOLOWER(analize_content_msg);
                }

                if (my_cli_hanlder.v_option.index != 0)
                {
                    char* match_pattern = malloc(strlen(my_cli_hanlder.v_option.pattern) + 1);
                    strcpy(match_pattern, my_cli_hanlder.v_option.pattern);
                    if (my_cli_hanlder.i_option != 0)
                    {
                        TOLOWER(match_pattern);
                    }
                    bool isMatch = STRNCT(analize_content_msg, match_pattern);
                    free(match_pattern);
                    if (isMatch == false)
                    {
                        continue;
                    }
                }
                char* num_str = strtok(buffer, ";");
                char* time_str = strtok(num_str, ",");
                time_str = strtok(NULL, ",");
                time_str = strtok(NULL, ",");
                long long ret = atoll(time_str);
                double time_s = ret / 1000000.0;

                // final printf
                printf("[ %.6lf] %s\n", time_s, content_msg);

            } else {
                // weird behavior
                close(epollfd);
                exit(1);
            }
        }
    }

    // program cannot get here
    return 0;
}
