//---------------------------------------------------------------------------

#include <vcl.h>
#include <dir.h>
#include <stdio.h>
#include <FileCtrl.hpp>
#include <Dialogs.hpp>
#pragma hdrstop
#include "fdupldel.h"
#include "fpreview.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormDuplDel *FormDuplDel;
//---------------------------------------------------------------------------
__fastcall TFormDuplDel::TFormDuplDel(TComponent* Owner)
  : TForm(Owner)
{
  mpListeFich = new TAdvStringList();
  mpListeFich->Sorted = true;
  mProcessing = false;
        GrdFichiers->ColCount=6;
      GrdFichiers->Cells[0][0]="Size";
      GrdFichiers->Cells[1][0]="Name";
      GrdFichiers->Cells[2][0]="Date";
      GrdFichiers->Cells[3][0]="Del ?";
      GrdFichiers->Cells[4][0]="Path";
      GrdFichiers->Cells[5][0]="CheckSum";

      GrdFichiers->ColWidths[1]=GrdFichiers->DefaultColWidth*2;
      GrdFichiers->ColWidths[4]=GrdFichiers->DefaultColWidth*4;
	GrdFichiers->RowCount =  2;
	PageControl1->ActivePageIndex=0;
}
//---------------------------------------------------------------------------


int __fastcall TAdvStringList::AddAdvObject(const AnsiString S, TAdvObject* AObject)
{
  TStringList::AddObject(S,(TObject *) AObject);
}

void __fastcall TAdvStringList::AdvSort(bool aInvert)
{
  SortInternal(aInvert,-1,-1);
}
int TAdvStringList::CmpLines(bool aInvert,int l1,int l2)
{
  TAdvObject *lObj1 = (TAdvObject *) Objects[l1];
  TAdvObject *lObj2 = (TAdvObject *) Objects[l2];
  return (aInvert) ? lObj1->Sign(*lObj2) : lObj2->Sign(*lObj1);
}
int TAdvStringList::SortInternal(bool aInvert,int aDeb,int aFin)
{
  int lPos;
  int lCmp;
  if (aDeb<0)
  {
    aDeb=0;
    aFin=Count-1;
    if (Count==0)
      return 1;
  }
  if (aDeb==aFin)
    return(1);
  int l1,l2,lMid;


  l1=aDeb;
  l2=aFin;
  lMid = (aDeb+aFin)/2;
  //MyPrintf("Sort %d %d %d\n",l1,l2,lMid);
  do
  {
    while(lCmp=CmpLines(aInvert,l1,lMid)<0)
    {
      l1++;
    }

    while(lCmp=CmpLines(aInvert,l2,lMid)>0)
    {
      l2--;
    }


    if (l1<=l2)
    {
      // Le Mid a Changé de place
      if (lMid==l1)
      {
        lMid=l2;
      }
      else
      {
        if (lMid==l2)
        {
          lMid = l1;
        }
      }
      if (l1!=l2)
      {
        Exchange(l1,l2);
      }

      //MyPrintf("Swap %d %d\n",l1,l2);
      l1++;
      l2--;
    }

    //for (li=aDeb;li<=aFin;li++)
    //{
    //  MyPrintf("%d=%s\n",li,AnsiString(Select(li,aCols[0])).c_str());
    //}
  }
  while(l1<=l2);
  if (l2 > aDeb)
    SortInternal(aInvert,aDeb,l2);
  if (l1 < aFin)
    SortInternal(aInvert,l1,aFin);


  return(1);



}



class TObjFile : public TAdvObject
{
  public :
  struct ffblk mFileBlk;
  AnsiString mFilePath;
  AnsiString mFileName;
  bool mToDelete;
  bool mYaSame;
  TObjFile(AnsiString aFilePath,struct ffblk aFileBlk,bool aDateDesc=false);
   virtual int  Sign(TAdvObject &b);
   int SameDatas(TObjFile &b);
   bool mDateDesc;
   TObjFile *mTheSame;
   int mCheckSum;
   void CalcCheckSum();
};


TObjFile::TObjFile(AnsiString aFilePath,struct ffblk aFileBlk,bool aDateDesc)
{
  mFilePath = aFilePath;
  mFileName=ExtractFileName(aFilePath);
  mFileBlk=aFileBlk;
  mToDelete = false;
  mDateDesc = aDateDesc;
  mYaSame = false;
  mTheSame = NULL;
  mCheckSum = -1;
}


int TObjFile::Sign(TAdvObject &b)
{
  TObjFile &lB=(TObjFile &)b;
	  if(mFileBlk.ff_fsize==lB.mFileBlk.ff_fsize)
	  {
		if((mFileName==lB.mFileName)&&(mFileBlk.ff_fdate==lB.mFileBlk.ff_fdate))
		{
			return 0;                                                                                    	
		}
		CalcCheckSum();
		lB.CalcCheckSum();
		if (lB.mCheckSum==mCheckSum)
		{
		  if(mFileBlk.ff_fdate==lB.mFileBlk.ff_fdate)
            return 0;
          else if(mFileBlk.ff_fdate<lB.mFileBlk.ff_fdate)
            return (mDateDesc) ? -1 : 1;
          else
            return (mDateDesc) ? 1 : -1;
        }
        else if (mCheckSum<lB.mCheckSum)
        {
          return 1;
        }
        else
        {
          return -1;
        }

      }
      else if(mFileBlk.ff_fsize<lB.mFileBlk.ff_fsize)
        return 1;
      else
        return -1;
};


void TObjFile::CalcCheckSum()
{
  if (mCheckSum>=0)
    return;
  mCheckSum=0;
  FILE *lF1= fopen(mFilePath.c_str(),"rb");
  unsigned char lBlk1[4096];
  if (lF1==NULL)
    return;
  int lNb=fread(lBlk1,1,4096,lF1);
  fclose(lF1);
  int li;
  for(li=0;li<lNb;li++)
  {
    mCheckSum+=lBlk1[li];
  }
}


#define CMPBLOCKSIZE 32768

int TObjFile::SameDatas(TObjFile &b)
{
  FILE *lF1= fopen(mFilePath.c_str(),"rb");
  FILE *lF2= fopen(b.mFilePath.c_str(),"rb");
  int lNb1,lNb2;

  if ((lF1==NULL) || (lF2==NULL))
  {
    if (lF1!=NULL)
      fclose(lF1);
    if (lF2!=NULL)
      fclose(lF2);
    return false;
  }
  char lBlk1[CMPBLOCKSIZE];
  char lBlk2[CMPBLOCKSIZE];
  int lYaDiff = 0;
  do
  {
	lNb1=fread(lBlk1,1,CMPBLOCKSIZE,lF1);
	lNb2=fread(lBlk2,1,CMPBLOCKSIZE,lF2);
	if (memcmp(lBlk1,lBlk2,CMPBLOCKSIZE))
      lYaDiff = 1;
  }
  while ((!lYaDiff)&& (lNb1==CMPBLOCKSIZE) && (lNb2==lNb1));
  fclose(lF1);
  fclose(lF2);
  return !lYaDiff;
}


int __fastcall TFormDuplDel::fParseFiles(
  AnsiString &path,
  AnsiString &aExt,
  TStringList *apList
  )
{
  Message("Search :"+path);
  if (!mProcessing)
    return 0;
  struct ffblk FileBlk;
  AnsiString lgpath;
  lgpath=path+"\\*.*";
  if(findfirst(lgpath.c_str(),&FileBlk,FA_DIREC|FA_ARCH)!=-1)
  {
    //attention sioux : ici recursivite
    if(FileBlk.ff_attrib & FA_DIREC)
    {
      AnsiString lpath=FileBlk.ff_name;
      if(lpath!="." && lpath!="..")
      	fParseFiles(lpath,aExt,apList);
    }
    else
    {
      AnsiString lpath=FileBlk.ff_name;
      AnsiString lExt=(lpath.SubString(lpath.AnsiPos(".")+1,lpath.Length()-1)).LowerCase();
      if ((aExt=="*") ||(lExt==aExt))
      {
        AnsiString lNewPath =path+"\\"+lpath;
        TObjFile * lpObj = new TObjFile(lNewPath,FileBlk,MenKeepNewestFile->Checked);
      	apList->AddObject(lNewPath,lpObj);
      }
    }
    TStringList *lpListe = new TStringList();
    lpListe->Sorted=true;
 		while(findnext(&FileBlk)!=-1)
    {
      AnsiString lpath=FileBlk.ff_name;
      if (lpListe->IndexOf(lpath)>=0)
      {
        continue;
      }
      else
      {

        lpListe->Add(lpath);
      }
	    if(FileBlk.ff_attrib & FA_DIREC)
	    {
		  if(lpath!="." && lpath!="..")
		  {
			  AnsiString pp = path+L"\\"+lpath;
			  fParseFiles(pp,aExt,apList);
		  }
	    }
	    else
	    {

        AnsiString lExt=(lpath.SubString(lpath.AnsiPos(".")+1,lpath.Length()-1)).LowerCase();
        //if(lExt=="cpp" && fileDebug!=AnsiString(FileBlk.ff_name))
        if ((aExt=="*") ||(lExt==aExt))
        {
          AnsiString lNewPath =path+"\\"+lpath;
          TObjFile * lpObj = new TObjFile(lNewPath,FileBlk,MenKeepNewestFile->Checked);
          apList->AddObject(lNewPath,lpObj);
        }

	    }
    }
    delete lpListe;
    findclose(&FileBlk);
  }
  return 0;
}

void __fastcall TFormDuplDel::Message(AnsiString aMessage)
{
  StatusBar1->SimpleText=aMessage;
  Application->ProcessMessages();
  MemoLogs->Lines->Add(aMessage);
}


void __fastcall TFormDuplDel::PshFindClick(TObject *Sender)
{
  mNbToDelete = 0;
  if (mProcessing)
  {
	mProcessing=false;
	PshFind->Caption="Find";
	return;
  }
  mpListeFich->Clear();
  MemoLogs->Lines->Clear();
  mProcessing = true;
  PshFind->Caption="Stop";
  //bool lExec = DirectoryExists(EdtDir->Text);
  bool lExec = true;

   FILE *lpLog=fopen("dupdel.log","wt");
	while(lExec)
	{
	  for(int ld=0;ld<LstDirs->Items->Count;ld++)
	  {
		  AnsiString lDir = LstDirs->Items->Strings[ld];
		  Message("Scanning :"+lDir);
		  if (lDir.SubString(lDir.Length(),1)=='\\')
		  {
			lDir = lDir.SubString(1,lDir.Length()-1);
		  }
		  AnsiString ext = EdtFilter->Text.SubString(3,10);
		  fParseFiles(lDir,ext,mpListeFich);
		  if (!mProcessing)
			break;
	  }
	  Message("Sort :"+AnsiString(mpListeFich->Count));
	  mpListeFich->AdvSort();
	  Message("Sort Ended:"+AnsiString(mpListeFich->Count));

	  if (!mProcessing)
		break;
	  int li;
	  int lSamePrec;
	  int lSameSuiv;
	  TObjFile *lpObj;
	  TObjFile *lpObjPrec;
	  TObjFile *lpObjSuiv;

	  GrdFichiers->Visible=false;
	  for (li=0;li<mpListeFich->Count;li++)
	  {
		lpObj = (TObjFile*) mpListeFich->Objects[li];
		lpObjPrec = (li>0) ? (TObjFile*) mpListeFich->Objects[li-1] : NULL;
		lpObjSuiv = (li<mpListeFich->Count-1) ? (TObjFile*) mpListeFich->Objects[li+1] : NULL;
		lSamePrec = (lpObjPrec==NULL) || ((lpObjPrec->mFileBlk.ff_fsize==lpObj->mFileBlk.ff_fsize) &&(lpObjPrec->mCheckSum==lpObj->mCheckSum) );
		lSameSuiv = (lpObjSuiv==NULL) || ((lpObjSuiv->mFileBlk.ff_fsize==lpObj->mFileBlk.ff_fsize)&&(lpObjSuiv->mCheckSum==lpObj->mCheckSum)) ;
		if ((!lSamePrec) && (!lSameSuiv))
		{
		  delete lpObj;
		  mpListeFich->Delete(li);
		  li--;
		}
	  }
	  if (!mProcessing)
		break;

	  lpObjPrec =  NULL;
	  AnsiString lCount=mpListeFich->Count;
	  for (li=0;li<mpListeFich->Count;li++)
	  {
		lpObj = (TObjFile*) mpListeFich->Objects[li];
		if ((lpObjPrec!=NULL)
		  &&(lpObjPrec->mFileBlk.ff_fsize==lpObj->mFileBlk.ff_fsize))
		{
		   Message("Compare :"+AnsiString(li)+"/"+AnsiString(lCount)+" "+lpObjPrec->mFileName+"<>"+lpObj->mFileName);
		   if (!mProcessing)
			  break;

		   if (lpObjPrec->SameDatas(*lpObj))
		   {
			  lpObj->mToDelete = true;
			  lpObj->mYaSame = true;
			  lpObj->mTheSame=lpObjPrec;
			  if(lpLog)
			  {
				  fprintf(lpLog,"Delete %s , same=%s\n",lpObj->mFilePath.c_str(),lpObjPrec->mFilePath.c_str());
			  }
		   }

		}
		lpObjPrec=lpObj;
	  }
	  // Virer les Size quand aucun fichier n'est a delete
	  int lOldSize=-1;
	  bool lYaDelete=false;
	  int lOldPos=-1;
	  int lj;
	  for (li=0;li<mpListeFich->Count;li++)
	  {
		lpObj = (TObjFile*) mpListeFich->Objects[li];
		lOldSize=lpObj->mFileBlk.ff_fsize;
		lYaDelete=false;
		lj=li+1;
		for(lj=li+1;
		  (lj<mpListeFich->Count) &&  (lpObjSuiv =(TObjFile*) mpListeFich->Objects[lj])->mFileBlk.ff_fsize==lOldSize;
		  lj++)
		{
		  if (lpObjSuiv->mToDelete)
			lYaDelete = true;
		}
		if (!lYaDelete)
		{
		  lj--;
		  while(lj>=li)
		  {
			lpObj = (TObjFile*) mpListeFich->Objects[lj];
			delete lpObj;
			mpListeFich->Delete(lj);
			lj--;
		  }
		  li--;
		}
		else
		{
		  li=lj-1;
		}
	  }
	  Message("Done");
	  GrdFichiers->RowCount = (mpListeFich->Count>0) ? mpListeFich->Count+1 : 2;
	  int lNbDel=0;
	  int lTotalDel=0;
	  for (li=0;li<mpListeFich->Count;li++)
	  {
		lpObj = (TObjFile*) mpListeFich->Objects[li];
		GrdFichiers->Cells[0][li+1]=AnsiString(lpObj->mFileBlk.ff_fsize);
		GrdFichiers->Cells[1][li+1]=AnsiString(lpObj->mFileBlk.ff_name);
		char lStrDate[30];
		unsigned int lDate=lpObj->mFileBlk.ff_fdate;
		int lDay = lDate & 0x1F;
		lDate = lDate>>5;
		int lMonth = lDate & 0x0F;
        lDate = lDate>>4;
        int lYear = 1980+lDate;
        sprintf(lStrDate,"%.2d/%.2d/%.4d",lDay,lMonth,lYear);
//Bits 0-4	Day
//Bits 5-8	Month
//Bits 9-15	Years since 1980 (for example 9 here means 1989)
        GrdFichiers->Cells[2][li+1]=AnsiString(lStrDate);
        GrdFichiers->Cells[3][li+1]=(lpObj->mToDelete) ? "Yes":"No";
		GrdFichiers->Cells[4][li+1]=lpObj->mFilePath;
        GrdFichiers->Cells[5][li+1]=AnsiString(lpObj->mCheckSum);
        if (lpObj->mToDelete)
        {
          lNbDel++;
          lTotalDel+=lpObj->mFileBlk.ff_fsize;
        }

      }

	  lExec=false;
	  Message(AnsiString(lNbDel)+" to Delete /"+AnsiString(lTotalDel/1024)+" Kos");
      mNbToDelete =lNbDel;
	}
  GrdFichiers->Visible=true;
  PshFind->Caption="Find";
  fclose(lpLog);
  if (!mProcessing)
  {
	Message("Canceled");
  }
  mProcessing=false;
}

BOOL VCDJDeleteFile(LPCTSTR pFileName, BOOL
							useRecycler)
{
	BOOL retCode;
	if (useRecycler)
	{
		SHFILEOPSTRUCT fOS;
		ZeroMemory(&fOS, sizeof(SHFILEOPSTRUCT));

		fOS.wFunc = FO_DELETE;
		fOS.pFrom = pFileName;
		// Avoid any kind of feedback to the
			 //		user during execution
		fOS.fFlags = FOF_SILENT |
					FOF_NOERRORUI | \
		   FOF_NOCONFIRMATION | FOF_ALLOWUNDO;

		retCode = (SHFileOperation(&fOS) == 0);
	}
	else
	{
		try
		{
			retCode = DeleteFile(pFileName);
		}
		catch(...)
		{
            return false;
        }
	}

	return retCode;
}




//---------------------------------------------------------------------------
void __fastcall TFormDuplDel::PshDeleteClick(TObject *Sender)
{
  TObjFile *lpObj;
  AnsiString lPath;
  int li;
  bool lUseRecycler = MenUseRecycler->Checked;
  bool lBatchPreview= MenUseBatchPreview->Checked;
  int lNbDeleted=0;
  if(lBatchPreview)
  {
	if(FormPreview==NULL)
	{
	  Application->CreateForm(__classid(TFormPreview), &FormPreview);
	}
	FormPreview->Clear();
	FormPreview->Show();



  }


  for (li=0;li<mpListeFich->Count;li++)
  {
    lpObj = (TObjFile*) mpListeFich->Objects[li];
    if (lpObj->mToDelete)
    {
	  lPath=lpObj->mFilePath;
	  AnsiString lSame;
	  if(lpObj->mTheSame)
	  {
	  	lSame=lpObj->mTheSame->mFilePath;
      }
	  if(lBatchPreview)
	  {
		FormPreview->AddFile(lPath,lSame);
	  }
	  else
	  {
		if(VCDJDeleteFile(lPath.c_str(),lUseRecycler))
			lNbDeleted++;
	  }
      Message(AnsiString(lNbDeleted)+"/"+AnsiString(mNbToDelete)+" Deleted");
	}
  }
  //Message(AnsiString(lNbDeleted)+" Files Deleted");
}
//---------------------------------------------------------------------------

void __fastcall TFormDuplDel::PshOptionsClick(TObject *Sender)
{
  PopupMenu1->Popup(Left+PshOptions->Left,Top+PshOptions->Top);
}
//---------------------------------------------------------------------------


void __fastcall TFormDuplDel::MenUseRecyclerClick(TObject *Sender)
{
  //MenUseRecycler->Checked =!MenUseRecycler->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormDuplDel::MenKeepOldestFileClick(TObject *Sender)
{
  //MenKeepOldestFile->Checked =!MenKeepOldestFile->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormDuplDel::MenKeepNewestFileClick(TObject *Sender)
{
  //MenKeepNewestFile->Checked = !  MenKeepNewestFile->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormDuplDel::PshOpenClick(TObject *Sender)
{

  //if(OpenDialog1->Execute())
  System::UnicodeString lDir=EdtDir->Text;
  //AnsiString lRoot=lDir;
  WideString lRoot="";
  //bool lExec=SelectDirectory("Select directory",lRoot,lDir);
  TSelectDirOpts lOpts;
  lOpts<<sdAllowCreate;
  //bool lExec=SelectDirectory(lDir,lOpts,(int)0);
  FileOpenDialog1->DefaultFolder = lDir;
  bool lExec = FileOpenDialog1->Execute();
  if (lExec)
  {
	  lDir = FileOpenDialog1->FileName;
	  mpListeFich->Clear();
      GrdFichiers->RowCount =  2;
    EdtDir->Text = lDir;
  }

}
//---------------------------------------------------------------------------

void __fastcall TFormDuplDel::PshAddClick(TObject *Sender)
{
	LstDirs->Items->Add(EdtDir->Text);
}
//---------------------------------------------------------------------------

void __fastcall TFormDuplDel::PshRemoveClick(TObject *Sender)
{
    LstDirs->Items->Delete(LstDirs->ItemIndex);
}
//---------------------------------------------------------------------------

