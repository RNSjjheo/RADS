object FormLogin: TFormLogin
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = #47196#44536#51064
  ClientHeight = 139
  ClientWidth = 329
  Color = clBtnFace
  Font.Charset = ANSI_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poOwnerFormCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Bevel1: TBevel
    Left = 8
    Top = 8
    Width = 313
    Height = 81
  end
  object Label1: TLabel
    Left = 46
    Top = 24
    Width = 50
    Height = 13
    Alignment = taRightJustify
    Caption = #49324#50857#51088' ID'
  end
  object Label2: TLabel
    Left = 50
    Top = 56
    Width = 46
    Height = 13
    Alignment = taRightJustify
    Caption = 'Password'
  end
  object edID: TEdit
    Left = 112
    Top = 21
    Width = 185
    Height = 21
    Ctl3D = True
    ImeName = 'Microsoft Office IME 2007'
    ParentCtl3D = False
    TabOrder = 0
  end
  object edPassword: TEdit
    Left = 112
    Top = 53
    Width = 185
    Height = 21
    Ctl3D = True
    ImeName = 'Microsoft Office IME 2007'
    ParentCtl3D = False
    PasswordChar = '*'
    TabOrder = 1
  end
  object btnOk: TButton
    Left = 166
    Top = 104
    Width = 75
    Height = 25
    Caption = #54869#51064
    Default = True
    Font.Charset = HANGEUL_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = #44404#47548#52404
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    OnClick = btnOkClick
  end
  object btnCancel: TButton
    Left = 247
    Top = 104
    Width = 75
    Height = 25
    Cancel = True
    Caption = #52712#49548
    TabOrder = 3
    OnClick = btnCancelClick
  end
end
