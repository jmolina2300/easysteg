//---------------------------------------------------------------------------

#ifndef frmDecodeH
#define frmDecodeH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFormDecode : public TForm
{
__published:	// IDE-managed Components
    TRichEdit *tbxMessage;
private:	// User declarations
public:		// User declarations
    __fastcall TFormDecode(TComponent* Owner);
    void __fastcall SetInitialText(AnsiString &initialText);

};
//---------------------------------------------------------------------------
extern PACKAGE TFormDecode *FormDecode;
//---------------------------------------------------------------------------
#endif
