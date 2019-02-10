//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "fpreview.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormPreview *FormPreview;
//---------------------------------------------------------------------------
__fastcall TFormPreview::TFormPreview(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------


void TFormPreview::Clear()
{
	try
	{
		Memo1->Lines->Clear();
	}
	catch(...)
	{
        
    }


}



void TFormPreview::AddFile(AnsiString aFileName,AnsiString aSame)
{
	if(!aSame.IsEmpty())
	{
		AnsiString lS="rem same : "+aSame+" as "+aFileName;
		Memo1->Lines->Add(lS);
	}
	AnsiString lAdd="del \""+aFileName+"\"";
	Memo1->Lines->Add(lAdd);                       	

}
