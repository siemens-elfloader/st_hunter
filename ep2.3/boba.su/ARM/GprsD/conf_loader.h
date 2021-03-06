/*
��������� ������������
(c) Rst7, Kibab
 ������������ �������� ������� � ������������� ������� �������������
*/
#ifndef _CONFLOADER_H_
  #define _CONFLOADER_H_

// ������� ����������� ����� ���������� � ���������� ������������
void InitConfig();

// ������� � ���������� �� �������� �������, ����� ��������� ��� �������
// �� �������
void FillProfile(char *profile);

// ��������� �������� ������ � �������� �������
#define coIntName   0x00   // ���������� ��� �������
#define coName      0x35   // ��� �������
#define coAPN_name  0x58   // ������������ ����� ����� �������
#define coDNS       0xA1   // �������� IP-������
#define coDNS_flag  0xA8   // ���� ������� DNS
#define coLogin     0x1AD  // ����� ������������ ����� �������
#define coPass      0x1E0  // ������ ����� �������
#define coTimeout   0x190  // ������� ������������

#endif
