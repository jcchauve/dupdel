object FormDuplDel: TFormDuplDel
  Left = 137
  Top = 174
  Caption = 'Duplicate Files Deletor'
  ClientHeight = 527
  ClientWidth = 735
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PopupMenu = PopupMenu1
  DesignSize = (
    735
    527)
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 16
    Top = 16
    Width = 42
    Height = 13
    Caption = 'Directory'
  end
  object Label2: TLabel
    Left = 320
    Top = 16
    Width = 22
    Height = 13
    Caption = 'Filter'
  end
  object EdtDir: TEdit
    Left = 72
    Top = 16
    Width = 170
    Height = 21
    TabOrder = 0
    Text = 'c:\'
  end
  object EdtFilter: TEdit
    Left = 352
    Top = 16
    Width = 121
    Height = 21
    TabOrder = 1
    Text = '*.*'
  end
  object PshFind: TButton
    Left = 488
    Top = 16
    Width = 75
    Height = 25
    Caption = 'Find'
    TabOrder = 2
    OnClick = PshFindClick
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 508
    Width = 735
    Height = 19
    Panels = <>
    SimplePanel = True
  end
  object PshDelete: TButton
    Left = 568
    Top = 16
    Width = 75
    Height = 25
    Caption = 'Delete'
    TabOrder = 4
    OnClick = PshDeleteClick
  end
  object PshOptions: TButton
    Left = 648
    Top = 16
    Width = 75
    Height = 25
    Caption = 'Options'
    TabOrder = 5
    OnClick = PshOptionsClick
  end
  object PshOpen: TButton
    Left = 248
    Top = 16
    Width = 25
    Height = 25
    Caption = '...'
    TabOrder = 6
    OnClick = PshOpenClick
  end
  object PageControl1: TPageControl
    Left = 0
    Top = 56
    Width = 735
    Height = 446
    ActivePage = TabSheet1
    Anchors = [akLeft, akTop, akBottom]
    TabOrder = 7
    object TabSheet1: TTabSheet
      Caption = 'Directories'
      ExplicitLeft = 8
      ExplicitTop = 28
      DesignSize = (
        727
        418)
      object PshAdd: TButton
        Left = 644
        Top = 48
        Width = 75
        Height = 25
        Anchors = [akTop, akRight]
        Caption = 'Add'
        TabOrder = 0
        OnClick = PshAddClick
      end
      object LstDirs: TListBox
        Left = 8
        Top = 16
        Width = 609
        Height = 402
        Anchors = [akLeft, akTop, akRight, akBottom]
        ItemHeight = 13
        TabOrder = 1
      end
      object PshRemove: TButton
        Left = 644
        Top = 192
        Width = 75
        Height = 25
        Caption = 'Remove'
        TabOrder = 2
        OnClick = PshRemoveClick
      end
    end
    object TabSheet2: TTabSheet
      Caption = 'Results'
      ImageIndex = 1
      object GrdFichiers: TStringGrid
        Left = 0
        Top = 0
        Width = 727
        Height = 418
        Align = alClient
        ColCount = 3
        DefaultRowHeight = 18
        FixedCols = 0
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing]
        TabOrder = 0
      end
    end
    object TabSheet3: TTabSheet
      Caption = 'Logs'
      ImageIndex = 2
      object MemoLogs: TMemo
        Left = 0
        Top = 0
        Width = 727
        Height = 418
        Align = alClient
        Lines.Strings = (
          'Memo1')
        TabOrder = 0
      end
    end
  end
  object PopupMenu1: TPopupMenu
    Left = 648
    Top = 8
    object UseDelete1: TMenuItem
      AutoCheck = True
      Caption = 'Use Delete'
      RadioItem = True
    end
    object MenUseBatchPreview: TMenuItem
      AutoCheck = True
      Caption = 'Use Batch Preview'
      Checked = True
      RadioItem = True
    end
    object MenUseRecycler: TMenuItem
      AutoCheck = True
      Caption = 'Use Recycler'
      RadioItem = True
      OnClick = MenUseRecyclerClick
    end
    object N1: TMenuItem
      Break = mbBreak
      Caption = '-'
    end
    object MenKeepOldestFile: TMenuItem
      Caption = 'Keep Oldest File'
      GroupIndex = 1
      RadioItem = True
      OnClick = MenKeepOldestFileClick
    end
    object MenKeepNewestFile: TMenuItem
      Caption = 'Keep Newest File'
      Checked = True
      GroupIndex = 1
      RadioItem = True
      OnClick = MenKeepNewestFileClick
    end
  end
  object FileOpenDialog1: TFileOpenDialog
    FavoriteLinks = <>
    FileTypes = <>
    Options = [fdoPickFolders]
    Left = 408
    Top = 48
  end
end
