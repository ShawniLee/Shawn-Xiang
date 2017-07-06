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
#define	Army_COUNT	5	// Ŀ����Ŀ
#define	BLOOD	20	// Ŀ����Ŀ
#define Weaponcount	10
extern struct Weapon {//��ը��Χ������Ч����

	char		szName[32];
	float			fMass;
	int			iWeaponHurt;//��ը������0-20
	int			iWeaponType;//��ը���ͣ�0��ͨ�ף�1ֱ�����϶��£�2ֱ���������ң�3ֱ�߷�Χ�˺���4���Ⱥ�ˣ�5�ɿ�
	int			iWeaponPower;//��ը�ȼ�
	int			iWeaponBoomRange;//��ը��Χ
	int			iDoing;
	//strTreasure*	pNext;			// ָ����һ���ڵ��ָ��
};
extern struct army
{
	int			g_iTargetblood;
	char		g_szTargetName[32];// Ŀ������ 
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
extern 	float		g_fGunStrength ;		// �ڵ�����
extern int				g_iCalStrength;// 1 : �ո�������У��������ȣ����Ƚ�����							         // 0 : ����������
extern float			g_fGunRotation;
#endif // _LESSON_X_H_