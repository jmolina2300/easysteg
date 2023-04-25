object FormEncode: TFormEncode
  Left = 290
  Top = 134
  BorderStyle = bsSingle
  Caption = 'Encode'
  ClientHeight = 310
  ClientWidth = 408
  Color = clBtnFace
  ParentFont = True
  KeyPreview = True
  OldCreateOrder = True
  Position = poOwnerFormCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 24
    Top = 24
    Width = 77
    Height = 13
    Caption = 'Secret Message'
  end
  object Label2: TLabel
    Left = 312
    Top = 48
    Width = 74
    Height = 13
    Caption = 'Bytes remaining'
  end
  object lblBytesLeft: TLabel
    Left = 352
    Top = 72
    Width = 6
    Height = 13
    Caption = '0'
  end
  object Label3: TLabel
    Left = 208
    Top = 280
    Width = 18
    Height = 13
    Caption = 'Key'
  end
  object btnOK: TButton
    Left = 316
    Top = 240
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    TabOrder = 0
    OnClick = btnOKClick
  end
  object btnCancel: TButton
    Left = 316
    Top = 270
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 1
  end
  object tbxMessage: TRichEdit
    Left = 24
    Top = 40
    Width = 273
    Height = 225
    HideScrollBars = False
    ScrollBars = ssVertical
    TabOrder = 2
    OnChange = tbxMessageChange
  end
  object tbxKey: TRichEdit
    Left = 232
    Top = 272
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
    TabOrder = 3
  end
end
