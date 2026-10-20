object HydroSetupCM: THydroSetupCM
  Left = 0
  Top = 0
  Caption = 'ChannelMaster'
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
    Width = 49
    Height = 14
    Caption = 'Command'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
  end
  object Label2: TLabel
    Left = 41
    Top = 489
    Width = 14
    Height = 14
    Caption = 'WF'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
  end
  object Label3: TLabel
    Left = 41
    Top = 519
    Width = 14
    Height = 14
    Caption = 'WN'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
  end
  object Label4: TLabel
    Left = 41
    Top = 550
    Width = 14
    Height = 14
    Caption = 'WS'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
  end
  object Label5: TLabel
    Left = 41
    Top = 581
    Width = 14
    Height = 14
    Caption = 'WP'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
  end
  object Label6: TLabel
    Left = 41
    Top = 612
    Width = 14
    Height = 14
    Caption = 'TP'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Courier New'
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
    TabOrder = 15
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
    TabOrder = 27
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
      'CF 01010'
      'CL 0'
      'VP 0'
      'WF 100'
      'WN 50'
      'WP 430'
      'WS 200'
      'WV 500'
      'TP 00:01,00'
      'CK')
    ParentFont = False
    ScrollBars = ssVertical
    TabOrder = 13
  end
  object edWF: TEdit
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
    TabOrder = 3
    Text = '100'
    OnKeyPress = edCommandKeyPress
  end
  object edWN: TEdit
    Left = 64
    Top = 518
    Width = 69
    Height = 21
    ImeName = 'Microsoft IME 2010'
    TabOrder = 5
    Text = '50'
    OnKeyPress = edCommandKeyPress
  end
  object edWS: TEdit
    Left = 64
    Top = 549
    Width = 69
    Height = 21
    ImeName = 'Microsoft IME 2010'
    TabOrder = 7
    Text = '200'
    OnKeyPress = edCommandKeyPress
  end
  object edWP: TEdit
    Left = 64
    Top = 580
    Width = 69
    Height = 21
    ImeName = 'Microsoft IME 2010'
    TabOrder = 9
    Text = '430'
    OnKeyPress = edCommandKeyPress
  end
  object edTP: TEdit
    Left = 64
    Top = 611
    Width = 69
    Height = 21
    ImeName = 'Microsoft IME 2010'
    TabOrder = 11
    Text = '00:01,00'
    OnKeyPress = edCommandKeyPress
  end
  object cbWF: TButton
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
    TabOrder = 4
    OnClick = cbWFClick
  end
  object cbWN: TButton
    Left = 136
    Top = 515
    Width = 32
    Height = 29
    Caption = #9654
    TabOrder = 6
    OnClick = cbWNClick
  end
  object cbWS: TButton
    Left = 136
    Top = 546
    Width = 32
    Height = 29
    Caption = #9654
    TabOrder = 8
    OnClick = cbWSClick
  end
  object cbWP: TButton
    Left = 136
    Top = 577
    Width = 32
    Height = 29
    Caption = #9654
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 10
    OnClick = cbWPClick
  end
  object cbTP: TButton
    Left = 136
    Top = 608
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
    OnClick = cbTPClick
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
    TabOrder = 14
    OnClick = edSaveClick
  end
  object edC: TButton
    Left = 367
    Top = 520
    Width = 32
    Height = 29
    Caption = 'C?'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
    TabOrder = 16
    OnClick = edCClick
  end
  object edE: TButton
    Left = 399
    Top = 520
    Width = 32
    Height = 29
    Caption = 'E?'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
    TabOrder = 17
    OnClick = edCClick
  end
  object edI: TButton
    Left = 431
    Top = 520
    Width = 32
    Height = 29
    Caption = 'I?'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
    TabOrder = 18
    OnClick = edCClick
  end
  object edM: TButton
    Left = 463
    Top = 520
    Width = 32
    Height = 29
    Caption = 'M?'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
    TabOrder = 19
    OnClick = edCClick
  end
  object edP: TButton
    Left = 496
    Top = 520
    Width = 32
    Height = 29
    Caption = 'P?'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
    TabOrder = 20
    OnClick = edCClick
  end
  object edS: TButton
    Left = 528
    Top = 520
    Width = 32
    Height = 29
    Caption = 'S?'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
    TabOrder = 21
    OnClick = edCClick
  end
  object edT: TButton
    Left = 560
    Top = 520
    Width = 32
    Height = 29
    Caption = 'T?'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
    TabOrder = 22
    OnClick = edCClick
  end
  object edV: TButton
    Left = 593
    Top = 520
    Width = 32
    Height = 29
    Caption = 'V?'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
    TabOrder = 23
    OnClick = edCClick
  end
  object edY: TButton
    Left = 625
    Top = 520
    Width = 32
    Height = 29
    Caption = 'Y?'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
    TabOrder = 24
    OnClick = edCClick
  end
  object edW: TButton
    Left = 657
    Top = 520
    Width = 32
    Height = 29
    Caption = 'W?'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
    TabOrder = 25
    OnClick = edCClick
  end
  object edHelp: TButton
    Left = 690
    Top = 520
    Width = 32
    Height = 29
    Caption = '?'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
    TabOrder = 26
    OnClick = edCClick
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
