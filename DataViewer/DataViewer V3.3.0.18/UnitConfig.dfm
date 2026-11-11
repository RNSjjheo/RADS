object FormConfig: TFormConfig
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = #54872#44221#49444#51221
  ClientHeight = 671
  ClientWidth = 796
  Color = clBtnFace
  Font.Charset = ANSI_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnActivate = FormActivate
  OnCreate = FormCreate
  DesignSize = (
    796
    671)
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
    Width = 796
    Height = 636
    ActivePage = TabSheet2
    Align = alTop
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    TabWidth = 70
    OnChange = pcConfigChange
    object TabSheet1: TTabSheet
      Caption = #51648#51216#51221#48372
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      DesignSize = (
        788
        607)
      object btnSave0: TButton
        Left = 707
        Top = 579
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
        788
        607)
      object Label22: TLabel
        Left = 57
        Top = 10
        Width = 30
        Height = 14
        BiDiMode = bdRightToLeft
        Caption = #51064#53552#48268
        ParentBiDiMode = False
      end
      object Label24: TLabel
        Left = 161
        Top = 10
        Width = 34
        Height = 14
        Caption = '('#48516':'#52488')'
      end
      object Label25: TLabel
        Left = 46
        Top = 37
        Width = 40
        Height = 14
        BiDiMode = bdRightToLeft
        Caption = #46041#51089#51452#44592
        ParentBiDiMode = False
      end
      object Label26: TLabel
        Left = 160
        Top = 36
        Width = 34
        Height = 14
        Caption = '('#48516':'#52488')'
      end
      object Label27: TLabel
        Left = 227
        Top = 10
        Width = 40
        Height = 14
        BiDiMode = bdRightToLeft
        Caption = #49884#51089#49884#44036
        ParentBiDiMode = False
      end
      object Label28: TLabel
        Left = 389
        Top = 10
        Width = 170
        Height = 14
        Caption = '('#47588#48516' '#49884#51089#54616#50668' 10'#48516' '#46041#50504' '#51088#47308#49688#51665')'
      end
      object btnSave1: TButton
        Tag = 1
        Left = 710
        Top = 579
        Width = 75
        Height = 25
        Anchors = [akRight, akBottom]
        Caption = #51200#51109
        TabOrder = 6
        OnClick = btnSaveClick
      end
      object med2Interval: TMaskEdit
        Left = 92
        Top = 6
        Width = 65
        Height = 22
        Hint = 'rsetup.value01|text'
        Ctl3D = True
        EditMask = '##:##;1'
        ImeName = 'Microsoft Office IME 2007'
        MaxLength = 5
        ParentCtl3D = False
        TabOrder = 0
        Text = '  :  '
      end
      object med2Period: TMaskEdit
        Left = 92
        Top = 33
        Width = 65
        Height = 22
        Hint = 'rsetup.value02|text'
        Ctl3D = True
        EditMask = '##:##;1'
        ImeName = 'Microsoft Office IME 2007'
        MaxLength = 5
        ParentCtl3D = False
        TabOrder = 1
        Text = '  :  '
      end
      object cbx2StartTime: TComboBox
        Left = 272
        Top = 6
        Width = 109
        Height = 22
        Hint = 'rsetup.value03|text'
        Style = csDropDownList
        Ctl3D = True
        ImeName = 'Microsoft Office IME 2007'
        ParentCtl3D = False
        TabOrder = 2
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
        Left = 4
        Top = 56
        Width = 577
        Height = 298
        Caption = '1'#48264#50976#49549#44228' '#49444#51221
        Ctl3D = True
        ParentCtl3D = False
        TabOrder = 3
        object Label34: TLabel
          Left = 33
          Top = 20
          Width = 54
          Height = 14
          BiDiMode = bdRightToLeft
          Caption = #51109#48708#51032' '#51333#47448
          ParentBiDiMode = False
        end
        object Label35: TLabel
          Left = 40
          Top = 44
          Width = 47
          Height = 14
          BiDiMode = bdRightToLeft
          Caption = 'Comport'
          ParentBiDiMode = False
        end
        object Label36: TLabel
          Left = 47
          Top = 69
          Width = 40
          Height = 14
          BiDiMode = bdRightToLeft
          Caption = #53685#49888#49549#46020
          ParentBiDiMode = False
        end
        object Label46: TLabel
          Left = 202
          Top = 135
          Width = 67
          Height = 14
          BiDiMode = bdRightToLeft
          Caption = 'Reverse '#50741#49496
          ParentBiDiMode = False
        end
        object Label137: TLabel
          Left = 212
          Top = 159
          Width = 57
          Height = 14
          BiDiMode = bdRightToLeft
          Caption = 'Idle Check'
          ParentBiDiMode = False
        end
        object Label141: TLabel
          Left = 37
          Top = 171
          Width = 50
          Height = 14
          Caption = #49472#50976#54952#48276#50948
        end
        object Label143: TLabel
          Left = 130
          Top = 171
          Width = 9
          Height = 14
          Caption = '~'
        end
        object Panel1: TPanel
          Left = 195
          Top = 15
          Width = 175
          Height = 110
          BevelInner = bvRaised
          BevelOuter = bvLowered
          TabOrder = 7
          object Label53: TLabel
            Left = 4
            Top = 26
            Width = 68
            Height = 14
            BiDiMode = bdRightToLeft
            Caption = #52769#51221' '#49472' '#52509#44060#49688
            ParentBiDiMode = False
          end
          object Label54: TLabel
            Left = 14
            Top = 52
            Width = 58
            Height = 14
            BiDiMode = bdRightToLeft
            Caption = #52769#51221' '#49472' '#53356#44592
            ParentBiDiMode = False
          end
          object Label55: TLabel
            Left = 14
            Top = 78
            Width = 58
            Height = 14
            BiDiMode = bdRightToLeft
            Caption = #44277#48177' '#44144#47532' '#49472
            ParentBiDiMode = False
          end
          object Label59: TLabel
            Left = 59
            Top = 6
            Width = 94
            Height = 14
            BiDiMode = bdLeftToRight
            Caption = 'AG/SL Series '#49444#51221
            ParentBiDiMode = False
          end
          object ed2H1WF: TEdit
            Left = 78
            Top = 78
            Width = 88
            Height = 22
            Hint = 'rsetup.value09'
            ImeName = 'Microsoft Office IME 2007'
            TabOrder = 0
            OnChange = FloatEditChange
          end
          object ed2H1WN: TEdit
            Left = 78
            Top = 26
            Width = 88
            Height = 22
            Hint = 'rsetup.value07'
            ImeName = 'Microsoft Office IME 2007'
            NumbersOnly = True
            TabOrder = 1
          end
          object ed2H1WS: TEdit
            Left = 78
            Top = 52
            Width = 88
            Height = 22
            Hint = 'rsetup.value08'
            ImeName = 'Microsoft Office IME 2007'
            TabOrder = 2
            OnChange = FloatEditChange
          end
        end
        object PanelAirmar: TPanel
          Left = 195
          Top = 15
          Width = 175
          Height = 110
          BevelInner = bvRaised
          BevelOuter = bvLowered
          TabOrder = 10
          object Label142: TLabel
            Left = 59
            Top = 3
            Width = 82
            Height = 14
            BiDiMode = bdLeftToRight
            Caption = 'Airmar '#53685#49888' '#49444#51221
            ParentBiDiMode = False
          end
          object Label139: TLabel
            Left = 21
            Top = 30
            Width = 47
            Height = 14
            BiDiMode = bdRightToLeft
            Caption = 'Comport'
            ParentBiDiMode = False
          end
          object Label140: TLabel
            Left = 28
            Top = 56
            Width = 40
            Height = 14
            BiDiMode = bdRightToLeft
            Caption = #53685#49888#49549#46020
            ParentBiDiMode = False
          end
          object cbx2H1AirmarPort: TComboBox
            Left = 78
            Top = 26
            Width = 90
            Height = 22
            Hint = 'rsetup.value02|text'
            Style = csDropDownList
            Ctl3D = True
            ImeName = 'Microsoft Office IME 2007'
            ParentCtl3D = False
            TabOrder = 0
            OnChange = cbx2H1AirmarPortChange
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
          object cbx2H1AirmarBaud: TComboBox
            Left = 78
            Top = 53
            Width = 90
            Height = 22
            Hint = 'rsetup.value03|text'
            Style = csDropDownList
            Ctl3D = True
            ImeName = 'Microsoft Office IME 2007'
            ParentCtl3D = False
            TabOrder = 1
            Items.Strings = (
              'NONE'
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
          object cbAirmarLinker: TCheckBox
            Left = 78
            Top = 81
            Width = 91
            Height = 17
            Caption = 'AirmarLinker'
            TabOrder = 2
          end
        end
        object cbx2H1Kind: TComboBox
          Left = 89
          Top = 15
          Width = 90
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
            'AG Series'
            'AccusonicA'
            'AccusonicB'
            'RQ-30'
            'RQ-30d')
        end
        object cbx2H1Comport: TComboBox
          Left = 89
          Top = 40
          Width = 90
          Height = 22
          Hint = 'rsetup.value02|text'
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 1
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
        object cbx2H1CommSpeed: TComboBox
          Left = 89
          Top = 66
          Width = 90
          Height = 22
          Hint = 'rsetup.value03|text'
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 2
          Items.Strings = (
            'NONE'
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
          Left = 88
          Top = 92
          Width = 90
          Height = 22
          Hint = 'rsetup.value04|text'
          Color = clWhite
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
        object cbx2H1ReverseOption: TComboBox
          Left = 273
          Top = 131
          Width = 90
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
        object led2Hz: TLabeledEdit
          Left = 89
          Top = 117
          Width = 90
          Height = 22
          Hint = 'rsetup.value04|text'
          Ctl3D = True
          EditLabel.Width = 50
          EditLabel.Height = 14
          EditLabel.Caption = #46041#51089#51452#54028#49688
          ImeName = 'Microsoft Office IME 2007'
          LabelPosition = lpLeft
          MaxLength = 100
          ParentCtl3D = False
          TabOrder = 5
          OnExit = OnOffsetExit
        end
        object led2SN: TLabeledEdit
          Left = 89
          Top = 142
          Width = 90
          Height = 22
          Hint = 'rsetup.value04|text'
          Ctl3D = True
          EditLabel.Width = 40
          EditLabel.Height = 14
          EditLabel.Caption = #51068#47144#48264#54840
          ImeName = 'Microsoft Office IME 2007'
          LabelPosition = lpLeft
          MaxLength = 100
          ParentCtl3D = False
          TabOrder = 6
          OnExit = OnOffsetExit
        end
        object cbx2H1IdleCheck: TComboBox
          Left = 273
          Top = 156
          Width = 40
          Height = 22
          Hint = 'rsetup.value02|text'
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 8
          Items.Strings = (
            'ON'
            'OFF')
        end
        object cbx2H1IdleCheckTime: TComboBox
          Left = 323
          Top = 156
          Width = 40
          Height = 22
          Hint = 'rsetup.value02|text'
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 9
          Items.Strings = (
            '1 hour'
            '2 hour'
            '3 hour'
            '4 hour'
            '5 hour'
            '6 hour')
        end
        object led2StartCell: TEdit
          Left = 89
          Top = 168
          Width = 40
          Height = 22
          TabOrder = 11
        end
        object led2EndCell: TEdit
          Left = 139
          Top = 168
          Width = 40
          Height = 22
          TabOrder = 12
        end
        object PanelRQ30d: TPanel
          Left = 379
          Top = 15
          Width = 183
          Height = 179
          BevelInner = bvRaised
          BevelOuter = bvLowered
          TabOrder = 13
          object lbHydro1SlavePort: TLabel
            Left = 26
            Top = 101
            Width = 47
            Height = 14
            BiDiMode = bdRightToLeft
            Caption = 'ComPort'
            ParentBiDiMode = False
            Visible = False
          end
          object lbHydro1SlaveBaudRate: TLabel
            Left = 33
            Top = 127
            Width = 40
            Height = 14
            BiDiMode = bdRightToLeft
            Caption = #53685#49888#49549#46020
            ParentBiDiMode = False
            Visible = False
          end
          object lbHydro1SlaveAddress: TLabel
            Left = 13
            Top = 153
            Width = 60
            Height = 14
            BiDiMode = bdRightToLeft
            Caption = 'Address(S)'
            ParentBiDiMode = False
            Visible = False
          end
          object Label146: TLabel
            Left = 62
            Top = 3
            Width = 65
            Height = 14
            BiDiMode = bdLeftToRight
            Caption = 'RQ-30d '#49444#51221
            ParentBiDiMode = False
          end
          object Label147: TLabel
            Left = 15
            Top = 25
            Width = 65
            Height = 14
            Caption = 'Slave Count'
            Transparent = True
          end
          object Label148: TLabel
            Left = 11
            Top = 50
            Width = 62
            Height = 14
            BiDiMode = bdRightToLeft
            Caption = 'Address(M)'
            ParentBiDiMode = False
          end
          object edHydro1SlaveAddress: TEdit
            Left = 81
            Top = 151
            Width = 90
            Height = 22
            Hint = 'Slave Port'#50640' '#50672#44208#46108' '#51109#48708#51032' '#51452#49548'('#51340#50504#44592#51456')(ex) 6,7,8,910'
            ImeName = 'Microsoft Office IME 2007'
            ParentShowHint = False
            ShowHint = True
            TabOrder = 5
            Visible = False
          end
          object cbxSlaveCount: TComboBox
            Left = 81
            Top = 22
            Width = 90
            Height = 22
            Hint = 'RQ-30d'#50640' '#50672#44208#46108' Slave'#51032' '#49688'(MasterPort + SlavePort) (RQ-30d '#51228#50808')'
            Style = csDropDownList
            Ctl3D = True
            ImeName = 'Microsoft Office IME 2007'
            ParentCtl3D = False
            ParentShowHint = False
            ShowHint = True
            TabOrder = 0
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
          object edHydro1MasterAddress: TEdit
            Left = 81
            Top = 47
            Width = 90
            Height = 22
            Hint = 'Master Port'#50640' '#50672#44208#46108' '#51109#48708#51032' '#51452#49548'('#51340#50504#44592#51456')(ex)5,4,3,2,1'
            ImeName = 'Microsoft Office IME 2007'
            ParentShowHint = False
            ShowHint = True
            TabOrder = 1
          end
          object cbxHydro1UseSlavePort: TCheckBox
            Left = 81
            Top = 73
            Width = 98
            Height = 17
            Hint = 'RQ-30d '#49468#49436#44032' '#46160' '#44060#51032' '#49884#47532#50620' '#54252#53944#47196' '#50672#44208#46104#50612' '#51080#51012' '#44221#50864' '#49324#50857
            Caption = 'Use Slave Port'
            ParentShowHint = False
            ShowHint = True
            TabOrder = 2
            OnClick = cbxHydro1UseSlavePortClick
          end
          object cbHydro1SlavePort: TComboBox
            Left = 81
            Top = 99
            Width = 90
            Height = 22
            Hint = 'Slave Port '#48264#54840
            Style = csDropDownList
            Ctl3D = True
            ImeName = 'Microsoft Office IME 2007'
            ParentCtl3D = False
            ParentShowHint = False
            ShowHint = True
            TabOrder = 3
            Visible = False
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
          object cbHydro1SlaveBaudRate: TComboBox
            Left = 81
            Top = 125
            Width = 90
            Height = 22
            Hint = 'Slave Port'#51032' '#53685#49888#49549#46020'(Default:9600bps)'
            Style = csDropDownList
            Ctl3D = True
            ImeName = 'Microsoft Office IME 2007'
            ParentCtl3D = False
            ParentShowHint = False
            ShowHint = True
            TabOrder = 4
            Visible = False
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
        end
      end
      object gbxHydroMeter2: TGroupBox
        Left = 3
        Top = 362
        Width = 382
        Height = 211
        Caption = '2'#48264#50976#49549#44228' '#49444#51221
        Ctl3D = True
        ParentCtl3D = False
        TabOrder = 4
        object Label37: TLabel
          Left = 33
          Top = 22
          Width = 54
          Height = 14
          BiDiMode = bdRightToLeft
          Caption = #51109#48708#51032' '#51333#47448
          ParentBiDiMode = False
        end
        object Label38: TLabel
          Left = 40
          Top = 48
          Width = 47
          Height = 14
          BiDiMode = bdRightToLeft
          Caption = 'Comport'
          ParentBiDiMode = False
        end
        object Label39: TLabel
          Left = 47
          Top = 74
          Width = 40
          Height = 14
          BiDiMode = bdRightToLeft
          Caption = #53685#49888#49549#46020
          ParentBiDiMode = False
        end
        object Label47: TLabel
          Left = 199
          Top = 134
          Width = 67
          Height = 14
          BiDiMode = bdRightToLeft
          Caption = 'Reverse '#50741#49496
          ParentBiDiMode = False
        end
        object Label49: TLabel
          Left = 199
          Top = 158
          Width = 67
          Height = 14
          BiDiMode = bdRightToLeft
          Caption = 'Up/Side '#50741#49496
          ParentBiDiMode = False
        end
        object Label138: TLabel
          Left = 209
          Top = 183
          Width = 57
          Height = 14
          BiDiMode = bdRightToLeft
          Caption = 'Idle Check'
          ParentBiDiMode = False
        end
        object Label144: TLabel
          Left = 37
          Top = 182
          Width = 50
          Height = 14
          Caption = #49472#50976#54952#48276#50948
        end
        object Label145: TLabel
          Left = 130
          Top = 182
          Width = 9
          Height = 14
          Caption = '~'
        end
        object Panel2: TPanel
          Left = 195
          Top = 19
          Width = 175
          Height = 110
          BevelInner = bvRaised
          BevelOuter = bvLowered
          TabOrder = 8
          object Label56: TLabel
            Left = 5
            Top = 26
            Width = 68
            Height = 14
            BiDiMode = bdRightToLeft
            Caption = #52769#51221' '#49472' '#52509#44060#49688
            ParentBiDiMode = False
          end
          object Label57: TLabel
            Left = 15
            Top = 52
            Width = 58
            Height = 14
            BiDiMode = bdRightToLeft
            Caption = #52769#51221' '#49472' '#53356#44592
            ParentBiDiMode = False
          end
          object Label58: TLabel
            Left = 15
            Top = 78
            Width = 58
            Height = 14
            BiDiMode = bdRightToLeft
            Caption = #44277#48177' '#44144#47532' '#49472
            ParentBiDiMode = False
          end
          object Label60: TLabel
            Left = 59
            Top = 3
            Width = 94
            Height = 14
            BiDiMode = bdLeftToRight
            Caption = 'AG/SL Series '#49444#51221
            ParentBiDiMode = False
          end
          object ed2H2WF: TEdit
            Left = 75
            Top = 78
            Width = 90
            Height = 22
            Hint = 'rsetup.value09'
            ImeName = 'Microsoft Office IME 2007'
            TabOrder = 0
            OnChange = FloatEditChange
          end
          object ed2H2WN: TEdit
            Left = 75
            Top = 26
            Width = 90
            Height = 22
            Hint = 'rsetup.value07'
            ImeName = 'Microsoft Office IME 2007'
            NumbersOnly = True
            TabOrder = 1
          end
          object ed2H2WS: TEdit
            Left = 75
            Top = 52
            Width = 90
            Height = 22
            Hint = 'rsetup.value08'
            ImeName = 'Microsoft Office IME 2007'
            TabOrder = 2
            OnChange = FloatEditChange
          end
        end
        object cbx2H2Kind: TComboBox
          Left = 89
          Top = 19
          Width = 90
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
            'AG Series'
            'AccusonicA'
            'AccusonicB')
        end
        object cbx2H2Comport: TComboBox
          Left = 89
          Top = 45
          Width = 90
          Height = 22
          Hint = 'rsetup.value02|text'
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 1
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
        object cbx2H2CommSpeed: TComboBox
          Left = 89
          Top = 72
          Width = 90
          Height = 22
          Hint = 'rsetup.value03|text'
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 2
          Items.Strings = (
            'NONE'
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
          Top = 99
          Width = 90
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
        object cbx2H2ReverseOption: TComboBox
          Left = 270
          Top = 131
          Width = 90
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
          Left = 270
          Top = 155
          Width = 90
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
        object led2Hz2: TLabeledEdit
          Left = 89
          Top = 126
          Width = 90
          Height = 22
          Hint = 'rsetup.value04|text'
          Ctl3D = True
          EditLabel.Width = 50
          EditLabel.Height = 14
          EditLabel.Caption = #46041#51089#51452#54028#49688
          ImeName = 'Microsoft Office IME 2007'
          LabelPosition = lpLeft
          MaxLength = 100
          ParentCtl3D = False
          TabOrder = 6
          OnExit = OnOffsetExit
        end
        object led2SN2: TLabeledEdit
          Left = 89
          Top = 153
          Width = 90
          Height = 22
          Hint = 'rsetup.value04|text'
          Ctl3D = True
          EditLabel.Width = 40
          EditLabel.Height = 14
          EditLabel.Caption = #51068#47144#48264#54840
          ImeName = 'Microsoft Office IME 2007'
          LabelPosition = lpLeft
          MaxLength = 100
          ParentCtl3D = False
          TabOrder = 7
          OnExit = OnOffsetExit
        end
        object cbx2H2IdleCheck: TComboBox
          Left = 270
          Top = 179
          Width = 40
          Height = 22
          Hint = 'rsetup.value02|text'
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 9
          Items.Strings = (
            'ON'
            'OFF')
        end
        object cbx2H2IdleCheckTime: TComboBox
          Left = 320
          Top = 179
          Width = 40
          Height = 22
          Hint = 'rsetup.value02|text'
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 10
          Items.Strings = (
            '1 hour'
            '2 hour'
            '3 hour'
            '4 hour'
            '5 hour'
            '6 hour')
        end
        object led2StartCell2: TEdit
          Left = 89
          Top = 179
          Width = 40
          Height = 22
          TabOrder = 11
        end
        object led2EndCell2: TEdit
          Left = 139
          Top = 179
          Width = 40
          Height = 22
          TabOrder = 12
        end
      end
      object gbxWaterLevel: TGroupBox
        Left = 587
        Top = 56
        Width = 189
        Height = 298
        Caption = #49688#50948#44228' '#49444#51221
        Ctl3D = True
        ParentCtl3D = False
        TabOrder = 5
        object Label40: TLabel
          Left = 28
          Top = 23
          Width = 54
          Height = 14
          BiDiMode = bdRightToLeft
          Caption = #51109#48708#51032' '#51333#47448
          ParentBiDiMode = False
        end
        object Label41: TLabel
          Left = 35
          Top = 49
          Width = 47
          Height = 14
          BiDiMode = bdRightToLeft
          Caption = 'Comport'
          ParentBiDiMode = False
        end
        object Label42: TLabel
          Left = 42
          Top = 75
          Width = 40
          Height = 14
          BiDiMode = bdRightToLeft
          Caption = #53685#49888#49549#46020
          ParentBiDiMode = False
        end
        object lbAtmosCaption: TLabel
          Left = 30
          Top = 177
          Width = 50
          Height = 14
          BiDiMode = bdRightToLeft
          Caption = #45824#44592#50517#49468#49436
          ParentBiDiMode = False
          Visible = False
        end
        object Label136: TLabel
          Left = 25
          Top = 246
          Width = 57
          Height = 14
          BiDiMode = bdRightToLeft
          Caption = 'Idle Check'
          ParentBiDiMode = False
        end
        object lb2WaterAtmos: TLabel
          Left = 32
          Top = 152
          Width = 50
          Height = 14
          BiDiMode = bdRightToLeft
          Caption = #45824#44592#50517#48372#51221
          ParentBiDiMode = False
          Visible = False
        end
        object lb2WaterAirmar: TLabel
          Left = 42
          Top = 224
          Width = 40
          Height = 14
          BiDiMode = bdRightToLeft
          Caption = #44592#49345#49468#49436
          ParentBiDiMode = False
          Visible = False
        end
        object Label161: TLabel
          Left = 31
          Top = 272
          Width = 50
          Height = 14
          BiDiMode = bdRightToLeft
          Caption = #49688#50948#45936#51060#53552
          ParentBiDiMode = False
        end
        object cbx2WLKind: TComboBox
          Left = 86
          Top = 17
          Width = 90
          Height = 22
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 0
          OnChange = cbx2WLKindChange
          Items.Strings = (
            'NONE'
            'AT200'
            'LT400+Atmos'
            'LT500'
            'OTTCBS'
            'RNSWATER')
        end
        object cbx2WLComport: TComboBox
          Left = 86
          Top = 45
          Width = 90
          Height = 22
          Hint = 'rsetup.value02|text'
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 1
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
        object cbx2WLCommSpeed: TComboBox
          Left = 86
          Top = 71
          Width = 90
          Height = 22
          Hint = 'rsetup.value03|text'
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 2
          Items.Strings = (
            'NONE'
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
          Left = 86
          Top = 98
          Width = 90
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
        object led2WLSN: TLabeledEdit
          Left = 86
          Top = 125
          Width = 90
          Height = 22
          Hint = 'rsetup.value04|text'
          Ctl3D = True
          EditLabel.Width = 40
          EditLabel.Height = 14
          EditLabel.Caption = #51068#47144#48264#54840
          ImeName = 'Microsoft Office IME 2007'
          LabelPosition = lpLeft
          MaxLength = 100
          ParentCtl3D = False
          TabOrder = 4
          OnExit = OnOffsetExit
        end
        object cbx2AtmosPort: TComboBox
          Left = 86
          Top = 173
          Width = 90
          Height = 22
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 5
          Visible = False
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
        object edBaseAtmos: TLabeledEdit
          Left = 86
          Top = 198
          Width = 90
          Height = 22
          Hint = 'rsetup.value04|text'
          Ctl3D = True
          EditLabel.Width = 50
          EditLabel.Height = 14
          EditLabel.Caption = #44592#51456#45824#44592#50517
          ImeName = 'Microsoft Office IME 2007'
          LabelPosition = lpLeft
          MaxLength = 100
          ParentCtl3D = False
          TabOrder = 6
          Visible = False
          OnExit = OnOffsetExit
        end
        object cbx2WLIdleCheck: TComboBox
          Left = 86
          Top = 245
          Width = 43
          Height = 22
          Hint = 'rsetup.value02|text'
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 7
          Items.Strings = (
            'ON'
            'OFF')
        end
        object cbx2WLIdleCheckTime: TComboBox
          Left = 134
          Top = 245
          Width = 43
          Height = 22
          Hint = 'rsetup.value02|text'
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 8
          Items.Strings = (
            '1 hour'
            '2 hour'
            '3 hour'
            '4 hour'
            '5 hour'
            '6 hour')
        end
        object cbx2WaterAtmos: TCheckBox
          Left = 86
          Top = 151
          Width = 91
          Height = 17
          Caption = 'Enable'
          TabOrder = 9
          OnClick = cbx2WaterAtmosClick
        end
        object cbx2WaterAirmar: TCheckBox
          Left = 88
          Top = 224
          Width = 91
          Height = 17
          Caption = 'Enable'
          TabOrder = 10
          OnClick = cbx2WaterAirmarClick
        end
        object cbx2WaterUseFlag: TComboBox
          Left = 85
          Top = 271
          Width = 92
          Height = 22
          Hint = #50976#49549#44228#51032' '#49688#50948'(RQ-30d), '#49688#50948#44228#51032' '#49688#50948' '#51473' '#50612#45712' '#44163#51012' '#50424' '#44163#51064' '#51648' '#50741#49496
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ItemIndex = 1
          ParentCtl3D = False
          ParentShowHint = False
          ShowHint = True
          TabOrder = 11
          Text = #49688#50948#44228'(RnsWater)'
          Items.Strings = (
            #50976#49549#44228'1(RQ-30d)'
            #49688#50948#44228'(RnsWater)')
        end
      end
      object GroupBox4: TGroupBox
        Left = 394
        Top = 362
        Width = 382
        Height = 211
        Caption = '3'#48264#50976#49549#44228' '#49444#51221
        Ctl3D = True
        ParentCtl3D = False
        TabOrder = 7
        object Label149: TLabel
          Left = 33
          Top = 22
          Width = 54
          Height = 14
          BiDiMode = bdRightToLeft
          Caption = #51109#48708#51032' '#51333#47448
          ParentBiDiMode = False
        end
        object Label150: TLabel
          Left = 40
          Top = 48
          Width = 47
          Height = 14
          BiDiMode = bdRightToLeft
          Caption = 'Comport'
          ParentBiDiMode = False
        end
        object Label151: TLabel
          Left = 47
          Top = 74
          Width = 40
          Height = 14
          BiDiMode = bdRightToLeft
          Caption = #53685#49888#49549#46020
          ParentBiDiMode = False
        end
        object Label152: TLabel
          Left = 199
          Top = 134
          Width = 67
          Height = 14
          BiDiMode = bdRightToLeft
          Caption = 'Reverse '#50741#49496
          ParentBiDiMode = False
        end
        object Label153: TLabel
          Left = 199
          Top = 158
          Width = 67
          Height = 14
          BiDiMode = bdRightToLeft
          Caption = 'Up/Side '#50741#49496
          ParentBiDiMode = False
        end
        object Label154: TLabel
          Left = 209
          Top = 183
          Width = 57
          Height = 14
          BiDiMode = bdRightToLeft
          Caption = 'Idle Check'
          ParentBiDiMode = False
        end
        object Label155: TLabel
          Left = 37
          Top = 182
          Width = 50
          Height = 14
          Caption = #49472#50976#54952#48276#50948
        end
        object Label156: TLabel
          Left = 130
          Top = 182
          Width = 9
          Height = 14
          Caption = '~'
        end
        object Panel3: TPanel
          Left = 195
          Top = 19
          Width = 175
          Height = 110
          BevelInner = bvRaised
          BevelOuter = bvLowered
          TabOrder = 8
          object Label157: TLabel
            Left = 5
            Top = 26
            Width = 68
            Height = 14
            BiDiMode = bdRightToLeft
            Caption = #52769#51221' '#49472' '#52509#44060#49688
            ParentBiDiMode = False
          end
          object Label158: TLabel
            Left = 15
            Top = 52
            Width = 58
            Height = 14
            BiDiMode = bdRightToLeft
            Caption = #52769#51221' '#49472' '#53356#44592
            ParentBiDiMode = False
          end
          object Label159: TLabel
            Left = 15
            Top = 78
            Width = 58
            Height = 14
            BiDiMode = bdRightToLeft
            Caption = #44277#48177' '#44144#47532' '#49472
            ParentBiDiMode = False
          end
          object Label160: TLabel
            Left = 59
            Top = 3
            Width = 94
            Height = 14
            BiDiMode = bdLeftToRight
            Caption = 'AG/SL Series '#49444#51221
            ParentBiDiMode = False
          end
          object ed2H3WF: TEdit
            Left = 75
            Top = 78
            Width = 90
            Height = 22
            Hint = 'rsetup.value09'
            ImeName = 'Microsoft Office IME 2007'
            TabOrder = 0
            OnChange = FloatEditChange
          end
          object ed2H3WN: TEdit
            Left = 75
            Top = 26
            Width = 90
            Height = 22
            Hint = 'rsetup.value07'
            ImeName = 'Microsoft Office IME 2007'
            NumbersOnly = True
            TabOrder = 1
          end
          object ed2H3WS: TEdit
            Left = 75
            Top = 52
            Width = 90
            Height = 22
            Hint = 'rsetup.value08'
            ImeName = 'Microsoft Office IME 2007'
            TabOrder = 2
            OnChange = FloatEditChange
          end
        end
        object cbx2H3Kind: TComboBox
          Left = 89
          Top = 19
          Width = 90
          Height = 22
          Hint = 'rsetup.value01|text'
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 0
          OnChange = cbx2H3KindChange
          Items.Strings = (
            'NONE'
            'ChannelMaster'
            'SL Series'
            'AG Series'
            'AccusonicA'
            'AccusonicB')
        end
        object cbx2H3Comport: TComboBox
          Left = 89
          Top = 45
          Width = 90
          Height = 22
          Hint = 'rsetup.value02|text'
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 1
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
        object cbx2H3CommSpeed: TComboBox
          Left = 89
          Top = 72
          Width = 90
          Height = 22
          Hint = 'rsetup.value03|text'
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 2
          Items.Strings = (
            'NONE'
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
        object led2H3Offset: TLabeledEdit
          Left = 89
          Top = 99
          Width = 90
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
        object cbx2H3ReverseOption: TComboBox
          Left = 270
          Top = 131
          Width = 90
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
        object cbx2H3UpSideOption: TComboBox
          Left = 270
          Top = 155
          Width = 90
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
        object led2Hz3: TLabeledEdit
          Left = 89
          Top = 125
          Width = 90
          Height = 22
          Hint = 'rsetup.value04|text'
          Ctl3D = True
          EditLabel.Width = 50
          EditLabel.Height = 14
          EditLabel.Caption = #46041#51089#51452#54028#49688
          ImeName = 'Microsoft Office IME 2007'
          LabelPosition = lpLeft
          MaxLength = 100
          ParentCtl3D = False
          TabOrder = 6
          OnExit = OnOffsetExit
        end
        object led2SN3: TLabeledEdit
          Left = 89
          Top = 153
          Width = 90
          Height = 22
          Hint = 'rsetup.value04|text'
          Ctl3D = True
          EditLabel.Width = 40
          EditLabel.Height = 14
          EditLabel.Caption = #51068#47144#48264#54840
          ImeName = 'Microsoft Office IME 2007'
          LabelPosition = lpLeft
          MaxLength = 100
          ParentCtl3D = False
          TabOrder = 7
          OnExit = OnOffsetExit
        end
        object cbx2H3IdleCheck: TComboBox
          Left = 270
          Top = 179
          Width = 40
          Height = 22
          Hint = 'rsetup.value02|text'
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 9
          Items.Strings = (
            'ON'
            'OFF')
        end
        object cbx2H3IdleCheckTime: TComboBox
          Left = 320
          Top = 179
          Width = 40
          Height = 22
          Hint = 'rsetup.value02|text'
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 10
          Items.Strings = (
            '1 hour'
            '2 hour'
            '3 hour'
            '4 hour'
            '5 hour'
            '6 hour')
        end
        object led2StartCell3: TEdit
          Left = 89
          Top = 179
          Width = 40
          Height = 22
          TabOrder = 11
        end
        object led2EndCell3: TEdit
          Left = 139
          Top = 179
          Width = 40
          Height = 22
          TabOrder = 12
        end
      end
    end
    object TabSheet3: TTabSheet
      Caption = #49468#45908
      ImageIndex = 2
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 580
      DesignSize = (
        788
        607)
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
        Left = 710
        Top = 579
        Width = 75
        Height = 25
        Anchors = [akRight, akBottom]
        Caption = #51200#51109
        TabOrder = 16
        OnClick = btnSaveClick
        ExplicitTop = 552
      end
      object cbx3ServerSendYN: TComboBox
        Left = 124
        Top = 41
        Width = 132
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
        Width = 132
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
        Width = 132
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
        Width = 132
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
        Width = 132
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
        Width = 132
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
        Width = 132
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
        Width = 132
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
        Width = 132
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
        Width = 132
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
        Width = 132
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
        object Label133: TLabel
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
        Width = 132
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
        Width = 132
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
        Left = 262
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
        Left = 599
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
        Left = 310
        Top = 383
        Width = 30
        Height = 25
        Anchors = [akRight, akBottom]
        Caption = '...'
        TabOrder = 22
        OnClick = btnPrivateKeyClick
        ExplicitTop = 356
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
        Left = 647
        Top = 383
        Width = 30
        Height = 25
        Anchors = [akRight, akBottom]
        Caption = '...'
        TabOrder = 24
        OnClick = btnPrivateKey2Click
        ExplicitTop = 356
      end
    end
    object TabSheet4: TTabSheet
      Caption = #44032#46300#45367
      ImageIndex = 3
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      DesignSize = (
        788
        607)
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
        Left = 20
        Top = 403
        Width = 93
        Height = 14
        BiDiMode = bdRightToLeft
        Caption = 'G/W '#47532#49483' '#45824#44592#49884#44036
        ParentBiDiMode = False
      end
      object sbtnSelectIF1: TSpeedButton
        Tag = 1
        Left = 422
        Top = 341
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
      object sbtnSelectIF2: TSpeedButton
        Tag = 2
        Left = 422
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
        Left = 707
        Top = 579
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
        Top = 317
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
        Width = 300
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
        Width = 300
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
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      DesignSize = (
        788
        607)
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
        Left = 707
        Top = 579
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
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Tahoma'
          Font.Style = []
          ImeName = 'Microsoft Office IME 2007'
          LabelPosition = lpLeft
          MaxLength = 100
          ParentCtl3D = False
          ParentFont = False
          TabOrder = 1
          Text = 'mail.rnsea.com'
        end
        object ledEmailAccount: TLabeledEdit
          Left = 95
          Top = 68
          Width = 149
          Height = 22
          Hint = 'rsetup.value03'
          TabStop = False
          Color = clBtnFace
          Ctl3D = True
          EditLabel.Width = 46
          EditLabel.Height = 14
          EditLabel.Caption = 'Account'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Tahoma'
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
          Height = 22
          Hint = 'rsetup.value04'
          TabStop = False
          Color = clBtnFace
          Ctl3D = True
          EditLabel.Width = 51
          EditLabel.Height = 14
          EditLabel.Caption = 'Password'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Tahoma'
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
          Height = 22
          Hint = 'rsetup.value05'
          TabStop = False
          Color = clBtnFace
          Ctl3D = True
          EditLabel.Width = 37
          EditLabel.Height = 14
          EditLabel.Caption = #49688#49888#51088'1'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Tahoma'
          Font.Style = []
          ImeName = 'Microsoft Office IME 2007'
          LabelPosition = lpLeft
          MaxLength = 100
          ParentCtl3D = False
          ParentFont = False
          ReadOnly = True
          TabOrder = 4
          Text = 'rads@rnsea.com'
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
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      DesignSize = (
        788
        607)
      object btnSave5: TButton
        Tag = 5
        Left = 707
        Top = 579
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
      Caption = #50976#47049#49328#52636
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ImageIndex = 6
      ParentFont = False
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      DesignSize = (
        788
        607)
      object Label1: TLabel
        Left = 14
        Top = 15
        Width = 66
        Height = 13
        BiDiMode = bdRightToLeft
        Caption = #50976#47049#49328#52636#48169#49885
        ParentBiDiMode = False
      end
      object btnSave6: TButton
        Tag = 6
        Left = 707
        Top = 579
        Width = 75
        Height = 25
        Anchors = [akRight, akBottom]
        Caption = #51200#51109
        TabOrder = 1
        OnClick = btnSaveClick
      end
      object PageControlFlowStream: TPageControl
        Left = 6
        Top = 45
        Width = 667
        Height = 376
        ActivePage = tsADVM
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        object tsADVM: TTabSheet
          Caption = #50976#49549#44228#51221#48372
          ExplicitLeft = 0
          ExplicitTop = 0
          ExplicitWidth = 0
          ExplicitHeight = 0
          object Label89: TLabel
            Left = 7
            Top = 7
            Width = 185
            Height = 15
            AutoSize = False
            Caption = #54616#52789#50976#49549#44228#51221#48372
          end
          object Label90: TLabel
            Left = 320
            Top = 7
            Width = 201
            Height = 13
            AutoSize = False
            Caption = #49345#52789#50976#49549#44228#51221#48372
          end
          object Bevel5: TBevel
            Left = 7
            Top = 23
            Width = 238
            Height = 3
            Shape = bsBottomLine
          end
          object Bevel6: TBevel
            Left = 320
            Top = 23
            Width = 238
            Height = 3
            Shape = bsBottomLine
          end
          object Label91: TLabel
            Left = 7
            Top = 211
            Width = 149
            Height = 15
            Alignment = taRightJustify
            AutoSize = False
            Caption = #50976#49549#45800#50948
          end
          object Label92: TLabel
            Left = 7
            Top = 189
            Width = 149
            Height = 16
            Alignment = taRightJustify
            AutoSize = False
            Caption = #50669#48169#54693' '#50976#49549' '#49444#51221
          end
          object Label93: TLabel
            Left = 348
            Top = 187
            Width = 121
            Height = 16
            Alignment = taRightJustify
            AutoSize = False
            Caption = #50669#48169#54693' '#50976#49549' '#49444#51221
          end
          object Label94: TLabel
            Left = 348
            Top = 209
            Width = 121
            Height = 15
            Alignment = taRightJustify
            AutoSize = False
            Caption = #50976#49549#45800#50948
          end
          object Label95: TLabel
            Left = 559
            Top = 262
            Width = 10
            Height = 15
            AutoSize = False
            Caption = '~'
          end
          object Label96: TLabel
            Left = 559
            Top = 240
            Width = 10
            Height = 15
            AutoSize = False
            Caption = '~'
          end
          object Label97: TLabel
            Left = 247
            Top = 262
            Width = 10
            Height = 15
            AutoSize = False
            Caption = '~'
          end
          object Label98: TLabel
            Left = 247
            Top = 237
            Width = 10
            Height = 15
            AutoSize = False
            Caption = '~'
          end
          object Label99: TLabel
            Left = 341
            Top = 289
            Width = 121
            Height = 16
            Alignment = taRightJustify
            AutoSize = False
            Caption = #49444#52824#54805#53468
          end
          object Label100: TLabel
            Left = 248
            Top = 35
            Width = 71
            Height = 18
            AutoSize = False
            Caption = '(00.00~99.99)'
          end
          object Label101: TLabel
            Left = 248
            Top = 59
            Width = 78
            Height = 18
            AutoSize = False
            Caption = '(-99.99~99.99)'
          end
          object Label102: TLabel
            Left = 248
            Top = 83
            Width = 40
            Height = 18
            AutoSize = False
            Caption = '0~360)'
          end
          object Label103: TLabel
            Left = 248
            Top = 109
            Width = 40
            Height = 18
            AutoSize = False
            Caption = '(0~360)'
          end
          object Label104: TLabel
            Left = 248
            Top = 135
            Width = 62
            Height = 18
            AutoSize = False
            Caption = '(0.0~180.0)'
          end
          object Label105: TLabel
            Left = 248
            Top = 160
            Width = 78
            Height = 18
            AutoSize = False
            Caption = '(0.00~99.99)'
          end
          object Label106: TLabel
            Left = 564
            Top = 35
            Width = 75
            Height = 18
            AutoSize = False
            Caption = '(00.00~99.99)'
          end
          object Label107: TLabel
            Left = 564
            Top = 59
            Width = 78
            Height = 18
            AutoSize = False
            Caption = '(-99.99~99.99)'
          end
          object Label108: TLabel
            Left = 564
            Top = 83
            Width = 40
            Height = 18
            AutoSize = False
            Caption = '0~360)'
          end
          object Label109: TLabel
            Left = 564
            Top = 109
            Width = 40
            Height = 18
            AutoSize = False
            Caption = '(0~360)'
          end
          object Label110: TLabel
            Left = 564
            Top = 135
            Width = 62
            Height = 18
            AutoSize = False
            Caption = '(0.0~180.0)'
          end
          object Label111: TLabel
            Left = 564
            Top = 160
            Width = 78
            Height = 18
            AutoSize = False
            Caption = '(0.00~99.99)'
          end
          object AXadvm: TLabeledEdit
            Left = 162
            Top = 32
            Width = 83
            Height = 21
            EditLabel.Width = 143
            EditLabel.Height = 13
            EditLabel.Caption = #51340#50504#51004#47196#48512#53552' '#50976#49549#44228' '#50948#52824'(m)'
            ImeName = 'Microsoft IME 2010'
            LabelPosition = lpLeft
            TabOrder = 0
          end
          object AZadvm: TLabeledEdit
            Left = 162
            Top = 57
            Width = 83
            Height = 21
            EditLabel.Width = 99
            EditLabel.Height = 13
            EditLabel.Caption = #50976#49549#44228' '#49444#52824' '#45458#51060'(m)'
            ImeName = 'Microsoft IME 2010'
            LabelPosition = lpLeft
            TabOrder = 1
          end
          object APan: TLabeledEdit
            Left = 162
            Top = 82
            Width = 83
            Height = 21
            EditLabel.Width = 135
            EditLabel.Height = 13
            EditLabel.Caption = #50976#49549#44228' '#49444#52824' Pan '#44050'(degree)'
            ImeName = 'Microsoft IME 2010'
            LabelPosition = lpLeft
            TabOrder = 2
          end
          object ATilt: TLabeledEdit
            Left = 162
            Top = 108
            Width = 83
            Height = 21
            EditLabel.Width = 131
            EditLabel.Height = 13
            EditLabel.Caption = #50976#49549#44228' '#49444#52824' Tilt '#44050'(degree)'
            ImeName = 'Microsoft IME 2010'
            LabelPosition = lpLeft
            TabOrder = 3
          end
          object APhi: TLabeledEdit
            Left = 162
            Top = 133
            Width = 83
            Height = 21
            EditLabel.Width = 139
            EditLabel.Height = 13
            EditLabel.Caption = 'PHI('#52769#51221' '#48169#54693' '#44033#46020'(gegree))'
            ImeName = 'Microsoft IME 2010'
            LabelPosition = lpLeft
            TabOrder = 4
          end
          object ASubmersion: TLabeledEdit
            Left = 162
            Top = 159
            Width = 83
            Height = 21
            EditLabel.Width = 118
            EditLabel.Height = 13
            EditLabel.Caption = #50976#49549#44228' '#54728#50857#51104#44608#44618#51060'(m)'
            ImeName = 'Microsoft IME 2010'
            LabelPosition = lpLeft
            TabOrder = 5
          end
          object AVMin: TLabeledEdit
            Left = 162
            Top = 234
            Width = 83
            Height = 21
            EditLabel.Width = 105
            EditLabel.Height = 13
            EditLabel.Caption = #50976#49549#44228' '#52769#51221#48276#50948'(m/s)'
            ImeName = 'Microsoft IME 2010'
            LabelPosition = lpLeft
            TabOrder = 8
          end
          object AStartCell: TLabeledEdit
            Left = 162
            Top = 261
            Width = 83
            Height = 21
            EditLabel.Width = 62
            EditLabel.Height = 13
            EditLabel.Caption = #50976#54952#49472'(1~N)'
            ImeName = 'Microsoft IME 2010'
            LabelPosition = lpLeft
            TabOrder = 10
          end
          object BXadvm: TLabeledEdit
            Left = 475
            Top = 32
            Width = 83
            Height = 21
            EditLabel.Width = 143
            EditLabel.Height = 13
            EditLabel.Caption = #51340#50504#51004#47196#48512#53552' '#50976#49549#44228' '#50948#52824'(m)'
            ImeName = 'Microsoft IME 2010'
            LabelPosition = lpLeft
            TabOrder = 12
          end
          object BZadvm: TLabeledEdit
            Left = 475
            Top = 57
            Width = 83
            Height = 21
            EditLabel.Width = 99
            EditLabel.Height = 13
            EditLabel.Caption = #50976#49549#44228' '#49444#52824' '#45458#51060'(m)'
            ImeName = 'Microsoft IME 2010'
            LabelPosition = lpLeft
            TabOrder = 13
          end
          object BPan: TLabeledEdit
            Left = 475
            Top = 82
            Width = 83
            Height = 21
            EditLabel.Width = 135
            EditLabel.Height = 13
            EditLabel.Caption = #50976#49549#44228' '#49444#52824' Pan '#44050'(degree)'
            ImeName = 'Microsoft IME 2010'
            LabelPosition = lpLeft
            TabOrder = 14
          end
          object BTilt: TLabeledEdit
            Left = 475
            Top = 106
            Width = 83
            Height = 21
            EditLabel.Width = 131
            EditLabel.Height = 13
            EditLabel.Caption = #50976#49549#44228' '#49444#52824' Tilt '#44050'(degree)'
            ImeName = 'Microsoft IME 2010'
            LabelPosition = lpLeft
            TabOrder = 15
          end
          object BPhi: TLabeledEdit
            Left = 475
            Top = 133
            Width = 83
            Height = 21
            EditLabel.Width = 139
            EditLabel.Height = 13
            EditLabel.Caption = 'PHI('#52769#51221' '#48169#54693' '#44033#46020'(gegree))'
            ImeName = 'Microsoft IME 2010'
            LabelPosition = lpLeft
            TabOrder = 16
          end
          object BSubmersion: TLabeledEdit
            Left = 475
            Top = 159
            Width = 83
            Height = 21
            EditLabel.Width = 118
            EditLabel.Height = 13
            EditLabel.Caption = #50976#49549#44228' '#54728#50857#51104#44608#44618#51060'(m)'
            ImeName = 'Microsoft IME 2010'
            LabelPosition = lpLeft
            TabOrder = 17
          end
          object BVMin: TLabeledEdit
            Left = 475
            Top = 235
            Width = 83
            Height = 21
            EditLabel.Width = 105
            EditLabel.Height = 13
            EditLabel.Caption = #50976#49549#44228' '#52769#51221#48276#50948'(m/s)'
            ImeName = 'Microsoft IME 2010'
            LabelPosition = lpLeft
            TabOrder = 20
          end
          object BStartCell: TLabeledEdit
            Left = 475
            Top = 261
            Width = 83
            Height = 21
            EditLabel.Width = 62
            EditLabel.Height = 13
            EditLabel.Caption = #50976#54952#49472'(1~N)'
            ImeName = 'Microsoft IME 2010'
            LabelPosition = lpLeft
            TabOrder = 22
          end
          object AReverseMode: TComboBox
            Left = 162
            Top = 184
            Width = 83
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 6
            Items.Strings = (
              '0: On('#50669#48169#54693' '#49444#51221')'
              '1: Off('#51221#48169#54693' '#49444#51221')')
          end
          object AUnitVelocity: TComboBox
            Left = 162
            Top = 210
            Width = 83
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 7
            Items.Strings = (
              '0: mm/s'
              '1: cm/s'
              '2: m/s')
          end
          object BUnitVelocity: TComboBox
            Left = 475
            Top = 210
            Width = 83
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 19
            Items.Strings = (
              '0: mm/s'
              '1: cm/s'
              '2: m/s')
          end
          object BReverseMode: TComboBox
            Left = 475
            Top = 184
            Width = 83
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 18
            Items.Strings = (
              '0: On('#50669#48169#54693' '#49444#51221')'
              '1: Off('#51221#48169#54693' '#49444#51221')')
          end
          object AVMax: TEdit
            Left = 257
            Top = 235
            Width = 83
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 9
          end
          object AEndCell: TEdit
            Left = 257
            Top = 262
            Width = 83
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 11
          end
          object BEndCell: TEdit
            Left = 570
            Top = 262
            Width = 83
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 23
          end
          object BVMax: TEdit
            Left = 570
            Top = 236
            Width = 83
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 21
          end
          object BType: TComboBox
            Left = 475
            Top = 288
            Width = 178
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 24
            Items.Strings = (
              'UP : '#49345#52789#50976#49549#44228
              'LEFTRIGHT : '#51340#50504'/'#50864#50504
              'SOUTHNORTH : '#45224#45800'/'#48513#45800)
          end
        end
        object tsHA: TTabSheet
          Caption = #49688#50948'_'#45800#47732#51201#44288#44228#44257#49440
          ImageIndex = 1
          ExplicitLeft = 0
          ExplicitTop = 0
          ExplicitWidth = 0
          ExplicitHeight = 0
          object Label112: TLabel
            Left = 169
            Top = 20
            Width = 10
            Height = 15
            AutoSize = False
            Caption = '~'
          end
          object Label113: TLabel
            Left = 274
            Top = 19
            Width = 71
            Height = 18
            AutoSize = False
            Caption = '(00.00~99.99)'
          end
          object Label114: TLabel
            Left = 169
            Top = 46
            Width = 120
            Height = 18
            AutoSize = False
            Caption = '(-999.999 ~ 999.999)'
          end
          object Label115: TLabel
            Left = 169
            Top = 73
            Width = 120
            Height = 18
            AutoSize = False
            Caption = '(-999.999 ~ 999.999)'
          end
          object Label116: TLabel
            Left = 169
            Top = 100
            Width = 120
            Height = 18
            AutoSize = False
            Caption = '(-999.999 ~ 999.999)'
          end
          object Label117: TLabel
            Left = 169
            Top = 127
            Width = 120
            Height = 18
            AutoSize = False
            Caption = '(-999.999 ~ 999.999)'
          end
          object Label118: TLabel
            Left = 169
            Top = 154
            Width = 120
            Height = 18
            AutoSize = False
            Caption = '(-999.999 ~ 999.999)'
          end
          object Label119: TLabel
            Left = 169
            Top = 181
            Width = 120
            Height = 18
            AutoSize = False
            Caption = '(-999.999 ~ 999.999)'
          end
          object Label120: TLabel
            Left = 169
            Top = 205
            Width = 120
            Height = 18
            AutoSize = False
            Caption = '(-999.999 ~ 999.999)'
          end
          object HAMinH: TLabeledEdit
            Left = 80
            Top = 16
            Width = 83
            Height = 21
            EditLabel.Width = 63
            EditLabel.Height = 13
            EditLabel.Caption = #50976#54952' '#49688#50948'(m)'
            ImeName = 'Microsoft IME 2010'
            LabelPosition = lpLeft
            TabOrder = 0
          end
          object HAA6: TLabeledEdit
            Left = 80
            Top = 43
            Width = 83
            Height = 21
            EditLabel.Width = 13
            EditLabel.Height = 13
            EditLabel.Caption = 'A6'
            ImeName = 'Microsoft IME 2010'
            LabelPosition = lpLeft
            TabOrder = 2
          end
          object HAA5: TLabeledEdit
            Left = 80
            Top = 70
            Width = 83
            Height = 21
            EditLabel.Width = 13
            EditLabel.Height = 13
            EditLabel.Caption = 'A5'
            ImeName = 'Microsoft IME 2010'
            LabelPosition = lpLeft
            TabOrder = 3
          end
          object HAA4: TLabeledEdit
            Left = 80
            Top = 97
            Width = 83
            Height = 21
            EditLabel.Width = 13
            EditLabel.Height = 13
            EditLabel.Caption = 'A4'
            ImeName = 'Microsoft IME 2010'
            LabelPosition = lpLeft
            TabOrder = 4
          end
          object HAA3: TLabeledEdit
            Left = 80
            Top = 124
            Width = 83
            Height = 21
            EditLabel.Width = 13
            EditLabel.Height = 13
            EditLabel.Caption = 'A3'
            ImeName = 'Microsoft IME 2010'
            LabelPosition = lpLeft
            TabOrder = 5
          end
          object HAA2: TLabeledEdit
            Left = 80
            Top = 151
            Width = 83
            Height = 21
            EditLabel.Width = 13
            EditLabel.Height = 13
            EditLabel.Caption = 'A2'
            ImeName = 'Microsoft IME 2010'
            LabelPosition = lpLeft
            TabOrder = 6
          end
          object HAA1: TLabeledEdit
            Left = 80
            Top = 178
            Width = 83
            Height = 21
            EditLabel.Width = 13
            EditLabel.Height = 13
            EditLabel.Caption = 'A1'
            ImeName = 'Microsoft IME 2010'
            LabelPosition = lpLeft
            TabOrder = 7
          end
          object HAMaxH: TEdit
            Left = 185
            Top = 16
            Width = 83
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 1
          end
          object HAA0: TLabeledEdit
            Left = 80
            Top = 205
            Width = 83
            Height = 21
            EditLabel.Width = 13
            EditLabel.Height = 13
            EditLabel.Caption = 'A0'
            ImeName = 'Microsoft IME 2010'
            LabelPosition = lpLeft
            TabOrder = 8
          end
        end
        object tsIVM: TTabSheet
          Caption = #51648#54364#50976#49549'_'#54217#44512#50976#49549#44288#44228#44257#49440
          ImageIndex = 2
          ExplicitLeft = 0
          ExplicitTop = 0
          ExplicitWidth = 0
          ExplicitHeight = 0
          object Label121: TLabel
            Left = 10
            Top = 10
            Width = 185
            Height = 15
            AutoSize = False
            Caption = #54616#52789#50976#49549#44228#51221#48372
          end
          object Label122: TLabel
            Left = 316
            Top = 10
            Width = 201
            Height = 13
            AutoSize = False
            Caption = #49345#52789#50976#49549#44228#51221#48372
          end
          object Bevel7: TBevel
            Left = 316
            Top = 26
            Width = 285
            Height = 3
            Shape = bsBottomLine
          end
          object Bevel8: TBevel
            Left = 10
            Top = 26
            Width = 285
            Height = 3
            Shape = bsBottomLine
          end
          object Label123: TLabel
            Left = 505
            Top = 39
            Width = 12
            Height = 15
            AutoSize = False
            Caption = '~'
          end
          object Label124: TLabel
            Left = 198
            Top = 39
            Width = 8
            Height = 15
            AutoSize = False
            Caption = '~'
          end
          object Label125: TLabel
            Left = 198
            Top = 64
            Width = 120
            Height = 18
            AutoSize = False
            Caption = '(-999.9999 ~ 999.9999)'
          end
          object Label126: TLabel
            Left = 198
            Top = 88
            Width = 120
            Height = 18
            AutoSize = False
            Caption = '(-999.9999 ~ 999.9999)'
          end
          object Label127: TLabel
            Left = 198
            Top = 114
            Width = 120
            Height = 18
            AutoSize = False
            Caption = '(-999.9999 ~ 999.9999)'
          end
          object Label128: TLabel
            Left = 505
            Top = 62
            Width = 120
            Height = 18
            AutoSize = False
            Caption = '(-999.9999 ~ 999.9999)'
          end
          object Label129: TLabel
            Left = 505
            Top = 86
            Width = 120
            Height = 18
            AutoSize = False
            Caption = '(-999.9999 ~ 999.9999)'
          end
          object Label130: TLabel
            Left = 505
            Top = 112
            Width = 120
            Height = 18
            AutoSize = False
            Caption = '(-999.9999 ~ 999.9999)'
          end
          object Label131: TLabel
            Left = 5
            Top = 53
            Width = 96
            Height = 18
            AutoSize = False
            Caption = '(-99.999 ~ 99.999)'
          end
          object Label132: TLabel
            Left = 314
            Top = 53
            Width = 96
            Height = 18
            AutoSize = False
            Caption = '(-99.999 ~ 99.999)'
          end
          object IVMAMinVi: TLabeledEdit
            Left = 109
            Top = 35
            Width = 83
            Height = 21
            EditLabel.Width = 91
            EditLabel.Height = 13
            EditLabel.Caption = #50976#54952#50976#49549#48276#50948'(m/s)'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = []
            ImeName = 'Microsoft IME 2010'
            LabelPosition = lpLeft
            ParentFont = False
            TabOrder = 0
          end
          object IVMAA: TLabeledEdit
            Left = 109
            Top = 60
            Width = 83
            Height = 21
            EditLabel.Width = 7
            EditLabel.Height = 13
            EditLabel.Caption = 'A'
            ImeName = 'Microsoft IME 2010'
            LabelPosition = lpLeft
            TabOrder = 2
          end
          object IVMAB: TLabeledEdit
            Left = 109
            Top = 85
            Width = 83
            Height = 21
            EditLabel.Width = 6
            EditLabel.Height = 13
            EditLabel.Caption = 'B'
            ImeName = 'Microsoft IME 2010'
            LabelPosition = lpLeft
            TabOrder = 3
          end
          object IVMAC: TLabeledEdit
            Left = 109
            Top = 111
            Width = 83
            Height = 21
            EditLabel.Width = 7
            EditLabel.Height = 13
            EditLabel.Caption = 'C'
            ImeName = 'Microsoft IME 2010'
            LabelPosition = lpLeft
            TabOrder = 4
          end
          object IVMBMinVi: TLabeledEdit
            Left = 416
            Top = 35
            Width = 83
            Height = 21
            EditLabel.Width = 91
            EditLabel.Height = 13
            EditLabel.Caption = #50976#54952#50976#49549#48276#50948'(m/s)'
            ImeName = 'Microsoft IME 2010'
            LabelPosition = lpLeft
            TabOrder = 5
          end
          object IVMBA: TLabeledEdit
            Left = 416
            Top = 60
            Width = 83
            Height = 21
            EditLabel.Width = 7
            EditLabel.Height = 13
            EditLabel.Caption = 'A'
            ImeName = 'Microsoft IME 2010'
            LabelPosition = lpLeft
            TabOrder = 7
          end
          object IVMBB: TLabeledEdit
            Left = 416
            Top = 85
            Width = 83
            Height = 21
            EditLabel.Width = 6
            EditLabel.Height = 13
            EditLabel.Caption = 'B'
            ImeName = 'Microsoft IME 2010'
            LabelPosition = lpLeft
            TabOrder = 8
          end
          object IVMBC: TLabeledEdit
            Left = 416
            Top = 111
            Width = 83
            Height = 21
            EditLabel.Width = 7
            EditLabel.Height = 13
            EditLabel.Caption = 'C'
            ImeName = 'Microsoft IME 2010'
            LabelPosition = lpLeft
            TabOrder = 9
          end
          object IVMAMaxVi: TEdit
            Left = 212
            Top = 35
            Width = 83
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 1
          end
          object IVMBMaxVi: TEdit
            Left = 518
            Top = 35
            Width = 83
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 6
          end
        end
      end
      object cbx7StreamMethod: TComboBox
        Left = 86
        Top = 11
        Width = 229
        Height = 21
        Hint = 'rsetup.value02|index'
        Style = csDropDownList
        Ctl3D = True
        ImeName = 'Microsoft Office IME 2007'
        ItemIndex = 0
        ParentCtl3D = False
        TabOrder = 2
        Text = '0: '#50976#47049#49328#52636' OFF'
        OnChange = cbx7StreamMethodChange
        Items.Strings = (
          '0: '#50976#47049#49328#52636' OFF'
          '1: '#51648#54364#50976#49549#48277)
      end
    end
    object TabSheet8: TTabSheet
      Caption = #54260#45908'/'#54252#53944
      ImageIndex = 7
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      DesignSize = (
        788
        607)
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
      object btnSave7: TButton
        Tag = 7
        Left = 707
        Top = 579
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
    end
  end
  object btnClose: TButton
    Left = 713
    Top = 642
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
    Left = 28
    Top = 609
  end
  object OpenDialog: TOpenDialog
    Left = 64
    Top = 608
  end
end
