//---------------------------------------------------------------------------

#ifndef fpreviewH
#define fpreviewH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TFormPreview : public TForm
{
__published:	// IDE-managed Components
	TMemo *Memo1;
private:	// User declarations
public:		// User declarations
	__fastcall TFormPreview(TComponent* Owner);
	void Clear();
	void AddFile(AnsiString aFileName,AnsiString aSame);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormPreview *FormPreview;
//---------------------------------------------------------------------------
#endif
