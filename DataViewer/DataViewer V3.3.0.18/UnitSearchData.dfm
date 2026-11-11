object FormSearchData: TFormSearchData
  Left = 0
  Top = 0
  Caption = #51088#47308#51312#54924
  ClientHeight = 626
  ClientWidth = 827
  Color = clBtnFace
  Font.Charset = ANSI_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Visible = True
  OnClose = FormClose
  OnCreate = FormCreate
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object PageControl2: TPageControl
    Left = 0
    Top = 312
    Width = 827
    Height = 314
    ActivePage = tsHydroMeter3
    Align = alBottom
    TabOrder = 3
    OnChange = PageControl2Change
    object tsHydroMeter1: TTabSheet
      Caption = 'tsHydroMeter1'
      ImageIndex = 1
      TabVisible = False
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object ChartHydroMeter1: TChart
        Left = 0
        Top = -11
        Width = 819
        Height = 315
        AllowPanning = pmNone
        Legend.Alignment = laTop
        Legend.TopPos = 0
        Legend.Visible = False
        MarginBottom = 10
        MarginLeft = 10
        MarginRight = 10
        MarginTop = 10
        MarginUnits = muPixels
        Title.Font.Color = clBlack
        Title.Text.Strings = (
          #50976#49549#44228)
        OnClickSeries = ChartHydroMeter1ClickSeries
        BottomAxis.Automatic = False
        BottomAxis.AutomaticMaximum = False
        BottomAxis.AutomaticMinimum = False
        BottomAxis.Grid.SmallDots = True
        BottomAxis.Grid.SmallSpace = 1
        BottomAxis.LabelsFormat.TextAlignment = taCenter
        BottomAxis.LabelStyle = talValue
        BottomAxis.Maximum = 24.000000000000000000
        BottomAxis.Title.Caption = #45216#51676'/'#49884#44036
        DepthAxis.LabelsFormat.TextAlignment = taCenter
        DepthTopAxis.LabelsFormat.TextAlignment = taCenter
        LeftAxis.Grid.SmallDots = True
        LeftAxis.Grid.SmallSpace = 1
        LeftAxis.LabelsFormat.TextAlignment = taCenter
        LeftAxis.LabelStyle = talValue
        RightAxis.LabelsFormat.TextAlignment = taCenter
        TopAxis.LabelsFormat.TextAlignment = taCenter
        View3D = False
        Zoom.Pen.Mode = pmNotXor
        OnGetAxisLabel = ChartWaterLevelGetAxisLabel
        Align = alBottom
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 0
        Anchors = [akLeft, akTop, akRight, akBottom]
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
        object stHydro1Y: TStaticText
          Left = 6
          Top = 10
          Width = 70
          Height = 17
          Caption = #50976#49549'(mm/sec)'
          TabOrder = 0
        end
        object AreaSeriesHydroMeter1: TAreaSeries
          LegendTitle = '1'#48264#50976#49549#44228
          Marks.Visible = False
          SeriesColor = clBlue
          AreaBrush = bsClear
          AreaChartBrush.Style = bsClear
          AreaChartBrush.BackColor = clDefault
          AreaLinesPen.Visible = False
          DrawArea = True
          Pointer.Brush.Gradient.EndColor = clBlue
          Pointer.Gradient.EndColor = clBlue
          Pointer.InflateMargins = True
          Pointer.Style = psCircle
          Pointer.Visible = True
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
    end
    object tsHydroMeter2: TTabSheet
      Caption = 'tsHydroMeter2'
      ImageIndex = 2
      TabVisible = False
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object ChartHydroMeter2: TChart
        Left = 0
        Top = -11
        Width = 819
        Height = 315
        AllowPanning = pmNone
        Legend.Alignment = laTop
        Legend.TopPos = 0
        Legend.Visible = False
        MarginBottom = 10
        MarginLeft = 10
        MarginRight = 10
        MarginTop = 10
        MarginUnits = muPixels
        Title.Font.Color = clBlack
        Title.Text.Strings = (
          #50976#49549#44228)
        OnClickSeries = ChartHydroMeter2ClickSeries
        BottomAxis.Automatic = False
        BottomAxis.AutomaticMaximum = False
        BottomAxis.AutomaticMinimum = False
        BottomAxis.Grid.SmallDots = True
        BottomAxis.Grid.SmallSpace = 1
        BottomAxis.LabelsFormat.TextAlignment = taCenter
        BottomAxis.LabelStyle = talValue
        BottomAxis.Maximum = 24.000000000000000000
        BottomAxis.Title.Caption = #45216#51676'/'#49884#44036
        DepthAxis.LabelsFormat.TextAlignment = taCenter
        DepthTopAxis.LabelsFormat.TextAlignment = taCenter
        LeftAxis.Grid.SmallDots = True
        LeftAxis.Grid.SmallSpace = 1
        LeftAxis.LabelsFormat.TextAlignment = taCenter
        LeftAxis.LabelStyle = talValue
        RightAxis.LabelsFormat.TextAlignment = taCenter
        TopAxis.LabelsFormat.TextAlignment = taCenter
        View3D = False
        Zoom.Pen.Mode = pmNotXor
        OnGetAxisLabel = ChartWaterLevelGetAxisLabel
        Align = alBottom
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 0
        Anchors = [akLeft, akTop, akRight, akBottom]
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
        object stHydro2Y: TStaticText
          Left = 6
          Top = 11
          Width = 70
          Height = 17
          Caption = #50976#49549'(mm/sec)'
          TabOrder = 0
        end
        object AreaSeriesHydroMeter2: TAreaSeries
          LegendTitle = '1'#48264#50976#49549#44228
          Marks.Visible = False
          SeriesColor = clBlue
          AreaBrush = bsClear
          AreaChartBrush.Style = bsClear
          AreaChartBrush.BackColor = clDefault
          AreaLinesPen.Visible = False
          DrawArea = True
          Pointer.Brush.Gradient.EndColor = clBlue
          Pointer.Gradient.EndColor = clBlue
          Pointer.InflateMargins = True
          Pointer.Style = psCircle
          Pointer.Visible = True
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
    end
    object tsHydroMeter3: TTabSheet
      Caption = 'tsHydroMeter3'
      ImageIndex = 5
      TabVisible = False
      object ChartHydroMeter3: TChart
        Left = 0
        Top = -11
        Width = 819
        Height = 315
        AllowPanning = pmNone
        Legend.Alignment = laTop
        Legend.TopPos = 0
        Legend.Visible = False
        MarginBottom = 10
        MarginLeft = 10
        MarginRight = 10
        MarginTop = 10
        MarginUnits = muPixels
        Title.Font.Color = clBlack
        Title.Text.Strings = (
          #50976#49549#44228)
        OnClickSeries = ChartHydroMeter3ClickSeries
        BottomAxis.Automatic = False
        BottomAxis.AutomaticMaximum = False
        BottomAxis.AutomaticMinimum = False
        BottomAxis.Grid.SmallDots = True
        BottomAxis.Grid.SmallSpace = 1
        BottomAxis.LabelsFormat.TextAlignment = taCenter
        BottomAxis.LabelStyle = talValue
        BottomAxis.Maximum = 24.000000000000000000
        BottomAxis.Title.Caption = #45216#51676'/'#49884#44036
        DepthAxis.LabelsFormat.TextAlignment = taCenter
        DepthTopAxis.LabelsFormat.TextAlignment = taCenter
        LeftAxis.Grid.SmallDots = True
        LeftAxis.Grid.SmallSpace = 1
        LeftAxis.LabelsFormat.TextAlignment = taCenter
        LeftAxis.LabelStyle = talValue
        RightAxis.LabelsFormat.TextAlignment = taCenter
        TopAxis.LabelsFormat.TextAlignment = taCenter
        View3D = False
        Zoom.Pen.Mode = pmNotXor
        OnGetAxisLabel = ChartWaterLevelGetAxisLabel
        Align = alBottom
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 0
        Anchors = [akLeft, akTop, akRight, akBottom]
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
        object stHydro3Y: TStaticText
          Left = 6
          Top = 11
          Width = 70
          Height = 17
          Caption = #50976#49549'(mm/sec)'
          TabOrder = 0
        end
        object AreaSeriesHydroMeter3: TAreaSeries
          LegendTitle = '1'#48264#50976#49549#44228
          Marks.Visible = False
          SeriesColor = clBlue
          AreaBrush = bsClear
          AreaChartBrush.Style = bsClear
          AreaChartBrush.BackColor = clDefault
          AreaLinesPen.Visible = False
          DrawArea = True
          Pointer.Brush.Gradient.EndColor = clBlue
          Pointer.Gradient.EndColor = clBlue
          Pointer.InflateMargins = True
          Pointer.Style = psCircle
          Pointer.Visible = True
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
    end
    object tsWaterLevel: TTabSheet
      Caption = 'tsWaterLevel'
      ImageIndex = 3
      TabVisible = False
      object ChartWaterLevel: TChart
        Left = 0
        Top = 0
        Width = 819
        Height = 304
        AllowPanning = pmNone
        Legend.Alignment = laBottom
        Legend.ColorWidth = 10
        Legend.LegendStyle = lsSeries
        Legend.Symbol.Pen.SmallSpace = 1
        Legend.Symbol.Width = 10
        Legend.Symbol.WidthUnits = lcsPixels
        Legend.TopPos = 0
        MarginBottom = 3
        MarginTop = 3
        Title.Font.Color = clBlack
        Title.Text.Strings = (
          #49688#50948#44228)
        BottomAxis.Automatic = False
        BottomAxis.AutomaticMaximum = False
        BottomAxis.AutomaticMinimum = False
        BottomAxis.Grid.SmallDots = True
        BottomAxis.Grid.SmallSpace = 1
        BottomAxis.LabelsFormat.TextAlignment = taCenter
        BottomAxis.LabelStyle = talValue
        BottomAxis.Maximum = 24.000000000000000000
        BottomAxis.Title.Caption = #45216#51676'/'#49884#44036
        DepthAxis.LabelsFormat.TextAlignment = taCenter
        DepthTopAxis.LabelsFormat.TextAlignment = taCenter
        LeftAxis.Automatic = False
        LeftAxis.AutomaticMaximum = False
        LeftAxis.AutomaticMinimum = False
        LeftAxis.Axis.SmallSpace = 1
        LeftAxis.Grid.SmallDots = True
        LeftAxis.Grid.SmallSpace = 1
        LeftAxis.Increment = 1.000000000000000000
        LeftAxis.LabelsFormat.TextAlignment = taCenter
        LeftAxis.Maximum = 10.000000000000000000
        LeftAxis.Minimum = -1.000000000000000000
        LeftAxis.Title.Caption = #49688#50948'(M)'
        RightAxis.Automatic = False
        RightAxis.AutomaticMaximum = False
        RightAxis.AutomaticMinimum = False
        RightAxis.Grid.Visible = False
        RightAxis.LabelsFormat.TextAlignment = taCenter
        RightAxis.Maximum = 15.000000000000000000
        RightAxis.Ticks.SmallSpace = 1
        RightAxis.Title.Caption = #54413#49549'(m/s)'
        TopAxis.LabelsFormat.TextAlignment = taCenter
        View3D = False
        Zoom.Pen.Mode = pmNotXor
        OnGetAxisLabel = ChartWaterLevelGetAxisLabel
        Align = alClient
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 0
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
        object StaticText1: TStaticText
          Left = 6
          Top = 7
          Width = 42
          Height = 17
          Caption = #49688#50948'(m)'
          TabOrder = 0
        end
        object AreaSeriesWaterLevel: TAreaSeries
          Marks.Visible = False
          SeriesColor = clBlue
          Title = #49688#50948
          AreaBrush = bsClear
          AreaChartBrush.Style = bsClear
          AreaChartBrush.BackColor = clDefault
          AreaLinesPen.Visible = False
          DrawArea = True
          Pointer.Brush.Gradient.EndColor = clBlue
          Pointer.Gradient.EndColor = clBlue
          Pointer.HorizSize = 3
          Pointer.InflateMargins = True
          Pointer.Style = psCircle
          Pointer.VertSize = 3
          Pointer.Visible = True
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
          Data = {
            0019000000000000000000F03F000000000000F03F000000000000F03F000000
            000000F03F000000000000F03F000000000000F03F000000000000F03F000000
            000000F03F000000000000F03F000000000000F03F000000000000F03F000000
            000000F03F000000000000F03F000000000000F03F000000000000F03F000000
            000000F03F000000000000F03F000000000000F03F000000000000F03F000000
            000000F03F000000000000F03F000000000000F03F000000000000F03F000000
            000000F03F000000000000F03F}
        end
        object AreaSeriesWindSpeed: TAreaSeries
          Marks.Visible = False
          SeriesColor = clRed
          Title = #54413#49549
          VertAxis = aRightAxis
          AreaBrush = bsClear
          AreaChartBrush.Style = bsClear
          AreaChartBrush.BackColor = clDefault
          AreaLinesPen.Visible = False
          DrawArea = True
          Pointer.Brush.Gradient.EndColor = clRed
          Pointer.Gradient.EndColor = clRed
          Pointer.HorizSize = 3
          Pointer.InflateMargins = True
          Pointer.Style = psCircle
          Pointer.VertSize = 3
          Pointer.Visible = True
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
          Data = {
            0019000000000000000000004000000000000000400000000000000040000000
            0000000040000000000000004000000000000000400000000000000040000000
            0000000040000000000000004000000000000000400000000000000040000000
            0000000040000000000000004000000000000000400000000000000040000000
            0000000040000000000000004000000000000000400000000000000040000000
            0000000040000000000000004000000000000000400000000000000040000000
            00000000400000000000000040}
        end
        object AreaSeriesWindGust: TAreaSeries
          Marks.Visible = False
          SeriesColor = clBlack
          Title = #52572#45824#54413#49549
          VertAxis = aRightAxis
          AreaBrush = bsClear
          AreaChartBrush.Style = bsClear
          AreaChartBrush.BackColor = clDefault
          AreaLinesPen.Visible = False
          DrawArea = True
          Pointer.Brush.Gradient.EndColor = clBlack
          Pointer.Gradient.EndColor = clBlack
          Pointer.HorizSize = 3
          Pointer.InflateMargins = True
          Pointer.Style = psCircle
          Pointer.VertSize = 3
          Pointer.Visible = True
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
          Data = {
            0019000000000000000000084000000000000008400000000000000840000000
            0000000840000000000000084000000000000008400000000000000840000000
            0000000840000000000000084000000000000008400000000000000840000000
            0000000840000000000000084000000000000008400000000000000840000000
            0000000840000000000000084000000000000008400000000000000840000000
            0000000840000000000000084000000000000008400000000000000840000000
            00000008400000000000000840}
        end
      end
    end
    object tsVTHLogger: TTabSheet
      Caption = 'tsVTHLogger'
      ImageIndex = 4
      TabVisible = False
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object ChartVTHLogger: TChart
        Left = 0
        Top = 4
        Width = 819
        Height = 300
        AllowPanning = pmNone
        Legend.Alignment = laTop
        Legend.TopPos = 0
        Title.Font.Color = clBlack
        Title.Text.Strings = (
          'VTHLogger')
        BottomAxis.LabelsFormat.TextAlignment = taCenter
        BottomAxis.Title.Caption = #45216#51676'/'#49884#44036
        DepthAxis.LabelsFormat.TextAlignment = taCenter
        DepthTopAxis.LabelsFormat.TextAlignment = taCenter
        LeftAxis.Automatic = False
        LeftAxis.AutomaticMaximum = False
        LeftAxis.AutomaticMinimum = False
        LeftAxis.Increment = 2.000000000000000000
        LeftAxis.LabelsFormat.TextAlignment = taCenter
        LeftAxis.Maximum = 20.000000000000000000
        Panning.MouseWheel = pmwNone
        RightAxis.Automatic = False
        RightAxis.AutomaticMaximum = False
        RightAxis.AutomaticMinimum = False
        RightAxis.Grid.Visible = False
        RightAxis.Increment = 10.000000000000000000
        RightAxis.LabelsFormat.TextAlignment = taCenter
        RightAxis.Maximum = 80.000000000000000000
        RightAxis.Minimum = -10.000000000000000000
        TopAxis.LabelsFormat.TextAlignment = taCenter
        View3D = False
        Zoom.Pen.Mode = pmNotXor
        OnGetAxisLabel = ChartWaterLevelGetAxisLabel
        Align = alBottom
        Color = clWhite
        TabOrder = 0
        Anchors = [akLeft, akTop, akRight, akBottom]
        DesignSize = (
          819
          300)
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
        object StaticText5: TStaticText
          Left = 24
          Top = 32
          Width = 39
          Height = 17
          Caption = 'DC Volt'
          TabOrder = 0
        end
        object StaticText6: TStaticText
          Left = 882
          Top = 48
          Width = 26
          Height = 17
          Anchors = [akTop, akRight]
          Caption = #50728#46020
          TabOrder = 1
        end
        object StaticText7: TStaticText
          Left = 743
          Top = 32
          Width = 26
          Height = 17
          Anchors = [akTop, akRight]
          Caption = #50728#46020
          TabOrder = 2
        end
        object SeriesDCCharge: TLineSeries
          LegendTitle = 'DC Charge'
          Marks.Visible = False
          SeriesColor = clBlue
          Brush.BackColor = clDefault
          Pointer.Brush.Gradient.EndColor = clBlue
          Pointer.Gradient.EndColor = clBlue
          Pointer.InflateMargins = True
          Pointer.Style = psCircle
          Pointer.Visible = True
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object SeriesDCBattery: TLineSeries
          LegendTitle = 'DC Battery'
          Marks.Visible = False
          SeriesColor = clRed
          Brush.BackColor = clDefault
          Pointer.Brush.Gradient.EndColor = clRed
          Pointer.Gradient.EndColor = clRed
          Pointer.InflateMargins = True
          Pointer.Style = psCircle
          Pointer.Visible = True
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object SeriesTemperature: TLineSeries
          LegendTitle = #50728#46020
          Marks.Visible = False
          SeriesColor = clFuchsia
          VertAxis = aRightAxis
          Brush.BackColor = clDefault
          Pointer.Brush.Gradient.EndColor = clFuchsia
          Pointer.Gradient.EndColor = clFuchsia
          Pointer.InflateMargins = True
          Pointer.Style = psCircle
          Pointer.Visible = True
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
    end
    object tsFlowStream: TTabSheet
      Caption = 'tsFlowStream'
      TabVisible = False
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object ChartFlowStream: TChart
        Left = 0
        Top = -1
        Width = 819
        Height = 305
        AllowPanning = pmNone
        Legend.Visible = False
        MarginBottom = 10
        MarginLeft = 10
        MarginRight = 10
        MarginTop = 10
        MarginUnits = muPixels
        Title.Font.Color = clBlack
        Title.Text.Strings = (
          #50976#47049)
        BottomAxis.Automatic = False
        BottomAxis.AutomaticMaximum = False
        BottomAxis.AutomaticMinimum = False
        BottomAxis.Grid.SmallDots = True
        BottomAxis.Grid.SmallSpace = 1
        BottomAxis.LabelsFormat.TextAlignment = taCenter
        BottomAxis.LabelStyle = talValue
        BottomAxis.Maximum = 24.000000000000000000
        BottomAxis.Title.Caption = #45216#51676'/'#49884#44036
        DepthAxis.LabelsFormat.TextAlignment = taCenter
        DepthTopAxis.LabelsFormat.TextAlignment = taCenter
        LeftAxis.Grid.SmallDots = True
        LeftAxis.Grid.SmallSpace = 1
        LeftAxis.LabelsFormat.TextAlignment = taCenter
        LeftAxis.LabelStyle = talValue
        RightAxis.LabelsFormat.TextAlignment = taCenter
        TopAxis.LabelsFormat.TextAlignment = taCenter
        View3D = False
        Zoom.Pen.Mode = pmNotXor
        OnGetAxisLabel = ChartWaterLevelGetAxisLabel
        Align = alBottom
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 0
        Anchors = [akLeft, akTop, akRight, akBottom]
        DefaultCanvas = 'TGDIPlusCanvas'
        PrintMargins = (
          15
          22
          15
          22)
        ColorPaletteIndex = 13
        object StaticText4: TStaticText
          Left = 6
          Top = 7
          Width = 68
          Height = 17
          Caption = #50976#47049'(m3/sec)'
          TabOrder = 0
        end
        object SeriesFlowStream: TAreaSeries
          Marks.Visible = False
          SeriesColor = clBlue
          AreaBrush = bsClear
          AreaChartBrush.Style = bsClear
          AreaChartBrush.BackColor = clDefault
          AreaLinesPen.Visible = False
          DrawArea = True
          Pointer.Brush.Gradient.EndColor = clBlue
          Pointer.Gradient.EndColor = clBlue
          Pointer.InflateMargins = True
          Pointer.Style = psCircle
          Pointer.Visible = True
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
    end
  end
  object PageControl1: TPageControl
    Left = 0
    Top = 41
    Width = 827
    Height = 22
    ActivePage = TabSheet5
    Align = alTop
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    TabWidth = 60
    OnChange = PageControl1Change
    object TabSheet2: TTabSheet
      Caption = '1'#48264#50976#49549#44228
      ImageIndex = 1
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
    end
    object TabSheet3: TTabSheet
      Caption = '2'#48264#50976#49549#44228
      ImageIndex = 2
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
    end
    object TabSheet5: TTabSheet
      Caption = '3'#48264#50976#49549#44228
      ImageIndex = 5
    end
    object TabSheet4: TTabSheet
      Caption = #49688#50948'/'#54413#49549
      ImageIndex = 3
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
    end
    object TabSheet9: TTabSheet
      Caption = 'VTH'
      ImageIndex = 4
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
    end
    object TabSheet1: TTabSheet
      Caption = #50976#47049
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
    end
  end
  object pnl2: TPanel
    Left = 0
    Top = 271
    Width = 827
    Height = 41
    Align = alBottom
    TabOrder = 2
    DesignSize = (
      827
      41)
    object sbtnDay: TSpeedButton
      Left = 648
      Top = 8
      Width = 39
      Height = 25
      Anchors = [akTop, akRight]
      GroupIndex = 1
      Down = True
      Caption = #51068
      OnClick = sbtnDayClick
      ExplicitLeft = 641
    end
    object sbtnWeek: TSpeedButton
      Left = 691
      Top = 8
      Width = 39
      Height = 25
      Anchors = [akTop, akRight]
      GroupIndex = 1
      Caption = #51452
      OnClick = sbtnWeekClick
      ExplicitLeft = 684
    end
    object sbtnMonth: TSpeedButton
      Left = 734
      Top = 8
      Width = 39
      Height = 25
      Anchors = [akTop, akRight]
      GroupIndex = 1
      Caption = #50900
      OnClick = sbtnMonthClick
      ExplicitLeft = 647
    end
    object lblDisplay: TLabel
      Left = 10
      Top = 14
      Width = 26
      Height = 13
      Caption = '   ~   '
    end
    object btnPrior: TButton
      Left = 605
      Top = 8
      Width = 39
      Height = 25
      Anchors = [akTop, akRight]
      Caption = '<<'
      TabOrder = 0
      OnClick = btnPriorClick
    end
    object btnNext: TButton
      Left = 777
      Top = 8
      Width = 39
      Height = 25
      Anchors = [akTop, akRight]
      Caption = '>>'
      TabOrder = 1
      OnClick = btnNextClick
    end
    object rbH1Velocity: TRadioButton
      Left = 493
      Top = 12
      Width = 50
      Height = 17
      Hint = #50976#49549#44228#51032' '#50976#49549' Chart'
      Anchors = [akTop, akRight]
      Caption = #50976#49549
      Checked = True
      ParentShowHint = False
      ShowHint = True
      TabOrder = 2
      TabStop = True
      OnClick = rbH1VelocityClick
    end
    object rbH1WaterLevel: TRadioButton
      Left = 549
      Top = 12
      Width = 50
      Height = 17
      Hint = #50976#49549#44228'(RQ-30d)'#51032' '#49688#50948' Chart'
      Anchors = [akTop, akRight]
      Caption = #49688#50948
      ParentShowHint = False
      ShowHint = True
      TabOrder = 3
      OnClick = rbH1WaterLevelClick
    end
  end
  object pnl1: TPanel
    Left = 0
    Top = 0
    Width = 827
    Height = 41
    Align = alTop
    TabOrder = 0
    DesignSize = (
      827
      41)
    object Label1: TLabel
      Left = 12
      Top = 11
      Width = 24
      Height = 13
      Caption = 'From'
    end
    object Label2: TLabel
      Left = 208
      Top = 11
      Width = 12
      Height = 13
      Caption = 'To'
    end
    object DateTimePicker1: TDateTimePicker
      Left = 40
      Top = 8
      Width = 81
      Height = 21
      Date = 41752.908747418980000000
      Time = 41752.908747418980000000
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 0
    end
    object DateTimePicker2: TDateTimePicker
      Left = 226
      Top = 8
      Width = 81
      Height = 21
      Date = 41752.908747418980000000
      Time = 41752.908747418980000000
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 1
    end
    object cbnQuery: TButton
      Left = 389
      Top = 6
      Width = 75
      Height = 25
      Caption = #51312#54924
      TabOrder = 2
      OnClick = cbnQueryClick
    end
    object se1Minute: TSpinEdit
      Left = 157
      Top = 8
      Width = 39
      Height = 22
      Hint = #48516
      EditorEnabled = False
      Increment = 10
      MaxLength = 2
      MaxValue = 50
      MinValue = 0
      ParentShowHint = False
      ShowHint = True
      TabOrder = 3
      Value = 0
    end
    object se1Hour: TSpinEdit
      Left = 119
      Top = 8
      Width = 39
      Height = 22
      Hint = #49884#44036
      EditorEnabled = False
      MaxLength = 2
      MaxValue = 23
      MinValue = 0
      ParentShowHint = False
      ShowHint = True
      TabOrder = 4
      Value = 0
    end
    object se2Hour: TSpinEdit
      Left = 306
      Top = 8
      Width = 39
      Height = 22
      Hint = #49884#44036
      EditorEnabled = False
      MaxLength = 2
      MaxValue = 23
      MinValue = 0
      ParentShowHint = False
      ShowHint = True
      TabOrder = 5
      Value = 0
    end
    object se2Minute: TSpinEdit
      Left = 344
      Top = 8
      Width = 39
      Height = 22
      Hint = #48516
      EditorEnabled = False
      Increment = 10
      MaxLength = 2
      MaxValue = 50
      MinValue = 0
      ParentShowHint = False
      ShowHint = True
      TabOrder = 6
      Value = 0
    end
    object cbnExport: TButton
      Left = 741
      Top = 6
      Width = 75
      Height = 25
      Anchors = [akTop, akRight]
      Caption = 'Export'
      TabOrder = 7
      OnClick = cbnExportClick
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 63
    Width = 827
    Height = 208
    Align = alClient
    Caption = 'Panel1'
    TabOrder = 4
    object DBGrid: TDBGrid
      Left = 1
      Top = 1
      Width = 825
      Height = 206
      Align = alClient
      DataSource = DataSource1
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Tahoma'
      Font.Style = []
      ImeName = 'Microsoft Office IME 2007'
      Options = [dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgTabs, dgRowSelect, dgConfirmDelete, dgCancelOnExit, dgTitleClick, dgTitleHotTrack]
      ParentFont = False
      TabOrder = 0
      TitleFont.Charset = ANSI_CHARSET
      TitleFont.Color = clWindowText
      TitleFont.Height = -11
      TitleFont.Name = 'Tahoma'
      TitleFont.Style = []
      OnCellClick = DBGridCellClick
    end
  end
  object query: TSimpleDataSet
    Aggregates = <>
    DataSet.MaxBlobSize = -1
    DataSet.Params = <>
    Params = <>
    Left = 680
    Top = 40
  end
  object DataSource1: TDataSource
    DataSet = queryMaster
    Left = 620
    Top = 41
  end
  object queryMaster: TSimpleDataSet
    Aggregates = <>
    DataSet.MaxBlobSize = -1
    DataSet.Params = <>
    Params = <>
    Left = 568
    Top = 40
  end
  object SaveDialog: TSaveDialog
    DefaultExt = '.csv'
    Filter = 'csv file|*.csv'
    Left = 736
    Top = 40
  end
end
