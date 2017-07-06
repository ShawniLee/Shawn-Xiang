#include <Stdio.h>
#include "CommonAPI.h"
#include "LessonX.h"
#include "math.h"

////////////////////////////////////////////////////////////////////////////////
//
//
int				g_iGameState		=	1;		// 游戏状态，0 -- 游戏结束等待开始状态；1 -- 按下空格键开始，初始化游戏；2 -- 游戏进行中
int			    g_iFireState = 0;// 开炮状态：0 - 等待开炮，1 - 炮弹飞行中，等炮弹爆炸之后才开始下一轮开炮流程
int				flag = 0;//操纵的精灵
int				flag2 = 0;//操纵的炮弹
const float	    g_fMaxRotation = 350.f;// 最大旋转角度，也是初始角度
const float	    g_fBumbForceY = 10.f;	// 炮弹发射之后，给它一个向下的常力，							             // 使其形成一个抛物线弹道轨迹
float			g_fKeyUp = 0.f;	// 上下键是否按下的变量：1按下0弹起，用于计算角度
float			g_fKeyDown = 0.f;	// 上下键是否按下的变量：1按下0弹起，用于计算角度
float			g_fRoundTime = 0.f;	// 炮弹发射之后，等待一定时间才开始下回合
static int		flagPlayer1=5, flagPlayer2=5;//双方角色存活数量
int				LevelCount = 0;
extern int bag[Army_COUNT][Weaponcount] = { 0 };
extern Weapon sWeapon[Weaponcount] = 
{
	{"NormalWeapon",5,8,0,1,7},
	{"Bird",10,15,1,3,1},
	{"GoldWeapon",3,1,0,2,4},
	{"Boomerang",5,10,2,4,5},
	{"tornado",0,15,3,0,1},
	{"PowderBarrel",20,17,0,9,6},
	{"Grenade",5,12,4,5,5},
	{"Trap",5,20,0,0,6},
	{"balloon",1,10,0,5,9}
};
struct army armyNPC[Army_COUNT];
	float		g_fGunStrength = 0.f;		// 炮的力度
	int				g_iCalStrength = 0;	// 1 : 空格键按下中，计算力度，力度渐增。							         // 0 : 不计算力度
	float			g_fGunRotation = g_fMaxRotation;	// 炮的朝向
	float		fForceX;
	float		fForceY;
//
void		GameInit();
void		GameRun( float fDeltaTime );
void		GameEnd();
void	ProcessBumbHit(const int iHitState, const char *szBumbName, const char *szTargetName);//处理炸弹精灵和其他精灵碰撞爆炸的效果。
int		IsGameWin();
void    Drawparalora(char * DandanGun, float fOldPosX, float fOldPosY, float g_fGunRotation, int g_fGunStrength);

//==============================================================================
//
// 大体的程序流程为：GameMainLoop函数为主循环函数，在引擎每帧刷新屏幕图像之后，都会被调用一次。


//==============================================================================
//
// 游戏主循环，此函数将被不停的调用，引擎每刷新一次屏幕，此函数即被调用一次
// 用以处理游戏的开始、进行中、结束等各种状态. 
// 函数参数fDeltaTime : 上次调用本函数到此次调用本函数的时间间隔，单位：秒
void GameMainLoop( float fDeltaTime )
{
	switch( g_iGameState )
	{
		// 初始化游戏，清空上一局相关数据
	case 1:
		{
			GameInit();
			g_iGameState	=	2; // 初始化之后，将游戏状态设置为进行中
		}
		break;

		// 游戏进行中，处理各种游戏逻辑
	case 2:
		{
			// TODO 修改此处游戏循环条件，完成正确游戏逻辑
			if(!IsGameWin())
			{
				GameRun( fDeltaTime );
			}
			else
			{
				// 游戏结束。调用游戏结算函数，并把游戏状态修改为结束状态
				g_iGameState	=	0;
				GameEnd();
			}
		}
		break;

		// 游戏结束/等待按空格键开始
	case 0:
	default:
		break;
	};
}

//==============================================================================
//
// 每局开始前进行初始化，清空上一局相关数据
void GameInit()
{
	g_iFireState = 0;
	for (int i = 0; i < Army_COUNT/2; i++)
	{
		sprintf(armyNPC[i].g_szTargetName, "armyNPC%d", i);
		dCloneSprite("armyNPCmuban1", armyNPC[i].g_szTargetName);
		armyNPC[i].g_iTargetblood = BLOOD;
		dSetSpritePosition(armyNPC[i].g_szTargetName, -17.480, 26.080);

	}
	//for (iLoop = 0; iLoop < Army_COUNT; iLoop++)
	//{
	//	armyNPC[iLoop].g_iTargetblood = 0;
	//	// 在X方向上，目标在0-45范围内随机移动
	//	armyNPC[iLoop].fPosX = dRandomRange(0, 45);
	//	armyNPC[iLoop].fPosY = dGetSpritePositionY(armyNPC[iLoop].g_szTargetName);
	//	dSpriteMoveTo(armyNPC[iLoop].g_szTargetName, armyNPC[iLoop].fPosX, armyNPC[iLoop].fPosY,40.f,1);
	//	dSetSpriteVisible(armyNPC[iLoop].g_szTargetName, 1);
	//	// 恢复初始动画/图片
	//	dAnimateSpritePlayAnimation(armyNPC[iLoop].g_szTargetName, "DandanTargetAnimation1", 0);
	//}
	for (int i = 0; i < Army_COUNT; i++)
	{
		dSetSpriteCollisionReceive(armyNPC[i].g_szTargetName, 1);//设置目标精灵为接收碰撞
		dSetSpriteCollisionSend(armyNPC[i].g_szTargetName, 1); //设置炮弹精灵为发送碰撞
	}
	dSetSpriteCollisionReceive("DandanBlock1", 1);  //设置石块精灵为接收碰撞
	dSetSpriteCollisionReceive("DandanPlatform", 1); //设置地面精灵为接收碰撞
	for (int i = 0; i < BLOOD; i++)
		dSetSpriteMountOwned(dMakeSpriteName("blood", armyNPC[i].g_iTargetblood), 1);
}

//==============================================================================
//
// 每局游戏进行中
void GameRun( float fDeltaTime )
{	
	if (0 == g_iFireState)
	{
		if (g_iCalStrength)
		{
			g_fGunStrength += 50.f * fDeltaTime;  // 50每秒
			if (g_fGunStrength > 200.f)
				g_fGunStrength = 200.f;
		}
		g_fGunRotation += (g_fKeyDown - g_fKeyUp) * 15.f * fDeltaTime; // 15度每秒
		if (g_fGunRotation < 280.f)
			g_fGunRotation = 280.f;
		else if (g_fGunRotation > g_fMaxRotation)
			g_fGunRotation = g_fMaxRotation;
	}
	else if (1 == g_iFireState)
	{
		g_fRoundTime -= fDeltaTime;
		if (g_fRoundTime <= 0.f)
		{
			// 重新初始化
			g_iFireState = 0;
			g_fGunRotation = g_fMaxRotation;
			g_fGunStrength = 0.f;
			// 目标每回合水平移动一次：
			// 用循环遍历g_szTargetName，如果该精灵的被击中次数
			// g_iTargetblood小于3才需要做
			// 需要做的2件事：0-45之间随机X坐标，Y坐标不变。
			// 然后使用dSpriteMoveTo移动精灵。请参考GameInit函数
			int		iLoop = 0;
			float 	fPosX = 0, fPosY = 0;
			for (iLoop = 0; iLoop < Army_COUNT; iLoop++)
			{
				if (armyNPC[iLoop].g_iTargetblood >= 3)
					continue;
				// 每回合在X方向上，目标在0-45范围内随机移动一次
				//fPosX = dRandomRange(0, 45);
				//fPosY = dGetSpritePositionY(g_szTargetName[iLoop]);
				//dSpriteMoveTo(g_szTargetName[iLoop], fPosX, fPosY, 40.f, 1);
			}
		}
	}
// 更新炮旋转角度
// 更新数值
	if (flagPlayer1 == 0 || flagPlayer2 == 0)
	{
		if(LevelCount==5)
		dLoadMap("OverWin.t2d");
		else if(flagPlayer1==0)
		{
			dLoadMap("YouLost.t2d");
		}
		else if (flagPlayer2 == 0)
		{
			dLoadMap("YouWin.t2d");
		}
	}

}
//==============================================================================
//
// 本局游戏结束
void GameEnd()
{
	// 马上又开始游戏，所以直接设置状态
	g_iGameState = 1;
}
//==============================================================================
//
//该函数处理炸弹精灵和其他精灵爆炸效果显示函数。其中，iHitState表示击中的目标，0表示飞出边界，1表示目标是armyNPC，2表示障碍物。炸弹精灵为szBumbName
void ProcessBumbHit(const int iHitState, const char * szBumbName, const char * szTargetName)
{
	float	fPosX = dGetSpritePositionX(szBumbName);
	float	fPosY = dGetSpritePositionY(szBumbName);
	if (1 == iHitState)
	{
		int iLoop = 0;
		for (iLoop = 0; iLoop < Army_COUNT; iLoop++)
		{
			if (stricmp(armyNPC[iLoop].g_szTargetName, szTargetName) == 0)
			{
				--armyNPC[iLoop].g_iTargetblood;
				dSetStaticSpriteImage(dMakeSpriteName("Targetblood",iLoop),dMakeSpriteName("blood",armyNPC[iLoop].g_iTargetblood), 10);//更改目标血条图片
				dSpriteMountToSprite(dMakeSpriteName("blood", armyNPC[iLoop].g_iTargetblood), armyNPC[iLoop].g_szTargetName, 0, -10);
				//删除精灵，绑定血条跟随删除
				if (armyNPC[iLoop].g_iTargetblood == 0)
				{dDeleteSprite(armyNPC[iLoop].g_szTargetName);
				if (iLoop < 5)flagPlayer1--;
				else flagPlayer2--;
				}
				break;
			}
		}
	}
	if (1 == iHitState || 2 == iHitState)
	{
		dPlayEffect("BumbExplode", 1.f, fPosX, fPosY, 0.f);//障碍物特效
	}
	dDeleteSprite(szBumbName);
}
//游戏胜利条件
int IsGameWin()
{
	int	iLoop = 0;
	for (iLoop = 0; iLoop < Army_COUNT; iLoop++)
	{
		if (armyNPC[iLoop].g_iTargetblood !=0)
			return 0;
	}
	return 1;
}
//抛物线轨迹
void Drawparalora(char * DandanGun,float fOldPosX,float fOldPosY, float	g_fGunRotation, int	g_fGunStrength)
{
	//float	fOldPosX = dGetSpriteLinkPointPosX("DandanGun", 1);
	//float	fOldPosY = dGetSpriteLinkPointPosY("DandanGun", 1);
	float	fNewPosX = 0.f, fNewPosY = 0.f;
	float	fVelocityX = dRotationToVectorX(g_fGunRotation) * g_fGunStrength;
	float	fVelocityY = dRotationToVectorY(g_fGunRotation) * g_fGunStrength;
	float	fMass = dGetSpriteMass(dMakeSpriteName("Weapon",flag2));
	float	fForceVelY = g_fBumbForceY / fMass;
	float	fHalfTime = -fVelocityY / (g_fBumbForceY / fMass);
	float	fTime = 0.f;
	float	fSimDelta = 0.0333f;
	for (fTime = 0.f; fTime < fHalfTime; fTime += fSimDelta)
	{
		fNewPosX = fOldPosX + fVelocityX * fSimDelta;
		fNewPosY = fOldPosY + (fVelocityY + fForceVelY * fTime) * fSimDelta;
		// 画线
		dDrawLine(fOldPosX, fOldPosY, fNewPosX, fNewPosY, 6.f, 0, 255 - fTime / fHalfTime * 255, 255 - fTime / fHalfTime * 255, 255, 255 - fTime / fHalfTime * 255);
		fOldPosX = fNewPosX;
		fOldPosY = fNewPosY;
	}
}
//==========================================================================
//
// 鼠标移动
// 参数 fMouseX, fMouseY：为鼠标当前坐标
void OnMouseMove( const float fMouseX, const float fMouseY )
{
	if (flag2 != 0)
	{
		g_fGunStrength = pow(fMouseX - armyNPC[flag].fPosX, 2) + pow(fMouseY - armyNPC[flag].fPosY, 2) / 51600 * 2;
		if (g_fGunStrength > 200)
		{
			g_fGunStrength = 200;
		}
		g_fGunRotation = dCalLineRotation(armyNPC[flag].fPosX, armyNPC[flag].fPosY, fMouseX, fMouseY);
		if (g_fGunRotation < 180) g_fGunRotation += 180;
		else g_fGunRotation -= 180;
		Drawparalora(sWeapon[flag2].szName, armyNPC[flag].fPosX, armyNPC[flag].fPosY, g_fGunRotation, g_fGunStrength);
	}
}
//==========================================================================
//
// 鼠标点击
// 参数 iMouseType：鼠标按键值，见 enum MouseTypes 定义
// 参数 fMouseX, fMouseY：为鼠标当前坐标
void OnMouseClick( const int iMouseType, const float fMouseX, const float fMouseY )
{
	for(flag=0;flag<Army_COUNT/2;flag++)//循环确定是哪个精灵
	if (iMouseType == 0 && 2 == g_iGameState && (0 == g_iFireState&& armyNPC[flag].fPosX-10<fMouseX<armyNPC[flag].fPosX + 10) && (armyNPC[flag].fPosY-10<fMouseY < armyNPC[flag].fPosY + 10))
	{
		g_iCalStrength = 1;//开始计算力度
		break;
		dSetSpriteVisible("dialogbox", 1);
		dSetTextString("Dialogboxtext", "请按下1-10决定选择哪个武器");
		for (int i = 0; i < Weaponcount; i++)
		{
			dSetTextValue(dMakeSpriteName("Weapon", i),bag[flag][i]);//克隆12个文字精灵，显示每一个的值
			if (bag[flag][i] != 0)
			{
			dSetSpriteColorAlpha(dMakeSpriteName("Weapon", i), 255);//点亮这个武器
			}
		}
	}
}
//==========================================================================
//
// 鼠标弹起
// 参数 iMouseType：鼠标按键值，见 enum MouseTypes 定义
// 参数 fMouseX, fMouseY：为鼠标当前坐标
void OnMouseUp( const int iMouseType, const float fMouseX, const float fMouseY )
{
	if (iMouseType == 0 && 2 == g_iGameState && 0 == g_iFireState)
	{
		g_iFireState = 1;
		g_iCalStrength = 0;
		g_fRoundTime = 2.f;
		float	fPosX = armyNPC[flag].fPosX;
		float	fPosY = armyNPC[flag].fPosY;
		char	*szName = NULL;
			szName = sWeapon[flag2].szName;//获取武器名字
			dCloneSprite(dMakeSpriteName("Weapon",flag2), szName);//dCloneSprite、BumbTemplate创建炮弹
			dSetSpritePosition(szName, fPosX, fPosY);
			dSetSpriteLinearVelocityPolar(szName, g_fGunStrength, g_fGunRotation);//dSetSpriteLinearVelocityPolar给炮弹设置速度
																			  //我们要模拟的是真实的炮弹，弹道轨迹是一个抛物线，所以我们需要给炮弹一个向下的力量
			dSetSpriteConstantForceY(szName, g_fBumbForceY);//炮弹受常力，但质量不同，所以飞翔距离不同，成功发射
			flag2 = 0;
		}
}
//==========================================================================
//
// 键盘按下
// 参数 iKey：被按下的键，值见 enum KeyCodes 宏定义
// 参数 iAltPress, iShiftPress，iCtrlPress：键盘上的功能键Alt，Ctrl，Shift当前是否也处于按下状态(0未按下，1按下)
void OnKeyDown( const int iKey, const bool bAltPress, const bool bShiftPress, const bool bCtrlPress )
{	
	if (23<=iKey<=32)//获取按下的武器
	{
		flag2 = iKey-22;//按下0是第一个武器，一共10个
	}

}
//==========================================================================
//
// 键盘弹起
// 参数 iKey：弹起的键，值见 enum KeyCodes 宏定义
void OnKeyUp(const int iKey)
{

}
//===========================================================================
//
// 精灵与精灵碰撞
// 参数 szSrcName：发起碰撞的精灵名字
// 参数 szTarName：被碰撞的精灵名字
void OnSpriteColSprite( const char *szSrcName, const char *szTarName )
{
	if (2 == g_iGameState  && strstr(szSrcName, "DandanBumb"))
	{
		// 判断被炮弹击中哪个是目标
		if (strstr(szTarName, "DandanTarget"))
			ProcessBumbHit(1, szSrcName, szTarName);
		else
			ProcessBumbHit(2, szSrcName, "");
	}
}
//===========================================================================
//
// 精灵与世界边界碰撞
// 参数 szName：碰撞到边界的精灵名字
// 参数 iColSide：碰撞到的边界 0 左边，1 右边，2 上边，3 下边
void OnSpriteColWorldLimit( const char *szName, const int iColSide )
{
	if (2 == g_iGameState  && strstr(szName, "DandanBumb"))
	{
		ProcessBumbHit(0, szName, "");
	}
}
void YouWin()
{
	for (int i = 0; i < Army_COUNT/2; i++)
	{
		if (dIsSpriteVisible(dMakeSpriteName("Player", i)))
			dSpriteMountToSprite("WinWenzi", dMakeSpriteName("Player", i), 0, 20);//显示文字位置，绑定文字与说话精灵
			dSetTextString("WinWenzi", "");/*填写胜利文字*/
			break;
	}//选一个人说话
	dLoadMap("Black.t2d");//新建一个全黑场景，回到主菜单
	dSetTextString("Wenzi", "进行下一关请按T，回到主菜单请按F");
	//dSetSpriteColorAlpha(dMakeSpriteName("Level", LevelCount), 255);//点亮这个地图精灵和临近的下一个的一半
	//dSetSpriteColorAlpha(dMakeSpriteName("Level", LevelCount+1), 100);
}
void YouLost()
{
	dSetSpriteVisible("YouLost", 1);
	Sleep(3000);
	dLoadMap(dMakeSpriteName("Level", LevelCount));//地图命名：Level＋关卡数
}