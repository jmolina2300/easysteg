object FormGetKey: TFormGetKey
  Left = 413
  Top = 134
  Width = 240
  Height = 200
  Caption = 'Decode'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label2: TLabel
    Left = 88
    Top = 32
    Width = 45
    Height = 13
    Caption = 'Enter key'
  end
  object tbxKey: TRichEdit
    Left = 80
    Top = 56
    Width = 65
    Height = 25
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Consolas'
    Font.Style = []
    Lines.Strings = (
      'AAAAAAAA')
    MaxLength = 8
    ParentFont = False
    TabOrder = 0
    OnKeyPress = tbxKeyKeyPress
  end
  object btnOK: TButton
    Left = 72
    Top = 96
    Width = 83
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 1
    OnClick = btnOKClick
  end
end
