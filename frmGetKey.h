//---------------------------------------------------------------------------

#ifndef frmGetKeyH
#define frmGetKeyH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TFormGetKey : public TForm
{
__published:	// IDE-managed Components
    TRichEdit *tbxKey;
    TLabel *Label2;
    TButton *btnOK;
    void __fastcall btnOKClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFormGetKey(TComponent* Owner);
    size_t KeyLength;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormGetKey *FormGetKey;
//---------------------------------------------------------------------------
#endif
