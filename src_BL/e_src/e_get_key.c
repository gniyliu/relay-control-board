#define U08 uint8_t
#define U16 uint16_t
#define BOOL uint8_t

#define COMPOSE_2CHARS(high_char,low_char)	(unsigned int)((high_char<<8)+low_char)
#define HIBYTE(x)	(((unsigned int)(x))>>8)
#define LOBYTE(x)	(((unsigned int)(x))&0xff)

#define MAX_VALUE(a,b)    (((a) > (b)) ? (a) : (b))
#define MIN_VALUE(a,b)    (((a) < (b)) ? (a) : (b))

union INT_CHAR{
    U16 i;
    U08 j[2];		/*	j[0]:i(0~7)	j[1]:i(8~15)	*/
};
union INT_CHAR g_KeyCode;
#define KEY_MASK	g_KeyCode.j[0]
#define KEY_CODE	g_KeyCode.j[1]

#define KEY_STAT_NULL		0xff
#define KEY_STAT_RELEASE	0xfe
#define KEY_STAT_LONG		0xfd
#define KEY_STAT_SHORT		0xfc
#define KEY_STAT_LIANXU		0xfb
#define KEY_STAT_PRESS		0xfa

enum KEY_ID{
KID_PWR,
KID_INC,
KID_DEC,

KID_K1,
KID_K2,
KID_K3,
KID_K4,
KID_K5,
KID_K6,
KID_K7,
KID_K8,
KID_K9,
KID_K10,
KID_K11,
KID_K12,
KID_K13,

KID_NULL,
};

#define IO_KEY_NUM		8	//	(13)
const U08 tblIoKey[IO_KEY_NUM] = {
KID_K1,
KID_K2,
KID_K3,
KID_K4,
KID_K5,
KID_K6,
KID_K7,
KID_K8,
//KID_K9,
//KID_K10,
//KID_K11,
//KID_K12,
//KID_K13,
};

// IO port definition

#define IO_KEY01_PORT		(GPIOG)
#define IO_KEY01_PIN		(GPIO_PIN_0)

#define IO_KEY02_PORT		(GPIOE)
#define IO_KEY02_PIN		(GPIO_PIN_2)

#define IO_KEY03_PORT		(GPIOE)
#define IO_KEY03_PIN		(GPIO_PIN_1)

#define IO_KEY04_PORT		(GPIOB)
#define IO_KEY04_PIN		(GPIO_PIN_7)

#define IO_KEY05_PORT		(GPIOB)
#define IO_KEY05_PIN		(GPIO_PIN_6)

#define IO_KEY06_PORT		(GPIOC)
#define IO_KEY06_PIN		(GPIO_PIN_1)

#define IO_KEY07_PORT		(GPIOB)
#define IO_KEY07_PIN		(GPIO_PIN_5)

#define IO_KEY08_PORT		(GPIOB)
#define IO_KEY08_PIN		(GPIO_PIN_4)

//#define IO_KEY09_PORT		(GPIOB)
//#define IO_KEY09_PIN		(GPIO_PIN_3)

//#define IO_KEY10_PORT		(GPIOB)
//#define IO_KEY10_PIN		(GPIO_PIN_2)

//#define IO_KEY11_PORT		(GPIOB)
//#define IO_KEY11_PIN		(GPIO_PIN_1)

//#define IO_KEY12_PORT		(GPIOB)
//#define IO_KEY12_PIN		(GPIO_PIN_0)

//#define IO_KEY13_PORT		(GPIOC)
//#define IO_KEY13_PIN		(GPIO_PIN_2)

const STRUCT_IO_PORT_PIN key_def[IO_KEY_NUM] = {
	{	IO_KEY01_PORT,		IO_KEY01_PIN},
	{	IO_KEY02_PORT,		IO_KEY02_PIN},
	{	IO_KEY03_PORT,		IO_KEY03_PIN},
	{	IO_KEY04_PORT,		IO_KEY04_PIN},
	{	IO_KEY05_PORT,		IO_KEY05_PIN},
	{	IO_KEY06_PORT,		IO_KEY06_PIN},
	{	IO_KEY07_PORT,		IO_KEY07_PIN},
	{	IO_KEY08_PORT,		IO_KEY08_PIN},
//	{	IO_KEY09_PORT,		IO_KEY09_PIN},
//	{	IO_KEY10_PORT,		IO_KEY10_PIN},
//	{	IO_KEY11_PORT,		IO_KEY11_PIN},
//	{	IO_KEY12_PORT,		IO_KEY12_PIN},
//	{	IO_KEY13_PORT,		IO_KEY13_PIN},
};


U16 g_KeyPinNormalStatus;
U16 g_Key_Error_Cnt[IO_KEY_NUM];

#define KEY_SCAN_FOSC			10
#define C_LIANXU_FST_TMR		(500/KEY_SCAN_FOSC)
#define C_LIANXU_LST_TMR		(150/KEY_SCAN_FOSC)
#define C_LONG_TMR				(1200/KEY_SCAN_FOSC)
U08 KeyTmrLianxu;
U08 KeyTmrLong;

BOOL bIoKeyPressed;
BOOL bIoKeyPressed_1;
static U08 IoKeyCode;
static U08 IoKeyPressTmr;

void KeyInit(void)
{
	U08 i;
	g_KeyPinNormalStatus = 0xffff;
	for(i=0; i<IO_KEY_NUM; i++)
	{
	  GPIO_Init(key_def[i].GPIOx, (GPIO_Pin_TypeDef)key_def[i].GPIO_Pin, GPIO_MODE_IN_PU_NO_IT);
	}
}

void GetIoKeyCode(void)
{
	#define JUDGE_COUNT		3
	U16 ioKeyTmp = 0;
	U08 i;
	static U08 ioKeyCnt = 0;
	static U16 ioKeyArray[JUDGE_COUNT];

	for(i=0; i<IO_KEY_NUM-4; i++)
	{
		if(g_KeyPinNormalStatus & (1<<i) )
		{
			if( !GPIO_ReadInputPin(key_def[i].GPIOx, (GPIO_Pin_TypeDef)key_def[i].GPIO_Pin) )
			{
				ioKeyTmp |= (1<<i);
				g_Key_Error_Cnt[i]++;
			}
			else
			{
				g_Key_Error_Cnt[i] = 0;
			}

		}
	}

	ioKeyArray[ioKeyCnt] = ioKeyTmp;

	if(++ioKeyCnt >= JUDGE_COUNT)
	{
		ioKeyCnt = 0;

		if( ( ioKeyArray[0] == ioKeyArray[1] )
			&& ( ioKeyArray[0] == ioKeyArray[2] )
			&& ( ioKeyArray[0] != 0 )
			)
		{
			for(i=0; i<IO_KEY_NUM; i++)
			{
				if(ioKeyArray[0] & (1<<i) )
				{
					bIoKeyPressed = 1;
					IoKeyPressTmr = 0;

					IoKeyCode = i;
					break;
				}
			}
//			printf("\r\n%4x", ioKeyArray[0]);
		}
		else
		{
//			IoKeyCode = 0;
		}
	}

}

#define KEY_ERROR_TIMEROUT	(10000/10)
void JudgeKeyError(void)
{
static U08 blink;
	U08 i;

	for(i=0; i<IO_KEY_NUM; i++)
	{
		if(g_Key_Error_Cnt[i] >= KEY_ERROR_TIMEROUT)
		{
//			g_KeyPinNormalStatus &= ~(1<<i);		//	½«°¸¼þÆÁ±Î
		}
		if(! (g_KeyPinNormalStatus & (U16)(1<<i) ) )
		{
			KeyLed(i, blink&0x01);
		}
	}
	blink++;

//	printf("\r\n %4x ", g_KeyPinNormalStatus);
}

void ScanKey(void)
/*	place this function in main()'s while(1) cycle and delay's function	*/
{
	if(KeyTmrLianxu>1)	--KeyTmrLianxu;
	if(KeyTmrLong>1)		--KeyTmrLong;
#if 1
        unsigned char i = 0;
        for( i = 1; i < 8; i++ )
        {
          if( GPIO_ReadInputPin(key_def[i].GPIOx, (GPIO_Pin_TypeDef)key_def[i].GPIO_Pin) )
          {
                  LightOnOff(i-1, 0);
          }
          else
          {
                  LightOnOff(i-1, 1);
          }
        }
#else
	if( GPIO_ReadInputPin(key_def[0].GPIOx, (GPIO_Pin_TypeDef)key_def[0].GPIO_Pin) )
	{
		LightOnOff(0, 0);
	}
	else
	{
		LightOnOff(0, 1);
	}

	if( GPIO_ReadInputPin(key_def[1].GPIOx, (GPIO_Pin_TypeDef)key_def[1].GPIO_Pin) )
	{
		LightOnOff(1, 0);
	}
	else
	{
		LightOnOff(1, 1);
	}

	if( GPIO_ReadInputPin(key_def[2].GPIOx, (GPIO_Pin_TypeDef)key_def[2].GPIO_Pin) )
	{
		LightOnOff(2, 0);
	}
	else
	{
		LightOnOff(2, 1);
	}

	if( GPIO_ReadInputPin(key_def[3].GPIOx, (GPIO_Pin_TypeDef)key_def[3].GPIO_Pin) )
	{
		LightOnOff(3, 0);
	}
	else
	{
		LightOnOff(3, 1);
	}
	if( GPIO_ReadInputPin(key_def[4].GPIOx, (GPIO_Pin_TypeDef)key_def[4].GPIO_Pin) )
	{
		LightOnOff(4, 0);
	}
	else
	{
		LightOnOff(4, 1);
	}

	if( GPIO_ReadInputPin(key_def[5].GPIOx, (GPIO_Pin_TypeDef)key_def[5].GPIO_Pin) )
	{
		LightOnOff(5, 0);
	}
	else
	{
		LightOnOff(5, 1);
	}

	if( GPIO_ReadInputPin(key_def[6].GPIOx, (GPIO_Pin_TypeDef)key_def[6].GPIO_Pin) )
	{
		LightOnOff(6, 0);
	}
	else
	{
		LightOnOff(6, 1);
	}

	if( GPIO_ReadInputPin(key_def[7].GPIOx, (GPIO_Pin_TypeDef)key_def[7].GPIO_Pin) )
	{
		LightOnOff(7, 0);
	}
	else
	{
		LightOnOff(7, 1);
	}
#endif
        
#define C_IO_KEY_PRESS_TMR		(70/KEY_SCAN_FOSC)			//	70ms
	if( (++IoKeyPressTmr) >= C_IO_KEY_PRESS_TMR)		bIoKeyPressed = 0;
//	GetIoKeyCode();

}

U16 KeyEncode(void)
/*	deal this function per 10ms	*/
{
U16 key_code;
U08 tmp;

	ScanKey();

	key_code = COMPOSE_2CHARS(KEY_STAT_NULL,KID_NULL);

	if( (bIoKeyPressed||bIoKeyPressed_1)
		)
	{
		tmp = tblIoKey[IoKeyCode];
	}

	if(bIoKeyPressed)
	{
		if(bIoKeyPressed_1 == 0)
		{
			bIoKeyPressed_1 = 1;
			KeyTmrLianxu = C_LIANXU_FST_TMR+1;
			KeyTmrLong = C_LONG_TMR+1;
			key_code = COMPOSE_2CHARS(KEY_STAT_PRESS,tmp);
		}
		else
		{
			if(KeyTmrLong == 1)
			{
				KeyTmrLong = 0;
				key_code = COMPOSE_2CHARS(KEY_STAT_LONG,tmp);
			}
			else if(KeyTmrLianxu == 1)
			{
				KeyTmrLianxu = C_LIANXU_LST_TMR+1;
				key_code = COMPOSE_2CHARS(KEY_STAT_LIANXU,tmp);
			}
		}
	}
	else if(bIoKeyPressed_1)
	{
		bIoKeyPressed_1 = 0;
		if(KeyTmrLong==0)	key_code = COMPOSE_2CHARS(KEY_STAT_RELEASE,tmp);
		else				key_code = COMPOSE_2CHARS(KEY_STAT_SHORT,tmp);
	}

	return key_code;
}



