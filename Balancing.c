/*
 * Balancing.c
 *
 *  Created on: 2021. 9. 15.
 *      Author: 82109
 */

#include "Coulomb.h"
#include "Defines.h"
#include "SIUL.h"
#include "BCC1.h"
#include "PIT.h"

//�ʱ⼳��� ���� ���� ���� �ϱ������

// 80%�� �� �� ��, 3.3181�� �ɶ� �ߴ��� �ʿ�

// ���� �� �޽ı⸦ ������ Ÿ�̸ӿ� ���� üũ�ϰ� ��ȣ�� �޾ƿ��� �뷷���� ����
// �뷱��... ���� ���� ���� �������� ��´�.
// Ž���ϱ� ���� ȿ������ �˰����� ����ƿ��



/* for(Cid = 1; Cid <= u8NoOfNodes; Cid++){
for(j = 1; j <= 14; j++){

	if (((float)(BatPack.u16VoltCell[Cid][j]))>Soc20to70_Voc[8]) Stop_Charging();
	}
}

*/

//case�� ������ ����

void PreCharging_Start(void){
	//���� ������!
	SIUL_SetPad(SIUL_PD9);
	//delay Ʈ���� �����ΰ�... �� Ʈ���ſ� ������ �ְ� ...�� �ʹ� ���� �װŵȴ�... �׳� �����̸� �ִ°� �´�.
	Delayms(1000);
	SIUL_SetPad(SIUL_PD10);
}

void Stop_PR_Off_And_CR_ON(void){ // Precharging Relay Off and Charging Relay On
	SIUL_SetPad(SIUL_PD11);
		//delay
	Delayms(1000);
	SIUL_ClearPad(SIUL_PD10);
		//delay
	Delayms(1000);
}

void Stop_Charging(void){
	SIUL_ClearPad(SIUL_PD9);
	SIUL_ClearPad(SIUL_PD10);
	SIUL_ClearPad(SIUL_PD11); // ��ȣ�ָ�... ������..?
}

/*
 * preCharging_Init
 * 	if (BatPack.u16VoltCell[Cid][0] < Soc20to70_Voc[0]*14){
 * 		Stop_PR_Off_And_CR_ON();
	};//�������к��� �۰ų� ���� ��Ȳ�̸�

	��� �ð��� ���� ���� (timer�� ����?)

	if (BatPack.u16VoltCell[Cid][0] < Soc20to70_Voc[10]*14){
		Stop_Charging();
	}

}
*/

/*
 * SIUL_EIRQ20
 * SIUL_EnableExtIntFallingEdge(SIUL_EIRQ20);
 * INTC_0.PSR[245].R = 0x800A;
 *
 *
 */

BCC_TError BCC_StartCellBalancing(uint8_t Cid, uint8_t u8NoOfCells){
	BCC_TError       res;
	uint16_t         RegVal;     /* Value of BCC_REG_CB_CFG_ADDR register. */
	RegVal = BCC_REG_CBX_CFG_DEFAULT | BCC_CB_ENABLED | BCC_CB_ON_TIMER(10);

	switch(u8NoOfCells){
		case 1:
			res = BCC1_WriteRegister(Cid, BCC_REG_CB1_CFG_ADDR, RegVal, NULL);
			break;
		case 2:
			res = BCC1_WriteRegister(Cid, BCC_REG_CB2_CFG_ADDR, RegVal, NULL);
			break;
		case 3:
			res = BCC1_WriteRegister(Cid, BCC_REG_CB3_CFG_ADDR, RegVal, NULL);
			break;
		case 4:
			res = BCC1_WriteRegister(Cid, BCC_REG_CB4_CFG_ADDR, RegVal, NULL);
			break;
		case 5:
			res = BCC1_WriteRegister(Cid, BCC_REG_CB5_CFG_ADDR, RegVal, NULL);
			break;
		case 6:
			res = BCC1_WriteRegister(Cid, BCC_REG_CB6_CFG_ADDR, RegVal, NULL);
			break;
		case 7:
			res = BCC1_WriteRegister(Cid, BCC_REG_CB7_CFG_ADDR, RegVal, NULL);
			break;
		case 8:
			res = BCC1_WriteRegister(Cid, BCC_REG_CB8_CFG_ADDR, RegVal, NULL);
			break;
		case 9:
			res = BCC1_WriteRegister(Cid, BCC_REG_CB9_CFG_ADDR, RegVal, NULL);
			break;
		case 10:
			res = BCC1_WriteRegister(Cid, BCC_REG_CB10_CFG_ADDR, RegVal, NULL);
			break;
		case 11:
			res = BCC1_WriteRegister(Cid, BCC_REG_CB11_CFG_ADDR, RegVal, NULL);
			break;
		case 12:
			res = BCC1_WriteRegister(Cid, BCC_REG_CB12_CFG_ADDR, RegVal, NULL);
			break;
		case 13:
			res = BCC1_WriteRegister(Cid, BCC_REG_CB13_CFG_ADDR, RegVal, NULL);
			break;
		case 14:
			res = BCC1_WriteRegister(Cid, BCC_REG_CB14_CFG_ADDR, RegVal, NULL);
			break;
	}
	return res;
}


BCC_TError BCC_StopCellBalancing(uint8_t Cid, uint8_t u8NoOfCells){
	BCC_TError       res;
	uint16_t         RegVal;     /* Value of BCC_REG_CB_CFG_ADDR register. */
	RegVal = BCC_REG_CBX_CFG_DEFAULT | BCC_CB_DISABLED | BCC_CB_ON_TIMER(0);

	switch(u8NoOfCells){
		case 1:
			res = BCC1_WriteRegister(Cid, BCC_REG_CB1_CFG_ADDR, RegVal, NULL);
			break;
		case 2:
			res = BCC1_WriteRegister(Cid, BCC_REG_CB2_CFG_ADDR, RegVal, NULL);
			break;
		case 3:
			res = BCC1_WriteRegister(Cid, BCC_REG_CB3_CFG_ADDR, RegVal, NULL);
			break;
		case 4:
			res = BCC1_WriteRegister(Cid, BCC_REG_CB4_CFG_ADDR, RegVal, NULL);
			break;
		case 5:
			res = BCC1_WriteRegister(Cid, BCC_REG_CB5_CFG_ADDR, RegVal, NULL);
			break;
		case 6:
			res = BCC1_WriteRegister(Cid, BCC_REG_CB6_CFG_ADDR, RegVal, NULL);
			break;
		case 7:
			res = BCC1_WriteRegister(Cid, BCC_REG_CB7_CFG_ADDR, RegVal, NULL);
			break;
		case 8:
			res = BCC1_WriteRegister(Cid, BCC_REG_CB8_CFG_ADDR, RegVal, NULL);
			break;
		case 9:
			res = BCC1_WriteRegister(Cid, BCC_REG_CB9_CFG_ADDR, RegVal, NULL);
			break;
		case 10:
			res = BCC1_WriteRegister(Cid, BCC_REG_CB10_CFG_ADDR, RegVal, NULL);
			break;
		case 11:
			res = BCC1_WriteRegister(Cid, BCC_REG_CB11_CFG_ADDR, RegVal, NULL);
			break;
		case 12:
			res = BCC1_WriteRegister(Cid, BCC_REG_CB12_CFG_ADDR, RegVal, NULL);
			break;
		case 13:
			res = BCC1_WriteRegister(Cid, BCC_REG_CB13_CFG_ADDR, RegVal, NULL);
			break;
		case 14:
			res = BCC1_WriteRegister(Cid, BCC_REG_CB14_CFG_ADDR, RegVal, NULL);
			break;
	}
	return res;
}

BCC_TError Cell_Balancing(uint8_t Cid){
	BCC_TError       res;
	uint8_t       	i, count;
	uint16_t 		minVoltCell;
	minVoltCell = BatPack.u16VoltCell[Cid][BatPack.u8MinIndexSocCell[Cid]];

	for(i=1;i<15;i++){
		if((BatPack.u16VoltCell[Cid][i] <= minVoltCell+10) && (BatPack.u16VoltCell[Cid][i] >= minVoltCell-10)){
			res = BCC_StopCellBalancing(Cid,i);
		}
		else if(BatPack.u16VoltCell[Cid][i] > minVoltCell+10){
			res = BCC_StartCellBalancing(Cid,i);
			count++;

		}
		else{
			res = BCC_StopCellBalancing(Cid,i);
		}
	}

	if(count == 0){
		BatPack.ChargingMode = 3;
	}
	return res;
}
