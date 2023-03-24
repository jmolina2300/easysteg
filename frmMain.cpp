//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "frmMain.h"
#include "frmEncode.h"
#include "wavread.h"


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


void __fastcall TFormMain::DisplaySoundFile()
{
    ScrollBox1->Hide();
    DBChart1->Show();
    // Clear the chart and info box
    DBChart1->Series[0]->Clear();
    tbxFileInfo->Clear();

    // Get information about the WAV file
    WaveInfo wi;
    FILE *infile = fopen(tbxFileName->Text.c_str(), "rb");
    if (infile == NULL) {
        Application->MessageBox("Unable to open file",
                                "Error", MB_OK | MB_ICONASTERISK);
        return;
    }
    bool acceptableFile = read_pcm_wav_info(infile, tbxFileName->Text.c_str(), &wi);
    if (acceptableFile) {
		AnsiString fileInfo;
		fileInfo.printf("        Name: %s\n"
						"        Bits: %d\n"
						" Sample Rate: %d\n"
						"    Channels: %d\n"
						"    Duration: %.2lf\n"
						"     Samples: %d\n",
						ExtractFileName(OpenDialog1->FileName),
						wi.BitsPerSample,
						wi.SampleRate,
						wi.NumChannels,
						wi.Duration,
						wi.NumSamples);
        tbxFileInfo->Text = fileInfo;
    } else {
        Application->MessageBox("Unsupported WAV format!",
                                "Error", MB_OK | MB_ICONASTERISK);
        fclose(infile);
        return;
    }
	
	/* At this point, the file pointer has been moved up to the data portion.
	 * So we can proceed to read the samples.
     */

    DBChart1->Series[0]->Clear();


    // Create buffer for the audio data
    size_t audio_window_ms = AUDIO_WINDOW_MS;
    size_t buffer_samples = wi.SampleRate * (audio_window_ms / 1000.0);
    int16_t *inbuffer = (int16_t*)malloc(buffer_samples * wi.BlockAlign);

    size_t  numSamplesTotal = 0;
    size_t  numSamplesRead = 0;
    TChartSeries* tmpSeries = (TChartSeries*)DBChart1->Series[0];

    // Represent a waveform of arbitrary length using a 2000-element array
    int16_t  valuesToPlot[PLOT_SCALE];
    size_t  valuesToPlotIndex = 0;
    size_t  delta = wi.NumSamples / PLOT_SCALE;
    size_t  deltaCounter = 0;
    do {
        
        numSamplesRead = fread(inbuffer, wi.BlockAlign, buffer_samples, infile);

        /* Add the values to the array using the delta value to tell when
         *   to actually add each one.
         */
        for (size_t i = 0; i < numSamplesRead; i++) {
            int16_t sample_amplitude = inbuffer[i];
            if (deltaCounter == delta) {
                valuesToPlot[valuesToPlotIndex] = sample_amplitude;
                valuesToPlotIndex++;
                deltaCounter = 0;
            }
            deltaCounter++;
        }
        numSamplesTotal += numSamplesRead;
    } while (numSamplesRead != 0) ;


    // Finally, add the values to the chart 
    for (size_t i = 0; i < PLOT_SCALE; i++)
        tmpSeries->AddXY(i, valuesToPlot[i]);

    DBChart1->RefreshData();
    
    
    free (inbuffer);
    fclose (infile);
}

void __fastcall TFormMain::DisplayImageFile()
{
    ScrollBox1->Show();
    DBChart1->Hide();
    if (!isBMPFile(OpenDialog1->FileName)) {
        Application->MessageBox("Please select a BMP file",
                                "Error", MB_OK | MB_ICONASTERISK);
        return;
    }
    try {
        StegImage->Picture->LoadFromFile(OpenDialog1->FileName);
        tbxFileName->Text = OpenDialog1->FileName;
    }
    catch (Exception &e) { }
}

void __fastcall TFormMain::Button1Click(TObject *Sender)
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

    // Put the FileName in the text box
    tbxFileName->Text = OpenDialog1->FileName;

    FileType ft = GetFileType(tbxFileName->Text);
    if (ft == T_SOUND) {
        DisplaySoundFile();
    } else if (ft == T_IMAGE) {
        DisplayImageFile();
    } else {
        Application->MessageBox("Please select a BMP or WAV file",
                                "Error", MB_OK | MB_ICONASTERISK);
    }

}
//---------------------------------------------------------------------------


void __fastcall TFormMain::FormCreate(TObject *Sender)
{
    this->fileType = T_NULL;
}
//---------------------------------------------------------------------------







void __fastcall TFormMain::Embed1Click(TObject *Sender)
{
    // Get the secret message to encode from the user
    FormEncode->ShowModal();
    if (FormEncode->ModalResult != mrOk) {
        return;
    }
    if (FormEncode->txtMessage->Text.Length() <= 0) {
        return;
    }

    // Do something with secret message

    
}
//---------------------------------------------------------------------------

