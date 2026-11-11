object FormUser: TFormUser
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = #49324#50857#51088' '#44288#47532
  ClientHeight = 368
  ClientWidth = 452
  Color = clBtnFace
  Font.Charset = ANSI_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 13
    Top = 60
    Width = 56
    Height = 13
    Caption = #49548#49549'('#54924#49324')'
  end
  object Label3: TLabel
    Left = 18
    Top = 11
    Width = 50
    Height = 13
    Caption = #49324#50857#51088' ID'
  end
  object Label4: TLabel
    Left = 24
    Top = 35
    Width = 46
    Height = 13
    Caption = 'Password'
  end
  object Label5: TLabel
    Left = 49
    Top = 85
    Width = 24
    Height = 13
    Caption = #51649#52293
  end
  object Label6: TLabel
    Left = 48
    Top = 109
    Width = 24
    Height = 13
    Caption = #51060#47492
  end
  object Label7: TLabel
    Left = 36
    Top = 133
    Width = 36
    Height = 13
    Caption = #50672#46973#52376
  end
  object Label2: TLabel
    Left = 48
    Top = 157
    Width = 24
    Height = 13
    Caption = #44428#54620
  end
  object edPw: TEdit
    Left = 203
    Top = 32
    Width = 121
    Height = 21
    Ctl3D = True
    ImeName = 'Microsoft Office IME 2007'
    ParentCtl3D = False
    PasswordChar = '*'
    TabOrder = 2
  end
  object Edit2: TEdit
    Left = 203
    Top = 56
    Width = 238
    Height = 21
    TabStop = False
    Color = clBtnFace
    Ctl3D = True
    ImeName = 'Microsoft Office IME 2007'
    ParentCtl3D = False
    ReadOnly = True
    TabOrder = 4
    Text = '(ex)'#50976#47049#51312#49324#49324#50629#45800'/'#47532#48260#50532#50472'/'#8230
  end
  object Edit3: TEdit
    Left = 203
    Top = 82
    Width = 238
    Height = 21
    TabStop = False
    Color = clBtnFace
    Ctl3D = True
    ImeName = 'Microsoft Office IME 2007'
    ParentCtl3D = False
    ReadOnly = True
    TabOrder = 6
    Text = '(ex)'#48512#51109'/'#45824#54364#51060#49324'/'#45824#47532'/'#44284#51109'/'#49324#50896
  end
  object Edit4: TEdit
    Left = 203
    Top = 130
    Width = 238
    Height = 21
    TabStop = False
    Color = clBtnFace
    Ctl3D = True
    ImeName = 'Microsoft Office IME 2007'
    ParentCtl3D = False
    ReadOnly = True
    TabOrder = 9
    Text = '(ex)010-9999-9999'
  end
  object btnClear: TButton
    Left = 8
    Top = 183
    Width = 75
    Height = 25
    Caption = 'Clear'
    TabOrder = 11
    OnClick = btnClearClick
  end
  object btnPost: TButton
    Left = 89
    Top = 183
    Width = 75
    Height = 25
    Caption = #51200#51109
    TabOrder = 12
    OnClick = btnPostClick
  end
  object btnDelete: TButton
    Left = 171
    Top = 183
    Width = 75
    Height = 25
    Caption = #49325#51228
    TabOrder = 13
    OnClick = btnDeleteClick
  end
  object DBGrid: TDBGrid
    Left = 8
    Top = 214
    Width = 433
    Height = 115
    TabStop = False
    DataSource = DataSource
    ImeName = 'Microsoft Office IME 2007'
    Options = [dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgTabs, dgRowSelect, dgConfirmDelete, dgCancelOnExit, dgTitleClick, dgTitleHotTrack]
    TabOrder = 14
    TitleFont.Charset = ANSI_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'Tahoma'
    TitleFont.Style = []
    OnCellClick = DBGridCellClick
  end
  object btnClose: TButton
    Left = 366
    Top = 335
    Width = 75
    Height = 25
    Caption = #51333#47308
    TabOrder = 15
    OnClick = btnCloseClick
  end
  object pnlPrivilege: TPanel
    Left = 76
    Top = 152
    Width = 176
    Height = 17
    BevelOuter = bvNone
    TabOrder = 10
    object rbtnPrivilege: TRadioGroup
      Left = -3
      Top = -47
      Width = 185
      Height = 105
      Caption = 'rbtnPrivilege'
      Columns = 2
      Items.Strings = (
        #51068#48152#49324#50857#51088
        #44288#47532#51088)
      TabOrder = 0
    end
  end
  object edUserId: TEdit
    Left = 76
    Top = 8
    Width = 121
    Height = 21
    ImeName = 'Microsoft Office IME 2007'
    TabOrder = 0
    Text = 'edUserId'
  end
  object edPassword: TEdit
    Left = 76
    Top = 32
    Width = 121
    Height = 21
    ImeName = 'Microsoft Office IME 2007'
    PasswordChar = '*'
    TabOrder = 1
    Text = 'edPassword'
  end
  object cbxCompany: TComboBox
    Left = 76
    Top = 57
    Width = 121
    Height = 21
    ImeName = 'Microsoft Office IME 2007'
    TabOrder = 3
    Text = 'cbxCompany'
  end
  object edPos: TEdit
    Left = 76
    Top = 82
    Width = 121
    Height = 21
    ImeName = 'Microsoft Office IME 2007'
    TabOrder = 5
    Text = 'edPos'
  end
  object edUserName: TEdit
    Left = 76
    Top = 106
    Width = 121
    Height = 21
    ImeName = 'Microsoft Office IME 2007'
    TabOrder = 7
    Text = 'edUserName'
  end
  object edPhone: TEdit
    Left = 76
    Top = 130
    Width = 121
    Height = 21
    ImeName = 'Microsoft Office IME 2007'
    TabOrder = 8
    Text = 'edPhone'
  end
  object DataSource: TDataSource
    DataSet = SimpleDataSet1
    Left = 176
    Top = 288
  end
  object SimpleDataSet1: TSimpleDataSet
    Aggregates = <>
    Connection = FormMain.SQLConnection
    DataSet.MaxBlobSize = -1
    DataSet.Params = <>
    Params = <>
    BeforePost = SimpleDataSet1BeforePost
    AfterScroll = SimpleDataSet1AfterScroll
    Left = 368
    Top = 8
  end
end
