//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "frmDecode.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormDecode *FormDecode;
//---------------------------------------------------------------------------
__fastcall TFormDecode::TFormDecode(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormDecode::SetInitialText(AnsiString &initialText)
{
    tbxMessage->Text = initialText;
}
