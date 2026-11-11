//---------------------------------------------------------------------------

#ifndef PacketBufferH
#define PacketBufferH
//---------------------------------------------------------------------------
class TPacketBuffer
{
protected:
private:
	char *Buffer;
	int fBufferSize;
	int iNewDataPos;
    TCriticalSection *cs;
	//property i/o
	int GetBufferSize();
	void SetBufferSize(int iNewSize);
public:
	__property int BufferSize = {read = GetBufferSize, write = SetBufferSize};
	TPacketBuffer();
	~TPacketBuffer();
    int GetFilledLength();
	int WriteByte(char cByte);
    int WriteBytes(char *caBuffer, int iSize);
	bool ReadPacket(char *&DestPacket, int &iSize);
};
//---------------------------------------------------------------------------
#endif
