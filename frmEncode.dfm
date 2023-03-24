object FormEncode: TFormEncode
  Left = 459
  Top = 153
  BorderStyle = bsDialog
  Caption = 'Encode'
  ClientHeight = 290
  ClientWidth = 408
  Color = clBtnFace
  ParentFont = True
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
  object btnOK: TButton
    Left = 316
    Top = 208
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 0
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
  object txtMessage: TRichEdit
    Left = 24
    Top = 40
    Width = 273
    Height = 225
    TabOrder = 2
  end
  object btnOpen: TButton
    Left = 312
    Top = 40
    Width = 75
    Height = 25
    Caption = 'Open'
    TabOrder = 3
  end
end
