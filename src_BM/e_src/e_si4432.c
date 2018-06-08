#if defined(STM8S207)
#define SPI_CS_PORT		GPIOA
#define SPI_CS_PIN		GPIO_PIN_1
#define SPI_CS_1	GPIO_WriteHigh(SPI_CS_PORT, SPI_CS_PIN)
#define SPI_CS_0	GPIO_WriteLow(SPI_CS_PORT, SPI_CS_PIN)

#define SPI_SDI_PORT		GPIOA
#define SPI_SDI_PIN			GPIO_PIN_1
#define SPI_SDI_1			GPIO_WriteHigh(SPI_SDI_PORT, SPI_SDI_PIN);
#define SPI_SDI_0			GPIO_WriteLow(SPI_SDI_PORT, SPI_SDI_PIN);

#define SPI_SDO_PORT		GPIOA
#define SPI_SDO_PIN			GPIO_PIN_1
#define SPI_SDO_STATUS		GPIO_ReadInputPin(SPI_SDO_PORT, SPI_SDO_PIN)

#define SPI_SCLK_PORT		GPIOA
#define SPI_SCLK_PIN		GPIO_PIN_1
#define SPI_SCLK_1			GPIO_WriteHigh(SPI_SCLK_PORT, SPI_SCLK_PIN)
#define SPI_SCLK_0			GPIO_WriteLow(SPI_SCLK_PORT, SPI_SCLK_PIN)

#define SPI_SDN_PORT		GPIOA
#define SPI_SDN_PIN			GPIO_PIN_1
#define SPI_SDN_1				GPIO_WriteHigh(SPI_SDN_PORT, SPI_SDN_PIN)
#define SPI_SDN_0				GPIO_WriteLow(SPI_SDN_PORT, SPI_SDN_PIN)

#define SPI_IRQ_PORT		GPIOA
#define SPI_IRQ_PIN			GPIO_PIN_1
#define SPI_IRQ_STATUS	GPIO_ReadInputPin(SPI_IRQ_PORT, SPI_IRQ_PIN)
#elif defined(STM8L15X_HD)||defined(STM8L15X_LD)||defined(STM8L15X_MD)
#define SPI_CS_PORT		GPIOB
#define SPI_CS_PIN		GPIO_Pin_4
#define SPI_CS_1	GPIO_SetBits(SPI_CS_PORT, SPI_CS_PIN)
#define SPI_CS_0	GPIO_ResetBits(SPI_CS_PORT, SPI_CS_PIN)

#define SPI_SDI_PORT		GPIOB
#define SPI_SDI_PIN			GPIO_Pin_6
#define SPI_SDI_1			GPIO_SetBits(SPI_SDI_PORT, SPI_SDI_PIN);
#define SPI_SDI_0			GPIO_ResetBits(SPI_SDI_PORT, SPI_SDI_PIN);

#define SPI_SDO_PORT		GPIOB
#define SPI_SDO_PIN			GPIO_Pin_7
#define SPI_SDO_STATUS		GPIO_ReadInputDataBit(SPI_SDO_PORT, SPI_SDO_PIN)

#define SPI_SCLK_PORT		GPIOB
#define SPI_SCLK_PIN		GPIO_Pin_5
#define SPI_SCLK_1			GPIO_SetBits(SPI_SCLK_PORT, SPI_SCLK_PIN)
#define SPI_SCLK_0			GPIO_ResetBits(SPI_SCLK_PORT, SPI_SCLK_PIN)

#define SPI_IRQ_PORT		GPIOC
#define SPI_IRQ_PIN			GPIO_Pin_0
#define SPI_IRQ_STATUS	GPIO_ReadInputDataBit(SPI_IRQ_PORT, SPI_IRQ_PIN)
#else
 #error "SPI master device not defined"

#endif

#define SI4432_DATA_DUMMY		0xff

#define SI4432_WT_CMD			0x80

#define  SI4432_PWRSTATE_READY		          01		// ģ�� Ready ״̬����
#define  SI4432_PWRSTATE_TX				      0x09		// ģ�� ����״̬����
#define  SI4432_PWRSTATE_RX				      05		// ģ�� ����״̬����
#define  SI4432_PACKET_SENT_INTERRUPT	      04		// ģ�� ��������ж�
#define  SI4432_Rx_packet_received_interrupt  0x02      // ģ�� �յ����ݰ��ж�

#define  TX1_RX0	Si4432_SPI_RW_Byte(0x0e|SI4432_WT_CMD, 0x01)		// ����״̬�����߿��ض���
#define  TX0_RX1	Si4432_SPI_RW_Byte(0x0e|SI4432_WT_CMD, 0x02)		// ����״̬�����߿��ض���
#define  TX0_RX0	Si4432_SPI_RW_Byte(0x0e|SI4432_WT_CMD, 0x00)         // �Ƿ��䣬����״̬�����߿��ض���

//#include "i_si4432.c"

void E_InitSi4432De(void)
{
#if defined(STM8S207)
	SPI_DeInit();


	CLK_PeripheralClockConfig(CLK_PERIPHERAL_SPI, ENABLE);

	GPIO_Init( SPI_SDN_PORT, SPI_SDN_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);

#if 0
  /* Set the MOSI,MISO and SCK at high level */
  GPIO_ExternalPullUpConfig(SPI_SCLK_PORT, SPI_SCLK_PIN , ENABLE);
	GPIO_ExternalPullUpConfig(SPI_SDO_PORT, SPI_SDO_PIN , ENABLE);
	GPIO_ExternalPullUpConfig(SPI_SDI_PORT, SPI_SDI_PIN, ENABLE);

  /* Configure FLASH_CS as Output push-pull, used as Flash Chip select */
  GPIO_Init(SPI_CS_PORT, SPI_CS_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
#else
	GPIO_Init(SPI_SCLK_PORT, SPI_SCLK_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(SPI_SDO_PORT, SPI_SDO_PIN, GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(SPI_SDI_PORT, SPI_SDI_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(SPI_CS_PORT, SPI_CS_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
#endif

	GPIO_Init(SPI_IRQ_PORT, SPI_IRQ_PIN, GPIO_MODE_IN_PU_NO_IT);
	GPIO_ExternalPullUpConfig(SPI_IRQ_PORT, SPI_IRQ_PIN, ENABLE);
//  GPIO_ExternalPullUpConfig(SPI_IRQ_PORT, SPI_IRQ_PIN, ENABLE);

	SPI_CS_1;

	SPI->CR2 |= SPI_CR2_SSI;
	SPI->CR2 |= SPI_CR2_SSM;
	SPI_Init(SPI_FIRSTBIT_MSB,
		SPI_BAUDRATEPRESCALER_4,
		SPI_MODE_MASTER,
		SPI_CLOCKPOLARITY_LOW,
		SPI_CLOCKPHASE_1EDGE,
		SPI_DATADIRECTION_2LINES_FULLDUPLEX,
		SPI_NSS_SOFT,
		7);

	SPI_Cmd(ENABLE);

	SPI_SDN_1;
	DelayMs(100);
	SPI_SDN_0;

#elif defined(STM8L15X_HD)||defined(STM8L15X_LD)||defined(STM8L15X_MD)
	SPI_DeInit(SPI1);

	CLK_PeripheralClockConfig(CLK_Peripheral_SPI1, ENABLE);

  /* Set the MOSI,MISO and SCK at high level */
  GPIO_ExternalPullUpConfig(SPI_SCLK_PORT, SPI_SCLK_PIN , ENABLE);
	GPIO_ExternalPullUpConfig(SPI_SDO_PORT, SPI_SDO_PIN , ENABLE);
	GPIO_ExternalPullUpConfig(SPI_SDI_PORT, SPI_SDI_PIN, ENABLE);

  /* Configure FLASH_CS as Output push-pull, used as Flash Chip select */
  GPIO_Init(SPI_CS_PORT, SPI_CS_PIN, GPIO_Mode_Out_PP_High_Slow);

	GPIO_Init(SPI_IRQ_PORT, SPI_IRQ_PIN, GPIO_Mode_Out_PP_High_Slow);

	SPI_CS_1;

	SPI_Init(SPI1,
		SPI_FirstBit_MSB,
		SPI_BaudRatePrescaler_4,
		SPI_Mode_Master,
		SPI_CPOL_Low,
		SPI_CPHA_1Edge,
		SPI_Direction_2Lines_FullDuplex,
		SPI_NSS_Soft,
		7);

	SPI_Cmd(SPI1, ENABLE);
#endif


}

#if defined(STM8S207)
uint8_t SPI_RW(uint8_t byte)
{
  /* Loop while DR register in not emplty */
  while (SPI_GetFlagStatus(SPI_FLAG_TXE) == RESET);

  /* Send byte through the SPI peripheral */
  SPI_SendData( byte);

  /* Wait to receive a byte */
  while (SPI_GetFlagStatus(SPI_FLAG_RXNE) == RESET);

  /* Return the byte read from the SPI bus */
  return SPI_ReceiveData();
}

uint8_t Si4432_SPI_RW_Byte(uint8_t addr, uint8_t byte)
{
	uint8_t rtn;

	SPI_CS_0;

  /* Loop while DR register in not emplty */
  while (SPI_GetFlagStatus(SPI_FLAG_TXE) == RESET);

  /* Send byte through the SPI peripheral */
  SPI_SendData( addr);

  /* Wait to receive a byte */
  while (SPI_GetFlagStatus(SPI_FLAG_RXNE) == RESET);

	SPI_ReceiveData();

  /* Loop while DR register in not emplty */
  while (SPI_GetFlagStatus(SPI_FLAG_TXE) == RESET);

  /* Send byte through the SPI peripheral */
  SPI_SendData( byte);

  /* Wait to receive a byte */
  while (SPI_GetFlagStatus(SPI_FLAG_RXNE) == RESET);

  /* Return the byte read from the SPI bus */
  rtn = SPI_ReceiveData();

	SPI_CS_1;

	return rtn;
}

void Si4432_SPI_RW_Buffer(uint8_t addr, uint8_t *byte, uint8_t length)
{
	uint8_t i;

	SPI_CS_0;

  /* Loop while DR register in not emplty */
  while (SPI_GetFlagStatus(SPI_FLAG_TXE) == RESET);

  /* Send byte through the SPI peripheral */
  SPI_SendData( addr);

  /* Wait to receive a byte */
  while (SPI_GetFlagStatus(SPI_FLAG_RXNE) == RESET);

	SPI_ReceiveData();

	for(i=0; i < length; i++)
	{
	  /* Loop while DR register in not emplty */
	  while (SPI_GetFlagStatus(SPI_FLAG_TXE) == RESET);

	  /* Send byte through the SPI peripheral */
	  SPI_SendData( *byte);

	  /* Wait to receive a byte */
	  while (SPI_GetFlagStatus(SPI_FLAG_RXNE) == RESET);

	  /* Return the byte read from the SPI bus */
		if(addr & SI4432_WT_CMD)
		{
	  	*(byte+i) = SPI_ReceiveData();
		}
		else
		{
	  	*(byte+i) = SPI_ReceiveData();
		}
	}

	SPI_CS_1;
}
#elif defined(STM8L15X_HD)||defined(STM8L15X_LD)||defined(STM8L15X_MD)
uint8_t SPI_RW(uint8_t byte)
{
  /* Loop while DR register in not emplty */
  while (SPI_GetFlagStatus(SPI1, SPI_FLAG_TXE) == RESET);

  /* Send byte through the SPI peripheral */
  SPI_SendData(SPI1, byte);

  /* Wait to receive a byte */
  while (SPI_GetFlagStatus(SPI1, SPI_FLAG_RXNE) == RESET);

  /* Return the byte read from the SPI bus */
  return SPI_ReceiveData(SPI1);
}


uint8_t Si4432_SPI_RW_Byte(uint8_t addr, uint8_t byte)
{
	uint8_t rtn;

	SPI_CS_0;

  /* Loop while DR register in not emplty */
  while (SPI_GetFlagStatus(SPI1, SPI_FLAG_TXE) == RESET);

  /* Send byte through the SPI peripheral */
  SPI_SendData(SPI1, addr);

  /* Wait to receive a byte */
  while (SPI_GetFlagStatus(SPI1, SPI_FLAG_RXNE) == RESET);

	SPI_ReceiveData(SPI1);

  /* Loop while DR register in not emplty */
  while (SPI_GetFlagStatus(SPI1, SPI_FLAG_TXE) == RESET);

  /* Send byte through the SPI peripheral */
  SPI_SendData(SPI1, byte);

  /* Wait to receive a byte */
  while (SPI_GetFlagStatus(SPI1, SPI_FLAG_RXNE) == RESET);

  /* Return the byte read from the SPI bus */
  rtn = SPI_ReceiveData(SPI1);

	SPI_CS_1;

	return rtn;
}

void Si4432_SPI_RW_Buffer(uint8_t addr, uint8_t *byte, uint8_t length)
{
	uint8_t i;

	SPI_CS_0;

  /* Loop while DR register in not emplty */
  while (SPI_GetFlagStatus(SPI1, SPI_FLAG_TXE) == RESET);

  /* Send byte through the SPI peripheral */
  SPI_SendData(SPI1, addr);

  /* Wait to receive a byte */
  while (SPI_GetFlagStatus(SPI1, SPI_FLAG_RXNE) == RESET);

	SPI_ReceiveData(SPI1);

	for(i=0; i < length; i++)
	{
	  /* Loop while DR register in not emplty */
	  while (SPI_GetFlagStatus(SPI1, SPI_FLAG_TXE) == RESET);

	  /* Send byte through the SPI peripheral */
	  SPI_SendData(SPI1, *byte);

	  /* Wait to receive a byte */
	  while (SPI_GetFlagStatus(SPI1, SPI_FLAG_RXNE) == RESET);

	  /* Return the byte read from the SPI bus */
		if(addr & SI4432_WT_CMD)
		{
	  	*(byte+i) = SPI_ReceiveData(SPI1);
		}
		else
		{
	  	*(byte+i) = SPI_ReceiveData(SPI1);
		}
	}

	SPI_CS_1;
}
#endif

void E_TestSi4432(void)
{
	static uint8_t addr = 0;
	uint8_t tmp2;

//	SPI_SDN_0;
//	DelayMs(120);

	printf("\n\r");
	for(addr = 0; addr < 0x8; addr++)
	{
		tmp2 = Si4432_SPI_RW_Byte(addr,0x00);
		printf("(%2x)=%2x ", addr, tmp2);
	}
	printf("\n\r");

//	printf("%x", tmp);


//		SPI_SDN_1;

}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


void ClrSi4432Inpt(void)
{
	Si4432_SPI_RW_Byte(0x03,0x00);				// ��RFģ���ж�
	Si4432_SPI_RW_Byte(0x04,0x00);
}

#define SI4432_ReadReg(x)				Si4432_SPI_RW_Byte(x, SI4432_DATA_DUMMY)
#define SI4432_WriteReg(x, y)		Si4432_SPI_RW_Byte( (x | SI4432_WT_CMD), y )
#define delay_ms(x)				DelayMs(x)
#define SI4432_NIRQ		SPI_IRQ_STATUS

u8 ItStatus1,ItStatus2,res;

void InitSi4432Reg(void)
{
	res = SI4432_ReadReg(0x00);
	delay_ms(20);   printf("res =%02x \n",(unsigned short)res );
	res = SI4432_ReadReg(0x01);
	delay_ms(20);	  printf("res =%02x \n",(unsigned short)res );
	res = SI4432_ReadReg(0x02);
	delay_ms(20);	   printf("res =%02x \n",(unsigned short)res );
	delay_ms(1000);
//��ȡ�ж�״̬ 	 ����ж�  �ͷ�	 SI4432_NIRQ
	ItStatus1 = SI4432_ReadReg(0x03);
	ItStatus2 = SI4432_ReadReg(0x04);
//�����λ
	SI4432_WriteReg(0x07, 0x80); //��0X07��ַ  д��0X80  �����λ

	while( SI4432_NIRQ );
	delay_ms(30);

	//��ȡ�ж�״̬ 	 ����ж�  �ͷ�	 SI4432_NIRQ
	ItStatus1 = SI4432_ReadReg(0x03);
	ItStatus2 = SI4432_ReadReg(0x04);
/////////////////////////��ʼ����
// Ƶ������ 434
	SI4432_WriteReg(0x75, 0x53);
	SI4432_WriteReg(0x76, 0x64);  //
	SI4432_WriteReg(0x77, 0x00);
// 1.2K bps ��������
	SI4432_WriteReg(0x2a, 0x14);
	SI4432_WriteReg(0x6e, 0x09);
	SI4432_WriteReg(0x6f, 0xd5);
	SI4432_WriteReg(0x70, 0x2c);//

//SpiWriteRegister(0x72, 0x48);	//(9.6kbps)
    SI4432_WriteReg(0x72, 0x38);	//Ƶ��ƫ��(1.2kbps)
// ��������ø���Silabs ��Excel	(9.6 kbps, deviation: 45 kHz, channel filter BW: 102.2 kHz
	SI4432_WriteReg(0x1C, 0x1b);															//write 0x1E to the IF Filter Bandwidth register
	SI4432_WriteReg(0x20, 0x83);															//write 0xD0 to the Clock Recovery Oversampling Ratio register
	SI4432_WriteReg(0x21, 0xc0);															//write 0x00 to the Clock Recovery Offset 2 register
	SI4432_WriteReg(0x22, 0x13);															//write 0x9D to the Clock Recovery Offset 1 register
	SI4432_WriteReg(0x23, 0xa9);															//write 0x49 to the Clock Recovery Offset 0 register
	SI4432_WriteReg(0x24, 0x00);															//write 0x00 to the Clock Recovery Timing Loop Gain 1 register
	SI4432_WriteReg(0x25, 0x03);															//write 0x24 to the Clock Recovery Timing Loop Gain 0 register
	SI4432_WriteReg(0x1D, 0x40);															//write 0x40 to the AFC Loop Gearshift Override register
	SI4432_WriteReg(0x1E, 0x0A);															//write 0x0A to the AFC Timing Control register
	SI4432_WriteReg(0x2A, 0x14);															//write 0x20 to the AFC Limiter register
//ǰ���� ͬ����
	SI4432_WriteReg(0x34, 0X0A);  // ����5�ֽڵ�Preamble
	SI4432_WriteReg(0x35, 0x2A);  // ��Ҫ��� Preamble
	SI4432_WriteReg(0x33, 0x02);// ͬ����3,2 ��ͬ����
	SI4432_WriteReg(0x36, 0x2d);  // ͬ����Ϊ 0x2dd4
	SI4432_WriteReg(0x37, 0xd4);
	SI4432_WriteReg(0x30, 0x8D);   // ʹ��PH+ FIFOģʽ����λ��ǰ�棬ʹ��CRCУ��	CCITTT
    SI4432_WriteReg(0x32, 0x00 );//	��ֹ֡ͷ
	SI4432_WriteReg(0x71, 0x63); // ���䲻��Ҫ CLK��FiFo �� FSKģʽ

//GPIO
	SI4432_WriteReg(0x0b, 0x12); //
	SI4432_WriteReg(0x0c, 0x15); //
//��������
	SI4432_WriteReg(0x09, 0xD7);  //  ���ص���
	SI4432_WriteReg(0x69, 0x60);  //AGC����
//���书��
	SI4432_WriteReg(0x6d, 0x1e);
//�ֶ��򿪽���
	SI4432_WriteReg(0x07, 0x05);
//�� �����ж�
	SI4432_WriteReg(0x05, 0x03);
	SI4432_WriteReg(0x06, 0x00);
//���ж�
	ItStatus1 = SI4432_ReadReg(0x03);	  //read the Interrupt Status1 register
	ItStatus2 = SI4432_ReadReg(0x04);	  //read the Interrupt Status2 register

	#if SI4432_RX()
		SI4432_WriteReg(0x08, 0x02);
		SI4432_WriteReg(0x08, 0x00);
		SI4432_WriteReg(0x07, 0x05); 	//�ֶ��򿪽���
//		TX0_RX1;
	#elif SI4432_TX()
		SI4432_WriteReg(0x07, 0x05);//�򿪽���
		SI4432_WriteReg(0x0d, 0xf4);//GPIO2��������
//		TX1_RX0;
	#endif
/////////////////////////////////

}

#if SI4432_RX()
u8 SI4432_RxBUFF[100];
u8 SI4432_RxCount,SI4432_RxLenth;
void Si4432DecodeCommand(void);
void E_SI4432_Rx(void)
{
	return;
	if(  SI4432_NIRQ == RESET )// == 0 )	//��������ж�
	{
		printf("\n\r---\n\r");
    ItStatus1 = SI4432_ReadReg(0x03);		//read the Interrupt Status1 register
    ItStatus2 = SI4432_ReadReg(0x04);		//read the Interrupt Status2 register

		if ( (ItStatus1 & 0x02) == 0x02 )
		{
			SI4432_RxLenth = SI4432_ReadReg(0x4B);
			for (SI4432_RxCount=0;SI4432_RxCount < SI4432_RxLenth;SI4432_RxCount++)
			{
				SI4432_RxBUFF[SI4432_RxCount] = SI4432_ReadReg(0x7F);
				printf("%x ", SI4432_RxBUFF[SI4432_RxCount]);
			}
			Si4432DecodeCommand();

			SI4432_WriteReg(0x08, 0x02);
			SI4432_WriteReg(0x08, 0x00);
			SI4432_WriteReg(0x07, 0x05); //�ֶ��򿪽�?

			Si4432_SPI_RW_Byte(0x08|0x80, 0x03);  					//�巢�䣬���ջ�����
			Si4432_SPI_RW_Byte(0x08|0x80, 0x00);  					//�巢�䣬���ջ�����

		}
	}
}

#elif SI4432_TX()
u8 SI4432_TxBUFF[50];
u8 SI4432_TxCount;
//=========================================��������================================================
void E_SI4432_Tx(void)
{
int8_t i;
	res = SI4432_ReadReg(0x02); delay_ms(20);printf("res =%02x \n",(unsigned short)res );

	SI4432_WriteReg(0x07, 0x01);	// rf ģ�����Ready ģʽ
	delay_ms(20);		// ��ʱ 5ms, ��ϵͳ�ȶ�

	SI4432_WriteReg(0x3e, SI4432_TxCount);  // �ܹ�����x���ֽڵ�����
	for(i=0; i<SI4432_TxCount; i++)
	{
		SI4432_WriteReg(0x7F, SI4432_TxBUFF[i]);
	}

	SI4432_WriteReg(0x05, 0x04);	// �������ݷ�����󣬲����ж�
	SI4432_WriteReg(0x06, 0x00);
	ItStatus1 = SI4432_ReadReg(0x03);//����ж�
	ItStatus2 = SI4432_ReadReg(0x04);
	SI4432_WriteReg(0x07, 0x09);//�򿪷���

	while ( SI4432_NIRQ == 1);
	DelayMs(10);

	//read interrupt status registers to release the interrupt flags
	ItStatus1 = SI4432_ReadReg(0x03);	  	printf("ItStatus1 =%02x \n",(unsigned short)ItStatus1 );
	ItStatus2 = SI4432_ReadReg(0x04);	    printf("ItStatus2 =%02x \n",(unsigned short)ItStatus2 );
}
#endif


void E_InitSi4432(void)
{
	E_InitSi4432De();

	InitSi4432Reg();

}


