//---------------------------------------------------------------------------

#ifndef UnitAnimateThreadH
#define UnitAnimateThreadH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
//---------------------------------------------------------------------------
class TAnimateThread : public TThread
{
private:
protected:
	void __fastcall Execute();
public:
	__fastcall TAnimateThread(bool CreateSuspended);
};
//---------------------------------------------------------------------------
#endif
