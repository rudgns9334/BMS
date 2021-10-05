/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

#include "derivative.h" /* include peripheral declarations */

extern void xcptn_xmpl(void);

//__attribute__ ((section(".text")))

#include "MPC5744P.h"
#include "BCC1.h"
#include "BCC1_tpl.h"
#include "Balancing_init.h"
#include "Balancing.h"
#include "Defines.h"
#include "ME.h"
#include "FCCU.h"
#include "ADC.h"
#include "DSPI.h"
#include "CAN.h"
#include "INTC.h"
#include "PIT.h"
#include "SIUL.h"
#include "CANGui.h"
#include "Power.h"
#include "Eswitch.h"
//#include "DeviceAPP.h"
#include "PwSBC.h"
#include <stdio.h>


uint8_t u8NoOfNodes;
uint8_t u8SeclecCID = 0;
TYPE_BatteryPack BatPack;
TYPE_BCCmode     BCCmode;
TYPE_ChargingMode	ChargingMode;
uint16_t    BCC_ConfigReg[BCC1_DEVICE_CNT][30];
uint16_t    BCC_StatusReg[BCC1_DEVICE_CNT][15];
uint8_t BCC_ConfigRegAddr[30];
uint8_t BCC_StatusRegAddr[15];
uint8_t test[5]={0x01,0x01,0x03,0x02,0x00};

void PIT0_isr(void) {
	static uint8_t counter=0;/* Increment ISR counter */
	BCC_TError  res;

	counter++;
	if(counter==600){
		if(ChargingMode==Balancing){
			res = Cell_Balancing(1);
			if(res==ERR_OK){
				SerialPrint_ln("\n");
				SerialPrint_val(3);
			}
		}
	}
    PIT_0.TIMER[0].TFLG.R |= 1;  /* Clear interrupt flag. w1c */
}


int main(void)
{

	BCC_TError  res;
	uint16_t    BCC_Status[BCC1_DEVICE_CNT][14];
	uint8_t 	CidNodes = 1;
	uint8_t 	u8Data = 0 ;
	uint8_t 	u8Command = 0;
	uint32_t    u32DiagRes = 0;
	uint16_t    ADC_result[4];
	uint16_t    u16SubData = 0, u16RegVal, RtermFlag = 0;



	/****System Init******/
//	FCCU_SetProtocol(FCCU_BI_STABLE);

	MC_MODE_INIT();
	xcptn_xmpl ();              /* Configure and Enable Interrupts */
	INTC_Init();
	SIUL_vInit();
    PIT_Init();

	ADC_Init(ADC_nb0, CH4Smask|CH6Smask, 0 ,ADC0_MCR);
	ADC_Init(ADC_nb1, CH5Smask, 0 ,ADC0_MCR);

    LINFLEX_InitUART(0, MCU_CLK, 115200);
    DSPI_Init(BCC_TX,1, MCU_CLK, BCC_BR, 8);
    DSPI_Init(BCC_RX,0, MCU_CLK, BCC_BR, 8);
    DSPI_Init(ES_SPI,1, MCU_CLK, BCC_BR, 8);
//    DSPI_Init(PWR_SPI,1, MCU_CLK, PWR_BR, 16);
//    DSPI_Enable_IRQ(PWR_SPI);
    DSPI_Enable_IRQ(BCC_RX);
//    DSPI_Enable_IRQ(ES_SPI);
//    SIUL_SetPad(SIUL_PE14);
	CAN_Init(0);
	CAN_Init(1);

	SPQ_Init();
//	Power_Init();

	GPIO_Init();
//	SIUL_EIRQ20_Init();
	PIT_0.TIMER[0].LDVAL.R = 50 * 1000 * 10000;
//	PIT_EnableInt(0);
//	INTC_0.PSR[226].R = 0x800A;
	PIT_EnableChannel(0);
//	PIT_0.MCR.B.FRZ = 0;


	CAN_0.MB[9].ID.R = 0x18800000; /* Ext.ID = 0x12340000 */
    CAN_0.MB[9].CS.R = 0x04200000; /* Rx active and empty + IDE */


	/****BCC Init****/
	BCCmode = BCC_Init;
	u8NoOfNodes = BCC1_DEVICE_CNT;
	u8NoOfNodes = 1;

	ChargingMode = PreCharing_Start;

	/******interrupt******/
	/*
	void SIULINT2_isr(){
		if (SIUL_GetExtIntFlag(SIUL_EIRQ20)){
			switch(ChargingMode){
				case PreCharing_Start:
					PreCharging_Start();
					break;

				case Charging_Start:
					Stop_PR_Off_And_CR_ON();
					break;

				case Charging_Stop:
					Stop_Charging();
					break;

				default:
					break;
			}
		}
		SIUL_ClearExtIntFlag(SIUL_EIRQ20);
	}

*/


	/*SerialPrint_ln("**start**\r\n%	");
	uint8_t c=1,arr[80];
	for(c=15;c>0;c--)
	{
		sprintf(arr,"%5d ",c);
		SerialPrint_ln(arr);
	}
	SerialPrint_ln("\r\n");
*/
	//SIUL_SetPad(SIUL_PD9);
	uint8_t countererer=0;
	SIUL_DigitalOutput(SIUL_PD9, SIUL_GPIO);
	/*********test*************/
	while(1) {
		//
		SerialPrint_val(PIT_0.TIMER[0].CVAL.R);
		if(countererer%2==0){
			SIUL_SetPad(SIUL_PD9);
		}
		else{
			SIUL_ClearPad(SIUL_PD9);
		}
		countererer++;
		if(BatPack.ChargingMode == 3){
			ChargingMode = 0;
		}
		//if(SIUL_GetPadState(SIUL_PB15)){
		switch(ChargingMode){
			case PreCharing_Start:
				PreCharging_Start();
				break;

			case Charging_Start:
				Stop_PR_Off_And_CR_ON();
				break;

			case Charging_Stop:
				Stop_Charging();
				ChargingMode = Balancing;
				break;

			case Balancing:
				res = Cell_Balancing(1);
				if(res==ERR_OK){

				}

			default:
				break;
			}
		//}


		switch(BCCmode)  {

			case BCC_Init:
//				LEDHandler(Off);
				/* initialize BCC TPL daisy chain */
				res = BCC1_Init(u8NoOfNodes);											// assigns CID
				if(res==ERR_OK)	{
//					BCC1_SOC_Init(u8NoOfNodes);
					for(CidNodes = 1; CidNodes <=u8NoOfNodes; CidNodes++){
						res = BCC1_ClearFaultStatus(CidNodes);
					}
					BCCmode = BCC_Measure;
				}
				else{
					//CAN_SendError2GUI(0, res);
					Delayms(1000);
					Delayms(1000);
				}
				break;

			case BCC_Measure:

				/* Start conversion. */
				ADC_StartNormalConversion(ADC_nb0, CH4Smask|CH6Smask);
				ADC_StartNormalConversion(ADC_nb1, CH5Smask);

				for(CidNodes = 1; CidNodes <=u8NoOfNodes; CidNodes++){

					if ((res = BCC1_StartConversion(CidNodes)) != ERR_OK) {
						//CAN_SendError2GUI(CidNodes, res);
					}

					Delayus(800);

					do{
						res = BCC1_IstConverting(CidNodes, &u16RegVal);
						if(u16RegVal == 0)
							break;
					}
					while(res == ERR_OK);

					if(res != ERR_OK)
						//CAN_SendError2GUI(CidNodes, res);

                   /****get Status register value ****/
					if( BCC1_GetFaultStatus(CidNodes,BCC_Status) == ERR_OK)  {
						//CAN_SendStatus2GUI(CidNodes,BCC_Status);
					}
					else{
						SerialPrint_ln("fault!\r\n");
					}
						//CAN_SendError2GUI(CidNodes, res);

    				/* Read measured values. */
					res = BCC1_GetRawMeasurements(CidNodes);
					if(res==ERR_OK)  {
//						BCC1_SOC_Calculation(CidNodes);
						//CAN_SendData2GUI(CidNodes);
						//SerialPrint_ln("Pass!\r\n");
						if(BatPack.ChargingMode == 1){
							ChargingMode = Charging_Start;
						}
						if(BatPack.ChargingMode == 2){
							ChargingMode = Charging_Stop;
						}
					}
					else
					{
						//CAN_SendError2GUI(CidNodes, res);
					}
                   /****Check Slave CID and send Term Resistor Status***/
					res = BCC1_CheckID(CidNodes, &u16RegVal);
					if(res != ERR_OK){
						if ((res = BCC1_CheckID(CidNodes, &u16RegVal)) != ERR_OK){
							BCCmode = BCC_Init;
							break;
						}
					}
				}

				ADC_result[0] = ADC_GetChannelValue(ADC_nb0, 4);
				ADC_result[1] = ADC_GetChannelValue(ADC_nb0, 6);
				ADC_result[2] = ADC_GetChannelValue(ADC_nb1, 5);
				//CAN_SendLVSig2GUI(ADC_result);

				BCCmode = BCC_Refresh;
				break;
		case BCC_Refresh:
            /*******send required register value to Gui***********/
			if((u8SeclecCID >0)&&(u8SeclecCID <=15)){
				res = BCC1_GetCtrlRegMap(u8SeclecCID, BCC_ConfigReg, BCC_ConfigRegAddr);
				if(res ==  ERR_OK){
					//CAN_SendCtrlReg2GUI(u8SeclecCID, BCC_ConfigReg, BCC_ConfigRegAddr);
			    }
				else{
				   // CAN_SendError2GUI(CidNodes, res);
				}
				res = BCC1_GetSTSRegMap(u8SeclecCID, BCC_StatusReg, BCC_StatusRegAddr);
				if(res ==  ERR_OK){
					//CAN_SendSTSReg2GUI(u8SeclecCID, BCC_StatusReg, BCC_StatusRegAddr);
					//CAN_SendError2GUI(CidNodes, 0x00);
			    }
				else{
				   // CAN_SendError2GUI(CidNodes, res);
				}
			}
			BCCmode = BCC_Measure;
			break;

			case BCC_Diag:

				break;

			case BCC_Idle:
				Delayms(1000);
				BCCmode = BCC_Idle;
				break;

			default:

				// no recovery implemented -> need to reset device
				Delayms(1000);
				BCCmode = BCC_Init;										// error handling
			break;

		}

		/* Process CAN command from GUI */

		if(0 == CAN_RevCmdProcess(&u8Command, &u8Data, &u16SubData))
		{
			switch(u8Command){
			case 0xC1: //Global Reset
				if(ERR_OK == BCC1_WriteRegisterGlobal(BCC_REG_SYS_CFG1_ADDR, BCC_W_SOFT_RST_MASK)){
					BCCmode = BCC_Idle;
				}
				break;
			case 0xC2: //initial
				u8NoOfNodes = u8Data;
				BCCmode = BCC_Init;
				break;
			case 0xC3: //reset
				break;
			case 0xC4: //sleep mode
				if(ERR_OK == BCC1_Sleep()){
					BCCmode = BCC_Idle;
				}
				break;
			case 0xC5: //wakeup */
				/* Wake-up BCC (in case of IDLE mode). */

				if (ERR_OK == BCC1_WakeUp()) {
					BCCmode = BCC_Measure;
				}
				break;
			case 0xC7:  /*Crash Diag */

				if(u8Data == 0x01){
					SIUL_ClearPad(SIUL_PD3);
				}
				else if(u8Data == 0x00){
					SIUL_SetPad(SIUL_PD3);
				}
				break;
			case 0xC8: //Eswitch control relay connector
				if((u8Data>>0)&0xF){
					SIUL_SetPad(SIUL_PD2);
				}
				else{
					SIUL_ClearPad(SIUL_PD2);
				}
				Delayms(10);
				/*PC control Eswitch*/

				ES_SetOutput_SPI(((u8Data>>0)&0xF)*100, ((u8Data>>4)&0xF)*100);
				break;
			case 0xCF:
				break;
			case 0xCE:
				u8SeclecCID = u16SubData&0xF;
				break;
			case 0xCD:
				res = BCC1_WriteRegister(u8SeclecCID, u8Data, u16SubData, NULL);
				break;
			case 0xCC:
				res = BCC1_WriteRegister(u8SeclecCID, u8Data, u16SubData, NULL);
				break;
			default:
				break;
			}
		}
	}
	return 0;
}
