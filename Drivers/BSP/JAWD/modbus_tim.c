/*
*********************************************************************************************************
*
*	ģ������ : Ӳ����ʱ��ģ��
*	�ļ����� : 
*	��    �� : 
*	˵    �� : ���ڿ���Ӳ����ʱ���뵥��΢�뼶��ʱ��
*
*	�޸ļ�¼ :
*		�汾��      ����         ����              ˵��
*		V1.0    2025-05-23  shisanxiang1        ��������ֲ
*
*********************************************************************************************************
*/


#include "modbus_tim.h"

/*
	��������Ӳ����ʱ����TIM�� ����ʹ TIM2 - TIM5
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

/* ���� TIM��ʱ�жϵ���ִ�еĻص�����ָ�� */
static void (*s_TIM_CallBack1)(void);
static void (*s_TIM_CallBack2)(void);
static void (*s_TIM_CallBack3)(void);
static void (*s_TIM_CallBack4)(void);

#ifdef TIM_HARD

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitHardTimer
*	����˵��: ���� TIMx������us����Ӳ����ʱ��TIMx���������У�����ֹͣ.
*			TIMx������TIM2 - TIM5 ֮���TIM, ��ЩTIM��4��ͨ��, ���� APB1 �ϣ�����ʱ��=SystemCoreClock / 2
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitHardTimer(void)
{
    TIM_HandleTypeDef  TimHandle = {0};
	uint32_t usPeriod;
	uint16_t usPrescaler;
	uint32_t uiTIMxCLK;
	TIM_TypeDef* TIMx = TIM_HARD;
	
	RCC_TIM_HARD_CLK_ENABLE();		/* ʹ��TIMʱ�� */
	
    /*-----------------------------------------------------------------------
		system_stm32f4xx.c �ļ��� void SetSysClock(void) ������ʱ�ӵ��������£�

		HCLK = SYSCLK / 1     (AHB1Periph)
		PCLK2 = HCLK / 2      (APB2Periph)
		PCLK1 = HCLK / 4      (APB1Periph)

		��ΪAPB1 prescaler != 1, ���� APB1�ϵ�TIMxCLK = PCLK1 x 2 = SystemCoreClock / 2;
		��ΪAPB2 prescaler != 1, ���� APB2�ϵ�TIMxCLK = PCLK2 x 2 = SystemCoreClock;

		APB1 ��ʱ���� TIM2, TIM3 ,TIM4, TIM5, TIM6, TIM7, TIM12, TIM13,TIM14
		APB2 ��ʱ���� TIM1, TIM8 ,TIM9, TIM10, TIM11

	----------------------------------------------------------------------- */
	uiTIMxCLK = SystemCoreClock / 2;

	usPrescaler = uiTIMxCLK / 1000000 - 1;	/* ��Ƶ�� = 1 */
	
	if (TIMx == TIM2 || TIMx == TIM5)
	{
		usPeriod = 0xFFFFFFFF;
	}
	else
	{
		usPeriod = 0xFFFF;
	}

	/* 
       ���÷�ƵΪusPrescaler����ô��ʱ����������1�ξ���1us
       ������usPeriod��ֵ�Ǿ�������������
       usPeriod = 0xFFFF ��ʾ���0xFFFF΢�롣
       usPeriod = 0xFFFFFFFF ��ʾ���0xFFFFFFFF΢�롣
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
		//����
	}

	/* ���ö�ʱ���жϣ���CC����Ƚ��ж�ʹ�� */
	{
		HAL_NVIC_SetPriority(TIM_HARD_IRQn, 0, 2);
		HAL_NVIC_EnableIRQ(TIM_HARD_IRQn);	
	}
    
    /* ������ʱ�� */
	HAL_TIM_Base_Start(&TimHandle);
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_StartHardTimer
*	����˵��: ʹ��TIM2-5�����ζ�ʱ��ʹ��, ��ʱʱ�䵽��ִ�лص�����������ͬʱ����4����ʱ��ͨ�����������š�
*             ��ʱ��������1us ����Ҫ�ķ��ڵ��ñ�������ִ��ʱ�䣩
*			  TIM2��TIM5 ��32λ��ʱ������ʱ��Χ�ܴ�
*			  TIM3��TIM4 ��16λ��ʱ����
*	��    ��: _CC : ����Ƚ�ͨ������1��2��3, 4
*             _uiTimeOut : ��ʱʱ��, ��λ 1us. ����16λ��ʱ������� 65.5ms; ����32λ��ʱ������� 4294��
*             _pCallBack : ��ʱʱ�䵽�󣬱�ִ�еĺ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_StartHardTimer(uint8_t _CC, uint32_t _uiTimeOut, void * _pCallBack)
{
    uint32_t cnt_now;
    uint32_t cnt_tar;
	TIM_TypeDef* TIMx = TIM_HARD;
	
    /* ���貹���ӳ٣�ʵ�⾫������1us */
    
    cnt_now = TIMx->CNT; 
    cnt_tar = cnt_now + _uiTimeOut;			/* ���㲶��ļ�����ֵ */
    if (_CC == 1)
    {
        s_TIM_CallBack1 = (void (*)(void))_pCallBack;

		TIMx->CCR1 = cnt_tar; 			    /* ���ò���Ƚϼ�����CC1 */
        TIMx->SR = (uint16_t)~TIM_IT_CC1;   /* ���CC1�жϱ�־ */
		TIMx->DIER |= TIM_IT_CC1;			/* ʹ��CC1�ж� */
			
	}
    else if (_CC == 2)
    {
		s_TIM_CallBack2 = (void (*)(void))_pCallBack;

		TIMx->CCR2 = cnt_tar;				/* ���ò���Ƚϼ�����CC2 */
        TIMx->SR = (uint16_t)~TIM_IT_CC2;	/* ���CC2�жϱ�־ */
		TIMx->DIER |= TIM_IT_CC2;			/* ʹ��CC2�ж� */
    }
    else if (_CC == 3)
    {
        s_TIM_CallBack3 = (void (*)(void))_pCallBack;

		TIMx->CCR3 = cnt_tar;				/* ���ò���Ƚϼ�����CC3 */
        TIMx->SR = (uint16_t)~TIM_IT_CC3;	/* ���CC3�жϱ�־ */
		TIMx->DIER |= TIM_IT_CC3;			/* ʹ��CC3�ж� */
    }
    else if (_CC == 4)
    {
        s_TIM_CallBack4 = (void (*)(void))_pCallBack;

		TIMx->CCR4 = cnt_tar;				/* ���ò���Ƚϼ�����CC4 */
        TIMx->SR = (uint16_t)~TIM_IT_CC4;	/* ���CC4�жϱ�־ */
		TIMx->DIER |= TIM_IT_CC4;			/* ʹ��CC4�ж� */
    }
	else
    {
        return;
    }
}

/*
*********************************************************************************************************
*	�� �� ��: TIMx_IRQHandler
*	����˵��: TIM �жϷ������
*	��    �Σ���
*	�� �� ֵ: ��
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
		TIMx->DIER &= (uint16_t)~TIM_IT_CC1;		/* ����CC1�ж� */	

        /* �ȹر��жϣ���ִ�лص���������Ϊ�ص�����������Ҫ������ʱ�� */
        s_TIM_CallBack1();
    }

	itstatus = TIMx->SR & TIM_IT_CC2;
	itenable = TIMx->DIER & TIM_IT_CC2;
	if ((itstatus != (uint16_t)RESET) && (itenable != (uint16_t)RESET))
	{
		TIMx->SR = (uint16_t)~TIM_IT_CC2;
		TIMx->DIER &= (uint16_t)~TIM_IT_CC2;		/* ����CC2�ж� */	

        /* �ȹر��жϣ���ִ�лص���������Ϊ�ص�����������Ҫ������ʱ�� */
        s_TIM_CallBack2();
    }

	itstatus = TIMx->SR & TIM_IT_CC3;
	itenable = TIMx->DIER & TIM_IT_CC3;
	if ((itstatus != (uint16_t)RESET) && (itenable != (uint16_t)RESET))
	{
		TIMx->SR = (uint16_t)~TIM_IT_CC3;
		TIMx->DIER &= (uint16_t)~TIM_IT_CC3;		/* ����CC2�ж� */	

        /* �ȹر��жϣ���ִ�лص���������Ϊ�ص�����������Ҫ������ʱ�� */
        s_TIM_CallBack3();
    }

	itstatus = TIMx->SR & TIM_IT_CC4;
	itenable = TIMx->DIER & TIM_IT_CC4;
	if ((itstatus != (uint16_t)RESET) && (itenable != (uint16_t)RESET))
	{
		TIMx->SR = (uint16_t)~TIM_IT_CC4;
		TIMx->DIER &= (uint16_t)~TIM_IT_CC4;		/* ����CC4�ж� */	

        /* �ȹر��жϣ���ִ�лص���������Ϊ�ص�����������Ҫ������ʱ�� */
        s_TIM_CallBack4();
    }	
}

#endif
