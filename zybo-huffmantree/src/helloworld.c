
#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"


int main_disable()
{
    init_platform();

    print("Hello World\n\r");
    print("Successfully ran Hello World application");


    cleanup_platform();
    return 0;
}
