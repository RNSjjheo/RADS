//---------------------------------------------------------------------------

//#include <vcl.h>
//#pragma hdrstop
#include "Precompile.h"

#include "UnitAlert.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TFormAlert *FormAlert;

//---------------------------------------------------------------------------
__fastcall TFormAlert::TFormAlert(TComponent* Owner)
	: TForm(Owner)
{
}

//---------------------------------------------------------------------------
void TFormAlert::ShowMessage(UnicodeString sMsg)
{
	Panel->Caption = sMsg;
}

