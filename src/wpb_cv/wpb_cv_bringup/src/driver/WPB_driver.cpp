/*********************************************************************
* Software License Agreement (BSD License)
* 
*  Copyright (c) 2017-2020, Waterplus http://www.6-robot.com
*  All rights reserved.
* 
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
* 
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above
*     copyright notice, this list of conditions and the following
*     disclaimer in the documentation and/or other materials provided
*     with the distribution.
*   * Neither the name of the WaterPlus nor the names of its
*     contributors may be used to endorse or promote products derived
*     from this software without specific prior written permission.
* 
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
*  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
*  FOOTPRINTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
*  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
*********************************************************************/
/*!******************************************************************
 @author     ZhangWanjie
 ********************************************************************/

#include <driver/WPB_driver.h>
#include <math.h>

static bool bFirstQuart = true;

CWPB_driver::CWPB_driver()
{
   	m_SendBuf = new unsigned char[1024];
	memset(m_SendBuf, 0, 1024);
	memset(m_ParseBuf, 0, 128);
	m_nRecvIndex = 0;
	m_lastRecv = 0;
	m_bFrameStart = false;
	m_nFrameLength = 14;

	for (int i = 0; i < 15;i++)
	{
		arValAD[i] = 0;
	}
	for (int i = 0; i < 4;i++)
	{
		arMotorCurrent[i] = 0;
		arMotorPos[i] = 0;
		arLastMotorVelocity[i] = 0;
	}

	bNewAD_0_4 = false;
	bNewAD_5_9 = false;
	bNewAD_10_14 = false;

	nParseCount = 0;
	fQuatW = 0;
	fQuatX = 0;
	fQuatY = 0;
	fQuatZ = 0;
	
	fGyroX = 0;
	fGyroY = 0;
	fGyroZ = 0;
	
	fAccX = 0;
	fAccY = 0;
	fAccZ = 0;

	fCurYaw = 0;
	fFirstYaw = 0;
	bCalFirstYaw = false; 


	fLinearAccLimit = 0.2;
	fAngularAccLimit = 0.1;
}
    
CWPB_driver::~CWPB_driver()
{
	delete []m_SendBuf;
}


void CWPB_driver::Parse(unsigned char inData)
{
	m_ParseBuf[m_nRecvIndex] = inData;

	if (m_lastRecv == 0x55 && inData == 0xAA && m_bFrameStart == 0)
	{
		m_bFrameStart = 1;
		m_ParseBuf[0] = m_lastRecv;
		m_ParseBuf[1] = inData;
		m_nRecvIndex = 2;
		m_lastRecv = 0x00;
		return;
	}

	if (m_bFrameStart)
	{
		if (m_nRecvIndex == 3)
		{
			m_nFrameLength = inData + 8;
		}

		//put received data into buffer
		m_ParseBuf[m_nRecvIndex] = inData;
		m_nRecvIndex++;

		//receive one frame, invoke ParseFrame to parse
		if (m_nRecvIndex == m_nFrameLength)
		{
			m_DisRecv();
			m_ParseFrame();
			m_bFrameStart = false;
		}

		//receive buffer overflow
		if (m_nRecvIndex >= 128)
		{
			//m_ResetRcvBuf();
			m_bFrameStart = 0;
		}
	}
	else
		m_lastRecv = inData;
}


void CWPB_driver::m_CalSendSum(unsigned char* pNewCmdBuf)
{
	int nLen = pNewCmdBuf[3] + 7;

	pNewCmdBuf[nLen - 1] = 0x00;
	for (int i = 0; i < nLen - 1; i++)
	{
		pNewCmdBuf[nLen - 1] += pNewCmdBuf[i];
	}
}


void CWPB_driver::m_ParseFrame()
{
	nParseCount = 0;
	if (m_ParseBuf[4] == 0x06)	//IO
	{
	}

	if (m_ParseBuf[4] == 0x07)	//AD
	{
		if (m_ParseBuf[8] == 0x01) //AD 0~4
		{
			for (int i = 0; i < 5; i++)
			{
				arValAD[i] = m_USFromChar(&m_ParseBuf[9 + i * 2]);
			}
			bNewAD_0_4 = true;
		}
		if (m_ParseBuf[8] == 0x02) //AD 5~9
		{
			for (int i = 0; i < 5; i++)
			{
				arValAD[i+5] = m_USFromChar(&m_ParseBuf[9 + i * 2]);
			}
			bNewAD_5_9 = true;
		}
		if (m_ParseBuf[8] == 0x03) //AD 10~14
		{
			for (int i = 0; i < 5; i++)
			{
				arValAD[i+10] = m_USFromChar(&m_ParseBuf[9 + i * 2]);
			}
			bNewAD_10_14 = true;
		}
	}

	if (m_ParseBuf[4] == 0x08)	//Motor
	{
		int nCurMotorID = m_ParseBuf[8]-1;
		if (nCurMotorID < 4)
		{
			arMotorCurrent[nCurMotorID] = m_IntFromChar(&m_ParseBuf[9]);
			arMotorPos[nCurMotorID] = m_IntFromChar(&m_ParseBuf[13]);
		}
	}

	if (m_ParseBuf[4] == 0x09)	//IMU
	{
		if(m_ParseBuf[6] == 0x01)	//GYRO
		{
			fGyroX = (float)m_Piece2int(&m_ParseBuf[7]);
			fGyroY = (float)m_Piece2int(&m_ParseBuf[11]);
			fGyroZ = (float)m_Piece2int(&m_ParseBuf[15]);
		}
		if(m_ParseBuf[6] == 0x02)	//ACC
		{
			fAccX = (float)m_Piece2int(&m_ParseBuf[7]);
			fAccY = (float)m_Piece2int(&m_ParseBuf[11]);
			fAccZ = (float)m_Piece2int(&m_ParseBuf[15]);
		}
		if(m_ParseBuf[6] == 0x03)	//QUAT-W-X
		{
			fQuatW = (float)m_Piece2int(&m_ParseBuf[7]);
			fQuatX = (float)m_Piece2int(&m_ParseBuf[11]);
		}
		if(m_ParseBuf[6] == 0x04)	//QUAT-Y-Z
		{
			fQuatY = (float)m_Piece2int(&m_ParseBuf[7]);
			fQuatZ = (float)m_Piece2int(&m_ParseBuf[11]);
			// yaw: (about Z axis)
    		//fCurYaw = atan2(2*fQuatX*fQuatY - 2*fQuatW*fQuatZ, 2*fQuatW*fQuatW + 2*fQuatX*fQuatX - 1);
			//printf("[CWPB_driver] fYaw = %.2f\n",fCurYaw);
			if(bFirstQuart == true)
			{
				//fFirstYaw = fCurYaw;
				bCalFirstYaw = true;
				bFirstQuart = false;
			}
		}
	}
}


void CWPB_driver::m_DisRecv()
{
	
}



int CWPB_driver::GenCmd(int inBuffOffset, int inDevID, int inModule, int inMethod, unsigned char* inData, int inDataLen)
{
	int nCmdLen = 0;

	int nTailIndex = inBuffOffset + 7 + inDataLen;
	if (nTailIndex >= 1024)
	{
		return nCmdLen;
	}

	unsigned char* pNewCmd = m_SendBuf + inBuffOffset;
	pNewCmd[0] = 0x55;
	pNewCmd[1] = 0xaa;
	pNewCmd[2] = (unsigned char)inDevID;
	pNewCmd[3] = (unsigned char)inDataLen;
	pNewCmd[4] = (unsigned char)inModule;
	pNewCmd[5] = (unsigned char)inMethod;
	memcpy(&pNewCmd[6], inData, inDataLen);

	m_CalSendSum(pNewCmd);

	nCmdLen = inDataLen + 7;
	return nCmdLen;
}


void CWPB_driver::SendMotors(int inMotor1, int inMotor2, int inMotor3, int inMotor4)
{
	static unsigned char arMotorSpeedData[12];
	arMotorSpeedData[0] = 0;
	arMotorSpeedData[1] = 0x01;
	m_Split4Bytes(arMotorSpeedData + 2, inMotor1);
	arMotorSpeedData[6] = 0;
	arMotorSpeedData[7] = 0x02;
	m_Split4Bytes(arMotorSpeedData + 8, inMotor2);
	int nFirstCmdLenght = GenCmd(0, 0x40, 0x08, 0x60, arMotorSpeedData, 12);

	arMotorSpeedData[0] = 0;
	arMotorSpeedData[1] = 0x03;
	m_Split4Bytes(arMotorSpeedData + 2, inMotor3);
	arMotorSpeedData[6] = 0;
	arMotorSpeedData[7] = 0x04;
	m_Split4Bytes(arMotorSpeedData + 8, inMotor4);
	int nSecondCmdLenght = GenCmd(nFirstCmdLenght, 0x40, 0x08, 0x60, arMotorSpeedData, 12);

	int nTotalLen = nFirstCmdLenght + nSecondCmdLenght;
	Send(m_SendBuf, nTotalLen);

	arLastMotorVelocity[0] = inMotor1;
	arLastMotorVelocity[1] = inMotor2;
	arLastMotorVelocity[2] = inMotor3;
	arLastMotorVelocity[3] = inMotor4;
}

static float vkx = (float)sqrt(3.0f)*0.5;
static float fKLinearMotorK = 20000.0/6;
static float fKAngularMotorK = 563;
void CWPB_driver::Omni(float inX, float inY, float inAngular)
{
	//upward backward
	int nVectorX = inX * fKLinearMotorK;

	//shift left right
	int nVectorY = inY * fKLinearMotorK;

	//Turning 
	int nVectorTurn = inAngular * fKAngularMotorK;

	//Speed Value
	int nMotorToSend[4];
	nMotorToSend[0] = 0;	//left front
	nMotorToSend[0] = -vkx*nVectorX + nVectorY*0.5 + nVectorTurn;

	nMotorToSend[1] = 0;	//right front
	nMotorToSend[1] = vkx*nVectorX + nVectorY*0.5 + nVectorTurn;

	nMotorToSend[2] = 0;	//back 
	nMotorToSend[2] = -nVectorY + nVectorTurn;

	nMotorToSend[3] = 0;	//NC

	SendMotors(nMotorToSend[0],nMotorToSend[1],nMotorToSend[2],nMotorToSend[3]);
}

static float fKMecanumLinearMotorK = 3170;
static float fKMecanumLinearMotorKY = 3500;
static float fKMecanumAngularMotorK = 770;
void CWPB_driver::Mecanum(float inX, float inY, float inAngular)
{
	int nMotorToSend[4];
	//upward backward
	int nTmpMotorVal = inX * fKMecanumLinearMotorK;
	nMotorToSend[0] = -nTmpMotorVal;
	nMotorToSend[1] = nTmpMotorVal;
	nMotorToSend[2] = nTmpMotorVal;
	nMotorToSend[3] = -nTmpMotorVal;

	//shif left right
	nTmpMotorVal = inY * fKMecanumLinearMotorKY;
	nMotorToSend[0] += nTmpMotorVal;
	nMotorToSend[1] += nTmpMotorVal;
	nMotorToSend[2] += -nTmpMotorVal;
	nMotorToSend[3] += -nTmpMotorVal;

	//Turning 
	nTmpMotorVal = inAngular * fKMecanumAngularMotorK;
	nMotorToSend[0] += nTmpMotorVal;
	nMotorToSend[1] += nTmpMotorVal;
	nMotorToSend[2] += nTmpMotorVal;
	nMotorToSend[3] += nTmpMotorVal;

	//printf("[CWPB_driver::Mecanum]-> [0]%d [1]%d [2]%d [3]%d \n", nMotorToSend[0], nMotorToSend[1], nMotorToSend[2], nMotorToSend[3]);

	SendMotors(nMotorToSend[0],nMotorToSend[1],nMotorToSend[2],nMotorToSend[3]);
}

static float fK2WDLinearMotorK = 3370;
static float fK2WDAngularMotorK = 470;
void CWPB_driver::Velocity2WD(float inX, float inAngular)
{
	int nMotorToSend[4];
	//upward backward
	int nTmpMotorVal = inX * fK2WDLinearMotorK;
	nMotorToSend[0] = -nTmpMotorVal;
	nMotorToSend[1] = nTmpMotorVal;
	//Turning 
	nTmpMotorVal = inAngular * fK2WDAngularMotorK;
	nMotorToSend[0] += nTmpMotorVal;
	nMotorToSend[1] += nTmpMotorVal;

	nMotorToSend[2] = arLastMotorVelocity[2];
	nMotorToSend[3] = arLastMotorVelocity[3];
	//printf("[CWPB_driver::Velocity2WD]-> [0]%d [1]%d [2]%d [3]%d \n", nMotorToSend[0], nMotorToSend[1], nMotorToSend[2], nMotorToSend[3]);

	SendMotors(nMotorToSend[0],nMotorToSend[1],nMotorToSend[2],nMotorToSend[3]);
}

static float fK4WDLinearMotorK = 3300;
static float fK4WDAngularMotorK = 630;
void CWPB_driver::Velocity4WD(float inX, float inAngular)
{
	int nMotorToSend[4];
	//upward backward
	int nTmpMotorVal = inX * fK4WDLinearMotorK;
	nMotorToSend[0] = -nTmpMotorVal;
	nMotorToSend[1] = nTmpMotorVal;
	nMotorToSend[2] = nTmpMotorVal;
	nMotorToSend[3] = -nTmpMotorVal;

	//Turning 
	nTmpMotorVal = inAngular * fK4WDAngularMotorK;
	nMotorToSend[0] += nTmpMotorVal;
	nMotorToSend[1] += nTmpMotorVal;
	nMotorToSend[2] += nTmpMotorVal;
	nMotorToSend[3] += nTmpMotorVal;

	//printf("[CWPB_driver::Mecanum]-> [0]%d [1]%d [2]%d [3]%d \n", nMotorToSend[0], nMotorToSend[1], nMotorToSend[2], nMotorToSend[3]);

	SendMotors(nMotorToSend[0],nMotorToSend[1],nMotorToSend[2],nMotorToSend[3]);
}

float CWPB_driver::GetYaw()
{
	float diffYaw = fCurYaw - fFirstYaw;
	return diffYaw;
}