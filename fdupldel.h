//---------------------------------------------------------------------------

#ifndef fdupldelH
#define fdupldelH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Menus.hpp>
#include <StdCtrls.hpp>
#include <Types.hpp>
#include <XPMan.hpp>
#include <Vcl.Dialogs.hpp>
//---------------------------------------------------------------------------


class TAdvObject: public TObject
{
  public :
    TAdvObject() :TObject(){};
   virtual int Sign(TAdvObject &B)=0;
   virtual int operator ==(TAdvObject &b)
   {
      return (Sign(b)==0);
   };
   virtual int operator <(TAdvObject &b)
   {
    return (Sign(b)<0);
   };
};



class TAdvStringList : public TStringList
{
  public :
  virtual int __fastcall AddAdvObject(const AnsiString S, TAdvObject* AObject);
  virtual void __fastcall AdvSort(bool aInvert=false);
  int SortInternal(bool aInvert,int aDeb,int aFin);
  int CmpLines(bool aInvert,int l1,int l2);
};


class TFormDuplDel : public TForm
{
__published:	// IDE-managed Components
  TEdit *EdtDir;
  TLabel *Label1;
  TLabel *Label2;
  TEdit *EdtFilter;
  TButton *PshFind;
  TStatusBar *StatusBar1;
  TButton *PshDelete;
  TPopupMenu *PopupMenu1;
  TMenuItem *MenUseRecycler;
  TMenuItem *MenKeepOldestFile;
  TMenuItem *MenKeepNewestFile;
  TButton *PshOptions;
  TMenuItem *N1;
  TButton *PshOpen;
	//TXPManifest *XPManifest1;
	TMenuItem *UseDelete1;
	TMenuItem *MenUseBatchPreview;
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TTabSheet *TabSheet2;
	TTabSheet *TabSheet3;
	TStringGrid *GrdFichiers;
	TButton *PshAdd;
	TListBox *LstDirs;
	TMemo *MemoLogs;
	TButton *PshRemove;
	TFileOpenDialog *FileOpenDialog1;
  void __fastcall PshFindClick(TObject *Sender);
  void __fastcall PshDeleteClick(TObject *Sender);
  void __fastcall PshOptionsClick(TObject *Sender);
  void __fastcall MenUseRecyclerClick(TObject *Sender);
  void __fastcall MenKeepOldestFileClick(TObject *Sender);
  void __fastcall MenKeepNewestFileClick(TObject *Sender);
  void __fastcall PshOpenClick(TObject *Sender);
	void __fastcall PshAddClick(TObject *Sender);
	void __fastcall PshRemoveClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TFormDuplDel(TComponent* Owner);
  TAdvStringList *mpListeFich;
  int __fastcall fParseFiles(
  AnsiString &path,
  AnsiString &aExt,
  TStringList *apList
  );

  void __fastcall Message(AnsiString aMessage);
  bool mProcessing;
  int mNbToDelete;

};
//---------------------------------------------------------------------------
extern PACKAGE TFormDuplDel *FormDuplDel;
//---------------------------------------------------------------------------
#endif
