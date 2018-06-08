
void On1ms(void)
{
	if(time_1ms_flag)
	{
		time_1ms_flag = 0;
	}
}

void On10ms(void)
{
	if(time_10ms_flag)
	{
		time_10ms_flag = 0;
		E_SI4432_Rx();

		g_KeyCode.i = KeyEncode();
		if( (KEY_MASK!=KEY_STAT_NULL) && (KEY_CODE!=KID_NULL) )
		{
			KeySrv();
//			printf("\n\rkey mask is %x, key code is %x \n\r", KEY_MASK, KEY_CODE);
//			KeySrv();
		}
	}
}

void On100ms(void)
{
	if(time_100ms_flag)
	{
		time_100ms_flag = 0;
		GPIO_WriteReverse(LED_STAT_PORT, (GPIO_Pin_TypeDef)LED_STAT_PINS);

		JudgeKeyError();

	}
}

U08 tmp = 0;
void On1s(void)
{
	if(time_1s_flag)
	{
		time_1s_flag = 0;

		if(g_RedLedTimer)
		{
			if(--g_RedLedTimer == 0)
			{
				if(!led_blue_static)
					BuleOn(1);
			}
		}

//		if(g_LedTimer < LED_TIMEROUT)
//		{
//			++g_LedTimer;
//		}
//		else
//		{
//			KeyLedAllOff();
//			LightAllOff();
//		}

//		LightOnOff( (tmp>2)?(tmp-3):tmp, (tmp>2)?1:0);
		if(++tmp >5)	tmp = 0;


		if(rmt_pair_timerout)	--rmt_pair_timerout;
//		EepromTest();
		printf("\n\r1 second timeout\n\r");
	}
}

