/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
 */

#include "hal_bsp.h"
#include "hal_base.h"
#include "hal_define.h"
#include "board.h"

extern void _start(void);

static struct GIC_AMP_IRQ_INIT_CFG irqsConfig[] = {
    /* Config the irqs here. */
    // todo...

    GIC_AMP_IRQ_CFG_ROUTE(AMP0_IRQn, 0xd0, CPU_GET_AFFINITY(0, 0)),
    GIC_AMP_IRQ_CFG_ROUTE(AMP1_IRQn, 0xd0, CPU_GET_AFFINITY(1, 0)),
    GIC_AMP_IRQ_CFG_ROUTE(AMP2_IRQn, 0xd0, CPU_GET_AFFINITY(2, 0)),
    GIC_AMP_IRQ_CFG_ROUTE(AMP3_IRQn, 0xd0, CPU_GET_AFFINITY(3, 0)),

    GIC_AMP_IRQ_CFG_ROUTE(0, 0, CPU_GET_AFFINITY(1, 0)),   /* sentinel */
};

static struct GIC_IRQ_AMP_CTRL irqConfig = {
    .cpuAff = CPU_GET_AFFINITY(1, 0),
    .defPrio = 0xd0,
    .defRouteAff = CPU_GET_AFFINITY(1, 0),
    .irqsCfg = &irqsConfig[0],
};

static void SPINLOCK_Init(void)
{
#ifdef HAL_SPINLOCK_MODULE_ENABLED
    uint32_t ownerID = HAL_CPU_TOPOLOGY_GetCurrentCpuId() << 1 | 1;
    HAL_SPINLOCK_Init(ownerID);
#endif
}

HAL_WEAK void HAL_App_Init(void)
{

}

int main(void)
{
    /* HAL BASE Init */
    HAL_Init();

    /* Interrupt Init */
    HAL_GIC_Init(&irqConfig);

    /* SPINLOCK Init */
    SPINLOCK_Init();

    /* BSP Init */
    BSP_Init();

    /* Board Init */
    Board_Init();

    printf("\n");
    printf("****************************************\n");
    printf("  Hello RK3308 Bare-metal using RK_HAL! \n");
    printf("   Fuzhou Rockchip Electronics Co.Ltd   \n");
    printf("              CPI_ID(%lu)               \n", HAL_CPU_TOPOLOGY_GetCurrentCpuId());
    printf("****************************************\n");
    rk_printf(" CPU(%lu) Initial OK!\n", HAL_CPU_TOPOLOGY_GetCurrentCpuId());
    printf("\n");

    HAL_App_Init();

    while (1) {
        /* TODO: Message loop */

        /* Enter cpu idle when no message */
        HAL_CPU_EnterIdle();
    }

    return 0;
}

void _start(void)
{
    main();
}
