//----------------COLORS----------------------
typedef char const* color;

const char NULLL[]={0,0,0,0};

const char WHITE[]={255,255,255,100};
const char BLACK[]={0,0,0,100};
const char GRAY[]={128,128,128,100};

const char RED[]={255,0,0,100};
const char GREEN[]={0,255,0,100};
const char BLUE[]={0,0,255,100};

const char YELLOW[]={255,255,0,100};
const char CYAN[]={0,255,255,100};
const char PURPLE[]={255,0,255,100};

const char ORANGE[]={255,165,0,100};
const char GOLD[]={255,215,0,100};
const char DARKGOLD[]={218,165,32,100};
const char BROWN[]={160,40,20,100};
const char DARKBROWN[]={0x86,0x45,0x13,100};


const char OBLACK[]={0,0,0,20};
const char SBLACK[]={0,0,0,50};
const char ORED[]={255,0,0,20};
const char OGREEN[]={0,255,0,20};
const char OBLUE[]={0,0,255,20};
const char OBROWN[]={160,40,20,20};
//----------------Terrain types------------
/*#define GROUND 1
#define BRICK 2

#define FOREST_UP 75
#define FOREST_CENT 76
#define FOREST_DOWN 77

#define SEA 21

#define FLAME 100

#define COAST_NW 17
#define COAST_N 18
#define COAST_NE 19
#define COAST_E 20
#define COAST_SE 23
#define COAST_S 24
#define COAST_SW 25
#define COAST_W 22*/

char *GetTerrainNames[255] = 
{
"�����",  //0
"�����",  //1
"�������",  //2
"�����",  //3
"�����",  //4
"��������",  //5
"�����2",  //6
"����",  //7
"�����3",  //8
"���� �������",  //9
"",  //10
"�����",  //11
"�����",  //12
"�����",  //13
"�����",  //14
"�����",  //15
"�����",  //16
"�� ��������",  //17
"� ��������",  //18
"�� ��������",  //19
"� ��������",  //20
"� ��������",  //21
"� ��������",  //22
"�� ��������",  //23
"� ��������",  //24
"�� ��������",  //25
"",  //26
"",  //27
"",  //28
"",  //29
"",  //30
"",  //31
"",  //32
"",  //33
"",  //34
"",  //35
"",  //36
"",  //37
"",  //38
"",  //39
"",  //40
"",  //41
"",  //42
"",  //43
"����� �-�",  //44
"����� �-�",  //45
"����� 3-�-�",  //46
"����� �-�",  //47
"����� �-�",  //48
"����� 3-�-�",  //49
"",  //50
"����� 3-�",  //51
"����� �-�",  //52
"",  //53
"",  //54
"",  //55
"�����",  //56
"",  //57
"",  //58
"",  //59
"",  //60
"",  //61
"",  //62
"",  //63
"",  //64
"",  //65
"",  //66
"",  //67
"",  //68
"",  //69
"",  //70
"",  //71
"",  //72
"",  //73
"",  //74
"���",  //75
"���",  //76
"���",  //77
"",  //78
"",  //79
"",  //80
"",  //81
"",  //82
"",  //83
"",  //84
"",  //85
"",  //86
"",  //87
"",  //88
"",  //89
"",  //90
"",  //91
"",  //92
"",  //93
"",  //94
"",  //95
"",  //96
"",  //97
"",  //98
"",  //99
"�����",  //100
"",  //101
"",  //102
"",  //103
"",  //104
"",  //105
"",  //106
"",  //107
"",  //108
"",  //109
"",  //110
"",  //111
"",  //112
"",  //113
"",  //114
"",  //115
"",  //116
"",  //117
"",  //118
"",  //119
"",  //120
"",  //121
"",  //122
"",  //123
"",  //124
"",  //125
"",  //126
"",  //127
"",  //128
"",  //129
"",  //130
"",  //131
"",  //132
"",  //133
"",  //134
"",  //135
"",  //136
"",  //137
"",  //138
"",  //139
"",  //140
"",  //141
"",  //142
"",  //143
"",  //144
"",  //145
"",  //146
"",  //147
"",  //148
"",  //149
"",  //150
"",  //151
"",  //152
"",  //153
"",  //154
"",  //155
"",  //156
"",  //157
"",  //158
"",  //159
"",  //160
"",  //161
"",  //162
"",  //163
"",  //164
"",  //165
"",  //166
"",  //167
"",  //168
"",  //169
"",  //170
"",  //171
"",  //172
"",  //173
"",  //174
"",  //175
"",  //176
"",  //177
"",  //178
"",  //179
"",  //180
"",  //181
"",  //182
"",  //183
"",  //184
"",  //185
"",  //186
"",  //187
"",  //188
"" //189
};
