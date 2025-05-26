#include <core.h>

void CORE_Init() {
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    Delay_Init();
    USART_Printf_Init(115200);
    Delay_Ms(2000);
}
