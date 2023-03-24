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
    TRichEdit *txtMessage;
    TLabel *Label1;
    TButton *btnOpen;
private:
public:
	virtual __fastcall TFormEncode(TComponent* AOwner);
};
//----------------------------------------------------------------------------
extern PACKAGE TFormEncode *FormEncode;
//----------------------------------------------------------------------------
#endif    
