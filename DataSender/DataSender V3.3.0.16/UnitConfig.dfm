object FormConfig: TFormConfig
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = #54872#44221#49444#51221
  ClientHeight = 532
  ClientWidth = 687
  Color = clBtnFace
  Font.Charset = ANSI_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnActivate = FormActivate
  OnClose = FormClose
  OnShow = FormShow
  DesignSize = (
    687
    532)
  PixelsPerInch = 96
  TextHeight = 14
  object Label14: TLabel
    Left = 64
    Top = 324
    Width = 41
    Height = 14
    BiDiMode = bdRightToLeft
    Caption = 'Interval'
    ParentBiDiMode = False
  end
  object pcConfig: TPageControl
    Left = 0
    Top = 0
    Width = 687
    Height = 493
    ActivePage = TabSheet3
    Align = alTop
    TabOrder = 0
    TabWidth = 70
    OnChange = pcConfigChange
    object TabSheet1: TTabSheet
      Caption = #51648#51216#51221#48372
      DesignSize = (
        679
        464)
      object btnSave0: TButton
        Left = 598
        Top = 436
        Width = 75
        Height = 25
        Anchors = [akRight, akBottom]
        Caption = #51200#51109
        TabOrder = 7
        OnClick = btnSaveClick
      end
      object led1PointCode: TLabeledEdit
        Left = 92
        Top = 18
        Width = 121
        Height = 22
        Hint = 'rsetup.value01'
        Ctl3D = True
        EditLabel.Width = 40
        EditLabel.Height = 14
        EditLabel.Caption = #51648#51216#53076#46300
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        ParentCtl3D = False
        TabOrder = 0
      end
      object led1PointName: TLabeledEdit
        Left = 92
        Top = 44
        Width = 121
        Height = 22
        Hint = 'rsetup.value02'
        Ctl3D = True
        EditLabel.Width = 30
        EditLabel.Height = 14
        EditLabel.Caption = #51648#51216#47749
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        ParentCtl3D = False
        TabOrder = 1
      end
      object led1PointAddress: TLabeledEdit
        Left = 92
        Top = 70
        Width = 409
        Height = 22
        Hint = 'rsetup.value03'
        Ctl3D = True
        EditLabel.Width = 40
        EditLabel.Height = 14
        EditLabel.Caption = #51648#51216#51452#49548
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        ParentCtl3D = False
        TabOrder = 2
      end
      object led1PointZipCode: TLabeledEdit
        Left = 92
        Top = 96
        Width = 121
        Height = 22
        Hint = 'rsetup.value04'
        Ctl3D = True
        EditLabel.Width = 40
        EditLabel.Height = 14
        EditLabel.Caption = #50864#54200#48264#54840
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        ParentCtl3D = False
        TabOrder = 3
      end
      object led1PointManager: TLabeledEdit
        Left = 92
        Top = 122
        Width = 121
        Height = 22
        Hint = 'rsetup.value05'
        Ctl3D = True
        EditLabel.Width = 30
        EditLabel.Height = 14
        EditLabel.Caption = #45812#45817#51088
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        ParentCtl3D = False
        TabOrder = 4
      end
      object led1PhoneTel: TLabeledEdit
        Left = 92
        Top = 148
        Width = 121
        Height = 22
        Hint = 'rsetup.value06'
        Ctl3D = True
        EditLabel.Width = 64
        EditLabel.Height = 14
        EditLabel.Caption = #45812#45817#51088' '#50672#46973#52376
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        ParentCtl3D = False
        TabOrder = 5
      end
      object led1PointEtc: TLabeledEdit
        Left = 92
        Top = 176
        Width = 409
        Height = 22
        Hint = 'rsetup.value07'
        Ctl3D = True
        EditLabel.Width = 40
        EditLabel.Height = 14
        EditLabel.Caption = #53945#51060#49324#54637
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        ParentCtl3D = False
        TabOrder = 6
      end
    end
    object TabSheet2: TTabSheet
      Caption = #47196#44144
      ImageIndex = 1
      DesignSize = (
        679
        464)
      object Label22: TLabel
        Left = 57
        Top = 22
        Width = 30
        Height = 14
        BiDiMode = bdRightToLeft
        Caption = #51064#53552#48268
        ParentBiDiMode = False
      end
      object Label24: TLabel
        Left = 161
        Top = 22
        Width = 34
        Height = 14
        Caption = '('#48516':'#52488')'
      end
      object Label25: TLabel
        Left = 47
        Top = 48
        Width = 40
        Height = 14
        BiDiMode = bdRightToLeft
        Caption = #46041#51089#51452#44592
        ParentBiDiMode = False
      end
      object Label26: TLabel
        Left = 161
        Top = 47
        Width = 34
        Height = 14
        Caption = '('#48516':'#52488')'
      end
      object Label27: TLabel
        Left = 47
        Top = 74
        Width = 40
        Height = 14
        BiDiMode = bdRightToLeft
        Caption = #49884#51089#49884#44036
        ParentBiDiMode = False
      end
      object Label28: TLabel
        Left = 209
        Top = 74
        Width = 170
        Height = 14
        Caption = '('#47588#48516' '#49884#51089#54616#50668' 10'#48516' '#46041#50504' '#51088#47308#49688#51665')'
      end
      object Label1: TLabel
        Left = 357
        Top = 22
        Width = 60
        Height = 14
        BiDiMode = bdRightToLeft
        Caption = #50976#47049#49328#52636#48169#49885
        ParentBiDiMode = False
      end
      object btnSave1: TButton
        Tag = 1
        Left = 598
        Top = 436
        Width = 75
        Height = 25
        Anchors = [akRight, akBottom]
        Caption = #51200#51109
        TabOrder = 7
        OnClick = btnSaveClick
      end
      object med2Interval: TMaskEdit
        Left = 92
        Top = 18
        Width = 65
        Height = 22
        Hint = 'rsetup.value01|text'
        Ctl3D = True
        EditMask = '##:##;1'
        ImeName = 'Microsoft Office IME 2007'
        MaxLength = 5
        ParentCtl3D = False
        TabOrder = 1
        Text = '  :  '
      end
      object med2Period: TMaskEdit
        Left = 93
        Top = 44
        Width = 65
        Height = 22
        Hint = 'rsetup.value02|text'
        Ctl3D = True
        EditMask = '##:##;1'
        ImeName = 'Microsoft Office IME 2007'
        MaxLength = 5
        ParentCtl3D = False
        TabOrder = 2
        Text = '  :  '
      end
      object cbx2StartTime: TComboBox
        Left = 92
        Top = 70
        Width = 109
        Height = 22
        Hint = 'rsetup.value03|text'
        Style = csDropDownList
        Ctl3D = True
        ImeName = 'Microsoft Office IME 2007'
        ParentCtl3D = False
        TabOrder = 3
        Items.Strings = (
          '0'
          '1'
          '2'
          '3'
          '4'
          '5'
          '6'
          '7'
          '8'
          '9')
      end
      object gbxHydroMeter1: TGroupBox
        Left = 3
        Top = 96
        Width = 437
        Height = 156
        Caption = #54616#52789#50976#49549#44228' '#49444#51221
        Ctl3D = True
        ParentCtl3D = False
        TabOrder = 4
        object Bevel1: TBevel
          Left = 218
          Top = 11
          Width = 201
          Height = 114
          Shape = bsFrame
        end
        object Label34: TLabel
          Left = 31
          Top = 23
          Width = 54
          Height = 14
          BiDiMode = bdRightToLeft
          Caption = #51109#48708#51032' '#51333#47448
          ParentBiDiMode = False
        end
        object Label35: TLabel
          Left = 39
          Top = 49
          Width = 47
          Height = 14
          BiDiMode = bdRightToLeft
          Caption = 'Comport'
          ParentBiDiMode = False
        end
        object Label36: TLabel
          Left = 46
          Top = 75
          Width = 40
          Height = 14
          BiDiMode = bdRightToLeft
          Caption = #53685#49888#49549#46020
          ParentBiDiMode = False
        end
        object Label53: TLabel
          Left = 249
          Top = 49
          Width = 68
          Height = 14
          BiDiMode = bdRightToLeft
          Caption = #52769#51221' '#49472' '#52509#44060#49688
          ParentBiDiMode = False
        end
        object Label54: TLabel
          Left = 259
          Top = 75
          Width = 58
          Height = 14
          BiDiMode = bdRightToLeft
          Caption = #52769#51221' '#49472' '#53356#44592
          ParentBiDiMode = False
        end
        object Label55: TLabel
          Left = 259
          Top = 101
          Width = 58
          Height = 14
          BiDiMode = bdRightToLeft
          Caption = #44277#48177' '#44144#47532' '#49472
          ParentBiDiMode = False
        end
        object Label59: TLabel
          Left = 271
          Top = 23
          Width = 94
          Height = 14
          BiDiMode = bdLeftToRight
          Caption = 'AG/SL Series '#49444#51221
          ParentBiDiMode = False
        end
        object Label46: TLabel
          Left = 19
          Top = 129
          Width = 67
          Height = 14
          BiDiMode = bdRightToLeft
          Caption = 'Reverse '#50741#49496
          ParentBiDiMode = False
        end
        object cbx2H1Kind: TComboBox
          Left = 89
          Top = 19
          Width = 109
          Height = 22
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 0
          OnChange = cbx2H1KindChange
          Items.Strings = (
            'NONE'
            'ChannelMaster'
            'SL Series'
            'AG Series')
        end
        object cbx2H1Comport: TComboBox
          Left = 89
          Top = 45
          Width = 109
          Height = 22
          Hint = 'rsetup.value02|text'
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 1
          Items.Strings = (
            'COM1'
            'COM2'
            'COM3'
            'COM4'
            'COM5'
            'COM6'
            'COM7'
            'COM8'
            'COM9'
            'COM10'
            'COM11'
            'COM12'
            'COM13'
            'COM14'
            'COM15'
            'COM16'
            'COM17'
            'COM18'
            'COM19'
            'COM20'
            'COM21'
            'COM22'
            'COM23'
            'COM24'
            'COM25'
            'COM26'
            'COM27'
            'COM28'
            'COM29'
            'COM30'
            'COM31'
            'COM32')
        end
        object cbx2H1CommSpeed: TComboBox
          Left = 89
          Top = 71
          Width = 109
          Height = 22
          Hint = 'rsetup.value03|text'
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 2
          Items.Strings = (
            '2400'
            '4800'
            '9600'
            '14400'
            '19200'
            '28800'
            '33600'
            '38400'
            '56000')
        end
        object led2H1Offset: TLabeledEdit
          Left = 89
          Top = 98
          Width = 109
          Height = 22
          Hint = 'rsetup.value04|text'
          Ctl3D = True
          EditLabel.Width = 34
          EditLabel.Height = 14
          EditLabel.Caption = 'Offset'
          ImeName = 'Microsoft Office IME 2007'
          LabelPosition = lpLeft
          MaxLength = 100
          ParentCtl3D = False
          TabOrder = 3
          OnExit = OnOffsetExit
        end
        object ed2H1WN: TEdit
          Left = 321
          Top = 45
          Width = 88
          Height = 22
          Hint = 'rsetup.value07'
          ImeName = 'Microsoft Office IME 2007'
          NumbersOnly = True
          TabOrder = 5
        end
        object ed2H1WS: TEdit
          Left = 321
          Top = 71
          Width = 88
          Height = 22
          Hint = 'rsetup.value08'
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 6
          OnChange = FloatEditChange
        end
        object ed2H1WF: TEdit
          Left = 321
          Top = 98
          Width = 88
          Height = 22
          Hint = 'rsetup.value09'
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 7
          OnChange = FloatEditChange
        end
        object cbx2H1ReverseOption: TComboBox
          Left = 89
          Top = 126
          Width = 109
          Height = 22
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 4
          Items.Strings = (
            'ON'
            'OFF')
        end
      end
      object gbxHydroMeter2: TGroupBox
        Left = 3
        Top = 255
        Width = 437
        Height = 178
        Caption = #49345#52789#50976#49549#44228' '#49444#51221
        Ctl3D = True
        ParentCtl3D = False
        TabOrder = 5
        object Bevel2: TBevel
          Left = 218
          Top = 11
          Width = 201
          Height = 114
          Shape = bsFrame
        end
        object Label37: TLabel
          Left = 31
          Top = 23
          Width = 54
          Height = 14
          BiDiMode = bdRightToLeft
          Caption = #51109#48708#51032' '#51333#47448
          ParentBiDiMode = False
        end
        object Label38: TLabel
          Left = 39
          Top = 49
          Width = 47
          Height = 14
          BiDiMode = bdRightToLeft
          Caption = 'Comport'
          ParentBiDiMode = False
        end
        object Label39: TLabel
          Left = 46
          Top = 75
          Width = 40
          Height = 14
          BiDiMode = bdRightToLeft
          Caption = #53685#49888#49549#46020
          ParentBiDiMode = False
        end
        object Label56: TLabel
          Left = 249
          Top = 49
          Width = 68
          Height = 14
          BiDiMode = bdRightToLeft
          Caption = #52769#51221' '#49472' '#52509#44060#49688
          ParentBiDiMode = False
        end
        object Label57: TLabel
          Left = 259
          Top = 75
          Width = 58
          Height = 14
          BiDiMode = bdRightToLeft
          Caption = #52769#51221' '#49472' '#53356#44592
          ParentBiDiMode = False
        end
        object Label58: TLabel
          Left = 259
          Top = 101
          Width = 58
          Height = 14
          BiDiMode = bdRightToLeft
          Caption = #44277#48177' '#44144#47532' '#49472
          ParentBiDiMode = False
        end
        object Label60: TLabel
          Left = 271
          Top = 23
          Width = 94
          Height = 14
          BiDiMode = bdLeftToRight
          Caption = 'AG/SL Series '#49444#51221
          ParentBiDiMode = False
        end
        object Label47: TLabel
          Left = 19
          Top = 127
          Width = 67
          Height = 14
          BiDiMode = bdRightToLeft
          Caption = 'Reverse '#50741#49496
          ParentBiDiMode = False
        end
        object Label49: TLabel
          Left = 19
          Top = 153
          Width = 67
          Height = 14
          BiDiMode = bdRightToLeft
          Caption = 'Up/Side '#50741#49496
          ParentBiDiMode = False
        end
        object cbx2H2Kind: TComboBox
          Left = 89
          Top = 19
          Width = 109
          Height = 22
          Hint = 'rsetup.value01|text'
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 0
          OnChange = cbx2H2KindChange
          Items.Strings = (
            'NONE'
            'ChannelMaster'
            'SL Series'
            'AG Series')
        end
        object cbx2H2Comport: TComboBox
          Left = 89
          Top = 45
          Width = 109
          Height = 22
          Hint = 'rsetup.value02|text'
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 1
          Items.Strings = (
            'COM1'
            'COM2'
            'COM3'
            'COM4'
            'COM5'
            'COM6'
            'COM7'
            'COM8'
            'COM9'
            'COM10'
            'COM11'
            'COM12'
            'COM13'
            'COM14'
            'COM15'
            'COM16'
            'COM17'
            'COM18'
            'COM19'
            'COM20'
            'COM21'
            'COM22'
            'COM23'
            'COM24'
            'COM25'
            'COM26'
            'COM27'
            'COM28'
            'COM29'
            'COM30'
            'COM31'
            'COM32')
        end
        object cbx2H2CommSpeed: TComboBox
          Left = 89
          Top = 71
          Width = 109
          Height = 22
          Hint = 'rsetup.value03|text'
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 2
          Items.Strings = (
            '2400'
            '4800'
            '9600'
            '14400'
            '19200'
            '28800'
            '33600'
            '38400'
            '56000')
        end
        object led2H2Offset: TLabeledEdit
          Left = 89
          Top = 98
          Width = 109
          Height = 22
          Hint = 'rsetup.value04|text'
          Ctl3D = True
          EditLabel.Width = 34
          EditLabel.Height = 14
          EditLabel.Caption = 'Offset'
          ImeName = 'Microsoft Office IME 2007'
          LabelPosition = lpLeft
          MaxLength = 100
          ParentCtl3D = False
          TabOrder = 3
          OnExit = OnOffsetExit
        end
        object ed2H2WN: TEdit
          Left = 321
          Top = 45
          Width = 88
          Height = 22
          Hint = 'rsetup.value07'
          ImeName = 'Microsoft Office IME 2007'
          NumbersOnly = True
          TabOrder = 6
        end
        object ed2H2WS: TEdit
          Left = 321
          Top = 71
          Width = 88
          Height = 22
          Hint = 'rsetup.value08'
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 7
          OnChange = FloatEditChange
        end
        object ed2H2WF: TEdit
          Left = 321
          Top = 98
          Width = 88
          Height = 22
          Hint = 'rsetup.value09'
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 8
          OnChange = FloatEditChange
        end
        object cbx2H2ReverseOption: TComboBox
          Left = 89
          Top = 124
          Width = 109
          Height = 22
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 4
          Items.Strings = (
            'ON'
            'OFF')
        end
        object cbx2H2UpSideOption: TComboBox
          Left = 89
          Top = 150
          Width = 109
          Height = 22
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 5
          Items.Strings = (
            'UP'
            'SIDE')
        end
      end
      object gbxWaterLevel: TGroupBox
        Left = 446
        Top = 96
        Width = 227
        Height = 337
        Caption = #49688#50948#44228' '#49444#51221
        Ctl3D = True
        ParentCtl3D = False
        TabOrder = 6
        object Label40: TLabel
          Left = 31
          Top = 23
          Width = 54
          Height = 14
          BiDiMode = bdRightToLeft
          Caption = #51109#48708#51032' '#51333#47448
          ParentBiDiMode = False
        end
        object Label41: TLabel
          Left = 39
          Top = 49
          Width = 47
          Height = 14
          BiDiMode = bdRightToLeft
          Caption = 'Comport'
          ParentBiDiMode = False
        end
        object Label42: TLabel
          Left = 46
          Top = 75
          Width = 40
          Height = 14
          BiDiMode = bdRightToLeft
          Caption = #53685#49888#49549#46020
          ParentBiDiMode = False
        end
        object cbx2WLKind: TComboBox
          Left = 89
          Top = 19
          Width = 109
          Height = 22
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 0
          Items.Strings = (
            'NONE'
            'LT500'
            'AT200')
        end
        object cbx2WLComport: TComboBox
          Left = 89
          Top = 45
          Width = 109
          Height = 22
          Hint = 'rsetup.value02|text'
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 1
          Items.Strings = (
            'COM1'
            'COM2'
            'COM3'
            'COM4'
            'COM5'
            'COM6'
            'COM7'
            'COM8'
            'COM9'
            'COM10'
            'COM11'
            'COM12'
            'COM13'
            'COM14'
            'COM15'
            'COM16'
            'COM17'
            'COM18'
            'COM19'
            'COM20'
            'COM21'
            'COM22'
            'COM23'
            'COM24'
            'COM25'
            'COM26'
            'COM27'
            'COM28'
            'COM29'
            'COM30'
            'COM31'
            'COM32')
        end
        object cbx2WLCommSpeed: TComboBox
          Left = 89
          Top = 71
          Width = 109
          Height = 22
          Hint = 'rsetup.value03|text'
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 2
          Items.Strings = (
            '2400'
            '4800'
            '9600'
            '14400'
            '19200'
            '28800'
            '33600'
            '38400'
            '56000')
        end
        object led2WLOffset: TLabeledEdit
          Left = 89
          Top = 98
          Width = 109
          Height = 22
          Hint = 'rsetup.value04|text'
          Ctl3D = True
          EditLabel.Width = 34
          EditLabel.Height = 14
          EditLabel.Caption = 'Offset'
          ImeName = 'Microsoft Office IME 2007'
          LabelPosition = lpLeft
          MaxLength = 100
          ParentCtl3D = False
          TabOrder = 3
          OnExit = OnOffsetExit
        end
      end
      object cbx2StreamMethod: TComboBox
        Left = 423
        Top = 18
        Width = 229
        Height = 22
        Hint = 'rsetup.value02|index'
        Style = csDropDownList
        Ctl3D = True
        ImeName = 'Microsoft Office IME 2007'
        ParentCtl3D = False
        TabOrder = 0
        Items.Strings = (
          '0: '#50976#47049#49328#52636' OFF'
          '1: '#50976#47049#49324#50629#45800' '#48169#49885
          '2: RNSEA '#51088#52404' '#48169#49885)
      end
    end
    object TabSheet3: TTabSheet
      Caption = #49356#45908
      ImageIndex = 2
      DesignSize = (
        679
        464)
      object Label6: TLabel
        Left = 382
        Top = 45
        Width = 76
        Height = 14
        BiDiMode = bdRightToLeft
        Caption = 'Rnsea'#49436#48260' '#51204#49569
        ParentBiDiMode = False
      end
      object Label15: TLabel
        Left = 37
        Top = 20
        Width = 84
        Height = 14
        BiDiMode = bdRightToLeft
        Caption = 'FTP File Format'
        ParentBiDiMode = False
      end
      object Label21: TLabel
        Left = 57
        Top = 45
        Width = 64
        Height = 14
        BiDiMode = bdRightToLeft
        Caption = #49688#51665#49436#48260' '#51204#49569
        ParentBiDiMode = False
      end
      object Label134: TLabel
        Left = 76
        Top = 229
        Width = 45
        Height = 14
        BiDiMode = bdRightToLeft
        Caption = 'FTP '#47784#46300
        ParentBiDiMode = False
      end
      object Label135: TLabel
        Left = 377
        Top = 229
        Width = 81
        Height = 14
        BiDiMode = bdRightToLeft
        Caption = 'Rnsea FTP '#47784#46300
        ParentBiDiMode = False
      end
      object btnSave2: TButton
        Tag = 2
        Left = 598
        Top = 436
        Width = 75
        Height = 25
        Anchors = [akRight, akBottom]
        Caption = #51200#51109
        TabOrder = 16
        OnClick = btnSaveClick
      end
      object cbx3ServerSendYN: TComboBox
        Left = 124
        Top = 41
        Width = 109
        Height = 22
        Hint = 'rsetup.value01|text'
        Style = csDropDownList
        Ctl3D = True
        ImeName = 'Microsoft Office IME 2007'
        ParentCtl3D = False
        TabOrder = 1
        Items.Strings = (
          'ON'
          'OFF')
      end
      object led3ServerDomain: TLabeledEdit
        Left = 124
        Top = 67
        Width = 121
        Height = 22
        Hint = 'rsetup.value02'
        Ctl3D = True
        EditLabel.Width = 90
        EditLabel.Height = 14
        EditLabel.Caption = #49688#51665#49436#48260' '#46020#47700#51064'/IP'
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        ParentCtl3D = False
        TabOrder = 2
      end
      object led3ServerPort: TLabeledEdit
        Left = 124
        Top = 93
        Width = 121
        Height = 22
        Hint = 'rsetup.value03'
        Ctl3D = True
        EditLabel.Width = 67
        EditLabel.Height = 14
        EditLabel.Caption = #49688#51665#49436#48260' Port'
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        ParentCtl3D = False
        TabOrder = 3
      end
      object led3ServerID: TLabeledEdit
        Left = 124
        Top = 119
        Width = 121
        Height = 22
        Hint = 'rsetup.value04'
        Ctl3D = True
        EditLabel.Width = 56
        EditLabel.Height = 14
        EditLabel.Caption = #49688#51665#49436#48260' ID'
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        ParentCtl3D = False
        TabOrder = 4
      end
      object led3ServerPW: TLabeledEdit
        Left = 124
        Top = 145
        Width = 121
        Height = 22
        Hint = 'rsetup.value05'
        Ctl3D = True
        EditLabel.Width = 63
        EditLabel.Height = 14
        EditLabel.Caption = #49688#51665#49436#48260' PW'
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        ParentCtl3D = False
        TabOrder = 5
      end
      object led3ServerReal: TLabeledEdit
        Left = 124
        Top = 171
        Width = 206
        Height = 22
        Hint = 'rsetup.value06'
        Ctl3D = True
        EditLabel.Width = 90
        EditLabel.Height = 14
        EditLabel.Caption = #49688#51665#49436#48260' Real '#54260#45908
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        ParentCtl3D = False
        TabOrder = 6
      end
      object led3ServerPast: TLabeledEdit
        Left = 124
        Top = 197
        Width = 206
        Height = 22
        Hint = 'rsetup.value07'
        Ctl3D = True
        EditLabel.Width = 91
        EditLabel.Height = 14
        EditLabel.Caption = #49688#51665#49436#48260' Past '#54260#45908
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        ParentCtl3D = False
        TabOrder = 7
      end
      object led3RnseaDomain: TLabeledEdit
        Left = 461
        Top = 67
        Width = 121
        Height = 22
        Hint = 'rsetup.value09|text'
        Ctl3D = True
        EditLabel.Width = 102
        EditLabel.Height = 14
        EditLabel.Caption = 'Rnsea'#49436#48260' '#46020#47700#51064'/IP'
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        ParentCtl3D = False
        TabOrder = 9
      end
      object led3RnseaPort: TLabeledEdit
        Left = 461
        Top = 93
        Width = 121
        Height = 22
        Hint = 'rsetup.value10|text'
        Ctl3D = True
        EditLabel.Width = 79
        EditLabel.Height = 14
        EditLabel.Caption = 'Rnsea'#49436#48260' Port'
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        ParentCtl3D = False
        TabOrder = 10
      end
      object led3RnseaID: TLabeledEdit
        Left = 461
        Top = 119
        Width = 121
        Height = 22
        Hint = 'rsetup.value11|text'
        Ctl3D = True
        EditLabel.Width = 68
        EditLabel.Height = 14
        EditLabel.Caption = 'Rnsea'#49436#48260' ID'
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        ParentCtl3D = False
        TabOrder = 11
      end
      object led3RnseaPW: TLabeledEdit
        Left = 461
        Top = 145
        Width = 121
        Height = 22
        Hint = 'rsetup.value12'
        Ctl3D = True
        EditLabel.Width = 75
        EditLabel.Height = 14
        EditLabel.Caption = 'Rnsea'#49436#48260' PW'
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        ParentCtl3D = False
        TabOrder = 12
      end
      object led3RnseaReal: TLabeledEdit
        Left = 461
        Top = 171
        Width = 206
        Height = 22
        Hint = 'rsetup.value13'
        Ctl3D = True
        EditLabel.Width = 102
        EditLabel.Height = 14
        EditLabel.Caption = 'Rnsea'#49436#48260' Real '#54260#45908
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        ParentCtl3D = False
        TabOrder = 13
      end
      object led3RnseaPast: TLabeledEdit
        Left = 461
        Top = 197
        Width = 206
        Height = 22
        Hint = 'rsetup.value14'
        Ctl3D = True
        EditLabel.Width = 103
        EditLabel.Height = 14
        EditLabel.Caption = 'Rnsea'#49436#48260' Past '#54260#45908
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        ParentCtl3D = False
        TabOrder = 14
      end
      object cbx3RnseaSendYN: TComboBox
        Left = 461
        Top = 41
        Width = 109
        Height = 22
        Hint = 'rsetup.value08|text'
        Style = csDropDownList
        Ctl3D = True
        ImeName = 'Microsoft Office IME 2007'
        ParentCtl3D = False
        TabOrder = 8
        Items.Strings = (
          'ON'
          'OFF')
      end
      object cbx3FtpFileFormat: TComboBox
        Left = 124
        Top = 16
        Width = 109
        Height = 22
        Hint = 'rsetup.value15|text'
        Style = csDropDownList
        Ctl3D = True
        ImeName = 'Microsoft Office IME 2007'
        ParentCtl3D = False
        TabOrder = 0
        Items.Strings = (
          'ADVM'
          'UVM(Accusonic)'
          'UVM(Hydrovision)'
          'EWSV'
          'EWSV+ADVM')
      end
      object gbxVTHLogger: TGroupBox
        Left = 11
        Top = 289
        Width = 656
        Height = 143
        Ctl3D = False
        ParentCtl3D = False
        TabOrder = 15
        object Bevel3: TBevel
          Left = 332
          Top = 9
          Width = 317
          Height = 124
          Shape = bsFrame
        end
        object Label29: TLabel
          Left = 84
          Top = 29
          Width = 66
          Height = 14
          BiDiMode = bdRightToLeft
          Caption = 'Retry Count'
          ParentBiDiMode = False
        end
        object Label30: TLabel
          Left = 89
          Top = 53
          Width = 61
          Height = 14
          BiDiMode = bdRightToLeft
          Caption = 'RebootFlag'
          ParentBiDiMode = False
        end
        object Label31: TLabel
          Left = 48
          Top = 102
          Width = 102
          Height = 14
          BiDiMode = bdRightToLeft
          Caption = 'RebootRetryCount'
          ParentBiDiMode = False
        end
        object Label32: TLabel
          Left = 373
          Top = 29
          Width = 113
          Height = 14
          BiDiMode = bdRightToLeft
          Caption = 'VTHLogger Comport'
          ParentBiDiMode = False
        end
        object Label33: TLabel
          Left = 380
          Top = 53
          Width = 106
          Height = 14
          BiDiMode = bdRightToLeft
          Caption = 'VTHLogger '#53685#49888#49549#46020
          ParentBiDiMode = False
        end
        object Label43: TLabel
          Left = 39
          Top = 78
          Width = 111
          Height = 14
          BiDiMode = bdRightToLeft
          Caption = 'Max Send File Count'
          ParentBiDiMode = False
        end
        object Bevel4: TBevel
          Left = 9
          Top = 9
          Width = 317
          Height = 124
          Shape = bsFrame
        end
        object Label44: TLabel
          Left = 344
          Top = 5
          Width = 56
          Height = 14
          Caption = ' VTH '#49444#51221' '
          Color = clWindow
          ParentColor = False
          Transparent = False
        end
        object Label45: TLabel
          Left = 24
          Top = 5
          Width = 77
          Height = 14
          Caption = ' FTP '#48512#44032' '#49444#51221' '
          Color = clWindow
          ParentColor = False
          Transparent = False
        end
        object Label89: TLabel
          Left = 422
          Top = 73
          Width = 65
          Height = 14
          BiDiMode = bdRightToLeft
          Caption = 'VTH Option'
          ParentBiDiMode = False
        end
        object cbx3RetryCount: TComboBox
          Left = 153
          Top = 25
          Width = 109
          Height = 22
          Hint = 'rsetup.value01|text'
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 0
          Items.Strings = (
            '1'
            '2'
            '3'
            '4'
            '5')
        end
        object cbx3RebootFlag: TComboBox
          Left = 153
          Top = 49
          Width = 109
          Height = 22
          Hint = 'rsetup.value02|text'
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 1
          Items.Strings = (
            'ON'
            'OFF')
        end
        object cbx3RebootRetryCount: TComboBox
          Left = 153
          Top = 98
          Width = 109
          Height = 22
          Hint = 'rsetup.value04|text'
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 3
          Items.Strings = (
            '1'
            '2'
            '3'
            '4'
            '5'
            '6'
            '7'
            '8'
            '9'
            '10'
            '11'
            '12'
            '13'
            '14'
            '15'
            '16'
            '17'
            '18'
            '19'
            '20')
        end
        object cbx3VTHComport: TComboBox
          Left = 489
          Top = 25
          Width = 109
          Height = 22
          Hint = 'rsetup.value05|text'
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 4
          Items.Strings = (
            'NONE'
            'COM1'
            'COM2'
            'COM3'
            'COM4'
            'COM5'
            'COM6'
            'COM7'
            'COM8'
            'COM9'
            'COM10'
            'COM11'
            'COM12'
            'COM13'
            'COM14'
            'COM15'
            'COM16'
            'COM17'
            'COM18'
            'COM19'
            'COM20'
            'COM21'
            'COM22'
            'COM23'
            'COM24'
            'COM25'
            'COM26'
            'COM27'
            'COM28'
            'COM29'
            'COM30'
            'COM31'
            'COM32')
        end
        object cbx3VTHCommSpeed: TComboBox
          Left = 489
          Top = 49
          Width = 109
          Height = 22
          Hint = 'rsetup.value06|text'
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 5
          Items.Strings = (
            '2400'
            '4800'
            '9600'
            '14400'
            '19200'
            '28800'
            '33600'
            '38400'
            '56000')
        end
        object cbx3MaxSendFileCount: TComboBox
          Left = 153
          Top = 73
          Width = 109
          Height = 22
          Hint = 'rsetup.value03|text'
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 2
          Items.Strings = (
            '1'
            '2'
            '3'
            '4'
            '5'
            '6'
            '7'
            '8'
            '9'
            '10'
            '11'
            '12'
            '13'
            '14'
            '15'
            '16'
            '17'
            '18'
            '19'
            '20')
        end
        object cbx3VTHOption: TComboBox
          Left = 489
          Top = 73
          Width = 109
          Height = 22
          Hint = 'rsetup.value06|text'
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 6
          Items.Strings = (
            'BATTERY'
            'AIN')
        end
      end
      object cbx3FTPMode: TComboBox
        Left = 124
        Top = 225
        Width = 154
        Height = 22
        Hint = 'rsetup.value01|text'
        Style = csDropDownList
        Ctl3D = True
        ImeName = 'Microsoft Office IME 2007'
        ParentCtl3D = False
        TabOrder = 17
        Items.Strings = (
          'Active'
          'Passive')
      end
      object cbx3RnseaMode: TComboBox
        Left = 461
        Top = 225
        Width = 154
        Height = 22
        Hint = 'rsetup.value08|text'
        Style = csDropDownList
        Ctl3D = True
        ImeName = 'Microsoft Office IME 2007'
        ParentCtl3D = False
        TabOrder = 18
        Items.Strings = (
          'Active'
          'Passive')
      end
      object cbx3FTPType: TComboBox
        Left = 251
        Top = 67
        Width = 66
        Height = 22
        Hint = 'rsetup.value01|text'
        Style = csDropDownList
        Ctl3D = True
        ImeName = 'Microsoft Office IME 2007'
        ParentCtl3D = False
        TabOrder = 19
        OnChange = cbx3FTPTypeChange
        Items.Strings = (
          'FTP'
          'SFTP')
      end
      object cbx3FTPType2: TComboBox
        Left = 588
        Top = 67
        Width = 66
        Height = 22
        Hint = 'rsetup.value01|text'
        Style = csDropDownList
        Ctl3D = True
        ImeName = 'Microsoft Office IME 2007'
        ParentCtl3D = False
        TabOrder = 20
        OnChange = cbx3FTPType2Change
        Items.Strings = (
          'FTP'
          'SFTP')
      end
      object led3PrivateKey: TLabeledEdit
        Left = 124
        Top = 253
        Width = 170
        Height = 22
        Hint = 'rsetup.value06'
        Ctl3D = True
        EditLabel.Width = 78
        EditLabel.Height = 14
        EditLabel.Caption = 'PrivateKey File'
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        ParentCtl3D = False
        TabOrder = 21
        Visible = False
      end
      object btnPrivateKey: TButton
        Tag = 2
        Left = 300
        Top = 251
        Width = 30
        Height = 25
        Anchors = [akRight, akBottom]
        Caption = '...'
        TabOrder = 22
        OnClick = btnPrivateKeyClick
      end
      object led3PrivateKey2: TLabeledEdit
        Left = 461
        Top = 253
        Width = 170
        Height = 22
        Hint = 'rsetup.value06'
        Ctl3D = True
        EditLabel.Width = 78
        EditLabel.Height = 14
        EditLabel.Caption = 'PrivateKey File'
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        ParentCtl3D = False
        TabOrder = 23
        Visible = False
      end
      object btnPrivateKey2: TButton
        Tag = 2
        Left = 637
        Top = 251
        Width = 30
        Height = 25
        Anchors = [akRight, akBottom]
        Caption = '...'
        TabOrder = 24
        OnClick = btnPrivateKey2Click
      end
    end
    object TabSheet4: TTabSheet
      Caption = #44032#46300#45367
      ImageIndex = 3
      DesignSize = (
        679
        464)
      object Label2: TLabel
        Left = 49
        Top = 22
        Width = 64
        Height = 14
        BiDiMode = bdRightToLeft
        Caption = #44032#46300#45367' '#54876#49457#54868
        ParentBiDiMode = False
      end
      object Label3: TLabel
        Left = 51
        Top = 46
        Width = 62
        Height = 14
        BiDiMode = bdRightToLeft
        Caption = #44032#46300#45367' Type'
        ParentBiDiMode = False
      end
      object Label4: TLabel
        Left = 41
        Top = 70
        Width = 72
        Height = 14
        BiDiMode = bdRightToLeft
        Caption = 'PC '#48512#54021#54624' '#49884#44036
        ParentBiDiMode = False
      end
      object Label5: TLabel
        Left = 192
        Top = 70
        Width = 91
        Height = 14
        Caption = '(Normal '#47784#46300#50640#49436')'
      end
      object Label7: TLabel
        Left = 192
        Top = 95
        Width = 44
        Height = 14
        Caption = '('#45800#50948':'#52488')'
      end
      object Label8: TLabel
        Left = 32
        Top = 121
        Width = 81
        Height = 14
        BiDiMode = bdRightToLeft
        Caption = 'Ping '#49884#51089#54624' '#49884#44036
        ParentBiDiMode = False
      end
      object Label9: TLabel
        Left = 49
        Top = 171
        Width = 64
        Height = 14
        BiDiMode = bdRightToLeft
        Caption = 'Ping2 '#53580#49828#53944
        ParentBiDiMode = False
      end
      object Label10: TLabel
        Left = 32
        Top = 221
        Width = 81
        Height = 14
        BiDiMode = bdRightToLeft
        Caption = 'Ping '#51116#49884#46020' '#54943#49688
        ParentBiDiMode = False
      end
      object Label11: TLabel
        Left = 231
        Top = 221
        Width = 122
        Height = 14
        Caption = '('#49892#54056#49884' '#50672#49549' '#51116#49884#46020' '#54943#49688')'
      end
      object Label12: TLabel
        Left = 35
        Top = 246
        Width = 78
        Height = 14
        BiDiMode = bdRightToLeft
        Caption = #48516#45817' '#51116#49884#46020' '#54943#49688
        ParentBiDiMode = False
      end
      object Label13: TLabel
        Left = 231
        Top = 246
        Width = 64
        Height = 14
        Caption = '('#50672#49549' '#49892#54056#49884')'
      end
      object Label16: TLabel
        Left = 192
        Top = 270
        Width = 184
        Height = 14
        Caption = '('#45800#50948':'#48516', '#52488#44284#46104#47732' '#45796#49884' '#50976#49440#51004#47196' '#51204#54872')'
      end
      object Label17: TLabel
        Left = 35
        Top = 296
        Width = 78
        Height = 14
        BiDiMode = bdRightToLeft
        Caption = #44277#50976#44592' '#47532#49483' '#51228#54620
        ParentBiDiMode = False
      end
      object Label18: TLabel
        Left = 231
        Top = 296
        Width = 116
        Height = 14
        Caption = '('#51228#54620' '#54943#49688#47732' PC '#47532#48512#54021')'
      end
      object Label19: TLabel
        Left = 27
        Top = 321
        Width = 86
        Height = 14
        BiDiMode = bdRightToLeft
        Caption = #52572#45824' PC '#47532#49483' '#54943#49688
        ParentBiDiMode = False
      end
      object Label20: TLabel
        Left = 231
        Top = 321
        Width = 194
        Height = 14
        Caption = '('#51228#54620' '#54943#49688#47732' '#45908#51060#49345' PC '#47532#48512#54021' '#54616#51648#50506#51020')'
      end
      object Label23: TLabel
        Left = 17
        Top = 402
        Width = 93
        Height = 14
        BiDiMode = bdRightToLeft
        Caption = 'G/W '#47532#49483' '#45824#44592#49884#44036
        ParentBiDiMode = False
      end
      object sbtnSelectIF1: TSpeedButton
        Tag = 1
        Left = 459
        Top = 341
        Width = 24
        Height = 24
        Glyph.Data = {
          F6060000424DF606000000000000360000002800000018000000180000000100
          180000000000C0060000120B0000120B00000000000000000000808000808000
          8080008080008080008080008080008080008080008080008080008080008080
          0080800080800080800080800080800080800080800080800080800080800080
          8000808000808000808000808000808000808000808000808000808000808000
          808000808000808000808000808000808000808000808000BB9887B28670B58D
          79BFB7B380800080800080800080800080800080800080800080800080800080
          8000808000808000808000808000808000808000808000808000808000C18E75
          B35020A7491BAB4E22BFA1928080008080008080008080008080008080008080
          0080800080800080800080800080800080800080800080800080800080800080
          8000C2A191CA612DCA561EC4551EC37650BFB6B3808000808000808000808000
          C1521DA14418934016934016934016944017994116A14518AF4B1AFD8043BFAC
          A2BCA195BB9887BC9581C57047D45C20D45C20CD632FC4A69880800080800080
          8000808000808000E26528D0591EC3531DC3531DC3531DC8551DCF581ED25A1F
          F87D40BF9A89B77C60AD6543AA603CB4623AC75D29D45A1FCA5720C2896D8080
          00808000808000808000808000808000ED7033E26528DA5D20DA5D20DC5F22DE
          6124DF6225F57A3DC29179BD6A41B75525B9511EC1531ECC5820D75E23D45A1F
          BD5827BB927FBFB6B0808000808000808000808000808000ED7033E26528DA5D
          20DB5E21DB5E21D75C20FA7D40C79479C96D40CC5A23D65C20E16529E96F33EF
          7438EE7235DF6326BD5928B27455BCA195808000808000808000808000808000
          ED7033E26528DA5D20D95D20D1591FDF672DC7A797D18058D96228DD6124E269
          2FE57D4BE69067EB9368F58853EE7438D05B21B15B32B88C7880800080800080
          8000808000808000ED7033E26528DA5D20D45A1FC1521CFD8043C8A798DC7542
          E06428DA6126D6794C808000808000808000D6A58AEE8552E2682CBD5725B67C
          60BFACA2808000808000808000808000ED7033E26528DA5D21CE581EB34C1AFA
          7D40CE9E86E27540E06428CC602C808000808000808000808000808000DD9571
          E66F35CA5A25B9724FBFA495808000808000808000808000ED7033E26528DA5D
          21CC571EAC4919FA7D40D19B81E57943DE6226BE5E3080800080800080800080
          8000808000D2987CE26D34CD5C24BF7450C2A495808000808000808000808000
          ED7033E26528DA5D20CB571EAA4819FD8043CEA491E8804DE1672BBB59288080
          00808000808000808000808000D2835CDC642ACF5F28C47D5AC2ADA380800080
          8000808000808000F5783BEE7134E86B2EE16427C5541DCD5F29CBAB9BE38D63
          EA7237CD5A22AD5E37808000808000808000BF876AD06533D86026D16936C88E
          71808000808000808000808000808000FF8447FF8447FE8144FB7E41ED7033E3
          6629FA7D40D6A185ED8450E76E32CC5A23B3582AAF5F39B76137C75D29D86025
          DA6830D1835CC6A6968080008080008080008080008080008080008080008080
          00808000808000808000808000C8B2A6DE9B7BEE8754EC773DE2692EDC6328DD
          6328E2692FE17039D5845CC7A798808000808000808000808000808000808000
          808000808000808000808000808000808000808000808000C8B2A6D8A188E496
          70EC8959ED824EE9824FE38659D59373C8A89880800080800080800080800080
          8000808000808000C1521CA14417933F15933F15933F15944117913E158B3B14
          C5602EFC8144FC8144FC8144FB7F42FA7E41DF6C35A9481A9B4317913E15903D
          15808000808000808000808000808000E76A2DD75C20C6541DC3531DC3531DC3
          531DBF511CB24C1AA14417964016933F15933F159C4319A7471AB24C1ABE511C
          C3531DBC501BAB4919808000808000808000808000808000FD8043F87B3EEC6F
          32E86B2EE86B2EE86B2EE76A2DE4672ADF6225DC5F22DB5E21DA5D20DB5E21DE
          6124E36629E6692CE86B2EE06326CD571E808000808000808000808000808000
          808000FF884BFF8447FE8144FE8144FE8144FE8144FE8144FE8144FE8144FE81
          44FE8144FE8144FE8144FE8144FE8144FB7E41F0733680800080800080800080
          8000808000808000808000808000808000808000808000808000808000808000
          8080008080008080008080008080008080008080008080008080008080008080
          0080800080800080800080800080800080800080800080800080800080800080
          8000808000808000808000808000808000808000808000808000808000808000
          8080008080008080008080008080008080008080008080008080008080008080
          0080800080800080800080800080800080800080800080800080800080800080
          8000808000808000808000808000808000808000808000808000}
        OnClick = OnSelectIF
      end
      object sbtnSelectIF2: TSpeedButton
        Tag = 2
        Left = 459
        Top = 371
        Width = 25
        Height = 24
        Glyph.Data = {
          F6060000424DF606000000000000360000002800000018000000180000000100
          180000000000C0060000120B0000120B00000000000000000000808000808000
          8080008080008080008080008080008080008080008080008080008080008080
          0080800080800080800080800080800080800080800080800080800080800080
          8000808000808000808000808000808000808000808000808000808000808000
          808000808000808000808000808000808000808000808000BB9887B28670B58D
          79BFB7B380800080800080800080800080800080800080800080800080800080
          8000808000808000808000808000808000808000808000808000808000C18E75
          B35020A7491BAB4E22BFA1928080008080008080008080008080008080008080
          0080800080800080800080800080800080800080800080800080800080800080
          8000C2A191CA612DCA561EC4551EC37650BFB6B3808000808000808000808000
          C1521DA14418934016934016934016944017994116A14518AF4B1AFD8043BFAC
          A2BCA195BB9887BC9581C57047D45C20D45C20CD632FC4A69880800080800080
          8000808000808000E26528D0591EC3531DC3531DC3531DC8551DCF581ED25A1F
          F87D40BF9A89B77C60AD6543AA603CB4623AC75D29D45A1FCA5720C2896D8080
          00808000808000808000808000808000ED7033E26528DA5D20DA5D20DC5F22DE
          6124DF6225F57A3DC29179BD6A41B75525B9511EC1531ECC5820D75E23D45A1F
          BD5827BB927FBFB6B0808000808000808000808000808000ED7033E26528DA5D
          20DB5E21DB5E21D75C20FA7D40C79479C96D40CC5A23D65C20E16529E96F33EF
          7438EE7235DF6326BD5928B27455BCA195808000808000808000808000808000
          ED7033E26528DA5D20D95D20D1591FDF672DC7A797D18058D96228DD6124E269
          2FE57D4BE69067EB9368F58853EE7438D05B21B15B32B88C7880800080800080
          8000808000808000ED7033E26528DA5D20D45A1FC1521CFD8043C8A798DC7542
          E06428DA6126D6794C808000808000808000D6A58AEE8552E2682CBD5725B67C
          60BFACA2808000808000808000808000ED7033E26528DA5D21CE581EB34C1AFA
          7D40CE9E86E27540E06428CC602C808000808000808000808000808000DD9571
          E66F35CA5A25B9724FBFA495808000808000808000808000ED7033E26528DA5D
          21CC571EAC4919FA7D40D19B81E57943DE6226BE5E3080800080800080800080
          8000808000D2987CE26D34CD5C24BF7450C2A495808000808000808000808000
          ED7033E26528DA5D20CB571EAA4819FD8043CEA491E8804DE1672BBB59288080
          00808000808000808000808000D2835CDC642ACF5F28C47D5AC2ADA380800080
          8000808000808000F5783BEE7134E86B2EE16427C5541DCD5F29CBAB9BE38D63
          EA7237CD5A22AD5E37808000808000808000BF876AD06533D86026D16936C88E
          71808000808000808000808000808000FF8447FF8447FE8144FB7E41ED7033E3
          6629FA7D40D6A185ED8450E76E32CC5A23B3582AAF5F39B76137C75D29D86025
          DA6830D1835CC6A6968080008080008080008080008080008080008080008080
          00808000808000808000808000C8B2A6DE9B7BEE8754EC773DE2692EDC6328DD
          6328E2692FE17039D5845CC7A798808000808000808000808000808000808000
          808000808000808000808000808000808000808000808000C8B2A6D8A188E496
          70EC8959ED824EE9824FE38659D59373C8A89880800080800080800080800080
          8000808000808000C1521CA14417933F15933F15933F15944117913E158B3B14
          C5602EFC8144FC8144FC8144FB7F42FA7E41DF6C35A9481A9B4317913E15903D
          15808000808000808000808000808000E76A2DD75C20C6541DC3531DC3531DC3
          531DBF511CB24C1AA14417964016933F15933F159C4319A7471AB24C1ABE511C
          C3531DBC501BAB4919808000808000808000808000808000FD8043F87B3EEC6F
          32E86B2EE86B2EE86B2EE76A2DE4672ADF6225DC5F22DB5E21DA5D20DB5E21DE
          6124E36629E6692CE86B2EE06326CD571E808000808000808000808000808000
          808000FF884BFF8447FE8144FE8144FE8144FE8144FE8144FE8144FE8144FE81
          44FE8144FE8144FE8144FE8144FE8144FB7E41F0733680800080800080800080
          8000808000808000808000808000808000808000808000808000808000808000
          8080008080008080008080008080008080008080008080008080008080008080
          0080800080800080800080800080800080800080800080800080800080800080
          8000808000808000808000808000808000808000808000808000808000808000
          8080008080008080008080008080008080008080008080008080008080008080
          0080800080800080800080800080800080800080800080800080800080800080
          8000808000808000808000808000808000808000808000808000}
        OnClick = OnSelectIF
      end
      object btnSave3: TButton
        Tag = 3
        Left = 598
        Top = 436
        Width = 75
        Height = 25
        Anchors = [akRight, akBottom]
        Caption = #51200#51109
        TabOrder = 17
        OnClick = btnSaveClick
      end
      object cbx4GuardnetYN: TComboBox
        Left = 116
        Top = 18
        Width = 109
        Height = 22
        Hint = 'rsetup.value01|text'
        Style = csDropDownList
        Ctl3D = True
        ImeName = 'Microsoft Office IME 2007'
        ParentCtl3D = False
        TabOrder = 0
        Items.Strings = (
          'ON'
          'OFF')
      end
      object cbx4GuardnetType: TComboBox
        Left = 116
        Top = 42
        Width = 109
        Height = 22
        Hint = 'rsetup.value02|text'
        Style = csDropDownList
        Ctl3D = True
        ImeName = 'Microsoft Office IME 2007'
        ParentCtl3D = False
        TabOrder = 1
        Items.Strings = (
          'Normal'
          'Network')
      end
      object med4PCBootingTime: TMaskEdit
        Left = 116
        Top = 68
        Width = 67
        Height = 22
        Hint = 'rsetup.value03|text'
        Ctl3D = True
        EditMask = '##:##:##;1'
        ImeName = 'Microsoft Office IME 2007'
        MaxLength = 8
        ParentCtl3D = False
        TabOrder = 2
        Text = '  :  :  '
      end
      object cbx4PingStartTime: TComboBox
        Left = 116
        Top = 117
        Width = 109
        Height = 22
        Hint = 'rsetup.value05|text'
        Style = csDropDownList
        Ctl3D = True
        ImeName = 'Microsoft Office IME 2007'
        ParentCtl3D = False
        TabOrder = 4
        Items.Strings = (
          '0'
          'Current')
      end
      object led4PingServer1: TLabeledEdit
        Left = 116
        Top = 141
        Width = 109
        Height = 22
        Hint = 'rsetup.value06|text'
        Ctl3D = True
        EditLabel.Width = 91
        EditLabel.Height = 14
        EditLabel.Caption = 'Ping server1 '#51452#49548
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        ParentCtl3D = False
        TabOrder = 5
      end
      object led4Server1Port: TLabeledEdit
        Left = 340
        Top = 141
        Width = 67
        Height = 22
        Hint = 'rsetup.value07|text'
        Ctl3D = True
        EditLabel.Width = 23
        EditLabel.Height = 14
        EditLabel.Caption = 'Port'
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        NumbersOnly = True
        ParentCtl3D = False
        TabOrder = 6
      end
      object led4Server2: TLabeledEdit
        Left = 116
        Top = 192
        Width = 109
        Height = 22
        Hint = 'rsetup.value09|text'
        Ctl3D = True
        EditLabel.Width = 91
        EditLabel.Height = 14
        EditLabel.Caption = 'Ping server2 '#51452#49548
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        ParentCtl3D = False
        TabOrder = 8
      end
      object led4Server2Port: TLabeledEdit
        Left = 340
        Top = 192
        Width = 67
        Height = 22
        Hint = 'rsetup.value10|text'
        Ctl3D = True
        EditLabel.Width = 23
        EditLabel.Height = 14
        EditLabel.Caption = 'Port'
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        NumbersOnly = True
        ParentCtl3D = False
        TabOrder = 9
      end
      object cbx4Ping2TestYN: TComboBox
        Left = 116
        Top = 167
        Width = 109
        Height = 22
        Hint = 'rsetup.value08|text'
        Style = csDropDownList
        Ctl3D = True
        ImeName = 'Microsoft Office IME 2007'
        ParentCtl3D = False
        TabOrder = 7
        Items.Strings = (
          'On'
          'Off')
      end
      object cbx4PingRetryCount: TComboBox
        Left = 116
        Top = 217
        Width = 109
        Height = 22
        Hint = 'rsetup.value11|text'
        Style = csDropDownList
        Ctl3D = True
        ImeName = 'Microsoft Office IME 2007'
        ParentCtl3D = False
        TabOrder = 10
        Items.Strings = (
          '1'
          '2'
          '3'
          '4'
          '5')
      end
      object cbx4PingRetryCountM: TComboBox
        Left = 116
        Top = 242
        Width = 109
        Height = 22
        Hint = 'rsetup.value12|text'
        Style = csDropDownList
        Ctl3D = True
        ImeName = 'Microsoft Office IME 2007'
        ParentCtl3D = False
        TabOrder = 11
        Items.Strings = (
          '1'
          '2'
          '3'
          '4'
          '5')
      end
      object cbx4GWResetCount: TComboBox
        Left = 116
        Top = 292
        Width = 109
        Height = 22
        Hint = 'rsetup.value14|text'
        Style = csDropDownList
        Ctl3D = True
        ImeName = 'Microsoft Office IME 2007'
        ParentCtl3D = False
        TabOrder = 12
        Items.Strings = (
          '1'
          '2'
          '3'
          '4'
          '5')
      end
      object cbx4MaxPCResetCount: TComboBox
        Left = 116
        Top = 316
        Width = 109
        Height = 22
        Hint = 'rsetup.value15|text'
        Style = csDropDownList
        Ctl3D = True
        ImeName = 'Microsoft Office IME 2007'
        ParentCtl3D = False
        TabOrder = 13
        Items.Strings = (
          '1'
          '2'
          '3'
          '4'
          '5'
          '6'
          '7'
          '8'
          '9'
          '10')
      end
      object cbx4GWResetDelay: TComboBox
        Left = 116
        Top = 399
        Width = 109
        Height = 22
        Hint = 'rsetup.value18|text'
        Style = csDropDownList
        Ctl3D = True
        ImeName = 'Microsoft Office IME 2007'
        ParentCtl3D = False
        TabOrder = 16
        Items.Strings = (
          '1'
          '2'
          '3'
          '4'
          '5')
      end
      object led4Interval: TLabeledEdit
        Left = 116
        Top = 92
        Width = 67
        Height = 22
        Hint = 'rsetup.value04|text'
        Ctl3D = True
        EditLabel.Width = 41
        EditLabel.Height = 14
        EditLabel.Caption = 'Interval'
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        NumbersOnly = True
        ParentCtl3D = False
        TabOrder = 3
      end
      object led4WirelessUseTime: TLabeledEdit
        Left = 116
        Top = 267
        Width = 67
        Height = 22
        Hint = 'rsetup.value13|text'
        Ctl3D = True
        EditLabel.Width = 64
        EditLabel.Height = 14
        EditLabel.Caption = #47924#49440' '#49324#50857#49884#44036
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 3
        NumbersOnly = True
        ParentCtl3D = False
        TabOrder = 18
      end
      object led4LineIF: TLabeledEdit
        Left = 116
        Top = 343
        Width = 337
        Height = 22
        Hint = 'rsetup.value16|text'
        Ctl3D = True
        EditLabel.Width = 42
        EditLabel.Height = 14
        EditLabel.Caption = 'WiredIF'
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        NumbersOnly = True
        ParentCtl3D = False
        TabOrder = 14
      end
      object led4WirelessIF: TLabeledEdit
        Left = 116
        Top = 371
        Width = 337
        Height = 22
        Hint = 'rsetup.value17|text'
        Ctl3D = True
        EditLabel.Width = 54
        EditLabel.Height = 14
        EditLabel.Caption = 'WirelessIF'
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        NumbersOnly = True
        ParentCtl3D = False
        TabOrder = 15
      end
      object pnlIF: TPanel
        Left = 376
        Top = 49
        Width = 425
        Height = 70
        BorderWidth = 2
        Caption = 'pnlIF'
        Color = clBlue
        ParentBackground = False
        TabOrder = 19
        Visible = False
        object lbxIF: TListBox
          Left = 3
          Top = 3
          Width = 419
          Height = 64
          Align = alClient
          ImeName = 'Microsoft Office IME 2007'
          ItemHeight = 14
          Items.Strings = (
            '1'
            '2')
          TabOrder = 0
          OnClick = lbxIFClick
          OnExit = lbxIFExit
        end
      end
    end
    object TabSheet5: TTabSheet
      Caption = #48624#50612
      ImageIndex = 4
      DesignSize = (
        679
        464)
      object Label61: TLabel
        Left = 23
        Top = 99
        Width = 74
        Height = 14
        BiDiMode = bdRightToLeft
        Caption = #47196#44536#51064' '#45824#44592#49884#44036
        ParentBiDiMode = False
      end
      object Label62: TLabel
        Left = 213
        Top = 99
        Width = 20
        Height = 14
        BiDiMode = bdRightToLeft
        Caption = '('#48516')'
        ParentBiDiMode = False
      end
      object btnSave4: TButton
        Tag = 4
        Left = 598
        Top = 436
        Width = 75
        Height = 25
        Anchors = [akRight, akBottom]
        Caption = #51200#51109
        TabOrder = 6
        OnClick = btnSaveClick
      end
      object lel5Ment: TLabeledEdit
        Left = 100
        Top = 17
        Width = 397
        Height = 22
        Hint = 'rsetup.value01'
        Ctl3D = True
        EditLabel.Width = 64
        EditLabel.Height = 14
        EditLabel.Caption = #50976#47049#49328#52636' '#47704#53944
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        ParentCtl3D = False
        TabOrder = 0
      end
      object led5LevelOverSea: TLabeledEdit
        Left = 100
        Top = 43
        Width = 61
        Height = 22
        Hint = 'rsetup.value03'
        Ctl3D = True
        EditLabel.Width = 60
        EditLabel.Height = 14
        EditLabel.Caption = #54644#48156#54364#44256'(m)'
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        NumbersOnly = True
        ParentCtl3D = False
        TabOrder = 1
      end
      object gbxGraph: TGroupBox
        Left = 3
        Top = 276
        Width = 330
        Height = 145
        Caption = #44592#48376' '#44536#47000#54532#52285' '#49444#51221
        TabOrder = 5
        object cbxWaterFlow: TCheckBox
          Left = 11
          Top = 23
          Width = 97
          Height = 17
          Hint = 'rsetup.value01|ON,OFF'
          Alignment = taLeftJustify
          Caption = '   '#50976#47049' '#44536#47000#54532
          TabOrder = 0
        end
        object cbxWaterLevel: TCheckBox
          Left = 11
          Top = 46
          Width = 97
          Height = 17
          Hint = 'rsetup.value02|ON,OFF'
          Alignment = taLeftJustify
          Caption = #49688#50948#44228' '#44536#47000#54532
          Enabled = False
          TabOrder = 1
        end
        object cbxHydroMeter1: TCheckBox
          Left = 11
          Top = 70
          Width = 97
          Height = 17
          Hint = 'rsetup.value03|ON,OFF'
          Alignment = taLeftJustify
          Caption = #50976#49549#44228' '#44536#47000#54532
          Enabled = False
          TabOrder = 2
        end
        object cbxLog: TCheckBox
          Left = 11
          Top = 94
          Width = 97
          Height = 17
          Hint = 'rsetup.value04|ON,OFF'
          Alignment = taLeftJustify
          Caption = '       '#47196#44536#54868#47732
          TabOrder = 3
        end
        object cbxVthLogger: TCheckBox
          Left = 11
          Top = 118
          Width = 97
          Height = 17
          Hint = 'rsetup.value05|ON,OFF'
          Alignment = taLeftJustify
          Caption = '   VTH Logger'
          TabOrder = 4
        end
      end
      object gbxEmail: TGroupBox
        Left = 5
        Top = 124
        Width = 668
        Height = 146
        Caption = #51216#44160#51068#51648' '#51060#47700#51068' '#49444#51221
        TabOrder = 4
        object Label48: TLabel
          Left = 52
          Top = 24
          Width = 40
          Height = 14
          BiDiMode = bdRightToLeft
          Caption = #51088#46041#51204#49569
          ParentBiDiMode = False
        end
        object cbxEmailAutoSend: TComboBox
          Left = 95
          Top = 20
          Width = 110
          Height = 22
          Hint = 'rsetup.value01|text'
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 0
          Items.Strings = (
            'ON'
            'OFF')
        end
        object ledEmailServer: TLabeledEdit
          Left = 95
          Top = 44
          Width = 216
          Height = 22
          Hint = 'rsetup.value02'
          Color = clBtnFace
          Ctl3D = True
          EditLabel.Width = 54
          EditLabel.Height = 14
          EditLabel.Caption = #51060#47700#51068' '#49436#48260
          ImeName = 'Microsoft Office IME 2007'
          LabelPosition = lpLeft
          MaxLength = 100
          ParentCtl3D = False
          TabOrder = 1
          Text = 'mail.rnsea.com'
        end
        object ledEmailAccount: TLabeledEdit
          Left = 95
          Top = 68
          Width = 149
          Height = 20
          Hint = 'rsetup.value03'
          TabStop = False
          Color = clBtnFace
          Ctl3D = True
          EditLabel.Width = 46
          EditLabel.Height = 14
          EditLabel.Caption = 'Account'
          Font.Charset = HANGEUL_CHARSET
          Font.Color = clRed
          Font.Height = -12
          Font.Name = #44404#47548#52404
          Font.Style = []
          ImeName = 'Microsoft Office IME 2007'
          LabelPosition = lpLeft
          MaxLength = 100
          ParentCtl3D = False
          ParentFont = False
          ReadOnly = True
          TabOrder = 2
          Text = 'rads@rnsea.com'
        end
        object ledEmailPassword: TLabeledEdit
          Left = 95
          Top = 92
          Width = 149
          Height = 20
          Hint = 'rsetup.value04'
          TabStop = False
          Color = clBtnFace
          Ctl3D = True
          EditLabel.Width = 51
          EditLabel.Height = 14
          EditLabel.Caption = 'Password'
          Font.Charset = HANGEUL_CHARSET
          Font.Color = clRed
          Font.Height = -12
          Font.Name = #44404#47548#52404
          Font.Style = []
          ImeName = 'Microsoft Office IME 2007'
          LabelPosition = lpLeft
          MaxLength = 100
          ParentCtl3D = False
          ParentFont = False
          PasswordChar = '*'
          ReadOnly = True
          TabOrder = 3
          Text = 'rads2014'
        end
        object ledEmailDest1: TLabeledEdit
          Left = 95
          Top = 116
          Width = 216
          Height = 20
          Hint = 'rsetup.value05'
          TabStop = False
          Color = clBtnFace
          Ctl3D = True
          EditLabel.Width = 37
          EditLabel.Height = 14
          EditLabel.Caption = #49688#49888#51088'1'
          Font.Charset = HANGEUL_CHARSET
          Font.Color = clRed
          Font.Height = -12
          Font.Name = #44404#47548#52404
          Font.Style = []
          ImeName = 'Microsoft Office IME 2007'
          LabelPosition = lpLeft
          MaxLength = 100
          ParentCtl3D = False
          ParentFont = False
          ReadOnly = True
          TabOrder = 4
          Text = 'songdeukbo@gmail.com'
        end
        object ledEmailDest2: TLabeledEdit
          Left = 420
          Top = 44
          Width = 216
          Height = 22
          Hint = 'rsetup.value06'
          Ctl3D = True
          EditLabel.Width = 37
          EditLabel.Height = 14
          EditLabel.Caption = #49688#49888#51088'2'
          ImeName = 'Microsoft Office IME 2007'
          LabelPosition = lpLeft
          MaxLength = 100
          ParentCtl3D = False
          TabOrder = 5
          Text = 'syiware@hotmail.com'
        end
        object ledEmailDest3: TLabeledEdit
          Left = 420
          Top = 68
          Width = 216
          Height = 22
          Hint = 'rsetup.value07'
          Ctl3D = True
          EditLabel.Width = 37
          EditLabel.Height = 14
          EditLabel.Caption = #49688#49888#51088'3'
          ImeName = 'Microsoft Office IME 2007'
          LabelPosition = lpLeft
          MaxLength = 100
          ParentCtl3D = False
          TabOrder = 6
        end
        object ledEmailDest4: TLabeledEdit
          Left = 420
          Top = 92
          Width = 216
          Height = 22
          Hint = 'rsetup.value08'
          Ctl3D = True
          EditLabel.Width = 37
          EditLabel.Height = 14
          EditLabel.Caption = #49688#49888#51088'4'
          ImeName = 'Microsoft Office IME 2007'
          LabelPosition = lpLeft
          MaxLength = 100
          ParentCtl3D = False
          TabOrder = 7
        end
        object ledEmailDest5: TLabeledEdit
          Left = 420
          Top = 116
          Width = 216
          Height = 22
          Hint = 'rsetup.value09'
          Ctl3D = True
          EditLabel.Width = 37
          EditLabel.Height = 14
          EditLabel.Caption = #49688#49888#51088'5'
          ImeName = 'Microsoft Office IME 2007'
          LabelPosition = lpLeft
          MaxLength = 100
          ParentCtl3D = False
          TabOrder = 8
        end
      end
      object led5CctvUrl: TLabeledEdit
        Left = 100
        Top = 69
        Width = 397
        Height = 22
        Hint = 'rsetup.value04'
        Ctl3D = True
        EditLabel.Width = 55
        EditLabel.Height = 14
        EditLabel.Caption = 'CCTV URL'
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        ParentCtl3D = False
        TabOrder = 2
      end
      object cbx5LoginDelayTime: TComboBox
        Left = 100
        Top = 96
        Width = 107
        Height = 22
        Style = csDropDownList
        Ctl3D = True
        DropDownCount = 30
        ImeName = 'Microsoft Office IME 2007'
        ParentCtl3D = False
        TabOrder = 3
        Items.Strings = (
          '1'
          '2'
          '3'
          '4'
          '5'
          '6'
          '7'
          '8'
          '9'
          '10'
          '11'
          '12'
          '13'
          '14'
          '15'
          '16'
          '17'
          '18'
          '19'
          '20'
          '21'
          '22'
          '23'
          '24'
          '25'
          '26'
          '27'
          '28'
          '29'
          '30'
          '31'
          '32'
          '33'
          '34'
          '35'
          '36'
          '37'
          '38'
          '39'
          '40'
          '41'
          '42'
          '43'
          '44'
          '45'
          '46'
          '47'
          '48'
          '49'
          '50'
          '51'
          '52'
          '53'
          '54'
          '55'
          '56'
          '57'
          '58'
          '59'
          '60'
          '61'
          '62'
          '63'
          '64'
          '65'
          '66'
          '67'
          '68'
          '69'
          '70'
          '71'
          '72'
          '73'
          '74'
          '75'
          '76'
          '77'
          '78'
          '79'
          '80'
          '81'
          '82'
          '83'
          '84'
          '85'
          '86'
          '87'
          '88'
          '89'
          '90'
          '91'
          '92'
          '93'
          '94'
          '95'
          '96'
          '97'
          '98'
          '99'
          '100'
          '101'
          '102'
          '103'
          '104'
          '105'
          '106'
          '107'
          '108'
          '109'
          '110'
          '111'
          '112'
          '113'
          '114'
          '115'
          '116'
          '117'
          '118'
          '119'
          '120')
      end
    end
    object TabSheet6: TTabSheet
      Caption = #50976#54952#48276#50948
      ImageIndex = 5
      DesignSize = (
        679
        464)
      object btnSave5: TButton
        Tag = 5
        Left = 598
        Top = 436
        Width = 75
        Height = 25
        Anchors = [akRight, akBottom]
        Caption = #51200#51109
        TabOrder = 0
        OnClick = btnSaveClick
      end
      object GroupBox1: TGroupBox
        Left = 3
        Top = 15
        Width = 333
        Height = 201
        Caption = 'VTHLogger'
        TabOrder = 1
        object Label63: TLabel
          Left = 33
          Top = 30
          Width = 81
          Height = 14
          Alignment = taRightJustify
          BiDiMode = bdLeftToRight
          Caption = #51077#47141#51204#50517'(AC,V):'
          ParentBiDiMode = False
        end
        object Label64: TLabel
          Left = 33
          Top = 58
          Width = 81
          Height = 14
          Alignment = taRightJustify
          BiDiMode = bdLeftToRight
          Caption = #52649#51204#51204#50517'(DC,V):'
          ParentBiDiMode = False
        end
        object Label65: TLabel
          Left = 23
          Top = 86
          Width = 91
          Height = 14
          Alignment = taRightJustify
          BiDiMode = bdLeftToRight
          Caption = #48176#53552#47532#51204#50517'(DC,V):'
          ParentBiDiMode = False
        end
        object Label66: TLabel
          Left = 40
          Top = 114
          Width = 74
          Height = 14
          Alignment = taRightJustify
          BiDiMode = bdLeftToRight
          Caption = #49884#49828#53596#50728#46020'('#8216'C):'
          ParentBiDiMode = False
        end
        object Label67: TLabel
          Left = 38
          Top = 142
          Width = 76
          Height = 14
          Alignment = taRightJustify
          BiDiMode = bdLeftToRight
          Caption = #49884#49828#53596#49845#46020'(%):'
          ParentBiDiMode = False
        end
        object Label68: TLabel
          Left = 208
          Top = 30
          Width = 17
          Height = 14
          Caption = ' ~ '
        end
        object Label69: TLabel
          Left = 208
          Top = 58
          Width = 17
          Height = 14
          Caption = ' ~ '
        end
        object Label70: TLabel
          Left = 208
          Top = 86
          Width = 17
          Height = 14
          Caption = ' ~ '
        end
        object Label71: TLabel
          Left = 208
          Top = 114
          Width = 17
          Height = 14
          Caption = ' ~ '
        end
        object Label72: TLabel
          Left = 208
          Top = 142
          Width = 17
          Height = 14
          Caption = ' ~ '
        end
        object ed6VoltMin: TEdit
          Left = 120
          Top = 27
          Width = 81
          Height = 22
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 0
        end
        object ed6ChargeMin: TEdit
          Left = 120
          Top = 55
          Width = 81
          Height = 22
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 2
        end
        object ed6ChargeMax: TEdit
          Left = 231
          Top = 55
          Width = 81
          Height = 22
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 3
        end
        object ed6BatteryMin: TEdit
          Left = 120
          Top = 83
          Width = 81
          Height = 22
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 4
        end
        object ed6BatteryMax: TEdit
          Left = 232
          Top = 83
          Width = 81
          Height = 22
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 5
        end
        object ed6SysTempMin: TEdit
          Left = 120
          Top = 111
          Width = 81
          Height = 22
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 6
        end
        object ed6SysTempMax: TEdit
          Left = 232
          Top = 111
          Width = 81
          Height = 22
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 7
        end
        object ed6SysHumidMin: TEdit
          Left = 120
          Top = 139
          Width = 81
          Height = 22
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 8
        end
        object ed6SysHumidMax: TEdit
          Left = 232
          Top = 139
          Width = 81
          Height = 22
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 9
        end
        object ed6VoltMax: TEdit
          Left = 231
          Top = 27
          Width = 81
          Height = 22
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 1
        end
      end
      object GroupBox2: TGroupBox
        Left = 342
        Top = 16
        Width = 333
        Height = 201
        Caption = #50976#49549#44228
        TabOrder = 2
        object Label73: TLabel
          Left = 70
          Top = 30
          Width = 44
          Height = 14
          Alignment = taRightJustify
          BiDiMode = bdLeftToRight
          Caption = #49688#50728'('#8216'C):'
          ParentBiDiMode = False
        end
        object Label74: TLabel
          Left = 70
          Top = 58
          Width = 44
          Height = 14
          Alignment = taRightJustify
          BiDiMode = bdLeftToRight
          Caption = #49688#50517'(m):'
          ParentBiDiMode = False
        end
        object Label75: TLabel
          Left = 77
          Top = 86
          Width = 37
          Height = 14
          Alignment = taRightJustify
          BiDiMode = bdLeftToRight
          Caption = #54588#52824'('#8216'):'
          ParentBiDiMode = False
        end
        object Label76: TLabel
          Left = 87
          Top = 114
          Width = 27
          Height = 14
          Alignment = taRightJustify
          BiDiMode = bdLeftToRight
          Caption = #47204'('#8216'):'
          ParentBiDiMode = False
        end
        object Label77: TLabel
          Left = 41
          Top = 142
          Width = 73
          Height = 14
          Alignment = taRightJustify
          BiDiMode = bdLeftToRight
          Caption = #50976#49549'EW(m/s):'
          ParentBiDiMode = False
        end
        object Label78: TLabel
          Left = 208
          Top = 30
          Width = 17
          Height = 14
          Caption = ' ~ '
        end
        object Label79: TLabel
          Left = 208
          Top = 58
          Width = 17
          Height = 14
          Caption = ' ~ '
        end
        object Label80: TLabel
          Left = 208
          Top = 86
          Width = 17
          Height = 14
          Caption = ' ~ '
        end
        object Label81: TLabel
          Left = 208
          Top = 114
          Width = 17
          Height = 14
          Caption = ' ~ '
        end
        object Label82: TLabel
          Left = 208
          Top = 142
          Width = 17
          Height = 14
          Caption = ' ~ '
        end
        object Label87: TLabel
          Left = 45
          Top = 170
          Width = 69
          Height = 14
          Alignment = taRightJustify
          BiDiMode = bdLeftToRight
          Caption = #50976#49549'NS(m/s):'
          ParentBiDiMode = False
        end
        object Label88: TLabel
          Left = 208
          Top = 170
          Width = 17
          Height = 14
          Caption = ' ~ '
        end
        object ed6TempMin: TEdit
          Left = 120
          Top = 27
          Width = 81
          Height = 22
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 0
        end
        object ed6PressureMin: TEdit
          Left = 120
          Top = 55
          Width = 81
          Height = 22
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 2
        end
        object ed6PressureMax: TEdit
          Left = 231
          Top = 55
          Width = 81
          Height = 22
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 3
        end
        object ed6PitchMin: TEdit
          Left = 120
          Top = 83
          Width = 81
          Height = 22
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 4
        end
        object ed6PitchMax: TEdit
          Left = 232
          Top = 83
          Width = 81
          Height = 22
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 5
        end
        object ed6RollMin: TEdit
          Left = 120
          Top = 111
          Width = 81
          Height = 22
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 6
        end
        object ed6RollMax: TEdit
          Left = 232
          Top = 111
          Width = 81
          Height = 22
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 7
        end
        object ed6VelocityEwMin: TEdit
          Left = 120
          Top = 139
          Width = 81
          Height = 22
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 8
        end
        object ed6VelocityEwMax: TEdit
          Left = 232
          Top = 139
          Width = 81
          Height = 22
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 9
        end
        object ed6TempMax: TEdit
          Left = 231
          Top = 27
          Width = 81
          Height = 22
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 1
        end
        object ed6VelocityNsMin: TEdit
          Left = 120
          Top = 167
          Width = 81
          Height = 22
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 10
        end
        object ed6VelocityNsMax: TEdit
          Left = 232
          Top = 167
          Width = 81
          Height = 22
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 11
        end
      end
      object GroupBox3: TGroupBox
        Left = 3
        Top = 222
        Width = 333
        Height = 201
        Caption = #49688#50948#44228
        TabOrder = 3
        object Label83: TLabel
          Left = 70
          Top = 30
          Width = 44
          Height = 14
          Alignment = taRightJustify
          BiDiMode = bdLeftToRight
          Caption = #49688#50948'(m):'
          ParentBiDiMode = False
        end
        object Label84: TLabel
          Left = 61
          Top = 58
          Width = 53
          Height = 14
          Alignment = taRightJustify
          BiDiMode = bdLeftToRight
          Caption = #50684#46020'(ppt):'
          ParentBiDiMode = False
        end
        object Label85: TLabel
          Left = 208
          Top = 30
          Width = 17
          Height = 14
          Caption = ' ~ '
        end
        object Label86: TLabel
          Left = 208
          Top = 58
          Width = 17
          Height = 14
          Caption = ' ~ '
        end
        object ed6WaterLevelMin: TEdit
          Left = 120
          Top = 27
          Width = 81
          Height = 22
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 0
        end
        object ed6SalinityMin: TEdit
          Left = 120
          Top = 55
          Width = 81
          Height = 22
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 2
        end
        object ed6SalinityMax: TEdit
          Left = 231
          Top = 55
          Width = 81
          Height = 22
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 3
        end
        object ed6WaterLevelMax: TEdit
          Left = 231
          Top = 27
          Width = 81
          Height = 22
          ImeName = 'Microsoft Office IME 2007'
          TabOrder = 1
        end
      end
    end
    object TabSheet7: TTabSheet
      Caption = #54260#45908'/'#54252#53944
      ImageIndex = 6
      DesignSize = (
        679
        464)
      object SpeedButton1: TSpeedButton
        Tag = 1
        Left = 526
        Top = 18
        Width = 23
        Height = 20
        Caption = '...'
        OnClick = OnFolderHelpClick
      end
      object SpeedButton2: TSpeedButton
        Tag = 2
        Left = 526
        Top = 43
        Width = 23
        Height = 20
        Caption = '...'
        OnClick = OnFolderHelpClick
      end
      object SpeedButton3: TSpeedButton
        Tag = 3
        Left = 526
        Top = 69
        Width = 23
        Height = 20
        Caption = '...'
        OnClick = OnFolderHelpClick
      end
      object SpeedButton4: TSpeedButton
        Tag = 4
        Left = 526
        Top = 94
        Width = 23
        Height = 20
        Caption = '...'
        OnClick = OnFolderHelpClick
      end
      object Label51: TLabel
        Left = 189
        Top = 225
        Width = 108
        Height = 14
        BiDiMode = bdLeftToRight
        Caption = '(FROM DataLogger)'
        ParentBiDiMode = False
      end
      object Label50: TLabel
        Left = 189
        Top = 251
        Width = 108
        Height = 14
        BiDiMode = bdLeftToRight
        Caption = '(FROM DataViewer)'
        ParentBiDiMode = False
      end
      object Label52: TLabel
        Left = 189
        Top = 277
        Width = 127
        Height = 14
        BiDiMode = bdLeftToRight
        Caption = '(FROM Logger/Sender)'
        ParentBiDiMode = False
      end
      object Label90: TLabel
        Left = 312
        Top = 128
        Width = 115
        Height = 14
        BiDiMode = bdLeftToRight
        Caption = #51116#51204#49569'(ADVM, EWSV)'
        ParentBiDiMode = False
      end
      object Label91: TLabel
        Left = 486
        Top = 153
        Width = 9
        Height = 14
        Caption = '~'
      end
      object btnSave6: TButton
        Tag = 6
        Left = 598
        Top = 436
        Width = 75
        Height = 25
        Anchors = [akRight, akBottom]
        Caption = #51200#51109
        TabOrder = 11
        OnClick = btnSaveClick
      end
      object ledHome: TLabeledEdit
        Left = 116
        Top = 18
        Width = 409
        Height = 22
        Hint = 'rsetup.value01'
        Ctl3D = True
        EditLabel.Width = 68
        EditLabel.Height = 14
        EditLabel.Caption = 'RADS '#54856' '#54260#45908
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        ParentCtl3D = False
        TabOrder = 0
      end
      object ledLogger: TLabeledEdit
        Left = 116
        Top = 43
        Width = 409
        Height = 22
        Hint = 'rsetup.value02'
        Ctl3D = True
        EditLabel.Width = 77
        EditLabel.Height = 14
        EditLabel.Caption = 'DataLogger '#54856
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        ParentCtl3D = False
        TabOrder = 1
      end
      object ledSender: TLabeledEdit
        Left = 116
        Top = 69
        Width = 409
        Height = 22
        Hint = 'rsetup.value03'
        Ctl3D = True
        EditLabel.Width = 78
        EditLabel.Height = 14
        EditLabel.Caption = 'DataSender '#54856
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        ParentCtl3D = False
        TabOrder = 2
      end
      object ledViewer: TLabeledEdit
        Left = 116
        Top = 94
        Width = 409
        Height = 22
        Hint = 'rsetup.value04'
        Ctl3D = True
        EditLabel.Width = 77
        EditLabel.Height = 14
        EditLabel.Caption = 'DataViewer '#54856
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        ParentCtl3D = False
        TabOrder = 3
      end
      object ledDataLoggerIP: TLabeledEdit
        Left = 116
        Top = 120
        Width = 121
        Height = 22
        Hint = 'rsetup.value04'
        Ctl3D = True
        EditLabel.Width = 78
        EditLabel.Height = 14
        EditLabel.Caption = 'DataLogger IP'
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        ParentCtl3D = False
        TabOrder = 4
      end
      object ledDataSenderIP: TLabeledEdit
        Left = 116
        Top = 146
        Width = 121
        Height = 22
        Hint = 'rsetup.value04'
        Ctl3D = True
        EditLabel.Width = 79
        EditLabel.Height = 14
        EditLabel.Caption = 'DataSender IP'
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        ParentCtl3D = False
        TabOrder = 5
      end
      object ledFirebirdIP: TLabeledEdit
        Left = 116
        Top = 171
        Width = 121
        Height = 22
        Hint = 'rsetup.value04'
        Ctl3D = True
        EditLabel.Width = 73
        EditLabel.Height = 14
        EditLabel.Caption = 'Firebird DB IP'
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        ParentCtl3D = False
        TabOrder = 6
      end
      object ledDataLoggerPort: TLabeledEdit
        Left = 116
        Top = 197
        Width = 67
        Height = 22
        Hint = 'rsetup.value04'
        Ctl3D = True
        EditLabel.Width = 90
        EditLabel.Height = 14
        EditLabel.Caption = 'DataLogger Port'
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        NumbersOnly = True
        ParentCtl3D = False
        TabOrder = 7
      end
      object ledDataSenderPortFromLogger: TLabeledEdit
        Left = 116
        Top = 222
        Width = 67
        Height = 22
        Hint = 'rsetup.value04'
        Ctl3D = True
        EditLabel.Width = 91
        EditLabel.Height = 14
        EditLabel.Caption = 'DataSender Port'
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        NumbersOnly = True
        ParentCtl3D = False
        TabOrder = 8
      end
      object ledDataSenderPortFromViewer: TLabeledEdit
        Left = 116
        Top = 248
        Width = 67
        Height = 22
        Hint = 'rsetup.value04'
        Ctl3D = True
        EditLabel.Width = 91
        EditLabel.Height = 14
        EditLabel.Caption = 'DataSender Port'
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        NumbersOnly = True
        ParentCtl3D = False
        TabOrder = 9
      end
      object ledWatchDogPort: TLabeledEdit
        Left = 116
        Top = 274
        Width = 67
        Height = 22
        Hint = 'rsetup.value04'
        Ctl3D = True
        EditLabel.Width = 85
        EditLabel.Height = 14
        EditLabel.Caption = 'WatchDog Port'
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        NumbersOnly = True
        ParentCtl3D = False
        TabOrder = 10
      end
      object MemoPingLog: TMemo
        Left = 116
        Top = 301
        Width = 352
        Height = 140
        Color = clBlack
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clYellow
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ImeName = 'Microsoft IME 2010'
        ParentFont = False
        ScrollBars = ssVertical
        TabOrder = 12
      end
      object ButtonPing: TButton
        Tag = 5
        Left = 474
        Top = 299
        Width = 143
        Height = 25
        Anchors = [akRight, akBottom]
        Caption = 'Ping Test(DataLogger)'
        TabOrder = 13
        OnClick = ButtonPingClick
      end
      object ButtonReboot: TButton
        Tag = 5
        Left = 474
        Top = 330
        Width = 143
        Height = 25
        Anchors = [akRight, akBottom]
        Caption = 'DataLogger PC Reboot'
        TabOrder = 14
        OnClick = ButtonRebootClick
      end
      object ReSendFromDate: TDateTimePicker
        Left = 312
        Top = 150
        Width = 102
        Height = 22
        Hint = #51116#51204#49569#44592#44036'(FromDate)'
        Date = 46066.419907337960000000
        Time = 46066.419907337960000000
        ParentShowHint = False
        ShowHint = True
        TabOrder = 15
      end
      object ReSendToDate: TDateTimePicker
        Left = 501
        Top = 150
        Width = 102
        Height = 22
        Hint = #51116#51204#49569#44592#44036'(ToDate)'
        Date = 46066.419907337960000000
        Time = 46066.419907337960000000
        ParentShowHint = False
        ShowHint = True
        TabOrder = 16
      end
      object btnReSend: TButton
        Left = 435
        Top = 124
        Width = 114
        Height = 25
        Hint = #51116#51204#49569#52376#47532#54616#47732' '#51060#48120' '#51204#49569#46108' adv(ewsv) '#54028#51068#51012' DataSender'#51032' '#51204#49569' '#49828#52992#51572#50640' '#46384#46972' '#45796#49884' '#51204#49569#54633#45768#45796'.'
        Caption = #51116#51204#49569' '#52376#47532
        ParentShowHint = False
        ShowHint = True
        TabOrder = 17
        OnClick = btnReSendClick
      end
      object ReSendFromTime: TMaskEdit
        Left = 416
        Top = 150
        Width = 64
        Height = 22
        Hint = #51116#51204#49569#44592#44036'(FromTime)(00:00:00~23:50:00)'
        EditMask = '!90:00:00;1;_'
        MaxLength = 8
        ParentShowHint = False
        ShowHint = True
        TabOrder = 18
        Text = '  :  :  '
      end
      object ReSendToTime: TMaskEdit
        Left = 605
        Top = 150
        Width = 64
        Height = 22
        Hint = #51116#51204#49569#44592#44036'(ToTime)(00:00:00~23:50:00)'
        EditMask = '!90:00:00;1;_'
        MaxLength = 8
        ParentShowHint = False
        ShowHint = True
        TabOrder = 19
        Text = '  :  :  '
      end
    end
  end
  object btnClose: TButton
    Left = 606
    Top = 499
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = #45803#44592
    TabOrder = 1
    OnClick = btnCloseClick
  end
  object SimpleDataSet: TSimpleDataSet
    Aggregates = <>
    DataSet.MaxBlobSize = -1
    DataSet.Params = <>
    Params = <>
    Left = 20
    Top = 481
  end
  object TimerPing: TTimer
    Enabled = False
    Interval = 2000
    OnTimer = TimerPingTimer
    Left = 136
    Top = 480
  end
  object OpenDialog: TOpenDialog
    Left = 80
    Top = 480
  end
end
