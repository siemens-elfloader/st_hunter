object Form1: TForm1
  Left = 183
  Top = 173
  Width = 914
  Height = 640
  Caption = 'Form1'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object ListBox1: TListBox
    Left = 1
    Top = 8
    Width = 248
    Height = 586
    ItemHeight = 13
    TabOrder = 0
  end
  object ListBox2: TListBox
    Left = 256
    Top = 8
    Width = 273
    Height = 585
    ItemHeight = 13
    TabOrder = 1
  end
  object Button1: TButton
    Left = 824
    Top = 504
    Width = 75
    Height = 25
    Caption = 'Save'
    TabOrder = 2
    OnClick = Button1Click
  end
  object ListBox3: TListBox
    Left = 536
    Top = 8
    Width = 273
    Height = 585
    ItemHeight = 13
    TabOrder = 3
  end
  object Button2: TButton
    Left = 824
    Top = 544
    Width = 73
    Height = 33
    Caption = 'Exit'
    TabOrder = 4
    OnClick = Button2Click
  end
end
