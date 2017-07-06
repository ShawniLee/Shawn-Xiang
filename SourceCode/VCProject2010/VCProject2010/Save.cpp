#include <Stdio.h>
#include "CommonAPI.h"
#include "Linked List.h"
FILE * mem;     //游戏存档文件句柄
// 设置地图全不可见
void SetMapInvisible()
{
	dSetSpriteVisible("map3", false);
	dSetSpriteVisible("map4", false);
	dSetSpriteVisible("map5", false);
}
// 闪出控制板
void ShiftOut(const char *szName)
{
	float temp = dGetScreenBottom() + dGetSpriteHeight(szName) / 2;
	dSpriteMoveTo(szName, 0, temp, 80.f, 1);
}
// 闪入控制板
void ShiftIn(const char *szName)
{
	dSetSpriteVisible(szName, true);
	dSpriteMoveTo(szName, 0, 0, 80.f, 1);
}
// 重置玩家
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
// 归零地图速度
void ZeroMapSpeed()
{
	map_fSpeedX = 0.f;
	map_fSpeedY = 0.f;    //地图速度
	dSetSpriteLinearVelocity(map, map_fSpeedX, map_fSpeedY);
}
// 设置暂停
void Pause()
{
	ZeroMapSpeed();	// 地图归零
	pause = 1;		// 设置状态数
}
// 设置继续
void GoOn()
{
	pause = 0;		// 设置状态数
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
	//打开存档文件
	if ((mem = fopen("memory.txt", "w+")) == NULL) {
		fprintf(stderr, "Can't open memory.txt!\n");
		exit(1);
	}
	//保存关卡数
	fwrite(&LevelCount, sizeof(LevelCount), 1, mem);
	fclose(mem);
}