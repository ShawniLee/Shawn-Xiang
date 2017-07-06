/////////////////////////////////////////////////////////////////////////////////
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////
#ifndef _LESSON_X_H_
#define _LESSON_X_H_
//
#include <Windows.h>
typedef struct Treasure strTreasure;
extern void	GameMainLoop( float	fDeltaTime );
extern void OnMouseMove( const float fMouseX, const float fMouseY );
extern void OnMouseClick( const int iMouseType, const float fMouseX, const float fMouseY );
extern void OnMouseUp( const int iMouseType, const float fMouseX, const float fMouseY );
extern void OnKeyDown( const int iKey, const bool bAltPress, const bool bShiftPress, const bool bCtrlPress );
extern void OnKeyUp( const int iKey );
extern void OnSpriteColSprite( const char *szSrcName, const char *szTarName );
extern void OnSpriteColWorldLimit( const char *szName, const int iColSide );
extern const float	    g_fBumbForceY;
extern int		LevelCount;
extern void load();
extern void save();
#define	Army_COUNT	5	// 目标数目
#define	BLOOD	20	// 目标数目
#define Weaponcount	10
extern struct Weapon {//爆炸范围利用特效定义

	char		szName[32];
	float			fMass;
	int			iWeaponHurt;//爆炸威力，0-20
	int			iWeaponType;//爆炸类型，0普通抛，1直线自上而下，2直线自左向右，3直线范围伤害，4落地群伤，5可控
	int			iWeaponPower;//爆炸等级
	int			iWeaponBoomRange;//爆炸范围
	int			iDoing;
	//strTreasure*	pNext;			// 指向下一个节点的指针
};
extern struct army
{
	int			g_iTargetblood;
	char		g_szTargetName[32];// 目标名字 
	float		fPosX, fPosY;
	float		fMass;
	float		fInert;
	int			iDoing;
};
extern struct Treasure{
	int			WeaponNum[Weaponcount];
	int			listNum;
	Treasure*	pNext;
};
extern 	float		g_fGunStrength ;		// 炮的力度
extern int				g_iCalStrength;// 1 : 空格键按下中，计算力度，力度渐增。							         // 0 : 不计算力度
extern float			g_fGunRotation;
#endif // _LESSON_X_H_