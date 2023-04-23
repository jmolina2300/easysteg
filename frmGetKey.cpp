//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "frmGetKey.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormGetKey *FormGetKey;
//---------------------------------------------------------------------------
__fastcall TFormGetKey::TFormGetKey(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormGetKey::btnOKClick(TObject *Sender)
{
    this->KeyLength = tbxKey->Text.Length();
}
//---------------------------------------------------------------------------
void __fastcall TFormGetKey::FormShow(TObject *Sender)
{
    this->KeyLength = 0;
    tbxKey->Clear();
}
//---------------------------------------------------------------------------
