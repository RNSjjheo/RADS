//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "PacketBuffer.h"
//#include "UnitMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TPacketBuffer::TPacketBuffer()
{
	memset(Buffer, 0x00, 4096);	// 4K Byte
	fBufferSize = 4096;
	iNewDataPos = 0;
}
//---------------------------------------------------------------------------
TPacketBuffer::~TPacketBuffer()
{
	// nothing to do..
}

//---------------------------------------------------------------------------
int TPacketBuffer::WriteByte(char cByte)
{
	Buffer[iNewDataPos] = cByte;
	iNewDataPos++;

	return fBufferSize - iNewDataPos;
}

//---------------------------------------------------------------------------
int TPacketBuffer::WriteBytes(char *caBuffer, int iSize)
{
	for(int i = 0; i < iSize; i++)
	{
		Buffer[iNewDataPos++] = (char)caBuffer[i];
	} // for i

	return fBufferSize - iNewDataPos;
}

//---------------------------------------------------------------------------
int TPacketBuffer::GetFilledLength()
{
	return iNewDataPos;
}

//---------------------------------------------------------------------------
bool TPacketBuffer::GetPacket(char *Packet, int &iSize)
{
	bool bResult = false;
	iSize = 0;
	for(int i = 0; i < iNewDataPos; i++)
	{
		if ((unsigned char)Buffer[i] == (unsigned char)0xfa) //stx
		{
			int iDataLen = Buffer[i + 2];
			char chEtx = Buffer[i + 2 + iDataLen + 2]; //stx + 2(type + len) + data + 2(crc + etx)
			if ((unsigned char)chEtx == (unsigned char)0xf5) //etx
			{
				iSize = iDataLen + 5; //Data + 5(stx + type + len + crc + etx)
				memcpy(Packet, Buffer+i, iSize);

				// heaven2 2020.07.14
				BYTE xBuffer[256] = "";
				memcpy(xBuffer, Buffer+i+iSize, ((iNewDataPos-iSize)>0) ? (iNewDataPos-iSize) : 0);
				memset(Buffer, 0x00, sizeof(Buffer));
				memcpy(Buffer, xBuffer, ((iNewDataPos-iSize)>0) ? (iNewDataPos-iSize) : 0);
				iNewDataPos = ((iNewDataPos-iSize)>0) ? (iNewDataPos-iSize) : 0;
				//iNewDataPos = 0;
				bResult = true;
				break;
			} // if
		} // if
	} // for i
	return bResult;
}
//---------------------------------------------------------------------------

