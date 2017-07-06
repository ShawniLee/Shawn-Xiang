#include <Stdio.h>
#include "CommonAPI.h"
#include "Linked List.h"
FILE * mem;     //��Ϸ�浵�ļ����
// ���õ�ͼȫ���ɼ�
void SetMapInvisible()
{
	dSetSpriteVisible("map3", false);
	dSetSpriteVisible("map4", false);
	dSetSpriteVisible("map5", false);
}
// �������ư�
void ShiftOut(const char *szName)
{
	float temp = dGetScreenBottom() + dGetSpriteHeight(szName) / 2;
	dSpriteMoveTo(szName, 0, temp, 80.f, 1);
}
// ������ư�
void ShiftIn(const char *szName)
{
	dSetSpriteVisible(szName, true);
	dSpriteMoveTo(szName, 0, 0, 80.f, 1);
}
// �������
void MoveOutPlayer()
{
	dSetSpritePosition("player10", 0, -50);
	dSetSpritePosition("player11", 0, -50);
	dSetSpritePosition("player12", 0, -50);
	dSetSpritePosition("player13", 0, -50);
	dSetSpritePosition("player20", 0, -50);
	dSetSpritePosition("player21", 0, -50);
	dSetSpritePosition("player22", 0, -50);
	dSetSpritePosition("player23", 0, -50);
}
// �����ͼ�ٶ�
void ZeroMapSpeed()
{
	map_fSpeedX = 0.f;
	map_fSpeedY = 0.f;    //��ͼ�ٶ�
	dSetSpriteLinearVelocity(map, map_fSpeedX, map_fSpeedY);
}
// ������ͣ
void Pause()
{
	ZeroMapSpeed();	// ��ͼ����
	pause = 1;		// ����״̬��
}
// ���ü���
void GoOn()
{
	pause = 0;		// ����״̬��
}

void load()
{
	if ((mem = fopen("memory.txt", "r")) == NULL) 
	{
		fprintf(stderr, "No Game Cache Here!\n");
		exit(2);
	}
	fread(&LevelCount, sizeof(LevelCount), 1, mem);
	fclose(mem);
}

void save() {
	//�򿪴浵�ļ�
	if ((mem = fopen("memory.txt", "w+")) == NULL) {
		fprintf(stderr, "Can't open memory.txt!\n");
		exit(1);
	}
	//����ؿ���
	fwrite(&LevelCount, sizeof(LevelCount), 1, mem);
	fclose(mem);
}