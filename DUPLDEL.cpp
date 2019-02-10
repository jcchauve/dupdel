//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("dupldel.res");
USEFORM("fdupldel.cpp", FormDuplDel);
//---------------------------------------------------------------------------
WINAPI int WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
  try
  {
     Application->Initialize();
     Application->Title = "Duplicate File Deleter";
     Application->CreateForm(__classid(TFormDuplDel), &FormDuplDel);
     Application->Run();
  }
  catch (Exception &exception)
  {
     Application->ShowException(&exception);
  }
  return 0;
}
//---------------------------------------------------------------------------
