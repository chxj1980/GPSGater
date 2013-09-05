#include "stdafx.h"
#include "GpsJTT808.h"

GpsJTT808::GpsJTT808(void)
{
}

GpsJTT808::~GpsJTT808(void)
{
}

int FanZhuanYi(char *Bfr,int Length)
{
	int Dptr=0,i;
	for(i=1;i<Length-1;i++)
	{
		if(Bfr[i]==0x7d)
		{
			i++;
			if(Bfr[i]==0x02)
				Bfr[Dptr++]=0x7e;
			else if(Bfr[i]==0x01)
				Bfr[Dptr++]=0x7d;
		}
		else if(Bfr[i]==0x7e)
		{
			return Dptr;
		}
		else
			Bfr[Dptr++]=Bfr[i];
	}
	return Dptr;
}
int ZhuanYi(char* InBfr,char *OutBfr,int Length)
{
	int Dptr=0,i;
	OutBfr[Dptr++]=0x7e; //0�ֽڸ��0x7e

	for(i=0;i<Length;i++)
	{//��0��ʼ �������� ������ 0x7e
		if(InBfr[i]==0x7e)
		{
			OutBfr[Dptr++]=0x7d;
			OutBfr[Dptr++]=0x02;
		}
		else if(InBfr[i]==0x7d)
		{
			OutBfr[Dptr++]=0x7d;
			OutBfr[Dptr++]=0x01;
		}
		else
			OutBfr[Dptr++]=InBfr[i];
	}
	return Dptr;
}
/*
��ʶλ ��Ϣͷ ��Ϣ�� ������ ��ʶλ
0x7e							 0x7e

�ҵ���һ��0x7e
�õ��ܳ���
ת�廹ԭ
У��
������Ϣ
*/
char *g_pbuf="7E 01 02 00 07 01 57 00 00 00 01 00 05 72 6F 70 65 6E 74 65 21 7E";

void RemoveBlank(char *pBuf)
{
	char *pSrc=pBuf;
	char *pDest = pBuf;
	char *pEnd = pBuf+strlen(pBuf);
	for (;pSrc!=pEnd,pDest!=pEnd;)
	{
		int x = *pSrc++;
		/*
		if(pSrc==' '){
			pSrc++;
		}
		else{
			memcpy(pDest,pSrc,1);		
			pSrc++;
			pDest++;
		}
		/**/
	}
}
int getCheckCode_xor(const char *pstrBuf,int nStrLen,char *strCheckCode)
{
	long nCode = 0;
	for(int i=0;i<nStrLen;i++)
	{
		nCode =nCode^(pstrBuf[i]);
	}
	*strCheckCode=nCode&0xFF;
	//sprintf(strCheckCode,"%.2X",nCode&0xFF);
	return nCode&0xFF;
}

void buf2HexStr2(const char *pSrcbuf,char *pDestBuf,int nLen)
{
	if( nLen> 1024/2)	nLen = 1024;
	char *pPos = pDestBuf;
	*pPos = '_';
	pPos++;
	for(int i=0;i<nLen;i++)
	{
		sprintf(pPos,"%02x",(unsigned char)pSrcbuf[i]);
		pPos+=2;
	}	
}
long GpsJTT808::getGpsInfo( char *pSrcbuf,int nbufLen,GPSINFO &gpsInfo )
{
	//7E 01 02 00 07 01 57 00 00 00 01 00 05 72 6F 70 65 6E 74 65 21 7E	
	char *pTempBuf = new char[nbufLen];
	memcpy(pTempBuf,pSrcbuf,nbufLen);

	int nRet = 1;
	int nBufferDataLen = max(strlen(pTempBuf),nbufLen);
	int iTrueLen=1;
	for(;iTrueLen<nBufferDataLen;iTrueLen++){
		if(pTempBuf[iTrueLen]== 0x7e)break;
	}
	iTrueLen -=1;//ȥ��ͷβ
	if(iTrueLen<2){
		delete[] pTempBuf;	
		return -1;
	}
	FanZhuanYi(pTempBuf,iTrueLen+2);
	const char *pBuf =pTempBuf;
	//pBuf = buf+1;
	int nMask = *(pBuf+iTrueLen-1)&0xFF;

	char pCheckCode;
	int nCheckCode = getCheckCode_xor(pBuf,iTrueLen-1,&pCheckCode);
	if(nMask !=nCheckCode)
	{
		delete[] pTempBuf;
		return -2;
	}

	
	tagMsgHead msgHead;
	msgHead.msgID = MAKEWORD(pBuf[1],pBuf[0]);

	int nAttribute = MAKEWORD(pBuf[3],pBuf[2]);
	msgHead.msgBodyAttribute =nAttribute;//(pBuf[3]<<8)|pBuf[2];
	

	const char *pSim = pBuf+4;
	sprintf(msgHead.sim,"%2.2x%2.2x%2.2x%2.2x%2.2x%2.2x",
		pSim[0]&0xFF,pSim[1]&0xFF,pSim[2]&0xFF,pSim[3]&0xFF,pSim[4]&0xFF,pSim[5]&0xFF);
	strcpy(gpsInfo.COMMADDR,msgHead.sim);
	msgHead.msgSN = MAKEWORD(pBuf[11],pBuf[10]);//pBuf[11]<<8+pBuf[10];
	if(msgHead.msgBodyAttribute.bPaket)
	{
		msgHead.msgPaket.msgTotalPakets = MAKEWORD(pBuf[13],pBuf[12]);;
		msgHead.msgPaket.msgPaketIdx = MAKEWORD(pBuf[15],pBuf[14]);;
	}
	//����msgid ����msgboday
	nRet = diposMsgBody(msgHead,pBuf+16,&gpsInfo);
	if(nRet==0)
	{
		//д���ļ���������
		char pdestBuf[1024];
		ZeroMemory(pdestBuf,1024);
		buf2HexStr2(pSrcbuf,pdestBuf,nbufLen);

		WriteLog(_T("jtt808"),logLevelError,pdestBuf);
	}
	delete[] pTempBuf;
	return 1;
}

int getbin(int x)
{
	if(x>='0' && x<='9')
		return x-'0';

	if(x>='A' && x<='F')
		return x-'A'+10;
	return x-'a'+10;
}
BOOL GpsJTT808::getResMsg( char *strDestBuf,GPSINFO &gpsInfo )
{
	if(gpsInfo.nMsgID==MSG_LOGIN){
		int nMsgID = atoi(gpsInfo.PreFix);
		//��Ȩ�� ����
		tagMsgHead msgHead;
		msgHead.msgID=nMsgID;
		msgHead.msgBodyAttribute.bPaket=0;
		msgHead.msgBodyAttribute.Rev =0;
		msgHead.msgBodyAttribute.msgDecodeType=0;

		strcpy(msgHead.sim,gpsInfo.COMMADDR);//bcd[6]
		//msgHead.msgSN=atoi(gpsInfo.PreFix);
		char pTempBuf[20];
		pTempBuf[0] = 0x7E;
		pTempBuf[1] = msgHead.msgID/256;
		pTempBuf[2] = msgHead.msgID;
		int nCurpos = 5;
		for(int i=0;i<12;i+=2){			
			pTempBuf[nCurpos++]=((msgHead.sim[i]-0x30)*16)|(msgHead.sim[i+1]-0x30);			
		}

		///header End//////////////////////////
		//body
		//0 		�ն���Ϣ��ˮ��word
		//2			��ϢIDword
		//4			���byte

		pTempBuf[11]=gpsInfo.SEQ[0];
		pTempBuf[12]=gpsInfo.SEQ[1];
		msgHead.msgBodyAttribute.msgBodyLen=0;
		if(msgHead.msgID== 0x8001){
			msgHead.msgBodyAttribute.msgBodyLen=5;
			pTempBuf[13]=gpsInfo.SEQ[2];
			pTempBuf[14]=gpsInfo.SEQ[3];	
			pTempBuf[15]= 0;
			nCurpos = 15;
		}else if(msgHead.msgID== 0x8100){
			nCurpos=13;
			pTempBuf[nCurpos++]=0;
			//726f70656e7465
			char *pCode ="726f70656e7465";
			int sLen = strlen(pCode);			
			for(int i=0;i<sLen;i+=2)
			{
				pTempBuf[nCurpos++] =getbin(pCode[i])*16|getbin(pCode[i+1]);
			}

			msgHead.msgBodyAttribute.msgBodyLen=nCurpos-11;
			nCurpos--;
		}

		WORD msgBodyAttribute = msgHead.msgBodyAttribute.getWordValue();
		pTempBuf[3] = msgBodyAttribute/256;
		pTempBuf[4] = msgBodyAttribute;

		getCheckCode_xor(pTempBuf+1,nCurpos,&pTempBuf[nCurpos+1]);
		memset(strDestBuf,0,30);
		int nLen = ZhuanYi(pTempBuf+1,strDestBuf,nCurpos+1);
		strDestBuf[nLen]=0x7E;
        // 0  1 2  3 4  5 6 7 8 9 0  1 2  3 4 5 6 7
		//7e 0102 0007 015700000001 0da7 726f70656e7465 8e 7e
		//7e 0102 0007 015700000001 0dad 726f70656e7465 84 7e
		//7e 8001 0008 015700000001 0dac 0dac800100     35 7e

		//7e 0102 0007 015700000001 0007 726f70656e7465 23 7e			
		//7e 8001 0005 015700000001 0007 0007800100     52 7e
		//7e 8001 0005 015700000001 0007 010200         d7 7e
		return nLen+1;

	}
	return 0;
}

int GpsJTT808::diposMsgBody( tagMsgHead msgHead,const char *pMsgBody,GPSINFO *gpsInfo )
{
	int nRet = 0;
	switch(msgHead.msgID){
		case 0x0102:
			//0 ��Ȩ�� string �ն��������ϴ���Ȩ��
			//��Ҫƽ̨Ӧ��
			gpsInfo->nMsgID =MSG_LOGIN;
			gpsInfo->bValid=false;			
			/*
			�ظ���Ҫ��
			�ն���Ϣ��ˮ��word
			��ϢIDword
			���byte
			*/
			sprintf(gpsInfo->PreFix,"%d",0x8001);
			gpsInfo->SEQ[0] = msgHead.msgSN/256;
			gpsInfo->SEQ[1] = msgHead.msgSN;

			gpsInfo->SEQ[2] = msgHead.msgID/256;
			gpsInfo->SEQ[3] = msgHead.msgID;
			nRet = 1;
			break;
		case 0x0100:
			//7e0100002f015728572436000100111111373034303444522d3230413120202020202020202020202020526f706530303101d4c14147323031310000527e
			gpsInfo->nMsgID =MSG_LOGIN;
			gpsInfo->bValid=false;	

			sprintf(gpsInfo->PreFix,"%d",0x8100);

			gpsInfo->SEQ[0] = msgHead.msgSN/256;
			gpsInfo->SEQ[1] = msgHead.msgSN;

			gpsInfo->SEQ[2] = msgHead.msgID/256;
			gpsInfo->SEQ[3] = msgHead.msgID;

			nRet =1;
			break;
		default:
			{
				

			}
			break;
	}
	return nRet;
}

BOOL GpsJTT808::isThisProtocol( char *buf ,GPSINFO *pGpsInfo)
{
	if(buf[0]==0x7E)
	{
		pGpsInfo->nDevID +=GPS_JTT808;
		pGpsInfo->nDevID +=GPSID_TID;
		return TRUE;
	}
	return FALSE;
}

long GpsJTT808::_handleCmd_overspeed( GPSCommand*pGpsCommand,int nMaxSpeed,int nMinSpeed,int nContinue,int nValid/*=1*/ )
{
	return 0;
}

long GpsJTT808::_handleCmd_SetArea( GPSCommand*pGpsCommand,TCHAR *pAreaID,TCHAR *palertType,TCHAR *pType,TCHAR *pLeftLat,TCHAR *prightlat,TCHAR *pleftlng,TCHAR *prightlng,TCHAR *pcenterlat,TCHAR *pcenterlng,TCHAR *pRadius )
{
	return 0;
}

long GpsJTT808::_handleCmd_CacelArea( GPSCommand*pGpsCommand,TCHAR *pAreaID )
{
	return 0;
}

long GpsJTT808::_handleCmd_SetHeartTime( GPSCommand*pGpsCommand,TCHAR *pInterval )
{

	return 1;
}


long GpsJTT808::_handleCmd_SetTimingTrack( GPSCommand*pGpsCommand,TCHAR *pInterval,TCHAR*pReportCount )
{
	return 1;
}

long GpsJTT808::_handleCmd_SetTurnReport( GPSCommand*pGpsCommand,TCHAR*pAngle )
{
	return 1;
}

long GpsJTT808::_handleCmd_SetGPRSParam( GPSCommand*pGpsCommand,TCHAR*pLinkMod,TCHAR*pIP,TCHAR*pPort,TCHAR*pAPN,TCHAR*pAPN_UserName,TCHAR*pAPN_PassWord )
{

	return 1;
}

long GpsJTT808::_handleCmd_Set_BackGPRSServer( GPSCommand*pGpsCommand,TCHAR*pIP,TCHAR*pPort )
{
	return 1;

}

long GpsJTT808::_handleCmd_Get_ErrorCode( GPSCommand*pGpsCommand )
{
	return 1;

}

long GpsJTT808::_handleCmd_Set_VehicleType( GPSCommand*pGpsCommand,TCHAR*pVehicleType )
{
	return 1;
}

long GpsJTT808::_handleCmd_Get_VehicleType( GPSCommand*pGpsCommand )
{

	return 1;
}

long GpsJTT808::_handleCmd_Set_OpenOBD( GPSCommand*pGpsCommand,TCHAR*pFlag )
{
	return 1;

}

long GpsJTT808::_handleCmd_Set_OBDParam( GPSCommand*pGpsCommand,int nParamCount,void *pstArrParams )
{
	return 1;
}

long GpsJTT808::_handleCmd_Set_Reset_Mileage_and_Runtime( GPSCommand*pGpsCommand )
{

	return 1;
}

long GpsJTT808::_handleCmd_Get_Device_Version_and_SN( GPSCommand*pGpsCommand,TCHAR*psetType )
{

	return 1;
}

long GpsJTT808::_handleCmd_Set_Oil_LowAlarm( GPSCommand*pGpsCommand,TCHAR*poilvalue )
{

	return 1;
}

void GpsJTT808::getMsgID( char *buf,GPSINFO &gpsInfo )
{

}

char* GpsJTT808::getProtocolName( int  &nDevID )
{
	nDevID = GPS_JTT808;
	return _T("GpsJTT808");
}
