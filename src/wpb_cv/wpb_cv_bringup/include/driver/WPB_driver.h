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

#include "SerialCom.h"
#pragma once

class CWPB_driver : public CSerialCom
{
public:
    CWPB_driver();
    ~CWPB_driver();
    void Parse(unsigned char inData);
	void Omni(float inX, float inY, float inAngular);
	void Mecanum(float inX, float inY, float inAngular);
	void Velocity2WD(float inX, float inAngular);
	void Velocity4WD(float inX, float inAngular);
	void SendMotors(int inMotor1, int inMotor2, int inMotor3, int inMotor4);
	float GetYaw();
	float fLinearAccLimit;
	float fAngularAccLimit;

	int arValIOInput[4];
	int arValIOOutput[8];
	int arValAD[15];
	int arMotorCurrent[4];
	int arMotorPos[4];
	int nParseCount;
	int arLastMotorVelocity[4];

	bool bNewAD_0_4;
	bool bNewAD_5_9;
	bool bNewAD_10_14;

	float fQuatW;
	float fQuatX;
	float fQuatY;
	float fQuatZ;
	
	float fGyroX;
	float fGyroY;
	float fGyroZ;
	
	float fAccX;
	float fAccY;
	float fAccZ;

	float fCurYaw;
	float fFirstYaw;
	bool bCalFirstYaw;

protected:
	unsigned char* m_SendBuf;
	unsigned char m_ParseBuf[128];
	int m_nRecvIndex;			//????????????
	unsigned char m_lastRecv;	//???????????????
	bool m_bFrameStart;			//???????????????
	int m_nFrameLength;			//?????????

	void m_CalSendSum(unsigned char* pNewCmdBuf);

	void m_ParseFrame();
	void m_DisRecv();
	int GenCmd(int inBuffOffset, int inDevID, int inModule, int inMethod, unsigned char* inData, int inDataLen);
};