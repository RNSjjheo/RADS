object frmVth: TfrmVth
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'VTH Logger'
  ClientHeight = 202
  ClientWidth = 344
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 8
    Top = 8
    Width = 322
    Height = 161
    Caption = 'VTH Display'
    Enabled = False
    TabOrder = 0
    object Shape1: TShape
      Left = 13
      Top = 34
      Width = 292
      Height = 89
      Brush.Color = clBlack
    end
    object lblVthStatus1: TLabel
      Left = 16
      Top = 35
      Width = 288
      Height = 42
      Alignment = taCenter
      AutoSize = False
      Caption = ' RNS VTH Logger '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clLime
      Font.Height = -29
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object lblVthStatus2: TLabel
      Left = 16
      Top = 79
      Width = 288
      Height = 42
      Alignment = taCenter
      AutoSize = False
      Caption = '                       '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clLime
      Font.Height = -27
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label2: TLabel
      Left = 66
      Top = 126
      Width = 181
      Height = 13
      Caption = 'DC Charge, DC Battery, Temperature'
    end
    object Label1: TLabel
      Left = 102
      Top = 142
      Width = 108
      Height = 13
      Caption = 'AIN0,AIN1,AIN2,AIN3'
    end
    object rbtnOn: TRadioButton
      Left = 216
      Top = 12
      Width = 41
      Height = 17
      Caption = 'On'
      Checked = True
      TabOrder = 0
      TabStop = True
    end
    object rbtnOff: TRadioButton
      Left = 263
      Top = 12
      Width = 41
      Height = 17
      Caption = 'Off'
      TabOrder = 1
    end
  end
  object btnPowerOff: TButton
    Left = 146
    Top = 173
    Width = 89
    Height = 25
    Caption = 'AC Power Off'
    TabOrder = 1
    OnClick = btnPowerOffClick
  end
  object btnClose: TButton
    Left = 241
    Top = 173
    Width = 89
    Height = 25
    Caption = #45803#44592
    TabOrder = 2
    OnClick = btnCloseClick
  end
  object tmrDisplayInfo: TTimer
    Enabled = False
    Interval = 5000
    OnTimer = tmrDisplayInfoTimer
    Left = 88
  end
  object tmrDelay: TTimer
    Enabled = False
    Interval = 60000
    OnTimer = tmrDelayTimer
    Left = 16
    Top = 151
  end
  object tmrRequest: TTimer
    Enabled = False
    Interval = 5000
    OnTimer = tmrRequestTimer
    Left = 64
    Top = 151
  end
end
