object FormDecode: TFormDecode
  Left = 346
  Top = 171
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
    Top = 0
    Width = 404
    Height = 341
    Align = alClient
    HideScrollBars = False
    ScrollBars = ssBoth
    TabOrder = 0
    WordWrap = False
  end
  object Panel1: TPanel
    Left = 0
    Top = 341
    Width = 404
    Height = 49
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 1
    object btnOK: TButton
      Left = 320
      Top = 8
      Width = 75
      Height = 25
      Caption = 'OK'
      ModalResult = 1
      TabOrder = 0
    end
  end
end
