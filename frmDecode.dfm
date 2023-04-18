object FormDecode: TFormDecode
  Left = 430
  Top = 244
  Width = 420
  Height = 429
  Caption = 'FormDecode'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object tbxMessage: TRichEdit
    Left = 0
    Top = 38
    Width = 412
    Height = 360
    Align = alClient
    HideScrollBars = False
    ScrollBars = ssBoth
    TabOrder = 0
    WordWrap = False
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 412
    Height = 38
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 1
    object Label1: TLabel
      Left = 16
      Top = 8
      Width = 46
      Height = 17
      Caption = 'Password'
    end
    object btnOK: TButton
      Left = 328
      Top = 8
      Width = 75
      Height = 25
      Caption = 'OK'
      ModalResult = 1
      TabOrder = 0
    end
    object RichEdit1: TRichEdit
      Left = 72
      Top = 8
      Width = 65
      Height = 25
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Consolas'
      Font.Pitch = fpFixed
      Font.Style = []
      MaxLength = 8
      ParentFont = False
      TabOrder = 1
    end
    object btnDecode: TButton
      Left = 144
      Top = 8
      Width = 57
      Height = 25
      Caption = 'Decode'
      TabOrder = 2
    end
  end
end
