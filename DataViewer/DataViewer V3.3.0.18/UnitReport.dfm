object FormReport: TFormReport
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = #51216#44160#51068#51648
  ClientHeight = 726
  ClientWidth = 509
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
    Left = 251
    Top = 67
    Width = 56
    Height = 13
    Caption = #49548#49549'('#54924#49324')'
  end
  object Label2: TLabel
    Left = 29
    Top = 19
    Width = 48
    Height = 13
    Caption = #51216#44160#44396#48516
  end
  object Label3: TLabel
    Left = 29
    Top = 43
    Width = 48
    Height = 13
    BiDiMode = bdRightToLeft
    Caption = #51216#44160#51068#51088
    ParentBiDiMode = False
  end
  object Label4: TLabel
    Left = 263
    Top = 92
    Width = 48
    Height = 13
    Caption = #49444#52824#48169#49885
  end
  object Label5: TLabel
    Left = 29
    Top = 117
    Width = 48
    Height = 13
    BiDiMode = bdRightToLeft
    Caption = #51216#44160#47785#51201
    ParentBiDiMode = False
  end
  object Label6: TLabel
    Left = 29
    Top = 186
    Width = 48
    Height = 13
    BiDiMode = bdRightToLeft
    Caption = #47928#51228#50896#51064
    ParentBiDiMode = False
  end
  object Label7: TLabel
    Left = 47
    Top = 202
    Width = 30
    Height = 13
    BiDiMode = bdRightToLeft
    Caption = '   '#48143'   '
    ParentBiDiMode = False
  end
  object Label8: TLabel
    Left = 29
    Top = 218
    Width = 48
    Height = 13
    BiDiMode = bdRightToLeft
    Caption = #51312#52824#49324#54637
    ParentBiDiMode = False
  end
  object Label9: TLabel
    Left = 17
    Top = 256
    Width = 60
    Height = 13
    BiDiMode = bdRightToLeft
    Caption = #48120#52376#47532#49324#54637
    ParentBiDiMode = False
  end
  object Label10: TLabel
    Left = 47
    Top = 272
    Width = 30
    Height = 13
    BiDiMode = bdRightToLeft
    Caption = '   '#48143'   '
    ParentBiDiMode = False
  end
  object Label11: TLabel
    Left = 29
    Top = 288
    Width = 48
    Height = 13
    BiDiMode = bdRightToLeft
    Caption = #48372#50756#44228#54925
    ParentBiDiMode = False
  end
  object Label12: TLabel
    Left = 29
    Top = 326
    Width = 48
    Height = 13
    BiDiMode = bdRightToLeft
    Caption = #44368#52404#47932#54408
    ParentBiDiMode = False
  end
  object Label13: TLabel
    Left = 29
    Top = 396
    Width = 48
    Height = 13
    BiDiMode = bdRightToLeft
    Caption = #44592#53440#49324#54637
    ParentBiDiMode = False
  end
  object Label14: TLabel
    Left = 29
    Top = 499
    Width = 48
    Height = 13
    BiDiMode = bdRightToLeft
    Caption = #44592#53440#49324#54637
    ParentBiDiMode = False
  end
  object Label15: TLabel
    Left = 176
    Top = 499
    Width = 20
    Height = 13
    Caption = '  ~  '
  end
  object Label16: TLabel
    Left = 275
    Top = 43
    Width = 36
    Height = 13
    BiDiMode = bdRightToLeft
    Caption = 'User ID'
    ParentBiDiMode = False
  end
  object Label17: TLabel
    Left = 41
    Top = 67
    Width = 36
    Height = 13
    Caption = #51216#44160#51088
  end
  object ledPointName: TLabeledEdit
    Left = 79
    Top = 89
    Width = 154
    Height = 21
    TabStop = False
    Color = clBtnFace
    Ctl3D = True
    EditLabel.Width = 36
    EditLabel.Height = 13
    EditLabel.Caption = #51648#51216#47749
    ImeName = 'Microsoft Office IME 2007'
    LabelPosition = lpLeft
    MaxLength = 40
    ParentCtl3D = False
    ReadOnly = True
    TabOrder = 5
  end
  object btnClear: TButton
    Left = 29
    Top = 464
    Width = 75
    Height = 25
    Caption = 'Clear'
    TabOrder = 12
    OnClick = btnClearClick
  end
  object btnPost: TButton
    Left = 110
    Top = 464
    Width = 75
    Height = 25
    Caption = #51200#51109
    TabOrder = 13
    OnClick = btnPostClick
  end
  object btnDelete: TButton
    Left = 192
    Top = 464
    Width = 75
    Height = 25
    Caption = #49325#51228
    TabOrder = 14
    OnClick = btnDeleteClick
  end
  object DBGrid: TDBGrid
    Left = 25
    Top = 524
    Width = 468
    Height = 153
    TabStop = False
    Ctl3D = True
    DataSource = DataSource
    ImeName = 'Microsoft Office IME 2007'
    Options = [dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgTabs, dgRowSelect, dgConfirmDelete, dgCancelOnExit, dgTitleClick, dgTitleHotTrack]
    ParentCtl3D = False
    TabOrder = 19
    TitleFont.Charset = ANSI_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'Tahoma'
    TitleFont.Style = []
    OnCellClick = DBGridCellClick
  end
  object btnClose: TButton
    Left = 418
    Top = 691
    Width = 75
    Height = 25
    Caption = #45803#44592
    TabOrder = 20
    OnClick = btnCloseClick
  end
  object dtpDate1: TDateTimePicker
    Left = 79
    Top = 495
    Width = 98
    Height = 21
    Date = 41722.835456574070000000
    Time = 41722.835456574070000000
    ImeName = 'Microsoft Office IME 2007'
    TabOrder = 16
  end
  object dtpDate2: TDateTimePicker
    Left = 208
    Top = 495
    Width = 98
    Height = 21
    Date = 41722.835456574070000000
    Time = 41722.835456574070000000
    ImeName = 'Microsoft Office IME 2007'
    TabOrder = 17
  end
  object btnOpenQuery: TButton
    Left = 312
    Top = 493
    Width = 75
    Height = 25
    Caption = #51312#54924
    TabOrder = 18
    OnClick = btnOpenQueryClick
  end
  object btnSendMail: TButton
    Left = 418
    Top = 493
    Width = 75
    Height = 25
    Caption = #47700#51068
    TabOrder = 15
    OnClick = btnSendMailClick
  end
  object pnlCheckFlag: TPanel
    Left = 79
    Top = 13
    Width = 273
    Height = 20
    BevelOuter = bvNone
    TabOrder = 0
    object rbtnCheckFlag: TRadioGroup
      Left = -3
      Top = -45
      Width = 282
      Height = 105
      Caption = 'rbtnCheckFlag'
      Columns = 4
      Items.Strings = (
        #51221#44592#51216#44160
        #49688#49884#51216#44160
        #49688#51473#51216#44160
        #44592#53440)
      TabOrder = 0
    end
  end
  object mmPorpose: TMemo
    Left = 79
    Top = 114
    Width = 414
    Height = 64
    ImeName = 'Microsoft Office IME 2007'
    Lines.Strings = (
      'mmPorpose')
    MaxLength = 1000
    ScrollBars = ssVertical
    TabOrder = 7
  end
  object mmManage: TMemo
    Left = 79
    Top = 183
    Width = 414
    Height = 64
    ImeName = 'Microsoft Office IME 2007'
    Lines.Strings = (
      'mmManage')
    MaxLength = 1000
    ScrollBars = ssVertical
    TabOrder = 8
  end
  object mmComplement: TMemo
    Left = 79
    Top = 253
    Width = 414
    Height = 64
    ImeName = 'Microsoft Office IME 2007'
    Lines.Strings = (
      'mmComplement')
    MaxLength = 1000
    ScrollBars = ssVertical
    TabOrder = 9
  end
  object mmReplace: TMemo
    Left = 79
    Top = 323
    Width = 414
    Height = 64
    ImeName = 'Microsoft Office IME 2007'
    Lines.Strings = (
      'mmReplace')
    MaxLength = 1000
    ScrollBars = ssVertical
    TabOrder = 10
  end
  object mmEtc: TMemo
    Left = 79
    Top = 393
    Width = 414
    Height = 64
    ImeName = 'Microsoft Office IME 2007'
    Lines.Strings = (
      'mmEtc')
    MaxLength = 1000
    ScrollBars = ssVertical
    TabOrder = 11
  end
  object meCheckDate: TMaskEdit
    Left = 79
    Top = 39
    Width = 154
    Height = 21
    EditMask = '####-##-##;0'
    ImeName = 'Microsoft Office IME 2007'
    MaxLength = 10
    TabOrder = 1
    Text = ''
  end
  object edUserID: TEdit
    Left = 317
    Top = 40
    Width = 176
    Height = 21
    TabStop = False
    Color = clBtnFace
    ImeName = 'Microsoft Office IME 2007'
    ReadOnly = True
    TabOrder = 2
    Text = 'edUserID'
  end
  object edName: TEdit
    Left = 79
    Top = 64
    Width = 154
    Height = 21
    ImeName = 'Microsoft Office IME 2007'
    TabOrder = 3
    Text = 'edName'
  end
  object cbxDataFormat: TComboBox
    Left = 317
    Top = 89
    Width = 176
    Height = 21
    Style = csDropDownList
    ImeName = 'Microsoft Office IME 2007'
    TabOrder = 6
    Items.Strings = (
      'ADVM'
      'UVM'
      'UVM(Accusonic)')
  end
  object cbxCompany: TComboBox
    Left = 317
    Top = 64
    Width = 176
    Height = 21
    ImeName = 'Microsoft Office IME 2007'
    TabOrder = 4
    Text = 'cbxCompany'
  end
  object DataSource: TDataSource
    DataSet = SimpleDataSet1
    OnDataChange = DataSourceDataChange
    Left = 216
    Top = 584
  end
  object IdSMTP: TIdSMTP
    Host = '127.0.0.1'
    SASLMechanisms = <>
    ValidateAuthLoginCapability = False
    Left = 344
    Top = 408
  end
  object SimpleDataSet1: TSimpleDataSet
    Aggregates = <>
    DataSet.MaxBlobSize = -1
    DataSet.Params = <>
    Params = <>
    BeforePost = SimpleDataSet1BeforePost
    AfterScroll = SimpleDataSet1AfterScroll
    Left = 232
    Top = 304
  end
end
