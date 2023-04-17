//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "frmMain.h"
#include "frmEncode.h"
#include "wavread.h"
#include "bmpread.h"
#include "steg.h"


#define PLOT_SCALE 2000

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormMain *FormMain;
//---------------------------------------------------------------------------
__fastcall TFormMain::TFormMain(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------



/**
 * getIllegalCharacter
 * 
 *  Get the first illegal character in a string.
 *  This ensures we dont try to open a file with a UTF8 character that is
 *  incorrectly encoded as a single (and probably illegal) char.
 *
 */
char getIllegalCharacter(const AnsiString &s) {
    if (s.Pos("?") > 0) {
        return '?';
    } else {
        return 0;
    }
}

/**
 * isLegalFileName
 *
 *  Check if the file name is legal. There are only a few cases to check for.
 */
bool isLegalFileName(const AnsiString &s)
{
    // Report any illegal chars and exit if any are found
    char illegalChar = getIllegalCharacter(s);
    if (illegalChar != 0) {
        return false;
    }

    // Could also check if its a device file name, etc...


    return true;
}

bool isBMPFile(const AnsiString &s)
{
  int index = s.Pos(".bmp");
  return (index > 0);
}

bool isSoundFile(const AnsiString &s)
{
  int index = s.Pos(".wav");
  return (index > 0);
}

AnsiString RemoveExtension(AnsiString filename) {
    int lastDotIndex = filename.LastDelimiter(".");
    return filename.SubString(1, lastDotIndex - 1);
}


FileType __fastcall TFormMain::GetFileType(const AnsiString &fileName)
{
    if (isSoundFile(fileName)) {
        return T_SOUND;
    } else if (isBMPFile(fileName)) {
        return T_IMAGE;
    } else {
        return T_NULL;
    }
}


void __fastcall TFormMain::DisplaySoundFile(const AnsiString &fileName)
{
    ScrollBox1->Hide();
    DBChart1->Show();

    // Clear the chart and info box
    DBChart1->Series[0]->Clear();
    tbxFileInfo->Clear();

    // Get information about the WAV file
    WaveFile wi;
    bool acceptableFile = wav_read_from_file(&wi, fileName.c_str() );
    if (acceptableFile) {
        this->availableStegSpace = wi.info.numSamples / 8;
        this->availableStegSpace -= this->availableStegSpace % KEY_LENGTH;
		AnsiString fileInfo;
		fileInfo.printf(" Name: %s\n"
						" Sample Rate: %d\n"
						" Channels: %d\n"
						" Duration: %.2lf seconds\n"
						" Samples: %d\n"
                        "\n"
                        " Available Space: %d bytes\n",
						ExtractFileName(OpenDialog1->FileName),
						wi.info.sampleRate,
						wi.info.numChannels,
						wi.info.duration,
						wi.info.numSamples,
                        this->availableStegSpace);
        tbxFileInfo->Text = fileInfo;
    } else {
        Application->MessageBox("Unsupported WAV format!",
                                "Error", MB_OK | MB_ICONASTERISK);
        return;
    }


    int16_t *wavSamples = (int16_t*)wi.data;

    TChartSeries* tmpSeries = (TChartSeries*)DBChart1->Series[0];

    // Represent a waveform of arbitrary length using a 2000-element array
    int16_t  valuesToPlot[PLOT_SCALE];
    size_t  valuesToPlotIndex = 0;
    size_t  sampleIndex = 0;
    size_t  delta = wi.info.numSamples / PLOT_SCALE;
    size_t  deltaCounter = 0;

    // Run through the wav data and pick every few samples to plot
    while (valuesToPlotIndex < PLOT_SCALE  && sampleIndex < wi.info.numSamples) {
        if (deltaCounter == 0) {
            valuesToPlot[valuesToPlotIndex] = wavSamples[sampleIndex];
            valuesToPlotIndex++;
        }
        deltaCounter++;
        if (deltaCounter == delta) {
            deltaCounter = 0;
        }
        sampleIndex++;
    }

    // Finally, add the values to the chart 
    for (size_t i = 0; i < PLOT_SCALE; i++) {
        tmpSeries->AddXY(i, valuesToPlot[i]);
    }
    DBChart1->RefreshData();

    // Free the memory!
    wav_destroy(&wi);
}

void __fastcall TFormMain::DisplayImageFile(const AnsiString &fileName)
{
    ScrollBox1->Show();
    DBChart1->Hide();
    if (!isBMPFile(fileName)) {
        Application->MessageBox("Please select a BMP file",
                                "Error", MB_OK | MB_ICONASTERISK);
        return;
    }
    try {
        StegImage->Picture->LoadFromFile(fileName);
        tbxFileName->Text = fileName;
    }
    catch (Exception &e) { }

    BmpImage bi;
    bmp_read_from_file(&bi, fileName.c_str());
    this->availableStegSpace = bi.info.datasize / 8;
    this->availableStegSpace -= this->availableStegSpace % KEY_LENGTH;

    // Display information about the BMP file in the text box
    AnsiString fileInfo;
	fileInfo.printf(" Name: %s\n"
					" Bits: %d\n"
					" FileSize: %d\n"
					" DataSize: %d\n"
					" Pixels: %d\n"
                    " \n"
                    " Available Space: %d bytes\n",

					ExtractFileName(fileName),
					bi.info.bits,
					bi.header.filesize,
					bi.info.datasize,
					bi.info.width * bi.info.height * 3,
                    this->availableStegSpace);
    tbxFileInfo->Text = fileInfo;

    bmp_destroy(&bi);
}

void __fastcall TFormMain::btnOpenClick(TObject *Sender)
{
    // Open dialog to get the desired file
    bool success = OpenDialog1->Execute();
    if (!success) {
        return;
    }
    if (!isLegalFileName(OpenDialog1->FileName)) {
        OpenDialog1->CleanupInstance();
        Application->MessageBox("Filepath contains illegal characters!",
                                "Error", MB_OK | MB_ICONASTERISK);
        return;
    }
    AnsiString chosenFileName = OpenDialog1->FileName;
    FileType fileType = GetFileType(chosenFileName);
    this->availableStegSpace = 0; // Reset available space (we dont know it yet)
    if (fileType == T_SOUND) {
        DisplaySoundFile(chosenFileName);
    } else if (fileType == T_IMAGE) {
        DisplayImageFile(chosenFileName);
    } else {
        Application->MessageBox("Please select a BMP or WAV file",
                                "Error", MB_OK | MB_ICONASTERISK);
        return;
    }

    // Put the FileName in the text box  if all went well
    tbxFileName->Text = chosenFileName;

}
//---------------------------------------------------------------------------


void __fastcall TFormMain::FormCreate(TObject *Sender)
{
    this->fileType = T_NULL;
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::EncodeImageFile(const AnsiString &s)
{
    AnsiString inputFile;
    AnsiString outputFile;
    inputFile.printf("%s", tbxFileName->Text.c_str() );
    outputFile = RemoveExtension(inputFile) + "-steg.bmp";

    // Add padding to the message
    size_t messagePadding = KEY_LENGTH - (FormEncode->tbxMessage->Text.Length() % KEY_LENGTH);
    if (messagePadding != KEY_LENGTH) {
        FormEncode->tbxMessage->Text.SetLength(FormEncode->tbxMessage->Text.Length() + messagePadding);
        for (size_t i = 0; i < messagePadding; i++) {
            FormEncode->tbxMessage->Text =  FormEncode->tbxMessage->Text + " ";
        }
    }
    AnsiString msg;
    msg.printf("Bytes required for padding: %d", messagePadding,FormEncode->tbxMessage->Text[FormEncode->tbxMessage->Text.Length() -1] );
    ShowMessage(msg);

    int success = steg_encode_bmp(
        inputFile.c_str(),                    // input file
        outputFile.c_str(),                   // output file
        "asd",                                // key
        FormEncode->tbxMessage->Text.c_str(), // the message
        FormEncode->tbxMessage->Text.Length() // the length
        );
    if (success) {
        Application->MessageBox("Success!",
                                "Success", MB_OK );
    } else {
        Application->MessageBox("Failed to encode message",
                                "Error", MB_OK | MB_ICONASTERISK);
    }

}

void __fastcall TFormMain::EncodeSoundFile(const AnsiString &s)
{
    AnsiString inputFile;
    AnsiString outputFile;
    inputFile.printf("%s", tbxFileName->Text.c_str() );
    outputFile = RemoveExtension(inputFile) + "-steg.wav";

    int success = steg_encode_wav(
        inputFile.c_str(),                    // input file
        outputFile.c_str(),                   // output file
        "asd",                                // key
        FormEncode->tbxMessage->Text.c_str(), // the message
        FormEncode->tbxMessage->Text.Length() // the length
        );
    if (success) {
        Application->MessageBox("Success!",
                                "Success", MB_OK );
    } else {
        AnsiString msg;
        msg.printf("length of message: %d\n", FormEncode->tbxMessage->Text.Length());

        ShowMessage("Failed to encode: " + msg);
        //Application->MessageBox("Failed to encode message" + msg,
                                //"Error", MB_OK | MB_ICONASTERISK);
    }
}

void __fastcall TFormMain::btnEncodeClick(TObject *Sender)
{
    // Get the secret message to encode from the user
    FormEncode->SetMaxMessageSize(availableStegSpace);
    FormEncode->ShowModal();
    if (FormEncode->ModalResult != mrOk) {
        return;
    }
    if (FormEncode->tbxMessage->Text.Length() <= 0) {
        return;
    }

    AnsiString chosenFileName = tbxFileName->Text;
    FileType fileType = GetFileType(chosenFileName);

    if (fileType == T_SOUND) {
        EncodeSoundFile(FormEncode->tbxMessage->Text);
    } else if (fileType == T_IMAGE) {
        EncodeImageFile(FormEncode->tbxMessage->Text);
    } else {
        ShowMessage("What kind of abomination is this?");
    }
    
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::CheckBox1Click(TObject *Sender)
{
    if (CheckBox1->Checked) {
        StegImage->Proportional = true;
        StegImage->Align = alClient;
    } else {

        StegImage->Proportional = false;
        StegImage->Align = alNone;
    }
    StegImage->Repaint();    
}
//---------------------------------------------------------------------------



void __fastcall TFormMain::btnDecodeClick(TObject *Sender)
{
    AnsiString chosenFileName = tbxFileName->Text;
    FileType fileType = GetFileType(chosenFileName);

    char *decodeBuffer = (char*)calloc(availableStegSpace + 1, sizeof(byte));
    if (decodeBuffer == NULL) {
        Application->MessageBox("Failed to decode message.\n\n"
                                "Could not allocate memory for message buffer",
                                "Error", MB_OK | MB_ICONASTERISK);
        return;
    }
    AnsiString msg;
    msg.printf("Decode %d bytes from %s\n",
        availableStegSpace,
        chosenFileName.c_str() );
    ShowMessage(msg);


    int decodeSuccess;
    if (fileType == T_SOUND)
    {
        decodeSuccess = steg_decode_wav(chosenFileName.c_str(), NULL, decodeBuffer, availableStegSpace);
    }
    else if (fileType == T_IMAGE)
    {
        decodeSuccess = steg_decode_bmp(chosenFileName.c_str(), NULL, decodeBuffer, availableStegSpace);
    }

    if (decodeSuccess) {
        ShowMessage(decodeBuffer);
    } else {
        Application->MessageBox("Failed to decode message.\n\n",
                                "Error", MB_OK | MB_ICONASTERISK);
    }

    free(decodeBuffer);


}
//---------------------------------------------------------------------------

