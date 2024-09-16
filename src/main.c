/*
 * main.c - main program of the v-rex-term program
 *      A terminal client for Docker using the clipdocker library.
 *
 * author - Abhishek Mishra
 * date - 16/09/2024
 */

#include <stdio.h>
#include <stdlib.h>
#include "docker_all.h"

int vrt_init();
void vrt_quit();

int main(int argc, char *argv[])
{
    printf("v-rex-term\n");

    if(vrt_init() != 0)
    {
        exit(1);
    }

    vrt_quit();

    exit(0);
}

int vrt_init()
{
    d_err_t res = docker_api_init();

    if (res == 0)
    {
        printf("docker api initialized.\n");
        return 0;
    }
    else
    {
        printf("docker api initialization error!\n");
        return 1;
    }
}

void vrt_quit()
{
    docker_api_cleanup();
}
