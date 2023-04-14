//----------------------------------------------------------------------------
#ifndef frmEncodeH
#define frmEncodeH
//----------------------------------------------------------------------------
#include <vcl\ExtCtrls.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Graphics.hpp>
#include <vcl\Classes.hpp>
#include <vcl\SysUtils.hpp>
#include <vcl\Windows.hpp>
#include <vcl\System.hpp>
#include <ComCtrls.hpp>
//----------------------------------------------------------------------------
class TFormEncode : public TForm
{
__published:
    TButton *btnOK;
    TButton *btnCancel;
    TRichEdit *tbxMessage;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *lblBytesLeft;
    void __fastcall btnOKClick(TObject *Sender);
    void __fastcall tbxMessageChange(TObject *Sender);
private:

    size_t m_maxMessageSize;
    size_t m_messageSize;

public:
	virtual __fastcall TFormEncode(TComponent* AOwner);
    void __fastcall SetMaxMessageSize(size_t size);

};
//----------------------------------------------------------------------------
extern PACKAGE TFormEncode *FormEncode;
//----------------------------------------------------------------------------
#endif    
