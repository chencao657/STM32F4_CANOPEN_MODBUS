/*
*********************************************************************************************************
*
*	模块名称 : 硬件定时器模块
*	文件名称 : 
*	版    本 : 
*	说    明 : 用于开启硬件定时器与单次微秒级定时器
*
*	修改记录 :
*		版本号      日期         作者              说明
*		V1.0    2025-05-23  shisanxiang1        安富莱移植
*
*********************************************************************************************************
*/


#include "modbus_tim.h"

/*
	定义用于硬件定时器的TIM， 可以使 TIM2 - TIM5
*/
#define USE_TIM2
//#define USE_TIM3
//#define USE_TIM4
//#define USE_TIM5

#ifdef USE_TIM2
	#define TIM_HARD					TIM2
	#define	RCC_TIM_HARD_CLK_ENABLE()	__HAL_RCC_TIM2_CLK_ENABLE()
	#define TIM_HARD_IRQn				TIM2_IRQn
	#define TIM_HARD_IRQHandler			TIM2_IRQHandler
#endif

#ifdef USE_TIM3
	#define TIM_HARD					TIM3
	#define	RCC_TIM_HARD_CLK_ENABLE()	__HAL_RCC_TIM3_CLK_ENABLE()	
	#define TIM_HARD_IRQn				TIM3_IRQn
	#define TIM_HARD_IRQHandler			TIM3_IRQHandler
#endif

#ifdef USE_TIM4
	#define TIM_HARD					TIM4
	#define	RCC_TIM_HARD_CLK_ENABLE()	__HAL_RCC_TIM4_CLK_ENABLE()
	#define TIM_HARD_IRQn				TIM4_IRQn
	#define TIM_HARD_IRQHandler			TIM4_IRQHandler
#endif

#ifdef USE_TIM5
	#define TIM_HARD					TIM5
	#define	RCC_TIM_HARD_CLK_ENABLE()	__HAL_RCC_TIM5_CLK_ENABLE()
	#define TIM_HARD_IRQn				TIM5_IRQn
	#define TIM_HARD_IRQHandler			TIM5_IRQHandler
#endif

/* 保存 TIM定时中断到后执行的回调函数指针 */
static void (*s_TIM_CallBack1)(void);
static void (*s_TIM_CallBack2)(void);
static void (*s_TIM_CallBack3)(void);
static void (*s_TIM_CallBack4)(void);

#ifdef TIM_HARD

/*
*********************************************************************************************************
*	函 数 名: bsp_InitHardTimer
*	功能说明: 配置 TIMx，用于us级别硬件定时。TIMx将自由运行，永不停止.
*			TIMx可以用TIM2 - TIM5 之间的TIM, 这些TIM有4个通道, 挂在 APB1 上，输入时钟=SystemCoreClock / 2
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitHardTimer(void)
{
    TIM_HandleTypeDef  TimHandle = {0};
	uint32_t usPeriod;
	uint16_t usPrescaler;
	uint32_t uiTIMxCLK;
	TIM_TypeDef* TIMx = TIM_HARD;
	
	RCC_TIM_HARD_CLK_ENABLE();		/* 使能TIM时钟 */
	
    /*-----------------------------------------------------------------------
		system_stm32f4xx.c 文件中 void SetSysClock(void) 函数对时钟的配置如下：

		HCLK = SYSCLK / 1     (AHB1Periph)
		PCLK2 = HCLK / 2      (APB2Periph)
		PCLK1 = HCLK / 4      (APB1Periph)

		因为APB1 prescaler != 1, 所以 APB1上的TIMxCLK = PCLK1 x 2 = SystemCoreClock / 2;
		因为APB2 prescaler != 1, 所以 APB2上的TIMxCLK = PCLK2 x 2 = SystemCoreClock;

		APB1 定时器有 TIM2, TIM3 ,TIM4, TIM5, TIM6, TIM7, TIM12, TIM13,TIM14
		APB2 定时器有 TIM1, TIM8 ,TIM9, TIM10, TIM11

	----------------------------------------------------------------------- */
	uiTIMxCLK = SystemCoreClock / 2;

	usPrescaler = uiTIMxCLK / 1000000 - 1;	/* 分频比 = 1 */
	
	if (TIMx == TIM2 || TIMx == TIM5)
	{
		usPeriod = 0xFFFFFFFF;
	}
	else
	{
		usPeriod = 0xFFFF;
	}

	/* 
       设置分频为usPrescaler后，那么定时器计数器计1次就是1us
       而参数usPeriod的值是决定了最大计数：
       usPeriod = 0xFFFF 表示最大0xFFFF微秒。
       usPeriod = 0xFFFFFFFF 表示最大0xFFFFFFFF微秒。
    */
	TimHandle.Instance = TIMx;
	TimHandle.Init.Prescaler         = usPrescaler;
	TimHandle.Init.Period            = usPeriod;
	TimHandle.Init.ClockDivision     = 0;
	TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
	TimHandle.Init.RepetitionCounter = 0;
    TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	
	if (HAL_TIM_Base_Init(&TimHandle) != HAL_OK)
	{
		//错误
	}

	/* 配置定时器中断，给CC捕获比较中断使用 */
	{
		HAL_NVIC_SetPriority(TIM_HARD_IRQn, 0, 2);
		HAL_NVIC_EnableIRQ(TIM_HARD_IRQn);	
	}
    
    /* 启动定时器 */
	HAL_TIM_Base_Start(&TimHandle);
}

/*
*********************************************************************************************************
*	函 数 名: bsp_StartHardTimer
*	功能说明: 使用TIM2-5做单次定时器使用, 定时时间到后执行回调函数。可以同时启动4个定时器通道，互不干扰。
*             定时精度正负1us （主要耗费在调用本函数的执行时间）
*			  TIM2和TIM5 是32位定时器。定时范围很大
*			  TIM3和TIM4 是16位定时器。
*	形    参: _CC : 捕获比较通道几，1，2，3, 4
*             _uiTimeOut : 超时时间, 单位 1us. 对于16位定时器，最大 65.5ms; 对于32位定时器，最大 4294秒
*             _pCallBack : 定时时间到后，被执行的函数
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_StartHardTimer(uint8_t _CC, uint32_t _uiTimeOut, void * _pCallBack)
{
    uint32_t cnt_now;
    uint32_t cnt_tar;
	TIM_TypeDef* TIMx = TIM_HARD;
	
    /* 无需补偿延迟，实测精度正负1us */
    
    cnt_now = TIMx->CNT; 
    cnt_tar = cnt_now + _uiTimeOut;			/* 计算捕获的计数器值 */
    if (_CC == 1)
    {
        s_TIM_CallBack1 = (void (*)(void))_pCallBack;

		TIMx->CCR1 = cnt_tar; 			    /* 设置捕获比较计数器CC1 */
        TIMx->SR = (uint16_t)~TIM_IT_CC1;   /* 清除CC1中断标志 */
		TIMx->DIER |= TIM_IT_CC1;			/* 使能CC1中断 */
			
	}
    else if (_CC == 2)
    {
		s_TIM_CallBack2 = (void (*)(void))_pCallBack;

		TIMx->CCR2 = cnt_tar;				/* 设置捕获比较计数器CC2 */
        TIMx->SR = (uint16_t)~TIM_IT_CC2;	/* 清除CC2中断标志 */
		TIMx->DIER |= TIM_IT_CC2;			/* 使能CC2中断 */
    }
    else if (_CC == 3)
    {
        s_TIM_CallBack3 = (void (*)(void))_pCallBack;

		TIMx->CCR3 = cnt_tar;				/* 设置捕获比较计数器CC3 */
        TIMx->SR = (uint16_t)~TIM_IT_CC3;	/* 清除CC3中断标志 */
		TIMx->DIER |= TIM_IT_CC3;			/* 使能CC3中断 */
    }
    else if (_CC == 4)
    {
        s_TIM_CallBack4 = (void (*)(void))_pCallBack;

		TIMx->CCR4 = cnt_tar;				/* 设置捕获比较计数器CC4 */
        TIMx->SR = (uint16_t)~TIM_IT_CC4;	/* 清除CC4中断标志 */
		TIMx->DIER |= TIM_IT_CC4;			/* 使能CC4中断 */
    }
	else
    {
        return;
    }
}

/*
*********************************************************************************************************
*	函 数 名: TIMx_IRQHandler
*	功能说明: TIM 中断服务程序
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void TIM_HARD_IRQHandler(void)
{
	uint16_t itstatus = 0x0, itenable = 0x0;
	TIM_TypeDef* TIMx = TIM_HARD;
	
    
  	itstatus = TIMx->SR & TIM_IT_CC1;
	itenable = TIMx->DIER & TIM_IT_CC1;
    
	if ((itstatus != (uint16_t)RESET) && (itenable != (uint16_t)RESET))
	{
		TIMx->SR = (uint16_t)~TIM_IT_CC1;
		TIMx->DIER &= (uint16_t)~TIM_IT_CC1;		/* 禁能CC1中断 */	

        /* 先关闭中断，再执行回调函数。因为回调函数可能需要重启定时器 */
        s_TIM_CallBack1();
    }

	itstatus = TIMx->SR & TIM_IT_CC2;
	itenable = TIMx->DIER & TIM_IT_CC2;
	if ((itstatus != (uint16_t)RESET) && (itenable != (uint16_t)RESET))
	{
		TIMx->SR = (uint16_t)~TIM_IT_CC2;
		TIMx->DIER &= (uint16_t)~TIM_IT_CC2;		/* 禁能CC2中断 */	

        /* 先关闭中断，再执行回调函数。因为回调函数可能需要重启定时器 */
        s_TIM_CallBack2();
    }

	itstatus = TIMx->SR & TIM_IT_CC3;
	itenable = TIMx->DIER & TIM_IT_CC3;
	if ((itstatus != (uint16_t)RESET) && (itenable != (uint16_t)RESET))
	{
		TIMx->SR = (uint16_t)~TIM_IT_CC3;
		TIMx->DIER &= (uint16_t)~TIM_IT_CC3;		/* 禁能CC2中断 */	

        /* 先关闭中断，再执行回调函数。因为回调函数可能需要重启定时器 */
        s_TIM_CallBack3();
    }

	itstatus = TIMx->SR & TIM_IT_CC4;
	itenable = TIMx->DIER & TIM_IT_CC4;
	if ((itstatus != (uint16_t)RESET) && (itenable != (uint16_t)RESET))
	{
		TIMx->SR = (uint16_t)~TIM_IT_CC4;
		TIMx->DIER &= (uint16_t)~TIM_IT_CC4;		/* 禁能CC4中断 */	

        /* 先关闭中断，再执行回调函数。因为回调函数可能需要重启定时器 */
        s_TIM_CallBack4();
    }	
}

#endif
