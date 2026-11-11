object FormHydroMeterCell: TFormHydroMeterCell
  Left = 0
  Top = 0
  Caption = #49472#48324' '#50976#49549'/'#49888#54840#49464#44592
  ClientHeight = 418
  ClientWidth = 658
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  OnShow = FormShow
  DesignSize = (
    658
    418)
  PixelsPerInch = 96
  TextHeight = 13
  object sbtnDay: TSpeedButton
    Left = 482
    Top = 8
    Width = 39
    Height = 25
    Anchors = [akTop, akRight]
    GroupIndex = 1
    Caption = #51068
    OnClick = sbtnDayClick
  end
  object sbtnWeek: TSpeedButton
    Left = 525
    Top = 8
    Width = 39
    Height = 25
    Anchors = [akTop, akRight]
    GroupIndex = 1
    Caption = #51452
    OnClick = sbtnWeekClick
  end
  object sbtnMonth: TSpeedButton
    Left = 568
    Top = 8
    Width = 39
    Height = 25
    Anchors = [akTop, akRight]
    GroupIndex = 1
    Caption = #50900
    OnClick = sbtnMonthClick
  end
  object lblDisplay: TLabel
    Left = 8
    Top = 8
    Width = 177
    Height = 17
    AutoSize = False
    Caption = '2014.09.10 10:10:00'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object sbtnMinute: TSpeedButton
    Left = 439
    Top = 8
    Width = 39
    Height = 25
    Anchors = [akTop, akRight]
    GroupIndex = 1
    Down = True
    Caption = #48516
    OnClick = sbtnMinuteClick
  end
  object btnPrior: TButton
    Left = 396
    Top = 8
    Width = 39
    Height = 25
    Anchors = [akTop, akRight]
    Caption = '<<'
    TabOrder = 0
    OnClick = btnPriorClick
  end
  object Button5: TButton
    Left = 611
    Top = 8
    Width = 39
    Height = 25
    Anchors = [akTop, akRight]
    Caption = '>>'
    TabOrder = 1
    OnClick = Button5Click
  end
  object Chart: TChart
    Left = 0
    Top = 39
    Width = 658
    Height = 379
    AllowPanning = pmNone
    BackWall.Pen.Visible = False
    BottomWall.Brush.Gradient.EndColor = clSilver
    BottomWall.Brush.Gradient.StartColor = clGray
    BottomWall.Brush.Gradient.Visible = True
    BottomWall.Pen.Color = clGray
    BottomWall.Size = 4
    Gradient.Direction = gdFromTopLeft
    Gradient.EndColor = clWhite
    Gradient.StartColor = clSilver
    LeftWall.Brush.Gradient.EndColor = clSilver
    LeftWall.Brush.Gradient.StartColor = clGray
    LeftWall.Brush.Gradient.Visible = True
    LeftWall.Color = clWhite
    LeftWall.Pen.Color = clGray
    LeftWall.Size = 4
    Legend.Alignment = laTop
    Legend.TextStyle = ltsPlain
    Legend.TopPos = 0
    Title.Font.Color = clBlack
    Title.Font.Shadow.Color = clGray
    Title.Font.Shadow.HorizSize = 1
    Title.Font.Shadow.SmoothBlur = 2
    Title.Font.Shadow.VertSize = 1
    Title.Font.Shadow.Visible = False
    Title.Text.Strings = (
      #49472#48324' '#50976#49549'/'#49888#54840#49464#44592)
    Title.VertMargin = 0
    BottomAxis.Grid.Color = 14540253
    BottomAxis.Grid.SmallDots = True
    BottomAxis.Grid.SmallSpace = 1
    BottomAxis.LabelsFormat.Font.Color = clGray
    BottomAxis.LabelsFormat.Font.Height = -9
    BottomAxis.LabelsFormat.TextAlignment = taCenter
    BottomAxis.LabelStyle = talValue
    BottomAxis.Title.Caption = 'Cell'
    Chart3DPercent = 100
    ClipPoints = False
    DepthAxis.LabelsFormat.TextAlignment = taCenter
    DepthTopAxis.LabelsFormat.TextAlignment = taCenter
    Frame.Visible = False
    LeftAxis.Automatic = False
    LeftAxis.AutomaticMaximum = False
    LeftAxis.AutomaticMinimum = False
    LeftAxis.Grid.Color = 14540253
    LeftAxis.Grid.SmallSpace = 1
    LeftAxis.Increment = 100.000000000000000000
    LeftAxis.LabelsFormat.Font.Color = clGray
    LeftAxis.LabelsFormat.Font.Height = -9
    LeftAxis.LabelsFormat.TextAlignment = taCenter
    LeftAxis.LabelStyle = talValue
    LeftAxis.Maximum = 1500.000000000000000000
    LeftAxis.Title.Caption = #50976#49549
    LeftAxis.Title.Visible = False
    RightAxis.Automatic = False
    RightAxis.AutomaticMaximum = False
    RightAxis.AutomaticMinimum = False
    RightAxis.AxisValuesFormat = '#,##0'
    RightAxis.Grid.SmallSpace = 1
    RightAxis.Grid.Visible = False
    RightAxis.LabelsFormat.TextAlignment = taCenter
    RightAxis.LabelStyle = talValue
    RightAxis.Maximum = 255.000000000000000000
    RightAxis.Title.Caption = #49888#54840#49464#44592
    RightAxis.Title.Visible = False
    TopAxis.LabelsFormat.TextAlignment = taCenter
    TopAxis.Title.Caption = #49472#48324' '#49888#54840#49464#44592
    View3D = False
    View3DOptions.Elevation = 350
    View3DOptions.Orthogonal = False
    View3DOptions.Perspective = 55
    View3DOptions.Zoom = 90
    Zoom.Animated = True
    Zoom.Pen.Mode = pmNotXor
    Zoom.UpLeftZooms = True
    Align = alBottom
    BevelOuter = bvNone
    BevelWidth = 2
    Color = clWhite
    TabOrder = 2
    Anchors = [akLeft, akTop, akRight, akBottom]
    DesignSize = (
      658
      379)
    DefaultCanvas = 'TGDIPlusCanvas'
    ColorPaletteIndex = 13
    object StaticText2: TStaticText
      Left = 24
      Top = 39
      Width = 70
      Height = 17
      Caption = #50976#49549'(mm/sec)'
      TabOrder = 0
    end
    object StaticText3: TStaticText
      Left = 587
      Top = 39
      Width = 48
      Height = 17
      Anchors = [akTop, akRight]
      Caption = #49888#54840#49464#44592
      TabOrder = 1
    end
    object Series1: TAreaSeries
      LegendTitle = #50976#49549
      Marks.Visible = False
      SeriesColor = clRed
      AreaBrush = bsClear
      AreaChartBrush.Style = bsClear
      AreaChartBrush.BackColor = clDefault
      AreaLinesPen.Visible = False
      DrawArea = True
      Pointer.Brush.Gradient.EndColor = clRed
      Pointer.Gradient.EndColor = clRed
      Pointer.InflateMargins = True
      Pointer.Style = psCircle
      Pointer.Visible = True
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
      Data = {
        001A0000000000000000F886400000000000F485400000000000548540000000
        0000EC8340000000000058814000000000008081400000000000707C40000000
        0000487740000000000030714000000000001870400000000000207740000000
        0000387D400000000000907A400000000000B073400000000000C06740000000
        00008061400000000000B06D400000000000C067400000000000805B40000000
        0000C04C40000000000080414000000000002052400000000000A05E40000000
        0000C0674000000000004065400000000000001C40}
    end
    object Series2: TAreaSeries
      LegendTitle = #49888#54840'1'
      Marks.Visible = False
      SeriesColor = clBlue
      VertAxis = aRightAxis
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
      Data = {
        001A000000000000000000F03F00000000000000400000000000000840000000
        0000001040000000000000144000000000000018400000000000001C40000000
        000000204000000000000022400000000000000000000000000000F03F000000
        0000000040000000000000084000000000000010400000000000001440000000
        00000018400000000000001C4000000000000020400000000000002240000000
        0000000000000000000000F03F00000000000000400000000000000840000000
        000000104000000000000014400000000000001840}
    end
    object Series3: TAreaSeries
      LegendTitle = #49888#54840'2'
      Marks.Visible = False
      SeriesColor = clFuchsia
      VertAxis = aRightAxis
      AreaBrush = bsClear
      AreaChartBrush.Style = bsClear
      AreaChartBrush.BackColor = clDefault
      AreaLinesPen.Visible = False
      DrawArea = True
      Pointer.Brush.Gradient.EndColor = clFuchsia
      Pointer.Gradient.EndColor = clFuchsia
      Pointer.InflateMargins = True
      Pointer.Style = psCircle
      Pointer.Visible = True
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
      Data = {
        001A000000000000000000F03F00000000000000400000000000000840000000
        0000001040000000000000144000000000000018400000000000001C40000000
        000000204000000000000022400000000000000000000000000000F03F000000
        0000000040000000000000084000000000000010400000000000001440000000
        00000018400000000000001C4000000000000020400000000000002240000000
        0000000000000000000000F03F00000000000000400000000000000840000000
        000000104000000000000014400000000000001840}
    end
  end
  object query: TSimpleDataSet
    Aggregates = <>
    DataSet.MaxBlobSize = -1
    DataSet.Params = <>
    Params = <>
    Left = 160
    Top = 40
  end
end
