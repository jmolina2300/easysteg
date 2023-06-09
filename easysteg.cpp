//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

USEFORM("frmMain.cpp", FormMain);
USEFORM("frmEncode.cpp", FormEncode);
USEFORM("frmDecode.cpp", FormDecode);
USEFORM("frmGetKey.cpp", FormGetKey);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
         Application->Initialize();
         Application->Title = "EasySteg";
         Application->CreateForm(__classid(TFormMain), &FormMain);
         Application->CreateForm(__classid(TFormEncode), &FormEncode);
         Application->CreateForm(__classid(TFormDecode), &FormDecode);
         Application->CreateForm(__classid(TFormGetKey), &FormGetKey);
         Application->Run();
    }
    catch (Exception &exception)
    {
         Application->ShowException(&exception);
    }
    catch (...)
    {
         try
         {
             throw Exception("");
         }
         catch (Exception &exception)
         {
             Application->ShowException(&exception);
         }
    }
    return 0;
}
//---------------------------------------------------------------------------
