
void SendKeyInfo(unsigned char key)
{
	putchar(0xff);
	putchar(key);
}

void Relay5Xor(void)
{
	static uint8_t sv = 0;
	if(sv)
	{
		LightOnOff(4, 0);
		KeyLed(1,0);
	}
	else
	{
		LightOnOff(4, 1);
		KeyLed(1,1);
	}
	sv ^= 1;
}

void Relay6Xor(void)
{
	static uint8_t sv = 0;
	if(sv)
	{
		LightOnOff(6, 0);
		KeyLed(1,0);
	}
	else
	{
		LightOnOff(6, 1);
		KeyLed(1,1);
	}
	sv ^= 1;
}

void Relay7Xor(void)
{
	static uint8_t sv = 0;
	if(sv)
	{
		LightOnOff(7, 0);
		KeyLed(2,0);
	}
	else
	{
		LightOnOff(7, 1);
		KeyLed(2,1);
	}
	sv ^= 1;
}

void KeySrv(void)
{
	switch(KEY_MASK)
	{
	case KEY_STAT_RELEASE:
		break;
	case KEY_STAT_LONG:
		break;
	case KEY_STAT_SHORT:
		break;
	case KEY_STAT_LIANXU:
		break;
	case KEY_STAT_PRESS:
		switch(KEY_CODE)
		{
		case KID_PWR:
			if(onOff)
			{
				onOff = 0;
				printf("\r\nmotor off\r\n");
			}
			else
			{
				onOff = 1;
				printf("\r\nmotor on\r\n");
			}
			break;

		case KID_INC:
			if( motorSpeed < MOTOR_SPEED_MAX )
				++motorSpeed;
			E_save_motor_speed();
			printf("speed = %d",motorSpeed);
			break;

		case KID_DEC:
			if( motorSpeed > MOTOR_SPEED_MIN )
				--motorSpeed;
			E_save_motor_speed();
			printf("speed = %d",motorSpeed);
			break;

		case KID_K1:		if(led_blue_static)	BuleOn(0);else	BuleOn(1);			g_RedLedTimer = RED_LED_TIMEROUT;			break;
		case KID_K2:		if(led_blue_static_1)	BuleOn_1(0);else	BuleOn_1(1);			g_RedLedTimer_1 = RED_LED_TIMEROUT;			break;
//		case KID_K2:		Relay6Xor();	break;
//		case KID_K3:		Relay7Xor();	break;
//		case KID_K4:		Relay7Xor();	break;
//		case KID_K5:		KeyLedAllOff();			LightAllOff();	LightOnOff(4, 1);		KeyLed(4,1);	g_LedTimer = 0;			break;
//		case KID_K6:		KeyLedAllOff();			LightAllOff();	LightOnOff(5, 1);		KeyLed(5,1);	g_LedTimer = 0;			break;
//		case KID_K7:		KeyLedAllOff();			LightAllOff();	LightOnOff(6, 1);		KeyLed(6,1);	g_LedTimer = 0;			break;
//		case KID_K8:		KeyLedAllOff();			LightAllOff();	LightOnOff(0, 1);		KeyLed(7,1);	g_LedTimer = 0;			break;
//		case KID_K9:		SendKeyInfo(KID_K9);		KeyLedAllOff();	KeyLed(8, 1);			g_LedTimer = 0;			break;
//		case KID_K10:		SendKeyInfo(KID_K10);		KeyLedAllOff();	KeyLed(9, 1);			g_LedTimer = 0;			break;
//		case KID_K11:		SendKeyInfo(KID_K11);		KeyLedAllOff();	KeyLed(10, 1);		g_LedTimer = 0;			break;
//		case KID_K12:		SendKeyInfo(KID_K12);		KeyLedAllOff();	KeyLed(11, 1);		g_LedTimer = 0;			break;
//		case KID_K13:		SendKeyInfo(KID_K13);		KeyLedAllOff();	KeyLed(12, 1);		g_LedTimer = 0;			break;

		default:
			break;
		}
		break;
	default:
		break;
	}


}


