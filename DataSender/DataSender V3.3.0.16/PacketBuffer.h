//---------------------------------------------------------------------------

#ifndef PacketBufferH
#define PacketBufferH
//---------------------------------------------------------------------------
class TPacketBuffer
{
protected:
private:
	char Buffer[4096];
	int  fBufferSize;
	int  iNewDataPos;
public:
	TPacketBuffer();
	~TPacketBuffer();
	int  GetFilledLength();
	int  WriteByte(char cByte);
	int  WriteBytes(char *caBuffer, int iSize);
	bool GetPacket(char *Packet, int &iSize);
};
//---------------------------------------------------------------------------
#endif
