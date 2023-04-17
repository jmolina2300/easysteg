object FormEncode: TFormEncode
  Left = 158
  Top = 180
  BorderStyle = bsDialog
  Caption = 'Encode'
  ClientHeight = 290
  ClientWidth = 408
  Color = clBtnFace
  ParentFont = True
  KeyPreview = True
  OldCreateOrder = True
  Position = poScreenCenter
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
  object btnOK: TButton
    Left = 316
    Top = 208
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    TabOrder = 0
    OnClick = btnOKClick
  end
  object btnCancel: TButton
    Left = 316
    Top = 238
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
    TabOrder = 2
    OnChange = tbxMessageChange
  end
end
