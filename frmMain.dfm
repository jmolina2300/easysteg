object FormMain: TFormMain
  Left = 347
  Top = 136
  Width = 849
  Height = 535
  Caption = 'Easysteg v0.0.0'
  Color = clBtnFace
  Constraints.MinHeight = 400
  Constraints.MinWidth = 400
  Font.Charset = ANSI_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  KeyPreview = True
  Menu = MainMenu1
  OldCreateOrder = False
  Position = poScreenCenter
  PrintScale = poPrintToFit
  OnCreate = FormCreate
  OnKeyDown = FormKeyDown
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 841
    Height = 201
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 0
    object Label1: TLabel
      Left = 24
      Top = 16
      Width = 16
      Height = 13
      Caption = 'File'
    end
    object Label2: TLabel
      Left = 16
      Top = 64
      Width = 32
      Height = 13
      Caption = 'Details'
    end
    object btnOpen: TButton
      Left = 304
      Top = 16
      Width = 75
      Height = 25
      Caption = 'Open'
      TabOrder = 0
      OnClick = btnOpenClick
    end
    object tbxFileInfo: TRichEdit
      Left = 64
      Top = 64
      Width = 313
      Height = 129
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Consolas'
      Font.Style = []
      ParentFont = False
      ReadOnly = True
      ScrollBars = ssHorizontal
      TabOrder = 1
      WordWrap = False
    end
    object tbxFileName: TEdit
      Left = 64
      Top = 16
      Width = 233
      Height = 21
      TabOrder = 2
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 201
    Width = 841
    Height = 283
    Align = alClient
    BevelOuter = bvNone
    Caption = 'Panel2'
    TabOrder = 1
    object DBChart1: TDBChart
      Left = 0
      Top = 25
      Width = 841
      Height = 258
      AllowPanning = pmNone
      BackWall.Brush.Color = clWhite
      BackWall.Brush.Style = bsClear
      Title.Text.Strings = (
        'TDBChart')
      Title.Visible = False
      LeftAxis.Automatic = False
      LeftAxis.AutomaticMaximum = False
      LeftAxis.AutomaticMinimum = False
      LeftAxis.Maximum = 32768
      LeftAxis.Minimum = -32768
      Legend.Visible = False
      View3D = False
      Align = alClient
      BevelOuter = bvLowered
      TabOrder = 0
      object Series1: TLineSeries
        Marks.ArrowLength = 8
        Marks.Visible = False
        SeriesColor = clRed
        Pointer.InflateMargins = True
        Pointer.Style = psRectangle
        Pointer.Visible = False
        XValues.DateTime = False
        XValues.Name = 'X'
        XValues.Multiplier = 1
        XValues.Order = loAscending
        YValues.DateTime = False
        YValues.Name = 'Y'
        YValues.Multiplier = 1
        YValues.Order = loNone
      end
    end
    object ScrollBox1: TScrollBox
      Left = 0
      Top = 25
      Width = 841
      Height = 258
      HorzScrollBar.Tracking = True
      VertScrollBar.ThumbSize = 12
      VertScrollBar.Tracking = True
      Align = alClient
      BevelInner = bvNone
      BevelOuter = bvNone
      DockSite = True
      Color = clMedGray
      ParentColor = False
      TabOrder = 1
      object StegImage: TImage
        Left = 0
        Top = 0
        Width = 534
        Height = 537
        AutoSize = True
        ParentShowHint = False
        ShowHint = False
      end
    end
    object Panel3: TPanel
      Left = 0
      Top = 0
      Width = 841
      Height = 25
      Align = alTop
      BevelOuter = bvNone
      BorderStyle = bsSingle
      TabOrder = 2
      object Label3: TLabel
        Left = 8
        Top = 4
        Width = 70
        Height = 13
        Caption = 'Image Preview'
      end
      object CheckBox1: TCheckBox
        Left = 120
        Top = 4
        Width = 97
        Height = 17
        Caption = 'Zoom Fit'
        TabOrder = 0
        OnClick = CheckBox1Click
      end
    end
  end
  object OpenDialog1: TOpenDialog
    Filter = 'Bitmap Files (*.bmp)|*.*|WAV Files (*.wav)|*.wav'
    FilterIndex = 0
    Left = 392
    Top = 16
  end
  object MainMenu1: TMainMenu
    Left = 16
    Top = 152
    object File1: TMenuItem
      Caption = 'File'
    end
    object Actions1: TMenuItem
      Caption = 'Actions'
      object btnEncode: TMenuItem
        Caption = 'Encode'
        OnClick = btnEncodeClick
      end
      object btnDecode: TMenuItem
        Caption = 'Decode'
        OnClick = btnDecodeClick
      end
    end
  end
end
