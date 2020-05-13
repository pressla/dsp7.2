/*
 * touch_xsp2046.c
 *
 *  Created on: 15.08.2017
 *      Author: kurt
 */

#include "main.h"
/*
 * der Touch Controller ist an SPI3 angeschlossen
 */

void set_touchCS(void);
void reset_touchCS(void);
void WR_CMD (uint16_t cmd);
uint16_t RD_AD(void);

#define XPT2046_PORT		  GPIOC
#define XPT2046_MISO_PIN	  GPIO_Pin_11
#define XPT2046_MOSI_PIN	  GPIO_Pin_12
#define XPT2046_SCK_PIN		  GPIO_Pin_10
#define XPT2046_CS_PIN		  GPIO_Pin_9

#define	CHX 	0xD0
#define	CHY 	0x90
#define	CHZ 	0xB0

int set_touchref(int newleft, int newright, int newtop, int newbottom)
{
	if(newright < 0 || newright > 500) return 0;
	if(newleft < 3500 || newleft > 4095) return 0;
	if(newbottom < 0 || newbottom > 700) return 0;
	if(newtop < 3500 || newtop > 4095) return 0;

	if(pcfgdata->display_reverse != 0)
	{
		pcfgdata->touch_left = newleft;
		pcfgdata->touch_right = newright;
		pcfgdata->touch_top = newtop;
		pcfgdata->touch_bottom = newbottom;
	}
	else
	{
		pcfgdata->touch_right = newleft;
		pcfgdata->touch_left = newright;
		pcfgdata->touch_bottom = newtop;
		pcfgdata->touch_top = newbottom;
	}

	return 1;
}

void touch_init()
{
	  GPIO_InitTypeDef GPIO_InitStruct;
	  SPI_InitTypeDef  SPI_InitStructure;

	  /* Configure GPIOs ---------------------------------------------------------*/
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	  /* Configure SPI3 pins: SCK, MISO and MOSI ---------------------------------*/
	  GPIO_InitStruct.GPIO_Pin = XPT2046_SCK_PIN | XPT2046_MISO_PIN | XPT2046_MOSI_PIN;
	  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	  GPIO_Init(XPT2046_PORT, &GPIO_InitStruct);

	  /* TP_CS */
	  GPIO_InitStruct.GPIO_Pin = XPT2046_CS_PIN;
	  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	  GPIO_Init(XPT2046_PORT, &GPIO_InitStruct);

	  GPIO_PinAFConfig(XPT2046_PORT, GPIO_PinSource10, GPIO_AF_SPI3);
	  GPIO_PinAFConfig(XPT2046_PORT, GPIO_PinSource11, GPIO_AF_SPI3);
	  GPIO_PinAFConfig(XPT2046_PORT, GPIO_PinSource12, GPIO_AF_SPI3);

	  set_touchCS();

	  /* Configure SPI -----------------------------------------------------------*/
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
	  /* DISABLE SPI3 */
	  SPI_Cmd(SPI3, DISABLE);
	  /* SPI3 Config -------------------------------------------------------------*/
	  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
	  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	  SPI_InitStructure.SPI_CRCPolynomial = 7;
	  SPI_Init(SPI3, &SPI_InitStructure);
	  /* Enable SPI3 */
	  SPI_Cmd(SPI3, ENABLE);
}

void set_touchCS(void)
{
  GPIO_SetBits(XPT2046_PORT, XPT2046_CS_PIN);
}

void reset_touchCS(void)
{
  GPIO_ResetBits(XPT2046_PORT, XPT2046_CS_PIN);
}

uint16_t Read_X(void)
{
  uint16_t curr_X;

  reset_touchCS();
  WR_CMD(CHX);
  curr_X=RD_AD();
  set_touchCS();
  return curr_X;
}

uint16_t Read_Y(void)
{
  uint16_t curr_Y;

  reset_touchCS();
  WR_CMD(CHY);
  curr_Y=RD_AD();
  set_touchCS();
  return curr_Y;
}

uint16_t Read_Z(void)
{
  uint16_t curr_Z;

  reset_touchCS();
  WR_CMD(CHZ);
  curr_Z=RD_AD();
  set_touchCS();
  return curr_Z;
}

#define MAXTOUCHLOOPS 10

void del_value(uint16_t *arr, int mode)
{
	uint16_t vmin = 0, vmax=29999;
	uint16_t di = 0;
	for(uint16_t i=0; i<MAXTOUCHLOOPS; i++)
	{
		if(mode)
		{
			if(arr[i] > vmin)
			{
				vmin = arr[i];
				di = i;
			}
		}
		else
		{
			if(arr[i] < vmax)
			{
				vmax = arr[i];
				di = i;
			}
		}
	}
	arr[di] = 29999;
}

#define TOUCHMAXDIFF	150
int lastx,lasty;

int XPT2046_GetAdXY(int *x,int *y, int *rawx, int *rawy)
{
uint16_t i,vx,vy,vz,xanz,yanz,zanz;
uint16_t xarr[MAXTOUCHLOOPS],yarr[MAXTOUCHLOOPS],zarr[MAXTOUCHLOOPS];
int ret = 0;

	// Lese n Werte
	for(i=0; i<MAXTOUCHLOOPS; i++)
	{
		xarr[i] = Read_X();
		yarr[i] = Read_Y();
		zarr[i] = Read_Z();
	}

	// Lösche höchsten und niedrigsten Wert
	del_value(xarr,0);
	del_value(xarr,1);
	del_value(yarr,0);
	del_value(yarr,1);
	del_value(zarr,0);
	del_value(zarr,1);

	// bilde Mittelwert aus den verbliebenen Werten
	vx = vy = vz = xanz = yanz = zanz =0;
	for(i=0; i<MAXTOUCHLOOPS; i++)
	{
		if(xarr[i] < 9999) { vx += xarr[i]; xanz++; }
		if(yarr[i] < 9999) { vy += yarr[i]; yanz++; }
		if(zarr[i] < 9999) { vz += zarr[i]; zanz++; }
	}

	*x= vx/xanz;
	*y= vy/yanz;
	int z= vz/zanz;

	if(z > 100 && *x<=(lastx+TOUCHMAXDIFF) && *x>=(lastx-TOUCHMAXDIFF) && *y<=(lasty+TOUCHMAXDIFF) && *y>=(lasty-TOUCHMAXDIFF))
		ret = 1;

	*rawx = lastx = *x;
	*rawy = lasty = *y;

	// rechne Touch- in Displaykoordinaten um
	if(pcfgdata->touch_left > pcfgdata->touch_right)
		*x = ((*x - pcfgdata->touch_right) * TFT_WIDTH) / (pcfgdata->touch_left - pcfgdata->touch_right);
	else
		*x = ((*x - pcfgdata->touch_left) * TFT_WIDTH) / (pcfgdata->touch_right - pcfgdata->touch_left);
	*x = TFT_WIDTH - *x;

	if(pcfgdata->touch_top > pcfgdata->touch_bottom)
		*y = ((*y - pcfgdata->touch_bottom) * TFT_HEIGHT) / (pcfgdata->touch_top - pcfgdata->touch_bottom);
	else
		*y = ((*y - pcfgdata->touch_top) * TFT_HEIGHT) / (pcfgdata->touch_bottom - pcfgdata->touch_top);
	*y = TFT_HEIGHT - *y;

	if(pcfgdata->display_reverse == 0)
	{
		*x = TFT_WIDTH - *x;
		*y = TFT_HEIGHT - *y;
	}

	return ret;
}

void WR_CMD (uint16_t cmd)
{
  /* Wait for SPI3 Tx buffer empty */
  while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET);
  /* Send SPI3 data */
  SPI_I2S_SendData(SPI3,cmd);
  /* Wait for SPI3 data reception */
  while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET);
  /* Read SPI3 received data */
  SPI_I2S_ReceiveData(SPI3);
}

uint16_t RD_AD(void)
{
  uint16_t buf, temp;
  /* Wait for SPI3 Tx buffer empty */
  while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET);
  /* Send SPI3 data */
  SPI_I2S_SendData(SPI3,0x0000);
  /* Wait for SPI3 data reception */
  while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET);
  /* Read SPI3 received data */
  temp=SPI_I2S_ReceiveData(SPI3);
  buf=temp<<8;
  while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET);
  /* Send SPI3 data */
  SPI_I2S_SendData(SPI3,0x0000);
  /* Wait for SPI3 data reception */
  while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET);
  /* Read SPI3 received data */
  temp=SPI_I2S_ReceiveData(SPI3);
  buf |= temp;
  buf>>=3;
  buf&=0xfff;
  return buf;
}


