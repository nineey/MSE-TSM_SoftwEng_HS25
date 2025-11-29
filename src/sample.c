/*
 * Copyright (c) 2023-2024, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * Original program by Andreas Zeller.
 */

#include "sample.h"
#include "McuRTOS.h"
#include "McuLog.h"
#include "McuShell.h"
#include "McuUtility.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "McuShellCdcDevice.h"

/* sample.c -- Sample C program to be debugged */ 
static void shell_sort(int a[], int size) {
	int i, j;
	int h = 1;
	do {
		h = h * 3 + 1;
	} while (h <= size);
	do {
		h /= 3;
		for (i = h; i < size; i++) {
			int v = a[i];
			for (j = i; j >= h && a[j - h] > v; j -= h) {
        a[j] = a[j - h];
      }
			if (i != j) {
			  a[j] = v;
      }
		}
	} while (h != 1);
}

static int process(int argc, char *argv[]) {
	int *a;
	int i;
	
	a = (int *)malloc((argc - 1) * sizeof(int));
	for (i = 0; i < argc - 1; i++) {
  	a[i] = atoi(argv[i + 1]);
  }
	shell_sort(a, argc);
  McuShell_printfIO(McuShellCdcDevice_GetStdio(), "Output: ");
	for (i = 0; i < argc - 1; i++) {
		McuShell_printfIO(McuShellCdcDevice_GetStdio(), "%d ", a[i]);
  }
	McuShellCdcDevice_WriteChar('\n');
 	free(a);
	return 0;
}

uint8_t ParseCommand(const uint8_t *cmd, bool *handled, McuShell_ConstStdIOType *io) {
    uint8_t res;
    
    if (McuUtility_strcmp((char*)cmd, McuShell_CMD_HELP)==0) {
        *handled = true;
        McuShell_SendStr((unsigned char*)"Sample program. Type in numbers, separated by spaces. Press <ENTER> at the end.\r\n", io->stdOut);
    } else if (*cmd>='0' && *cmd<='9') {
        int argc;
        char *argv[64];
        unsigned char *arg;
        unsigned char *p;
        size_t len;
        
        *handled = true;
        extern void allocMem(uint32_t);
        allocMem(xTaskGetTickCount());
        argc = 0;
        argv[argc++] = "sample";
        len = McuUtility_strlen(cmd);
        arg = malloc(len+1);
        if (arg==NULL) {
            return ERR_FAILED;
        }
        McuUtility_strcpy(arg, len+1, cmd);
        p = arg;
        argv[argc++] = p++;
        do {
            if (*p == ' ') {
                *p = '\0';
                argv[argc++] = p+1;
            }
            p++;
        } while(*p!='\0' && argc<sizeof(argv)/sizeof(argv[0]));
        res = process(argc, argv);
        free(arg);
        return res;
    }
    return ERR_OK;
}

static const McuShell_ParseCommandCallback CmdParserTable[] =
{
    ParseCommand,
    NULL /* sentinel */
};

static void sampleTask(void *pv) {
    unsigned char cmdBuf[128] = "";

    for(;;) {
        McuShell_ReadAndParseWithCommandTableExt(cmdBuf, sizeof(cmdBuf), McuShellCdcDevice_GetStdio(), CmdParserTable, true);
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

void Sample_Init(void) {
  if (xTaskCreate(
      sampleTask,  /* pointer to the task */
      "Sample", /* task name for kernel awareness debugging */
      1500/sizeof(StackType_t), /* task stack size */
      (void*)NULL, /* optional task startup argument */
      tskIDLE_PRIORITY+2,  /* initial priority */
      (TaskHandle_t*)NULL /* optional task handle to create */
    ) != pdPASS)
  {
    McuLog_fatal("Failed creating task");
    for(;;){} /* error! probably out of memory */
  }
}
