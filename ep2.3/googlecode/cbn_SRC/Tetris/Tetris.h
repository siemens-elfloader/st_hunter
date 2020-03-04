
void onredraw(void);
void init();
void *memset(void *mem, int val, int size);
void drawtetris();
void drawhelp();
void savegame();
void loadgame();
int putfig(int action);
int newfig();
void delines();
void drawstak();
void newgame();
void down(int loop);

signed char fig[]={ // �����. ����� � ����������
//�������
0,0,1,0,0,1,1,1,
0,0,1,0,0,1,1,1,
0,0,1,0,0,1,1,1,
0,0,1,0,0,1,1,1,
//�����-�
-1,0,0,0,1,0,0,1,
0,0,1,0,0,1,0,-1,
0,0,1,0,-1,0,0,-1,
0,0,-1,0,0,-1,0,1,
//������
0,0,-1,0,1,0,2,0,
0,0,0,-1,0,1,0,2,
0,0,-1,0,1,0,2,0,
0,0,0,-1,0,1,0,2,
//��������� �����
0,0,-1,0,-1,-1,0,1,
0,0,1,0,0,1,-1,1,
0,0,-1,0,-1,-1,0,1,
0,0,1,0,0,1,-1,1,
//��������� ������
0,0,0,-1,-1,0,-1,1,
0,0,-1,0,0,1,1,1,
0,0,0,-1,-1,0,-1,1,
0,0,-1,0,0,1,1,1,
//�
0,0,-1,0,1,0,1,1,
0,0,0,-1,1,-1,0,1,
0,0,-1,0,1,0,-1,-1,
0,0,0,-1,0,1,-1,1,
//���. �
0,0,1,0,-1,0,-1,1,
0,0,0,-1,0,1,1,1,
0,0,-1,0,1,0,1,-1,
0,0,0,-1,0,1,-1,-1};