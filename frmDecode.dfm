object FormDecode: TFormDecode
  Left = 350
  Top = 186
  Width = 416
  Height = 429
  Caption = 'FormDecode'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  PixelsPerInch = 96
  TextHeight = 13
  object tbxMessage: TRichEdit
    Left = 0
    Top = 0
    Width = 408
    Height = 360
    Align = alClient
    HideScrollBars = False
    ScrollBars = ssBoth
    TabOrder = 0
    WordWrap = False
  end
  object Panel1: TPanel
    Left = 0
    Top = 360
    Width = 408
    Height = 38
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 1
    object btnOK: TButton
      Left = 328
      Top = 8
      Width = 75
      Height = 25
      Caption = 'OK'
      ModalResult = 1
      TabOrder = 0
    end
  end
end
