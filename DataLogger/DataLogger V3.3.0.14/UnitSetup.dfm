object FormSetup: TFormSetup
  Left = 191
  Top = 0
  BorderIcons = [biSystemMenu]
  Caption = #54872#44221#49444#51221
  ClientHeight = 655
  ClientWidth = 786
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnShow = FormShow
  DesignSize = (
    786
    655)
  PixelsPerInch = 96
  TextHeight = 13
  object PageSetup: TPageControl
    Left = 0
    Top = 0
    Width = 786
    Height = 618
    ActivePage = TabSheetLoggerInfo
    Align = alTop
    TabOrder = 0
    TabWidth = 70
    OnChange = PageSetupChange
    object TabSheetPointInfo: TTabSheet
      Caption = #51648#51216#51221#48372
      DesignSize = (
        778
        590)
      object cbSavePoint: TButton
        Left = 700
        Top = 562
        Width = 75
        Height = 25
        Anchors = [akRight, akBottom]
        Caption = #51200#51109
        TabOrder = 7
        OnClick = cbSavePointClick
      end
      object edPointCode: TLabeledEdit
        Left = 92
        Top = 18
        Width = 121
        Height = 21
        Hint = 'rsetup.value01'
        Ctl3D = True
        EditLabel.Width = 44
        EditLabel.Height = 13
        EditLabel.Caption = #51648#51216#53076#46300
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        ParentCtl3D = False
        TabOrder = 0
      end
      object edPointName: TLabeledEdit
        Left = 92
        Top = 44
        Width = 121
        Height = 21
        Hint = 'rsetup.value02'
        Ctl3D = True
        EditLabel.Width = 33
        EditLabel.Height = 13
        EditLabel.Caption = #51648#51216#47749
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        ParentCtl3D = False
        TabOrder = 1
      end
      object edPointAddress: TLabeledEdit
        Left = 92
        Top = 70
        Width = 409
        Height = 21
        Hint = 'rsetup.value03'
        Ctl3D = True
        EditLabel.Width = 44
        EditLabel.Height = 13
        EditLabel.Caption = #51648#51216#51452#49548
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        ParentCtl3D = False
        TabOrder = 2
      end
      object edZipCode: TLabeledEdit
        Left = 92
        Top = 96
        Width = 121
        Height = 21
        Hint = 'rsetup.value04'
        Ctl3D = True
        EditLabel.Width = 44
        EditLabel.Height = 13
        EditLabel.Caption = #50864#54200#48264#54840
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        ParentCtl3D = False
        TabOrder = 3
      end
      object edManager: TLabeledEdit
        Left = 92
        Top = 122
        Width = 121
        Height = 21
        Hint = 'rsetup.value05'
        Ctl3D = True
        EditLabel.Width = 33
        EditLabel.Height = 13
        EditLabel.Caption = #45812#45817#51088
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        ParentCtl3D = False
        TabOrder = 4
      end
      object edManagerTel: TLabeledEdit
        Left = 92
        Top = 148
        Width = 121
        Height = 21
        Hint = 'rsetup.value06'
        Ctl3D = True
        EditLabel.Width = 69
        EditLabel.Height = 13
        EditLabel.Caption = #45812#45817#51088' '#50672#46973#52376
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        ParentCtl3D = False
        TabOrder = 5
      end
      object edEtc: TLabeledEdit
        Left = 92
        Top = 176
        Width = 409
        Height = 21
        Hint = 'rsetup.value07'
        Ctl3D = True
        EditLabel.Width = 44
        EditLabel.Height = 13
        EditLabel.Caption = #53945#51060#49324#54637
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        ParentCtl3D = False
        TabOrder = 6
      end
    end
    object TabSheetLoggerInfo: TTabSheet
      Caption = #47196#44144
      ImageIndex = 1
      DesignSize = (
        778
        590)
      object Label22: TLabel
        Left = 54
        Top = 13
        Width = 33
        Height = 13
        BiDiMode = bdRightToLeft
        Caption = #51064#53552#48268
        ParentBiDiMode = False
      end
      object Label24: TLabel
        Left = 185
        Top = 13
        Width = 34
        Height = 13
        Caption = '('#48516':'#52488')'
      end
      object Label25: TLabel
        Left = 43
        Top = 40
        Width = 44
        Height = 13
        BiDiMode = bdRightToLeft
        Caption = #46041#51089#51452#44592
        ParentBiDiMode = False
      end
      object Label26: TLabel
        Left = 185
        Top = 40
        Width = 34
        Height = 13
        Caption = '('#48516':'#52488')'
      end
      object Label27: TLabel
        Left = 227
        Top = 13
        Width = 44
        Height = 13
        BiDiMode = bdRightToLeft
        Caption = #49884#51089#49884#44036
        ParentBiDiMode = False
      end
      object Label28: TLabel
        Left = 375
        Top = 13
        Width = 175
        Height = 13
        Caption = '('#47588#48516' '#49884#51089#54616#50668' 10'#48516' '#46041#50504' '#51088#47308#49688#51665')'
      end
      object cbSaveLogger: TButton
        Tag = 1
        Left = 700
        Top = 562
        Width = 75
        Height = 25
        Anchors = [akRight, akBottom]
        Caption = #51200#51109
        TabOrder = 6
        OnClick = cbSaveLoggerClick
      end
      object meInterval: TMaskEdit
        Left = 93
        Top = 10
        Width = 90
        Height = 21
        Hint = 'rsetup.value01|text'
        Ctl3D = True
        EditMask = '##:##;1'
        ImeName = 'Microsoft Office IME 2007'
        MaxLength = 5
        ParentCtl3D = False
        TabOrder = 0
        Text = '  :  '
      end
      object mePeriod: TMaskEdit
        Left = 93
        Top = 37
        Width = 90
        Height = 21
        Hint = 'rsetup.value02|text'
        Ctl3D = True
        EditMask = '##:##;1'
        ImeName = 'Microsoft Office IME 2007'
        MaxLength = 5
        ParentCtl3D = False
        TabOrder = 1
        Text = '  :  '
      end
      object cbxStartTime: TComboBox
        Left = 274
        Top = 10
        Width = 90
        Height = 21
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
        Left = 3
        Top = 59
        Width = 576
        Height = 287
        Caption = '1'#48264#50976#49549#44228' '#49444#51221
        Ctl3D = True
        ParentCtl3D = False
        TabOrder = 3
        object Label34: TLabel
          Left = 26
          Top = 19
          Width = 58
          Height = 13
          BiDiMode = bdRightToLeft
          Caption = #51109#48708#51032' '#51333#47448
          ParentBiDiMode = False
        end
        object Label35: TLabel
          Left = 45
          Top = 43
          Width = 41
          Height = 13
          BiDiMode = bdRightToLeft
          Caption = 'Comport'
          ParentBiDiMode = False
        end
        object Label36: TLabel
          Left = 42
          Top = 71
          Width = 44
          Height = 13
          BiDiMode = bdRightToLeft
          Caption = #53685#49888#49549#46020
          ParentBiDiMode = False
        end
        object Label46: TLabel
          Left = 198
          Top = 129
          Width = 65
          Height = 13
          BiDiMode = bdRightToLeft
          Caption = 'Reverse '#50741#49496
          ParentBiDiMode = False
        end
        object Label18: TLabel
          Left = 213
          Top = 159
          Width = 50
          Height = 13
          BiDiMode = bdRightToLeft
          Caption = 'Idle Check'
          ParentBiDiMode = False
        end
        object Label80: TLabel
          Left = 29
          Top = 171
          Width = 55
          Height = 13
          Caption = #49472#50976#54952#48276#50948
        end
        object Label82: TLabel
          Left = 131
          Top = 172
          Width = 8
          Height = 13
          Caption = '~'
        end
        object PanelEWSV: TPanel
          Left = 191
          Top = 13
          Width = 180
          Height = 110
          BevelInner = bvRaised
          BevelOuter = bvLowered
          TabOrder = 14
          object Label85: TLabel
            Left = 49
            Top = 3
            Width = 117
            Height = 13
            BiDiMode = bdLeftToRight
            Caption = 'EWSVConvert '#49444#51221' '#49324#54637
            ParentBiDiMode = False
          end
          object Label86: TLabel
            Left = 7
            Top = 27
            Width = 58
            Height = 13
            BiDiMode = bdRightToLeft
            Caption = #54217#44512#44050' '#49328#51221
            ParentBiDiMode = False
          end
          object Label87: TLabel
            Left = 7
            Top = 84
            Width = 72
            Height = 13
            BiDiMode = bdRightToLeft
            Caption = #49468#49436' '#54217#44512' '#49328#51221
            ParentBiDiMode = False
          end
          object Label90: TLabel
            Left = 7
            Top = 55
            Width = 66
            Height = 13
            Caption = #49468#49436#46041#51089#49884#44036
          end
          object cbxEWSVAvg: TComboBox
            Left = 81
            Top = 25
            Width = 90
            Height = 21
            Hint = 'RADS '#50640#49436' '#54217#44512#44050#51012' '#49328#51221#54616#45716' '#48169#49885' '#51648#51221'(Default: '#51473#44036#44050')'
            Style = csDropDownList
            Ctl3D = True
            ImeName = 'Microsoft Office IME 2007'
            ParentCtl3D = False
            ParentShowHint = False
            ShowHint = True
            TabOrder = 0
            Items.Strings = (
              #54217#44512#44050
              #51473#44036#44050)
          end
          object cbxEWSVSensorAvg: TComboBox
            Left = 81
            Top = 79
            Width = 90
            Height = 21
            Hint = #49468#49436#51032' '#50976#49549','#49688#50948#51032' '#49328#51221' '#48169#49885'(Default: '#51473#44036#44050')'
            Style = csDropDownList
            Ctl3D = True
            ImeName = 'Microsoft Office IME 2007'
            ParentCtl3D = False
            ParentShowHint = False
            ShowHint = True
            TabOrder = 1
            Items.Strings = (
              #54217#44512#44050
              #51473#44036#44050)
          end
          object edEWSVWorkTime: TEdit
            Left = 81
            Top = 52
            Width = 90
            Height = 21
            Hint = #49468#49436#51032' '#46041#51089' '#49884#44036'(Default:230'#52488')'
            ParentShowHint = False
            ShowHint = True
            TabOrder = 2
            Text = '0'
          end
        end
        object Panel1: TPanel
          Left = 191
          Top = 13
          Width = 180
          Height = 110
          BevelInner = bvRaised
          BevelOuter = bvLowered
          TabOrder = 8
          object Label53: TLabel
            Left = 2
            Top = 27
            Width = 72
            Height = 13
            BiDiMode = bdRightToLeft
            Caption = #52769#51221' '#49472' '#52509#44060#49688
            ParentBiDiMode = False
          end
          object Label54: TLabel
            Left = 13
            Top = 55
            Width = 61
            Height = 13
            BiDiMode = bdRightToLeft
            Caption = #52769#51221' '#49472' '#53356#44592
            ParentBiDiMode = False
          end
          object Label55: TLabel
            Left = 13
            Top = 79
            Width = 61
            Height = 13
            BiDiMode = bdRightToLeft
            Caption = #44277#48177' '#44144#47532' '#49472
            ParentBiDiMode = False
          end
          object Label59: TLabel
            Left = 45
            Top = 3
            Width = 86
            Height = 13
            BiDiMode = bdLeftToRight
            Caption = 'AG/SL Series '#49444#51221
            ParentBiDiMode = False
          end
          object edHydro1Blank: TEdit
            Left = 80
            Top = 78
            Width = 90
            Height = 21
            Hint = 'rsetup.value09'
            ImeName = 'Microsoft Office IME 2007'
            TabOrder = 2
          end
          object edHydro1CellNo: TEdit
            Left = 80
            Top = 25
            Width = 90
            Height = 21
            Hint = 'rsetup.value07'
            ImeName = 'Microsoft Office IME 2007'
            NumbersOnly = True
            TabOrder = 0
          end
          object edHydro1CellSize: TEdit
            Left = 80
            Top = 52
            Width = 90
            Height = 21
            Hint = 'rsetup.value08'
            ImeName = 'Microsoft Office IME 2007'
            TabOrder = 1
          end
        end
        object PanelAirmar: TPanel
          Left = 191
          Top = 13
          Width = 180
          Height = 110
          BevelInner = bvRaised
          BevelOuter = bvLowered
          TabOrder = 11
          object Label81: TLabel
            Left = 49
            Top = 3
            Width = 81
            Height = 13
            BiDiMode = bdLeftToRight
            Caption = 'Airmar '#53685#49888' '#49444#51221
            ParentBiDiMode = False
          end
          object Label78: TLabel
            Left = 29
            Top = 27
            Width = 41
            Height = 13
            BiDiMode = bdRightToLeft
            Caption = 'Comport'
            ParentBiDiMode = False
          end
          object Label79: TLabel
            Left = 26
            Top = 55
            Width = 44
            Height = 13
            BiDiMode = bdRightToLeft
            Caption = #53685#49888#49549#46020
            ParentBiDiMode = False
          end
          object cbxAirmar1ComPort: TComboBox
            Left = 80
            Top = 24
            Width = 90
            Height = 21
            Hint = 'rsetup.value02|text'
            Style = csDropDownList
            Ctl3D = True
            ImeName = 'Microsoft Office IME 2007'
            ParentCtl3D = False
            TabOrder = 0
            OnChange = cbxAirmar1ComPortChange
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
          object cbxAirmar1BaudRate: TComboBox
            Left = 80
            Top = 51
            Width = 90
            Height = 21
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
            Left = 80
            Top = 78
            Width = 90
            Height = 17
            Caption = 'AirmarLinker'
            TabOrder = 2
          end
        end
        object cbxHydro1Type: TComboBox
          Left = 90
          Top = 13
          Width = 90
          Height = 21
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 0
          OnChange = cbxHydro1TypeChange
          Items.Strings = (
            'NONE'
            'ChannelMaster'
            'SL Series'
            'AG Series'
            'AccusonicA'
            'AccusonicB'
            'RQ-30'
            'RQ-30d'
            'EWSVConverter')
        end
        object cbxHydro1ComPort: TComboBox
          Left = 90
          Top = 39
          Width = 90
          Height = 21
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
        object cbxHydro1BaudRate: TComboBox
          Left = 90
          Top = 64
          Width = 90
          Height = 21
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
        object edHydro1Offset: TLabeledEdit
          Left = 90
          Top = 90
          Width = 90
          Height = 21
          Hint = 'rsetup.value04|text'
          Ctl3D = True
          EditLabel.Width = 31
          EditLabel.Height = 13
          EditLabel.Caption = 'Offset'
          ImeName = 'Microsoft Office IME 2007'
          LabelPosition = lpLeft
          MaxLength = 100
          ParentCtl3D = False
          TabOrder = 3
        end
        object cbxHydro1Reverse: TComboBox
          Left = 270
          Top = 129
          Width = 90
          Height = 21
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 9
          Items.Strings = (
            'ON'
            'OFF')
        end
        object edHydro1Hz: TLabeledEdit
          Left = 90
          Top = 116
          Width = 90
          Height = 21
          Hint = 'rsetup.value04|text'
          Ctl3D = True
          EditLabel.Width = 55
          EditLabel.Height = 13
          EditLabel.Caption = #46041#51089#51452#54028#49688
          ImeName = 'Microsoft Office IME 2007'
          LabelPosition = lpLeft
          MaxLength = 100
          ParentCtl3D = False
          TabOrder = 4
        end
        object edHydro1SN: TLabeledEdit
          Left = 90
          Top = 142
          Width = 90
          Height = 21
          Hint = 'rsetup.value04|text'
          Ctl3D = True
          EditLabel.Width = 44
          EditLabel.Height = 13
          EditLabel.Caption = #51068#47144#48264#54840
          ImeName = 'Microsoft Office IME 2007'
          LabelPosition = lpLeft
          MaxLength = 100
          ParentCtl3D = False
          TabOrder = 5
        end
        object cbxHydro1IdleCheck: TComboBox
          Left = 270
          Top = 156
          Width = 42
          Height = 21
          Hint = 'rsetup.value02|text'
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 10
          Items.Strings = (
            'ON'
            'OFF')
        end
        object cbxHydro1IdleCheckTime: TComboBox
          Left = 318
          Top = 156
          Width = 42
          Height = 21
          Hint = 'rsetup.value02|text'
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 12
          Items.Strings = (
            '1 hour'
            '2 hour'
            '3 hour'
            '4 hour'
            '5 hour'
            '6 hour')
        end
        object edHydro1StartCell: TEdit
          Left = 90
          Top = 169
          Width = 40
          Height = 21
          TabOrder = 6
        end
        object edHydro1EndCell: TEdit
          Left = 140
          Top = 169
          Width = 40
          Height = 21
          TabOrder = 7
        end
        object PanelRQ30d: TPanel
          Left = 379
          Top = 13
          Width = 180
          Height = 179
          BevelInner = bvRaised
          BevelOuter = bvLowered
          TabOrder = 13
          object lbHydro1SlavePort: TLabel
            Left = 32
            Top = 101
            Width = 41
            Height = 13
            BiDiMode = bdRightToLeft
            Caption = 'ComPort'
            ParentBiDiMode = False
            Visible = False
          end
          object lbHydro1SlaveBaudRate: TLabel
            Left = 29
            Top = 127
            Width = 44
            Height = 13
            BiDiMode = bdRightToLeft
            Caption = #53685#49888#49549#46020
            ParentBiDiMode = False
            Visible = False
          end
          object lbHydro1SlaveAddress: TLabel
            Left = 20
            Top = 153
            Width = 53
            Height = 13
            BiDiMode = bdRightToLeft
            Caption = 'Address(S)'
            ParentBiDiMode = False
            Visible = False
          end
          object Label88: TLabel
            Left = 62
            Top = 3
            Width = 62
            Height = 13
            BiDiMode = bdLeftToRight
            Caption = 'RQ-30d '#49444#51221
            ParentBiDiMode = False
          end
          object LabelSlaveCount: TLabel
            Left = 15
            Top = 25
            Width = 58
            Height = 13
            Caption = 'Slave Count'
            Transparent = True
          end
          object Label89: TLabel
            Left = 18
            Top = 50
            Width = 55
            Height = 13
            BiDiMode = bdRightToLeft
            Caption = 'Address(M)'
            ParentBiDiMode = False
          end
          object edHydro1SlaveAddress: TEdit
            Left = 81
            Top = 151
            Width = 90
            Height = 21
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
            Height = 21
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
            Height = 21
            Hint = 'Master Port'#50640' '#50672#44208#46108' '#51109#48708#51032' '#51452#49548'('#51340#50504#44592#51456')(ex)5,4,3,2,1'
            ImeName = 'Microsoft Office IME 2007'
            ParentShowHint = False
            ShowHint = True
            TabOrder = 1
          end
          object cbxHydro1UseSlavePort: TCheckBox
            Left = 81
            Top = 73
            Width = 90
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
            Height = 21
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
            Height = 21
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
        Left = 2
        Top = 352
        Width = 382
        Height = 205
        Caption = '2'#48264#50976#49549#44228' '#49444#51221
        Ctl3D = True
        ParentCtl3D = False
        TabOrder = 4
        object Label37: TLabel
          Left = 27
          Top = 21
          Width = 58
          Height = 13
          BiDiMode = bdRightToLeft
          Caption = #51109#48708#51032' '#51333#47448
          ParentBiDiMode = False
        end
        object Label38: TLabel
          Left = 44
          Top = 46
          Width = 41
          Height = 13
          BiDiMode = bdRightToLeft
          Caption = 'Comport'
          ParentBiDiMode = False
        end
        object Label39: TLabel
          Left = 41
          Top = 74
          Width = 44
          Height = 13
          BiDiMode = bdRightToLeft
          Caption = #53685#49888#49549#46020
          ParentBiDiMode = False
        end
        object Label47: TLabel
          Left = 200
          Top = 129
          Width = 65
          Height = 13
          BiDiMode = bdRightToLeft
          Caption = 'Reverse '#50741#49496
          ParentBiDiMode = False
        end
        object Label49: TLabel
          Left = 203
          Top = 154
          Width = 62
          Height = 13
          BiDiMode = bdRightToLeft
          Caption = 'Up/Side '#50741#49496
          ParentBiDiMode = False
        end
        object Label76: TLabel
          Left = 215
          Top = 178
          Width = 50
          Height = 13
          BiDiMode = bdRightToLeft
          Caption = 'Idle Check'
          ParentBiDiMode = False
        end
        object Label83: TLabel
          Left = 30
          Top = 177
          Width = 55
          Height = 13
          Caption = #49472#50976#54952#48276#50948
        end
        object Label84: TLabel
          Left = 131
          Top = 178
          Width = 8
          Height = 13
          Caption = '~'
        end
        object Panel2: TPanel
          Left = 191
          Top = 13
          Width = 180
          Height = 110
          BevelInner = bvRaised
          BevelOuter = bvLowered
          TabOrder = 12
          object Label56: TLabel
            Left = 4
            Top = 28
            Width = 72
            Height = 13
            BiDiMode = bdRightToLeft
            Caption = #52769#51221' '#49472' '#52509#44060#49688
            ParentBiDiMode = False
          end
          object Label57: TLabel
            Left = 15
            Top = 52
            Width = 61
            Height = 13
            BiDiMode = bdRightToLeft
            Caption = #52769#51221' '#49472' '#53356#44592
            ParentBiDiMode = False
          end
          object Label58: TLabel
            Left = 15
            Top = 78
            Width = 61
            Height = 13
            BiDiMode = bdRightToLeft
            Caption = #44277#48177' '#44144#47532' '#49472
            ParentBiDiMode = False
          end
          object Label60: TLabel
            Left = 45
            Top = 3
            Width = 86
            Height = 13
            BiDiMode = bdLeftToRight
            Caption = 'AG/SL Series '#49444#51221
            ParentBiDiMode = False
          end
          object edHydro2Blank: TEdit
            Left = 80
            Top = 78
            Width = 90
            Height = 21
            Hint = 'rsetup.value09'
            ImeName = 'Microsoft Office IME 2007'
            TabOrder = 0
          end
          object edHydro2CellNo: TEdit
            Left = 80
            Top = 25
            Width = 90
            Height = 21
            Hint = 'rsetup.value07'
            ImeName = 'Microsoft Office IME 2007'
            NumbersOnly = True
            TabOrder = 1
          end
          object edHydro2CellSize: TEdit
            Left = 80
            Top = 52
            Width = 90
            Height = 21
            Hint = 'rsetup.value08'
            ImeName = 'Microsoft Office IME 2007'
            TabOrder = 2
          end
        end
        object cbxHydro2Type: TComboBox
          Left = 91
          Top = 15
          Width = 90
          Height = 21
          Hint = 'rsetup.value01|text'
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 0
          OnChange = cbxHydro2TypeChange
          Items.Strings = (
            'NONE'
            'ChannelMaster'
            'SL Series'
            'AG Series'
            'AccusonicA'
            'AccusonicB')
        end
        object cbxHydro2ComPort: TComboBox
          Left = 90
          Top = 42
          Width = 90
          Height = 21
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
        object cbxHydro2BaudRate: TComboBox
          Left = 90
          Top = 68
          Width = 90
          Height = 21
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
        object edHydro2Offset: TLabeledEdit
          Left = 90
          Top = 95
          Width = 90
          Height = 21
          Hint = 'rsetup.value04|text'
          Ctl3D = True
          EditLabel.Width = 31
          EditLabel.Height = 13
          EditLabel.Caption = 'Offset'
          ImeName = 'Microsoft Office IME 2007'
          LabelPosition = lpLeft
          MaxLength = 100
          ParentCtl3D = False
          TabOrder = 3
        end
        object cbxHydro2Reverse: TComboBox
          Left = 270
          Top = 128
          Width = 90
          Height = 21
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 8
          Items.Strings = (
            'ON'
            'OFF')
        end
        object cbxHydro2UpSide: TComboBox
          Left = 270
          Top = 152
          Width = 90
          Height = 21
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 9
          Items.Strings = (
            'UP'
            'SIDE')
        end
        object edHydro2Hz: TLabeledEdit
          Left = 90
          Top = 121
          Width = 90
          Height = 21
          Hint = 'rsetup.value04|text'
          Ctl3D = True
          EditLabel.Width = 55
          EditLabel.Height = 13
          EditLabel.Caption = #46041#51089#51452#54028#49688
          ImeName = 'Microsoft Office IME 2007'
          LabelPosition = lpLeft
          MaxLength = 100
          ParentCtl3D = False
          TabOrder = 4
        end
        object edHydro2SN: TLabeledEdit
          Left = 90
          Top = 147
          Width = 90
          Height = 21
          Hint = 'rsetup.value04|text'
          Ctl3D = True
          EditLabel.Width = 44
          EditLabel.Height = 13
          EditLabel.Caption = #51068#47144#48264#54840
          ImeName = 'Microsoft Office IME 2007'
          LabelPosition = lpLeft
          MaxLength = 100
          ParentCtl3D = False
          TabOrder = 5
        end
        object cbxHydro2IdleCheck: TComboBox
          Left = 270
          Top = 176
          Width = 42
          Height = 21
          Hint = 'rsetup.value02|text'
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 10
          Items.Strings = (
            'ON'
            'OFF')
        end
        object cbxHydro2IdleCheckTime: TComboBox
          Left = 318
          Top = 176
          Width = 42
          Height = 21
          Hint = 'rsetup.value02|text'
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 11
          Items.Strings = (
            '1 hour'
            '2 hour'
            '3 hour'
            '4 hour'
            '5 hour'
            '6 hour')
        end
        object edHydro2StartCell: TEdit
          Left = 90
          Top = 174
          Width = 40
          Height = 21
          TabOrder = 6
        end
        object edHydro2EndCell: TEdit
          Left = 140
          Top = 174
          Width = 40
          Height = 21
          TabOrder = 7
        end
      end
      object gbxHydroMeter3: TGroupBox
        Left = 390
        Top = 352
        Width = 382
        Height = 205
        Caption = '3'#48264#50976#49549#44228' '#49444#51221
        Ctl3D = True
        ParentCtl3D = False
        TabOrder = 7
        object Label91: TLabel
          Left = 28
          Top = 21
          Width = 58
          Height = 13
          BiDiMode = bdRightToLeft
          Caption = #51109#48708#51032' '#51333#47448
          ParentBiDiMode = False
        end
        object Label92: TLabel
          Left = 45
          Top = 46
          Width = 41
          Height = 13
          BiDiMode = bdRightToLeft
          Caption = 'Comport'
          ParentBiDiMode = False
        end
        object Label93: TLabel
          Left = 42
          Top = 74
          Width = 44
          Height = 13
          BiDiMode = bdRightToLeft
          Caption = #53685#49888#49549#46020
          ParentBiDiMode = False
        end
        object Label94: TLabel
          Left = 200
          Top = 129
          Width = 65
          Height = 13
          BiDiMode = bdRightToLeft
          Caption = 'Reverse '#50741#49496
          ParentBiDiMode = False
        end
        object Label95: TLabel
          Left = 203
          Top = 154
          Width = 62
          Height = 13
          BiDiMode = bdRightToLeft
          Caption = 'Up/Side '#50741#49496
          ParentBiDiMode = False
        end
        object Label96: TLabel
          Left = 215
          Top = 178
          Width = 50
          Height = 13
          BiDiMode = bdRightToLeft
          Caption = 'Idle Check'
          ParentBiDiMode = False
        end
        object Label97: TLabel
          Left = 31
          Top = 177
          Width = 55
          Height = 13
          Caption = #49472#50976#54952#48276#50948
        end
        object Label98: TLabel
          Left = 131
          Top = 178
          Width = 8
          Height = 13
          Caption = '~'
        end
        object Panel3: TPanel
          Left = 191
          Top = 13
          Width = 180
          Height = 110
          BevelInner = bvRaised
          BevelOuter = bvLowered
          TabOrder = 12
          object Label99: TLabel
            Left = 4
            Top = 28
            Width = 72
            Height = 13
            BiDiMode = bdRightToLeft
            Caption = #52769#51221' '#49472' '#52509#44060#49688
            ParentBiDiMode = False
          end
          object Label100: TLabel
            Left = 15
            Top = 52
            Width = 61
            Height = 13
            BiDiMode = bdRightToLeft
            Caption = #52769#51221' '#49472' '#53356#44592
            ParentBiDiMode = False
          end
          object Label101: TLabel
            Left = 15
            Top = 78
            Width = 61
            Height = 13
            BiDiMode = bdRightToLeft
            Caption = #44277#48177' '#44144#47532' '#49472
            ParentBiDiMode = False
          end
          object Label102: TLabel
            Left = 45
            Top = 3
            Width = 86
            Height = 13
            BiDiMode = bdLeftToRight
            Caption = 'AG/SL Series '#49444#51221
            ParentBiDiMode = False
          end
          object edHydro3Blank: TEdit
            Left = 80
            Top = 78
            Width = 90
            Height = 21
            Hint = 'rsetup.value09'
            ImeName = 'Microsoft Office IME 2007'
            TabOrder = 0
          end
          object edHydro3CellNo: TEdit
            Left = 80
            Top = 25
            Width = 90
            Height = 21
            Hint = 'rsetup.value07'
            ImeName = 'Microsoft Office IME 2007'
            NumbersOnly = True
            TabOrder = 1
          end
          object edHydro3CellSize: TEdit
            Left = 80
            Top = 52
            Width = 90
            Height = 21
            Hint = 'rsetup.value08'
            ImeName = 'Microsoft Office IME 2007'
            TabOrder = 2
          end
        end
        object cbxHydro3Type: TComboBox
          Left = 91
          Top = 15
          Width = 90
          Height = 21
          Hint = 'rsetup.value01|text'
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 0
          OnChange = cbxHydro3TypeChange
          Items.Strings = (
            'NONE'
            'ChannelMaster'
            'SL Series'
            'AG Series'
            'AccusonicA'
            'AccusonicB')
        end
        object cbxHydro3ComPort: TComboBox
          Left = 90
          Top = 42
          Width = 90
          Height = 21
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
        object cbxHydro3BaudRate: TComboBox
          Left = 90
          Top = 68
          Width = 90
          Height = 21
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
        object edHydro3Offset: TLabeledEdit
          Left = 90
          Top = 95
          Width = 90
          Height = 21
          Hint = 'rsetup.value04|text'
          Ctl3D = True
          EditLabel.Width = 31
          EditLabel.Height = 13
          EditLabel.Caption = 'Offset'
          ImeName = 'Microsoft Office IME 2007'
          LabelPosition = lpLeft
          MaxLength = 100
          ParentCtl3D = False
          TabOrder = 3
        end
        object cbxHydro3Reverse: TComboBox
          Left = 270
          Top = 128
          Width = 90
          Height = 21
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 8
          Items.Strings = (
            'ON'
            'OFF')
        end
        object cbxHydro3UpSide: TComboBox
          Left = 270
          Top = 152
          Width = 90
          Height = 21
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 9
          Items.Strings = (
            'UP'
            'SIDE')
        end
        object edHydro3Hz: TLabeledEdit
          Left = 90
          Top = 121
          Width = 90
          Height = 21
          Hint = 'rsetup.value04|text'
          Ctl3D = True
          EditLabel.Width = 55
          EditLabel.Height = 13
          EditLabel.Caption = #46041#51089#51452#54028#49688
          ImeName = 'Microsoft Office IME 2007'
          LabelPosition = lpLeft
          MaxLength = 100
          ParentCtl3D = False
          TabOrder = 4
        end
        object edHydro3SN: TLabeledEdit
          Left = 90
          Top = 147
          Width = 90
          Height = 21
          Hint = 'rsetup.value04|text'
          Ctl3D = True
          EditLabel.Width = 44
          EditLabel.Height = 13
          EditLabel.Caption = #51068#47144#48264#54840
          ImeName = 'Microsoft Office IME 2007'
          LabelPosition = lpLeft
          MaxLength = 100
          ParentCtl3D = False
          TabOrder = 5
        end
        object cbxHydro3IdleCheck: TComboBox
          Left = 270
          Top = 176
          Width = 42
          Height = 21
          Hint = 'rsetup.value02|text'
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 10
          Items.Strings = (
            'ON'
            'OFF')
        end
        object cbxHydro3IdleCheckTime: TComboBox
          Left = 318
          Top = 176
          Width = 42
          Height = 21
          Hint = 'rsetup.value02|text'
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 11
          Items.Strings = (
            '1 hour'
            '2 hour'
            '3 hour'
            '4 hour'
            '5 hour'
            '6 hour')
        end
        object edHydro3StartCell: TEdit
          Left = 90
          Top = 174
          Width = 40
          Height = 21
          TabOrder = 6
        end
        object edHydro3EndCell: TEdit
          Left = 140
          Top = 174
          Width = 40
          Height = 21
          TabOrder = 7
        end
      end
      object gbxWaterLevel: TGroupBox
        Left = 585
        Top = 59
        Width = 187
        Height = 287
        Caption = #49688#50948#44228' '#49444#51221
        Ctl3D = True
        ParentCtl3D = False
        TabOrder = 5
        object Label40: TLabel
          Left = 25
          Top = 23
          Width = 47
          Height = 13
          BiDiMode = bdRightToLeft
          Caption = #51109#48708' '#51333#47448
          ParentBiDiMode = False
        end
        object Label41: TLabel
          Left = 31
          Top = 48
          Width = 41
          Height = 13
          BiDiMode = bdRightToLeft
          Caption = 'Comport'
          ParentBiDiMode = False
        end
        object Label42: TLabel
          Left = 28
          Top = 74
          Width = 44
          Height = 13
          BiDiMode = bdRightToLeft
          Caption = #53685#49888#49549#46020
          ParentBiDiMode = False
        end
        object lbAtmosCap: TLabel
          Left = 17
          Top = 168
          Width = 55
          Height = 13
          BiDiMode = bdRightToLeft
          Caption = #45824#44592#50517#49468#49436
          ParentBiDiMode = False
          Visible = False
        end
        object Label77: TLabel
          Left = 22
          Top = 239
          Width = 50
          Height = 13
          BiDiMode = bdRightToLeft
          Caption = 'Idle Check'
          ParentBiDiMode = False
        end
        object lbAirmarCap: TLabel
          Left = 28
          Top = 217
          Width = 44
          Height = 13
          BiDiMode = bdRightToLeft
          Caption = #44592#49345#49468#49436
          ParentBiDiMode = False
        end
        object lbAtmosEnable: TLabel
          Left = 14
          Top = 145
          Width = 58
          Height = 13
          BiDiMode = bdRightToLeft
          Caption = ' '#45824#44592#50517#48372#51221
          ParentBiDiMode = False
        end
        object Label103: TLabel
          Left = 14
          Top = 263
          Width = 55
          Height = 13
          BiDiMode = bdRightToLeft
          Caption = #49688#50948#45936#51060#53552
          ParentBiDiMode = False
        end
        object cbxWaterType: TComboBox
          Left = 75
          Top = 19
          Width = 90
          Height = 21
          Style = csDropDownList
          Ctl3D = True
          ImeName = 'Microsoft Office IME 2007'
          ParentCtl3D = False
          TabOrder = 0
          OnChange = cbxWaterTypeChange
          Items.Strings = (
            'NONE'
            'AT200'
            'LT400+Atmos'
            'LT500'
            'OTTCBS'
            'RNSWATER')
        end
        object cbxWaterComPort: TComboBox
          Left = 75
          Top = 43
          Width = 90
          Height = 21
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
        object cbxWaterBaudRate: TComboBox
          Left = 75
          Top = 68
          Width = 90
          Height = 21
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
        object edWaterOffset: TLabeledEdit
          Left = 75
          Top = 93
          Width = 90
          Height = 21
          Hint = 'rsetup.value04|text'
          Ctl3D = True
          EditLabel.Width = 31
          EditLabel.Height = 13
          EditLabel.Caption = 'Offset'
          ImeName = 'Microsoft Office IME 2007'
          LabelPosition = lpLeft
          MaxLength = 100
          ParentCtl3D = False
          TabOrder = 3
        end
        object edWaterSN: TLabeledEdit
          Left = 75
          Top = 117
          Width = 90
          Height = 21
          Hint = 'rsetup.value04|text'
          Ctl3D = True
          EditLabel.Width = 44
          EditLabel.Height = 13
          EditLabel.Caption = #51068#47144#48264#54840
          ImeName = 'Microsoft Office IME 2007'
          LabelPosition = lpLeft
          MaxLength = 100
          ParentCtl3D = False
          TabOrder = 4
        end
        object cbxAtmosPort: TComboBox
          Left = 75
          Top = 165
          Width = 90
          Height = 21
          Hint = 'rsetup.value02|text'
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
          Left = 75
          Top = 189
          Width = 90
          Height = 21
          Hint = 'rsetup.value04|text'
          Ctl3D = True
          EditLabel.Width = 55
          EditLabel.Height = 13
          EditLabel.Caption = #44592#51456#45824#44592#50517
          ImeName = 'Microsoft Office IME 2007'
          LabelPosition = lpLeft
          MaxLength = 100
          ParentCtl3D = False
          TabOrder = 6
          Visible = False
        end
        object cbxWaterIdleCheck: TComboBox
          Left = 75
          Top = 236
          Width = 42
          Height = 21
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
        object cbxWaterIdleCheckTime: TComboBox
          Left = 126
          Top = 236
          Width = 42
          Height = 21
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
        object cxWaterAirmar: TCheckBox
          Left = 75
          Top = 215
          Width = 90
          Height = 17
          Hint = 'RnsWater '#49468#49436#51032' '#44592#49345#45936#51060#53552' '#49688#51665' '#50976#47924
          Caption = 'Enable'
          ParentShowHint = False
          ShowHint = True
          TabOrder = 9
          OnClick = cxWaterAirmarClick
        end
        object cxWaterAtmos: TCheckBox
          Left = 75
          Top = 142
          Width = 90
          Height = 17
          Hint = #45824#44592#50517#49468#49436#50640' '#51032#54620' '#49688#50948' '#48372#51221
          Caption = 'Enable'
          ParentShowHint = False
          ShowHint = True
          TabOrder = 10
          OnClick = cxWaterAtmosClick
        end
        object cbxWaterUseFlag: TComboBox
          Left = 75
          Top = 260
          Width = 94
          Height = 21
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
    end
    object TabSheetFlowStream: TTabSheet
      Caption = #50976#47049#49328#52636
      ImageIndex = 3
      DesignSize = (
        778
        590)
      object Label2: TLabel
        Left = 12
        Top = 19
        Width = 66
        Height = 13
        BiDiMode = bdRightToLeft
        Caption = #50976#47049#49328#52636#48169#49885
        ParentBiDiMode = False
      end
      object cbxFlowStreamType: TComboBox
        Left = 81
        Top = 14
        Width = 229
        Height = 21
        Hint = 'rsetup.value02|index'
        Style = csDropDownList
        Ctl3D = True
        ImeName = 'Microsoft Office IME 2007'
        ParentCtl3D = False
        TabOrder = 0
        OnChange = cbxFlowStreamTypeChange
        Items.Strings = (
          '0: '#50976#47049#49328#52636' OFF'
          '1: '#51648#54364#50976#49549#48277)
      end
      object cbSaveFlowStream: TButton
        Tag = 5
        Left = 700
        Top = 562
        Width = 75
        Height = 25
        Anchors = [akRight, akBottom]
        Caption = #51200#51109
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
        OnClick = cbSaveFlowStreamClick
      end
      object PageControlFlowStream: TPageControl
        Left = 6
        Top = 50
        Width = 667
        Height = 380
        ActivePage = tsIVM
        TabOrder = 2
        object tsADVM: TTabSheet
          Caption = #50976#49549#44228#51221#48372
          object Label1: TLabel
            Left = 7
            Top = 7
            Width = 185
            Height = 15
            AutoSize = False
            Caption = #54616#52789#50976#49549#44228#51221#48372
          end
          object Label3: TLabel
            Left = 320
            Top = 7
            Width = 201
            Height = 13
            AutoSize = False
            Caption = #49345#52789#50976#49549#44228#51221#48372
          end
          object Bevel3: TBevel
            Left = 7
            Top = 23
            Width = 238
            Height = 3
            Shape = bsBottomLine
          end
          object Bevel4: TBevel
            Left = 320
            Top = 23
            Width = 238
            Height = 3
            Shape = bsBottomLine
          end
          object Label6: TLabel
            Left = 7
            Top = 211
            Width = 149
            Height = 15
            Alignment = taRightJustify
            AutoSize = False
            Caption = #50976#49549#45800#50948
          end
          object Label7: TLabel
            Left = 7
            Top = 189
            Width = 149
            Height = 16
            Alignment = taRightJustify
            AutoSize = False
            Caption = #50669#48169#54693' '#50976#49549' '#49444#51221
          end
          object Label4: TLabel
            Left = 348
            Top = 187
            Width = 121
            Height = 16
            Alignment = taRightJustify
            AutoSize = False
            Caption = #50669#48169#54693' '#50976#49549' '#49444#51221
          end
          object Label5: TLabel
            Left = 348
            Top = 209
            Width = 121
            Height = 15
            Alignment = taRightJustify
            AutoSize = False
            Caption = #50976#49549#45800#50948
          end
          object Label8: TLabel
            Left = 559
            Top = 262
            Width = 10
            Height = 15
            AutoSize = False
            Caption = '~'
          end
          object Label9: TLabel
            Left = 559
            Top = 240
            Width = 10
            Height = 15
            AutoSize = False
            Caption = '~'
          end
          object Label10: TLabel
            Left = 247
            Top = 262
            Width = 10
            Height = 15
            AutoSize = False
            Caption = '~'
          end
          object Label11: TLabel
            Left = 247
            Top = 237
            Width = 10
            Height = 15
            AutoSize = False
            Caption = '~'
          end
          object Label12: TLabel
            Left = 341
            Top = 289
            Width = 121
            Height = 16
            Alignment = taRightJustify
            AutoSize = False
            Caption = #49444#52824#54805#53468
          end
          object Label19: TLabel
            Left = 248
            Top = 35
            Width = 71
            Height = 18
            AutoSize = False
            Caption = '(00.00~99.99)'
          end
          object Label20: TLabel
            Left = 248
            Top = 59
            Width = 78
            Height = 18
            AutoSize = False
            Caption = '(-99.99~99.99)'
          end
          object Label21: TLabel
            Left = 248
            Top = 83
            Width = 40
            Height = 18
            AutoSize = False
            Caption = '0~360)'
          end
          object Label23: TLabel
            Left = 248
            Top = 109
            Width = 40
            Height = 18
            AutoSize = False
            Caption = '(0~360)'
          end
          object Label29: TLabel
            Left = 248
            Top = 135
            Width = 62
            Height = 18
            AutoSize = False
            Caption = '(0.0~180.0)'
          end
          object Label30: TLabel
            Left = 248
            Top = 160
            Width = 78
            Height = 18
            AutoSize = False
            Caption = '(0.00~99.99)'
          end
          object Label31: TLabel
            Left = 564
            Top = 35
            Width = 75
            Height = 18
            AutoSize = False
            Caption = '(00.00~99.99)'
          end
          object Label32: TLabel
            Left = 564
            Top = 59
            Width = 78
            Height = 18
            AutoSize = False
            Caption = '(-99.99~99.99)'
          end
          object Label33: TLabel
            Left = 564
            Top = 83
            Width = 40
            Height = 18
            AutoSize = False
            Caption = '0~360)'
          end
          object Label43: TLabel
            Left = 564
            Top = 109
            Width = 40
            Height = 18
            AutoSize = False
            Caption = '(0~360)'
          end
          object Label44: TLabel
            Left = 564
            Top = 135
            Width = 62
            Height = 18
            AutoSize = False
            Caption = '(0.0~180.0)'
          end
          object Label45: TLabel
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
            TabOrder = 6
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
            TabOrder = 7
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
            TabOrder = 8
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
            TabOrder = 9
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
            TabOrder = 10
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
            TabOrder = 11
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
            TabOrder = 12
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
            TabOrder = 13
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
            TabOrder = 14
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
            TabOrder = 15
          end
          object AReverseMode: TComboBox
            Left = 162
            Top = 184
            Width = 83
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 16
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
            TabOrder = 17
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
            TabOrder = 18
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
            TabOrder = 19
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
            TabOrder = 20
          end
          object AEndCell: TEdit
            Left = 257
            Top = 262
            Width = 83
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 21
          end
          object BEndCell: TEdit
            Left = 570
            Top = 262
            Width = 83
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 22
          end
          object BVMax: TEdit
            Left = 570
            Top = 236
            Width = 83
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 23
          end
          object BType: TComboBox
            Left = 475
            Top = 288
            Width = 83
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
          object Label13: TLabel
            Left = 169
            Top = 20
            Width = 10
            Height = 15
            AutoSize = False
            Caption = '~'
          end
          object Label48: TLabel
            Left = 274
            Top = 19
            Width = 71
            Height = 18
            AutoSize = False
            Caption = '(00.00~99.99)'
          end
          object Label61: TLabel
            Left = 169
            Top = 46
            Width = 120
            Height = 18
            AutoSize = False
            Caption = '(-999.999 ~ 999.999)'
          end
          object Label62: TLabel
            Left = 169
            Top = 73
            Width = 120
            Height = 18
            AutoSize = False
            Caption = '(-999.999 ~ 999.999)'
          end
          object Label63: TLabel
            Left = 169
            Top = 100
            Width = 120
            Height = 18
            AutoSize = False
            Caption = '(-999.999 ~ 999.999)'
          end
          object Label64: TLabel
            Left = 169
            Top = 127
            Width = 120
            Height = 18
            AutoSize = False
            Caption = '(-999.999 ~ 999.999)'
          end
          object Label65: TLabel
            Left = 169
            Top = 154
            Width = 120
            Height = 18
            AutoSize = False
            Caption = '(-999.999 ~ 999.999)'
          end
          object Label66: TLabel
            Left = 169
            Top = 181
            Width = 120
            Height = 18
            AutoSize = False
            Caption = '(-999.999 ~ 999.999)'
          end
          object Label67: TLabel
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
            TabOrder = 1
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
            TabOrder = 2
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
            TabOrder = 3
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
            TabOrder = 4
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
            TabOrder = 5
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
            TabOrder = 6
          end
          object HAMaxH: TEdit
            Left = 185
            Top = 16
            Width = 83
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 7
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
          object Label14: TLabel
            Left = 10
            Top = 10
            Width = 185
            Height = 15
            AutoSize = False
            Caption = #54616#52789#50976#49549#44228#51221#48372
          end
          object Label15: TLabel
            Left = 316
            Top = 10
            Width = 201
            Height = 13
            AutoSize = False
            Caption = #49345#52789#50976#49549#44228#51221#48372
          end
          object Bevel5: TBevel
            Left = 316
            Top = 26
            Width = 285
            Height = 3
            Shape = bsBottomLine
          end
          object Bevel6: TBevel
            Left = 10
            Top = 26
            Width = 285
            Height = 3
            Shape = bsBottomLine
          end
          object Label16: TLabel
            Left = 505
            Top = 39
            Width = 12
            Height = 15
            AutoSize = False
            Caption = '~'
          end
          object Label17: TLabel
            Left = 198
            Top = 39
            Width = 8
            Height = 15
            AutoSize = False
            Caption = '~'
          end
          object Label68: TLabel
            Left = 198
            Top = 64
            Width = 120
            Height = 18
            AutoSize = False
            Caption = '(-999.9999 ~ 999.9999)'
          end
          object Label69: TLabel
            Left = 198
            Top = 88
            Width = 120
            Height = 18
            AutoSize = False
            Caption = '(-999.9999 ~ 999.9999)'
          end
          object Label70: TLabel
            Left = 198
            Top = 114
            Width = 120
            Height = 18
            AutoSize = False
            Caption = '(-999.9999 ~ 999.9999)'
          end
          object Label71: TLabel
            Left = 505
            Top = 62
            Width = 120
            Height = 18
            AutoSize = False
            Caption = '(-999.9999 ~ 999.9999)'
          end
          object Label72: TLabel
            Left = 505
            Top = 86
            Width = 120
            Height = 18
            AutoSize = False
            Caption = '(-999.9999 ~ 999.9999)'
          end
          object Label73: TLabel
            Left = 505
            Top = 112
            Width = 120
            Height = 18
            AutoSize = False
            Caption = '(-999.9999 ~ 999.9999)'
          end
          object Label74: TLabel
            Left = 5
            Top = 53
            Width = 96
            Height = 18
            AutoSize = False
            Caption = '(-99.999 ~ 99.999)'
          end
          object Label75: TLabel
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
            ImeName = 'Microsoft IME 2010'
            LabelPosition = lpLeft
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
            TabOrder = 1
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
            TabOrder = 2
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
            TabOrder = 3
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
            TabOrder = 4
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
            TabOrder = 5
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
            TabOrder = 6
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
            TabOrder = 7
          end
          object IVMAMaxVi: TEdit
            Left = 212
            Top = 35
            Width = 83
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 8
          end
          object IVMBMaxVi: TEdit
            Left = 518
            Top = 35
            Width = 83
            Height = 21
            ImeName = 'Microsoft IME 2010'
            TabOrder = 9
          end
        end
      end
    end
    object TabSheetEtc: TTabSheet
      Caption = #54260#45908'/'#54252#53944
      ImageIndex = 5
      DesignSize = (
        778
        590)
      object SpeedButton1: TSpeedButton
        Tag = 1
        Left = 526
        Top = 18
        Width = 23
        Height = 20
        Caption = '...'
        OnClick = SpeedButton1Click
      end
      object SpeedButton2: TSpeedButton
        Tag = 2
        Left = 526
        Top = 43
        Width = 23
        Height = 20
        Caption = '...'
        OnClick = SpeedButton1Click
      end
      object SpeedButton3: TSpeedButton
        Tag = 3
        Left = 526
        Top = 69
        Width = 23
        Height = 20
        Caption = '...'
        OnClick = SpeedButton1Click
      end
      object SpeedButton4: TSpeedButton
        Tag = 4
        Left = 526
        Top = 94
        Width = 23
        Height = 20
        Caption = '...'
        OnClick = SpeedButton1Click
      end
      object Label51: TLabel
        Left = 189
        Top = 225
        Width = 96
        Height = 13
        BiDiMode = bdLeftToRight
        Caption = '(FROM DataLogger)'
        ParentBiDiMode = False
      end
      object Label50: TLabel
        Left = 189
        Top = 251
        Width = 95
        Height = 13
        BiDiMode = bdLeftToRight
        Caption = '(FROM DataViewer)'
        ParentBiDiMode = False
      end
      object Label52: TLabel
        Left = 189
        Top = 277
        Width = 111
        Height = 13
        BiDiMode = bdLeftToRight
        Caption = '(FROM Logger/Sender)'
        ParentBiDiMode = False
      end
      object cbSaveEtc: TButton
        Tag = 5
        Left = 700
        Top = 562
        Width = 75
        Height = 25
        Anchors = [akRight, akBottom]
        Caption = #51200#51109
        TabOrder = 14
        OnClick = cbSaveEtcClick
      end
      object edRADSHome: TLabeledEdit
        Left = 116
        Top = 18
        Width = 409
        Height = 21
        Hint = 'rsetup.value01'
        Ctl3D = True
        EditLabel.Width = 66
        EditLabel.Height = 13
        EditLabel.Caption = 'RADS '#54856' '#54260#45908
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        ParentCtl3D = False
        TabOrder = 0
      end
      object edLoggerHome: TLabeledEdit
        Left = 116
        Top = 43
        Width = 409
        Height = 21
        Hint = 'rsetup.value02'
        Ctl3D = True
        EditLabel.Width = 70
        EditLabel.Height = 13
        EditLabel.Caption = 'DataLogger '#54856
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        ParentCtl3D = False
        TabOrder = 1
      end
      object edSenderHome: TLabeledEdit
        Left = 116
        Top = 69
        Width = 409
        Height = 21
        Hint = 'rsetup.value03'
        Ctl3D = True
        EditLabel.Width = 71
        EditLabel.Height = 13
        EditLabel.Caption = 'DataSender '#54856
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        ParentCtl3D = False
        TabOrder = 2
      end
      object edViewerHome: TLabeledEdit
        Left = 116
        Top = 94
        Width = 409
        Height = 21
        Hint = 'rsetup.value04'
        Ctl3D = True
        EditLabel.Width = 69
        EditLabel.Height = 13
        EditLabel.Caption = 'DataViewer '#54856
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        ParentCtl3D = False
        TabOrder = 3
      end
      object edLoggerIP: TLabeledEdit
        Left = 116
        Top = 120
        Width = 121
        Height = 21
        Hint = 'rsetup.value04'
        Ctl3D = True
        EditLabel.Width = 69
        EditLabel.Height = 13
        EditLabel.Caption = 'DataLogger IP'
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        ParentCtl3D = False
        TabOrder = 4
      end
      object edSenderIP: TLabeledEdit
        Left = 116
        Top = 146
        Width = 121
        Height = 21
        Hint = 'rsetup.value04'
        Ctl3D = True
        EditLabel.Width = 70
        EditLabel.Height = 13
        EditLabel.Caption = 'DataSender IP'
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        ParentCtl3D = False
        TabOrder = 5
      end
      object edDatabaseIP: TLabeledEdit
        Left = 116
        Top = 171
        Width = 121
        Height = 21
        Hint = 'rsetup.value04'
        Ctl3D = True
        EditLabel.Width = 59
        EditLabel.Height = 13
        EditLabel.Caption = 'Database IP'
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        ParentCtl3D = False
        TabOrder = 6
      end
      object edLoggerPort: TLabeledEdit
        Left = 116
        Top = 197
        Width = 67
        Height = 21
        Hint = 'rsetup.value04'
        Ctl3D = True
        EditLabel.Width = 79
        EditLabel.Height = 13
        EditLabel.Caption = 'DataLogger Port'
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        NumbersOnly = True
        ParentCtl3D = False
        TabOrder = 7
      end
      object edSenderLoggerPort: TLabeledEdit
        Left = 116
        Top = 222
        Width = 67
        Height = 21
        Hint = 'rsetup.value04'
        Ctl3D = True
        EditLabel.Width = 80
        EditLabel.Height = 13
        EditLabel.Caption = 'DataSender Port'
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        NumbersOnly = True
        ParentCtl3D = False
        TabOrder = 8
      end
      object edSenderViewerPort: TLabeledEdit
        Left = 116
        Top = 248
        Width = 67
        Height = 21
        Hint = 'rsetup.value04'
        Ctl3D = True
        EditLabel.Width = 80
        EditLabel.Height = 13
        EditLabel.Caption = 'DataSender Port'
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        NumbersOnly = True
        ParentCtl3D = False
        TabOrder = 9
      end
      object edWatchDogPort: TLabeledEdit
        Left = 116
        Top = 274
        Width = 67
        Height = 21
        Hint = 'rsetup.value04'
        Ctl3D = True
        EditLabel.Width = 73
        EditLabel.Height = 13
        EditLabel.Caption = 'WatchDog Port'
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpLeft
        MaxLength = 100
        NumbersOnly = True
        ParentCtl3D = False
        TabOrder = 10
      end
      object ButtonPing: TButton
        Tag = 5
        Left = 487
        Top = 420
        Width = 143
        Height = 25
        Anchors = [akRight, akBottom]
        Caption = 'Ping Test(DataSender)'
        TabOrder = 12
        OnClick = ButtonPingClick
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
        TabOrder = 11
      end
      object ButtonReboot: TButton
        Tag = 5
        Left = 487
        Top = 451
        Width = 143
        Height = 25
        Anchors = [akRight, akBottom]
        Caption = 'DataSender PC Reboot'
        TabOrder = 13
        OnClick = ButtonRebootClick
      end
    end
  end
  object BitBtn1: TBitBtn
    Left = 686
    Top = 624
    Width = 92
    Height = 30
    Anchors = [akRight, akBottom]
    Kind = bkOK
    NumGlyphs = 2
    TabOrder = 1
    OnClick = BitBtn1Click
  end
  object TimerPing: TTimer
    Enabled = False
    Interval = 5000
    OnTimer = TimerPingTimer
    Left = 576
    Top = 584
  end
end
