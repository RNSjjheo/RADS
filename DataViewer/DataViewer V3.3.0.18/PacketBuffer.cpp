//---------------------------------------------------------------------------

#pragma hdrstop

#include <vcl.h>
#include "PacketBuffer.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TPacketBuffer::TPacketBuffer()
{
    cs = new TCriticalSection;
	Buffer = NULL;
	fBufferSize = 0;
	iNewDataPos = 0;
}
//---------------------------------------------------------------------------
TPacketBuffer::~TPacketBuffer()
{
    delete cs;
	delete[] Buffer;
	fBufferSize = 0;
	iNewDataPos = 0;
}
//---------------------------------------------------------------------------
int TPacketBuffer::GetBufferSize()
{
	return fBufferSize;
}
//---------------------------------------------------------------------------
void TPacketBuffer::SetBufferSize(int iNewSize)
{
    cs->Acquire();
	if (Buffer) delete[] Buffer;
	Buffer = new char[iNewSize];
	fBufferSize = iNewSize;
	iNewDataPos = 0;
    cs->Release();
}
//---------------------------------------------------------------------------
int TPacketBuffer::WriteByte(char cByte)
{
    cs->Acquire();
	Buffer[iNewDataPos] = cByte;
	iNewDataPos++;
    cs->Release();

	return fBufferSize - iNewDataPos;
}
//---------------------------------------------------------------------------
int TPacketBuffer::WriteBytes(char *caBuffer, int iSize)
{
    cs->Acquire();
    for(int i = 0; i < iSize; i++)
    {
        Buffer[iNewDataPos++] = (char)caBuffer[i];
    } // for i
    cs->Release();

	return fBufferSize - iNewDataPos;
}
//---------------------------------------------------------------------------
int TPacketBuffer::GetFilledLength()
{
    return iNewDataPos;
}
//---------------------------------------------------------------------------
bool TPacketBuffer::ReadPacket(char *&DestPacket, int &iSize)
{
    cs->Acquire();
	bool bResult = False;
	DestPacket = NULL;
	iSize = 0;
	int idx_i = -1;
	for(int i = 0; i < iNewDataPos; i++)
	{
		if ((unsigned char)Buffer[i] == (unsigned char)0xfa) //stx
		{
			int iDataLen = Buffer[i + 2];
			char chEtx = Buffer[i + 2 + iDataLen + 2]; //stx + 2(type + len) + data + 2(crc + etx)
			if ((unsigned char)chEtx == (unsigned char)0xf5) //etx
			{
				//패킷 복사
				iSize = iDataLen + 5; //Data + 5(stx + type + len + crc + etx)
				DestPacket = new char[iSize];
				ZeroMemory(DestPacket, iSize);
				memcpy(DestPacket, Buffer + i, iSize);
				//메모리에서 제거
				int iBackupSize = fBufferSize - (i + iSize);
				char *Backup = new char[iBackupSize];
				memcpy(Backup, Buffer + i + iSize, iBackupSize);
				ZeroMemory(Buffer, fBufferSize);
				memcpy(Buffer, Backup, iBackupSize);
				iNewDataPos = iNewDataPos - (i + iSize);
                delete[] Backup;

				bResult = True;
				break;
			} // if
		} // if
	} // for i
    cs->Release();

	return bResult;
}

