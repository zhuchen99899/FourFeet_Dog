#include <stdio.h>
#include "my_comp.h"

void my_comp_test(void)
{
#if(CONFIG_MY_COMP_ENABLE == y)
printf("my_comp is configed\n");
#else
printf("not config");
#endif
}
