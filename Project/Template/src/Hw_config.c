/* Includes ------------------------------------------------------------------*/
#include "Hw_config.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
  #define GETCHAR_PROTOTYPE int __io_getchar(void)  
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
  #define GETCHAR_PROTOTYPE int fgetc(FILE *f)  
#endif /* __GNUC__ */

// TODO: for printf function , need to confirm use USART2 or USART2
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART2, (uint8_t) ch);

  /* Loop until transmit data register is empty */
  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
  {}

  return ch;
}

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t uwTimingDelay;

/*uart variable*/
__IO uint8_t UartRxBuffer = 0;
__IO uint8_t UartRxFlag = FALSE;

/*PWM variable*/
uint16_t TimerPeriod = 0;

const uint32_t RGBColorTable[300]=
{
	0x000000,0x272727,0x3C3C3C,0x4F4F4F,0x5B5B5B,0x6C6C6C,0x7B7B7B,0x8E8E8E,0x9D9D9D,0xADADAD,0xBEBEBE,0xd0d0d0,0xE0E0E0,0xF0F0F0,0xFCFCFC,0xFFFFFF,
	0x2F0000,0x4D0000,0x600000,0x750000,0x930000,0xAE0000,0xCE0000,0xEA0000,0xFF0000,0xFF2D2D,0xFF5151,0xff7575,0xFF9797,0xFFB5B5,0xFFD2D2,0xFFECEC,
	0x600030,0x820041,0x9F0050,0xBF0060,0xD9006C,0xF00078,0xFF0080,0xFF359A,0xFF60AF,0xFF79BC,0xFF95CA,0xffaad5,0xFFC1E0,0xFFD9EC,0xFFECF5,0xFFF7FB,
	0x460046,0x5E005E,0x750075,0x930093,0xAE00AE,0xD200D2,0xE800E8,0xFF00FF,0xFF44FF,0xFF77FF,0xFF8EFF,0xffa6ff,0xFFBFFF,0xFFD0FF,0xFFE6FF,0xFFF7FF,
	0x28004D,0x3A006F,0x4B0091,0x5B00AE,0x6F00D2,0x8600FF,0x921AFF,0x9F35FF,0xB15BFF,0xBE77FF,0xCA8EFF,0xd3a4ff,0xDCB5FF,0xE6CAFF,0xF1E1FF,0xFAF4FF,
	0x000079,0x000093,0x0000C6,0x0000C6,0x0000E3,0x2828FF,0x4A4AFF,0x6A6AFF,0x7D7DFF,0x9393FF,0xAAAAFF,0xB9B9FF,0xCECEFF,0xDDDDFF,0xECECFF,0xFBFBFF,
	0x000079,0x003D79,0x004B97,0x005AB5,0x0066CC,0x0072E3,0x0080FF,0x2894FF,0x46A3FF,0x66B3FF,0x84C1FF,0x97CBFF,0xACD6FF,0xC4E1FF,0xD2E9FF,0xECF5FF,
	0x003E3E,0x005757,0x007979,0x009393,0x00AEAE,0x00CACA,0x00E3E3,0x00FFFF,0x4DFFFF,0x80FFFF,0xA6FFFF,0xBBFFFF,0xCAFFFF,0xD9FFFF,0xECFFFF,0xFDFFFF,
	0x006030,0x01814A,0x019858,0x01B468,0x02C874,0x02DF82,0x02F78E,0x1AFD9C,0x4EFEB3,0x7AFEC6,0x96FED1,0xADFEDC,0xC1FFE4,0xD7FFEE,0xE8FFF5,0xFBFFFD,
	0x006000,0x007500,0x009100,0x00A600,0x00BB00,0x00DB00,0x00EC00,0x28FF28,0x53FF53,0x79FF79,0x93FF93,0xA6FFA6,0xBBFFBB,0xCEFFCE,0xDFFFDF,0xF0FFF0,
	0x467500,0x548C00,0x64A600,0x73BF00,0x82D900,0x8CEA00,0x9AFF02,0xA8FF24,0xB7FF4A,0xC2FF68,0xCCFF80,0xD3FF93,0xDEFFAC,0xE8FFC4,0xEFFFD7,0xF5FFE8,
	0x424200,0x5B5B00,0x737300,0x8C8C00,0xA6A600,0xC4C400,0xE1E100,0xF9F900,0xFFFF37,0xFFFF6F,0xFFFF93,0xFFFFAA,0xFFFFB9,0xFFFFCE,0xFFFFDF,0xFFFFF4,
	0x5B4B00,0x796400,0x977C00,0xAE8F00,0xC6A300,0xD9B300,0xEAC100,0xFFD306,0xFFDC35,0xFFE153,0xFFE66F,0xFFED97,0xFFF0AC,0xFFF4C1,0xFFF8D7,0xFFFCEC,
	0x844200,0x9F5000,0xBB5E00,0xD26900,0xEA7500,0xFF8000,0xFF9224,0xFFA042,0xFFAF60,0xFFBB77,0xFFC78E,0xFFD1A4,0xFFDCB9,0xFFE4CA,0xFFEEDD,0xFFFAF4,
	0x642100,0x842B00,0xA23400,0xBB3D00,0xD94600,0xF75000,0xFF5809,0xFF8040,0xFF8F59,0xFF9D6F,0xFFAD86,0xFFBD9D,0xFFCBB3,0xFFDAC8,0xFFE6D9,0xFFF3EE,
	0x613030,0x743A3A,0x804040,0x984B4B,0xAD5A5A,0xB87070,0xC48888,0xCF9E9E,0xD9B3B3,0xE1C4C4,0xEBD6D6,0xF2E6E6,							
	0x616130,0x707038,0x808040,0x949449,0xA5A552,0xAFAF61,0xB9B973,0xC2C287,0xCDCD9A,0xD6D6AD,0xDEDEBE,0xE8E8D0,							
	0x336666,0x3D7878,0x408080,0x4F9D9D,0x5CADAD,0x6FB7B7,0x81C0C0,0x95CACA,0xA3D1D1,0xB3D9D9,0xC4E1E1,0xD1E9E9,							
	0x484891,0x5151A2,0x5A5AAD,0x7373B9,0x8080C0,0x9999CC,0xA6A6D2,0xB8B8DC,0xC7C7E2,0xD8D8EB,0xE6E6F2,0xF3F3FA,							
	0x6C3365,0x7E3D76,0x8F4586,0x9F4D95,0xAE57A4,0xB766AD,0xC07AB8,0xCA8EC2,0xD2A2CC,0xDAB1D5,0xE2C2DE,0xEBD3E8,						
};

#define	MixedLIghtLED2_R(hex)		TIM_SetCompare2(TIM1,((uint16_t) (((uint32_t) ((hex*100)>>8) * (TimerPeriod - 1)) / 100)))
#define	MixedLIghtLED2_G(hex)		TIM_SetCompare3(TIM1,((uint16_t) (((uint32_t) ((hex*100)>>8) * (TimerPeriod - 1)) / 100)))
#define	MixedLIghtLED2_B(hex)		TIM_SetCompare1(TIM1,((uint16_t) (((uint32_t) ((hex*100)>>8) * (TimerPeriod - 1)) / 100)))

#define	MixedLIghtLED1_R(hex)		TIM_SetCompare2(TIM3,((uint16_t) (((uint32_t) ((hex*100)>>8) * (TimerPeriod - 1)) / 100)))
#define	MixedLIghtLED1_G(hex)		TIM_SetCompare1(TIM3,((uint16_t) (((uint32_t) ((hex*100)>>8) * (TimerPeriod - 1)) / 100)))
#define	MixedLIghtLED1_B(hex)		TIM_SetCompare3(TIM3,((uint16_t) (((uint32_t) ((hex*100)>>8) * (TimerPeriod - 1)) / 100)))

#define	MixedLIghtLED1_R_ON(void)	(GPIO_SetBits(GPIOB,GPIO_Pin_5))
#define	MixedLIghtLED1_G_ON(void)	(GPIO_SetBits(GPIOB,GPIO_Pin_4))
#define	MixedLIghtLED1_B_ON(void)	(GPIO_SetBits(GPIOB,GPIO_Pin_0))
#define	MixedLIghtLED1_R_OFF(void)	(GPIO_ResetBits(GPIOB,GPIO_Pin_5))
#define	MixedLIghtLED1_G_OFF(void)	(GPIO_ResetBits(GPIOB,GPIO_Pin_4))
#define	MixedLIghtLED1_B_OFF(void)	(GPIO_ResetBits(GPIOB,GPIO_Pin_0))

#define	MixedLIghtLED2_R_ON(void)	(GPIO_SetBits(GPIOA,GPIO_Pin_9))
#define	MixedLIghtLED2_G_ON(void)	(GPIO_SetBits(GPIOA,GPIO_Pin_10))
#define	MixedLIghtLED2_B_ON(void)	(GPIO_SetBits(GPIOA,GPIO_Pin_8))
#define	MixedLIghtLED2_R_OFF(void)	(GPIO_ResetBits(GPIOA,GPIO_Pin_9))
#define	MixedLIghtLED2_G_OFF(void)	(GPIO_ResetBits(GPIOA,GPIO_Pin_10))
#define	MixedLIghtLED2_B_OFF(void)	(GPIO_ResetBits(GPIOA,GPIO_Pin_8))

uint8_t MixedLIghtLED2Timer = 0;
uint8_t MixedLIghtLED2Flag = 1;

uint8_t MixedLIghtLED1Timer = 0;
uint8_t MixedLIghtLED1Flag = 0;
uint16_t MixedLIghtLED1Index = 0;

uint32_t LED1_Hex = 0xFFFFFF;
uint32_t LED2_Hex = 0xFFFFFF;
uint32_t LED3_Hex = 0xFFFFFF;
uint32_t LED4_Hex = 0xFFFFFF;
uint32_t LED5_Hex = 0xFFFFFF;

/* Private functions ---------------------------------------------------------*/

void loop(int32_t bright,uint8_t LEDx,uint8_t Color)
{
	#define usDelay		(6)
	
	switch(LEDx)
	{
		case LED1:
			switch(Color)
			{
				case COLOR_R:
				    MixedLIghtLED1_R_ON();
				    Delay(bright * usDelay);
				    MixedLIghtLED1_R_OFF();
				    Delay((255 - bright) * usDelay);
				    if((bright++) >= 255) 
				    {
						bright = 0;
				    }					
					break;
				case COLOR_G:
				    MixedLIghtLED1_G_ON();
				    Delay(bright * usDelay);
				    MixedLIghtLED1_G_OFF();
				    Delay((255 - bright) * usDelay);
				    if((bright++) >= 255) 
				    {
						bright = 0;
				    }					
					break;
				case COLOR_B:
				    MixedLIghtLED1_B_ON();
				    Delay(bright * usDelay);
				    MixedLIghtLED1_B_OFF();
				    Delay((255 - bright) * usDelay);
				    if((bright++) >= 255) 
				    {
						bright = 0;
				    }					
					break;					
			}			
			break;

		case LED2:
			switch(Color)
			{
				case COLOR_R:
				    MixedLIghtLED2_R_ON();
				    Delay(bright * 1);
				    MixedLIghtLED2_R_OFF();
				    Delay((255 - bright) * 1);
				    if((bright++) >= 255) 
				    {
						bright = 0;
				    }					
					break;
				case COLOR_G:
				    MixedLIghtLED2_G_ON();
				    Delay(bright * 1);
				    MixedLIghtLED2_G_OFF();
				    Delay((255 - bright) * 1);
				    if((bright++) >= 255) 
				    {
						bright = 0;
				    }					
					break;
				case COLOR_B:
				    MixedLIghtLED2_B_ON();
				    Delay(bright * 1);
				    MixedLIghtLED2_B_OFF();
				    Delay((255 - bright) * 1);
				    if((bright++) >= 255) 
				    {
						bright = 0;
				    }					
					break;					
			}
			
			break;

	}

}

void LEDx_Handler(uint8_t LEDx,uint32_t Hex)
{
	uint8_t hex_R = 0;
	uint8_t hex_G = 0;
	uint8_t hex_B = 0;

	hex_R =  (Hex>>16);
	hex_G =  ((Hex>>8)& 0x00FF);
	hex_B =  ((Hex)&0x0000FF);

	loop(hex_R,LEDx,COLOR_R);
	loop(hex_G,LEDx,COLOR_G);
	loop(hex_B,LEDx,COLOR_B);
}


void MixedLIghtLED2_Demo(void)	//RRGGBB
{
	//table refer to http://www.ifreesite.com/color/
	
	/*
		Duty: 0 ~ 100
		Hex : 0 ~ 0xFF

		formula : 
		Duty = (Hex*100) >>8 
		Hex = (Duty<<8)/100
	*/

	uint8_t hex_R = 0;
	uint8_t hex_G = 0;
	uint8_t hex_B = 0;

	static uint8_t State = COLOR_R;

	if (MixedLIghtLED2Flag)
	{
		if (MixedLIghtLED2Timer)
		{
			switch (State)
			{
				case COLOR_R:
					hex_R = 0xFF;
					hex_G = 0x00;
					hex_B = 0x00;
					State = COLOR_G;
					break;
				case COLOR_G:
					hex_R = 0x00;
					hex_G = 0xFF;
					hex_B = 0x00;
					State = COLOR_B;
					break;
				case COLOR_B:
					hex_R = 0x00;
					hex_G = 0x00;
					hex_B = 0xFF;
					State = COLOR_R;
					break;				
			}

			LEDx_Handler(LED2,(hex_R<<16)|(hex_G<<8)|(hex_B));

			MixedLIghtLED2Timer = 0;
		}
	}	
}

/*
	B : TIM3_CH3/PA8
	R :	TIM3_CH2/PA9
	G :	TIM3_CH1/PA10
*/

void MixedLIghtLED2_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
//	RCC_PCLK1Config(RCC_HCLK_Div2);
	
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void MixedLIghtLED1_Demo(void)	//RRGGBB
{
	//table refer to http://www.ifreesite.com/color/
	
	/*
		Duty: 0 ~ 100
		Hex : 0 ~ 0xFF

		formula : 
		Duty = (Hex*100) >>8 
		Hex = (Duty<<8)/100
	*/
	
	if (MixedLIghtLED1Flag)
	{
//		if (MixedLIghtLED1Timer)
		{
			LEDx_Handler(LED1,RGBColorTable[MixedLIghtLED1Index]);

			#if 0	//debug	
			printf("%s(%3d,0x%8X)\r\n",__FUNCTION__,MixedLIghtLED1Index,RGBColorTable[MixedLIghtLED1Index]);
			#endif
			
			MixedLIghtLED1Index = (MixedLIghtLED1Index>=(300-1))?(0):(MixedLIghtLED1Index+1);
			MixedLIghtLED1Timer = 0;
		}
	}
			
}

/*
	B : TIM3_CH3/PB0
	R :	TIM3_CH2/PB5
	G :	TIM3_CH1/PB4
*/

void MixedLIghtLED1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
//	RCC_PCLK1Config(RCC_HCLK_Div2);
	
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}


void PollingProcedure(void)
{	
	USART_Test();

	MixedLIghtLED1_Demo();	

//	MixedLIghtLED2_Demo();
}
void LED_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIO clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

}

/*
	TIMxCLK = PCLK1 = HCLK = SystemCoreClock
	TIMx counter clock = TIMxCLK /((Prescaler + 1)*(Period + 1))
	                = 84 MHz / ((11+1)*(6999+1))
	                = 1000 Hz 
     ==> TIMx counter period = 1 ms
*/
void TIM4_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	/* PCLK1 = HCLK/4 */
	RCC_PCLK1Config(RCC_HCLK_Div2);//TIM3CLK = (HCLK/4)x2 = (180 MHz/4)x2 = 90 MHz 
	
	/* TIM3 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	/* TIMx base configuration */
	TIM_TimeBaseStructure.TIM_Period = (7000 -1);
	TIM_TimeBaseStructure.TIM_Prescaler = (12 -1);
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	/* TIMx Interrupts enable */
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

	/* TIMx enable counter */
	TIM_Cmd(TIM4, ENABLE);

	/* Enable the TIMx gloabal Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	

}

/*
	TIMxCLK = PCLK1 = HCLK = SystemCoreClock
	TIMx counter clock = TIMxCLK /((Prescaler + 1)*(Period + 1))
	                = 84 MHz / ((11+1)*(6999+1))
	                = 1000 Hz 
     ==> TIMx counter period = 1 ms
*/
void TIM3_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	/* PCLK1 = HCLK/4 */
	RCC_PCLK1Config(RCC_HCLK_Div2);//TIM3CLK = (HCLK/4)x2 = (180 MHz/4)x2 = 90 MHz 
	
	/* TIM3 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	/* TIMx base configuration */
	TIM_TimeBaseStructure.TIM_Period = (7000 -1);
	TIM_TimeBaseStructure.TIM_Prescaler = (12 -1);
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	/* TIMx Interrupts enable */
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

	/* TIMx enable counter */
	TIM_Cmd(TIM3, ENABLE);

	/* Enable the TIMx gloabal Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	

}

void USART_Test(void)
{
	if (UartRxFlag)
	{
		switch (UartRxBuffer)
		{
			case '4':	//demo start
				MixedLIghtLED1Flag = 1;
				break;
			case '5':	//demo stop	
				MixedLIghtLED1Flag = 0;
				MixedLIghtLED1Index = 0;	//color table re-start as well
				break;

			case '6':	//demo start
				MixedLIghtLED2Flag = 1;
				break;
			case '7':	//demo stop	
				MixedLIghtLED2Flag = 0;
				MixedLIghtLED2_R_OFF();
				MixedLIghtLED2_G_OFF();
				MixedLIghtLED2_B_OFF();
				break;
				
			case 'Z':
			case 'z':				
				NVIC_SystemReset();
				
				break;				
			default : 
				UartRxFlag = FALSE;				
				break;
		}
		UartRxFlag = FALSE;
	}
}

void USART_Config(void)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable GPIO clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	/* Enable UART clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	/* Connect PXx to USARTx_Tx*/
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);

	/* Connect PXx to USARTx_Rx*/
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);

	/* Configure USART Tx as alternate function  */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure USART Rx as alternate function  */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* USARTx configured as follows:
	- BaudRate = 115200 baud  
	- Word Length = 8 Bits
	- One Stop Bit
	- No parity
	- Hardware flow control disabled (RTS and CTS signals)
	- Receive and transmit enabled
	*/
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	/* USART configuration */
	USART_Init(USART2, &USART_InitStructure);

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); 

	while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
	{}
	
	/* NVIC configuration */
	/* Enable the USARRx Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable USART */
	USART_Cmd(USART2, ENABLE);


	printf("\n\rUSART Printf Example: retarget the C library printf function to the USART\n\r");


}

void SysTickConfig(void)
{
	RCC_ClocksTypeDef RCC_Clocks;
	/* This function fills the RCC_ClockFreq structure with the current
	 frequencies of different on chip clocks (for debug purpose) */	
	RCC_GetClocksFreq(&RCC_Clocks);
	
	printf("===========================\r\n");
	printf("SYSCLK_Frequency = %d Hz\n\r",RCC_Clocks.SYSCLK_Frequency);	
	printf("AHB = %d Hz\n\r",RCC_Clocks.HCLK_Frequency);
	printf("APB1 = %d Hz\n\r",RCC_Clocks.PCLK1_Frequency);
	printf("APB2 = %d Hz\n\r",RCC_Clocks.PCLK2_Frequency);
	
	/* Setup SysTick Timer for 1ms interrupts  */
	if (SysTick_Config(SystemCoreClock / 255000))
	{
		/* Capture error */
		while (1);
	}
	
	/* Configure the SysTick handler priority */
	NVIC_SetPriority(SysTick_IRQn, 0x00);
	
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay(__IO uint32_t uTime)
{ 
	uwTimingDelay = uTime;
	while(uwTimingDelay != 0);
}


/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (uwTimingDelay != 0x00)
  { 
    uwTimingDelay--;
  }
}

//currently not use
/*

void SystemClkDelay(void)
{
	uint32_t i;
	i = 0xffff;
	while(i--);
}

void wtPutChar(uint8_t ccc)
{
	UART1_SendData8(ccc);
	// Loop until the end of transmission 
	while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);	
}

u16 GetAbsTime(u16 a,u16 b)
{
	u16 c;
	if(a>=b) c=(a-b);
	else c=65535-(b-a);	
	
	return c;
}
*/
uint8_t UART_GetByte(void)
{
	while ( USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);
	{
	}
	return (uint8_t)USART_ReceiveData(USART2);
}

void UART_SendByte(uint8_t Data)
{
	USART_SendData(USART2 , (unsigned char)Data);
	while (USART_GetFlagStatus(USART2 , USART_FLAG_TC)==RESET);
	{
	}
}

void UART_SendString(uint8_t* Data,uint16_t len)
{
	#if 1
	uint16_t i=0;
	for(i=0;i<len;i++ )
	{
		UART_SendByte(Data[i]);
	}
	#else	//ignore len
    while(*Data)  
    {  
        USART_SendData(USART2, (unsigned char) *Data++);  
        /* Loop until the end of transmission */  
        while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);  //USART_FLAG_TXE
    } 
	#endif
}

void SystemClkDelay(uint32_t u32Delay)
{
	//uint32_t i;
	//i = 0xffff;
	while(u32Delay--);
}


