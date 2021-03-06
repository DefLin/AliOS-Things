/*
 * Copyright (C) 2015-2018 Alibaba Group Holding Limited
 */
#include "board.h"
#include <aos/aos.h>
#include <aos/kernel.h>
#include <stdio.h>
#include <stdlib.h>

/*
main task stask size(byte)
*/
#define OS_MAIN_TASK_STACK 0x400
#define OS_MAIN_TASK_PRI 32

/*  For user config
    kinit.argc = 0;
    kinit.argv = NULL;
    kinit.cli_enable = 1;
*/
static kinit_t kinit = {0, NULL, 1};
static ktask_t *g_main_task;


extern void board_init(void);

extern int aos_kernel_init(kinit_t *kinit);

static void sys_init(void)
{
    /* user code start*/

    /*insert driver to enable irq for example: starting to run tick time.
     drivers to trigger irq is forbidden before aos_start, which will start core schedule.
    */
    /*user_trigger_irq();*/  //for example

    /*aos components init including middleware and protocol and so on 
    jump to app entry: application_start !*/
    aos_kernel_init(&kinit);
}

int main(void)
{
    /*irq initialized is approved here.But irq triggering is forbidden, which will enter CPU scheduling.
    Put them in sys_init which will be called after aos_start.
    Irq for task schedule should be enabled here, such as PendSV for cortex-M4.
    */
    board_init();   //including aos_heap_set();  flash_partition_init();

    /*kernel init, malloc can use after this!*/
    krhino_init();

    /*main task to run */
    krhino_task_dyn_create(&g_main_task, "main_task", 0, OS_MAIN_TASK_PRI, 0, OS_MAIN_TASK_STACK, (task_entry_t)sys_init, 1);

    /*kernel start schedule!*/
    krhino_start();

    /*never run here*/
    return 0;
}

