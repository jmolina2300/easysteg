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
class TForm1 : public TForm
{
__published:	// IDE-managed Components
    TButton *Button1;
    TDBChart *DBChart1;
    TLineSeries *Series1;
    TPanel *Panel1;
    TSplitter *Splitter1;
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
    TMenuItem *Embed1;
    TMenuItem *Extract1;
    TPanel *Panel3;
    TCheckBox *CheckBox1;
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
    FileType fileType;
    FileType __fastcall GetFileType(const AnsiString &s);
    void __fastcall DisplaySoundFile();
    void __fastcall DisplayImageFile();
public:		// User declarations
    __fastcall TForm1(TComponent* Owner);
};



//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
