object HydroSetupAG: THydroSetupAG
  Left = 0
  Top = 0
  Caption = 'AG1500K'
  ClientHeight = 662
  ClientWidth = 734
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  OnCloseQuery = FormCloseQuery
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 6
    Top = 460
    Width = 47
    Height = 13
    Caption = 'Command'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label2: TLabel
    Left = 6
    Top = 482
    Width = 56
    Height = 31
    AutoSize = False
    Caption = 'Blank Distance'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    WordWrap = True
  end
  object Label3: TLabel
    Left = 6
    Top = 521
    Width = 29
    Height = 13
    Caption = 'NCells'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label4: TLabel
    Left = 6
    Top = 553
    Width = 36
    Height = 13
    Caption = 'CellSize'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label5: TLabel
    Left = 6
    Top = 583
    Width = 48
    Height = 13
    Caption = #52769#51221#44060#49688
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label6: TLabel
    Left = 6
    Top = 614
    Width = 48
    Height = 13
    Caption = #52769#51221#49884#44036
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Bevel1: TBevel
    Left = 178
    Top = 486
    Width = 2
    Height = 151
  end
  object Memo: TMemo
    Left = 0
    Top = 0
    Width = 734
    Height = 450
    Align = alTop
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Courier New'
    Font.Style = []
    ImeName = 'Microsoft IME 2010'
    ParentFont = False
    ScrollBars = ssVertical
    TabOrder = 0
    WordWrap = False
  end
  object edCommand: TEdit
    Left = 64
    Top = 458
    Width = 560
    Height = 21
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ImeName = 'Microsoft IME 2010'
    ParentFont = False
    TabOrder = 1
    OnKeyPress = edCommandKeyPress
  end
  object cbSend: TButton
    Left = 633
    Top = 454
    Width = 96
    Height = 29
    Caption = 'Send'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    OnClick = cbSendClick
  end
  object cbBreak: TButton
    Left = 469
    Top = 485
    Width = 96
    Height = 29
    Caption = '<BREAK>'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    OnClick = cbBreakClick
  end
  object StatusBar: TStatusBar
    Left = 0
    Top = 643
    Width = 734
    Height = 19
    Panels = <>
    SimplePanel = True
  end
  object cbClose: TButton
    Left = 633
    Top = 608
    Width = 96
    Height = 29
    Caption = 'Close'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 5
    OnClick = cbCloseClick
  end
  object MemoCmd: TMemo
    Left = 189
    Top = 485
    Width = 170
    Height = 152
    Color = clBlack
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clYellow
    Font.Height = -12
    Font.Name = 'Courier New'
    Font.Style = []
    ImeName = 'Microsoft IME 2010'
    Lines.Strings = (
      'AutoSleep OFF'
      'OutFormat ASCII'
      'OutMode POLLED'
      'Recorder OFF'
      'NCells 50'
      'CellSize 2'
      'BlankDistance 1.5'
      'AvgInterval 450'
      'ProfileInterval 450'
      'CoordSystem ENU'
      'BurstMode NO'
      'SSU')
    ParentFont = False
    ScrollBars = ssVertical
    TabOrder = 6
  end
  object edBlankDistance: TEdit
    Left = 64
    Top = 488
    Width = 69
    Height = 21
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ImeName = 'Microsoft IME 2010'
    ParentFont = False
    TabOrder = 7
    Text = '1'
    OnKeyPress = edCommandKeyPress
  end
  object edNCells: TEdit
    Left = 64
    Top = 518
    Width = 69
    Height = 21
    ImeName = 'Microsoft IME 2010'
    TabOrder = 8
    Text = '50'
    OnKeyPress = edCommandKeyPress
  end
  object edCellSize: TEdit
    Left = 64
    Top = 550
    Width = 69
    Height = 21
    ImeName = 'Microsoft IME 2010'
    TabOrder = 9
    Text = '2'
    OnKeyPress = edCommandKeyPress
  end
  object edCellCount: TEdit
    Left = 64
    Top = 580
    Width = 69
    Height = 21
    Color = clBtnFace
    ImeName = 'Microsoft IME 2010'
    TabOrder = 10
    Text = '430'
    OnKeyPress = edCommandKeyPress
  end
  object edMeasureTime: TEdit
    Left = 64
    Top = 611
    Width = 69
    Height = 21
    Color = clBtnFace
    Enabled = False
    ImeName = 'Microsoft IME 2010'
    TabOrder = 11
    Text = 'AUTO'
    OnKeyPress = edCommandKeyPress
  end
  object cbBlankDistance: TButton
    Left = 136
    Top = 485
    Width = 32
    Height = 29
    Caption = #9654
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 12
    OnClick = cbBlankDistanceClick
  end
  object cbNCells: TButton
    Left = 136
    Top = 515
    Width = 32
    Height = 29
    Caption = #9654
    TabOrder = 13
    OnClick = cbNCellsClick
  end
  object cbCellSize: TButton
    Left = 136
    Top = 546
    Width = 32
    Height = 29
    Caption = #9654
    TabOrder = 14
    OnClick = cbCellSizeClick
  end
  object cbCellCount: TButton
    Left = 136
    Top = 577
    Width = 32
    Height = 29
    Caption = #9654
    Enabled = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 15
    OnClick = cbCellCountClick
  end
  object cbMeasureTime: TButton
    Left = 136
    Top = 608
    Width = 32
    Height = 29
    Caption = #9654
    Enabled = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 16
    OnClick = cbMeasureTimeClick
  end
  object edSave: TButton
    Left = 367
    Top = 485
    Width = 96
    Height = 29
    Caption = #49444#51221
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 17
    OnClick = edSaveClick
  end
  object edShow: TButton
    Left = 367
    Top = 520
    Width = 96
    Height = 29
    Caption = 'Show'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 18
    OnClick = edShowClick
  end
  object edShowConf: TButton
    Left = 367
    Top = 549
    Width = 96
    Height = 29
    Caption = 'Show Conf'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 19
    OnClick = edShowClick
  end
  object edShowSystem: TButton
    Left = 367
    Top = 578
    Width = 96
    Height = 29
    Caption = 'Show System'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 20
    OnClick = edShowClick
  end
  object edShowSetup: TButton
    Left = 367
    Top = 608
    Width = 96
    Height = 29
    Caption = 'Show Setup'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 21
    OnClick = edShowClick
  end
  object edShowDeploy: TButton
    Left = 469
    Top = 520
    Width = 96
    Height = 29
    Caption = 'Show Deploy'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 22
    OnClick = edShowClick
  end
  object edSetTime: TButton
    Left = 469
    Top = 549
    Width = 96
    Height = 29
    Caption = 'SetTime'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 23
    OnClick = edSetTimeClick
  end
  object edTime: TButton
    Left = 469
    Top = 578
    Width = 96
    Height = 29
    Caption = 'Help'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 24
    OnClick = edShowClick
  end
  object Timer: TTimer
    Enabled = False
    OnTimer = TimerTimer
    Left = 16
    Top = 16
  end
  object TimerClose: TTimer
    Enabled = False
    Interval = 5000
    OnTimer = TimerCloseTimer
    Left = 64
    Top = 16
  end
end
