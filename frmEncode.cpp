//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "frmEncode.h"
//---------------------------------------------------------------------
#pragma resource "*.dfm"
TFormEncode *FormEncode;
//--------------------------------------------------------------------- 
__fastcall TFormEncode::TFormEncode(TComponent* AOwner)
	: TForm(AOwner)
{
    m_maxMessageSize = 0;
}
//---------------------------------------------------------------------

void __fastcall TFormEncode::btnOKClick(TObject *Sender)
{
    // Make sure the message isn't too big
    size_t messageSize =  tbxMessage->Text.Length();
    if (messageSize > m_maxMessageSize) {
        ShowMessage("Message too large!");
        return;
    }
    if (messageSize == 0) {
        ShowMessage("No message to encode!");
        return;
    }

    size_t keySize = tbxKey->Text.Length();
    size_t STEG_KEY_LENGTH = tbxKey->MaxLength;
    if (keySize < STEG_KEY_LENGTH) {
        ShowMessage("Key is too short (must be 8 characters)");
        return;
    }


    m_messageSize = messageSize;
    this->ModalResult = mrOk;

}
//---------------------------------------------------------------------------

void __fastcall TFormEncode::SetMaxMessageSize(size_t size)
{
    m_maxMessageSize = size;
    tbxMessage->MaxLength = m_maxMessageSize;

    // Set the initial "bytes remaining" text
    size_t bytesLeft = m_maxMessageSize - tbxMessage->Text.Length();
    AnsiString bytesLeftText;
    bytesLeftText.printf("%d", bytesLeft);
    lblBytesLeft->Caption = bytesLeftText;
    lblBytesLeft->Update();
}




void __fastcall TFormEncode::tbxMessageChange(TObject *Sender)
{
    size_t bytesLeft = m_maxMessageSize - tbxMessage->Text.Length();
    AnsiString bytesLeftText;
    bytesLeftText.printf("%d", bytesLeft);

    lblBytesLeft->Caption = bytesLeftText;
    lblBytesLeft->Update();
}
//---------------------------------------------------------------------------


void __fastcall TFormEncode::FormShow(TObject *Sender)
{
    tbxMessage->Clear();
    tbxKey->Clear();
}
//---------------------------------------------------------------------------

