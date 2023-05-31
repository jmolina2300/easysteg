//---------------------------------------------------------------------------

#ifndef frmMainH
#define frmMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Chart.hpp>
#include <DbChart.hpp>
#include <ExtCtrls.hpp>
#include <Series.hpp>
#include <TeEngine.hpp>
#include <TeeProcs.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <Graphics.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------

typedef enum {
    T_IMAGE,
    T_SOUND,
    T_NULL
} FileType;
class TFormMain : public TForm
{
__published:	// IDE-managed Components
    TButton *btnOpen;
    TDBChart *DBChart1;
    TLineSeries *Series1;
    TPanel *Panel1;
    TRichEdit *tbxFileInfo;
    TLabel *Label1;
    TEdit *tbxFileName;
    TOpenDialog *OpenDialog1;
    TLabel *Label2;
    TImage *StegImage;
    TPanel *Panel2;
    TScrollBox *ScrollBox1;
    TMainMenu *MainMenu1;
    TMenuItem *File1;
    TMenuItem *Actions1;
    TMenuItem *btnEncode;
    TMenuItem *btnDecode;
    TPanel *Panel3;
    TCheckBox *CheckBox1;
    TLabel *Label3;
    TMenuItem *Help1;
    TMenuItem *btnAbout;
    void __fastcall btnOpenClick(TObject *Sender);
    void __fastcall btnEncodeClick(TObject *Sender);
    void __fastcall CheckBox1Click(TObject *Sender);
    void __fastcall btnDecodeClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall btnAboutClick(TObject *Sender);
private:	// User declarations
    size_t availableStegSpace;
    FileType __fastcall GetFileType(const AnsiString &s);
    void __fastcall DisplaySoundFile(const AnsiString &s);
    void __fastcall DisplayImageFile(const AnsiString &s);

    void __fastcall FitImage(bool yes);

    void __fastcall Decode();
    void __fastcall Encode();

    void __fastcall EncodeSoundFile(const AnsiString &message, const AnsiString &key);
    void __fastcall EncodeImageFile(const AnsiString &message, const AnsiString &key);

    void __fastcall TFormMain::getVersionNumber(int *versionNumber);
public:		// User declarations
    __fastcall TFormMain(TComponent* Owner);
};



//---------------------------------------------------------------------------
extern PACKAGE TFormMain *FormMain;
//---------------------------------------------------------------------------
#endif
