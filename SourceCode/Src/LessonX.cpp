#include <Stdio.h>
#include "CommonAPI.h"
#include "LessonX.h"
#include "math.h"

////////////////////////////////////////////////////////////////////////////////
//
//
int				g_iGameState		=	1;		// ��Ϸ״̬��0 -- ��Ϸ�����ȴ���ʼ״̬��1 -- ���¿ո����ʼ����ʼ����Ϸ��2 -- ��Ϸ������
int			    g_iFireState = 0;// ����״̬��0 - �ȴ����ڣ�1 - �ڵ������У����ڵ���ը֮��ſ�ʼ��һ�ֿ�������
int				flag = 0;//���ݵľ���
int				flag2 = 0;//���ݵ��ڵ�
const float	    g_fMaxRotation = 350.f;// �����ת�Ƕȣ�Ҳ�ǳ�ʼ�Ƕ�
const float	    g_fBumbForceY = 10.f;	// �ڵ�����֮�󣬸���һ�����µĳ�����							             // ʹ���γ�һ�������ߵ����켣
float			g_fKeyUp = 0.f;	// ���¼��Ƿ��µı�����1����0�������ڼ���Ƕ�
float			g_fKeyDown = 0.f;	// ���¼��Ƿ��µı�����1����0�������ڼ���Ƕ�
float			g_fRoundTime = 0.f;	// �ڵ�����֮�󣬵ȴ�һ��ʱ��ſ�ʼ�»غ�
static int		flagPlayer1=5, flagPlayer2=5;//˫����ɫ�������
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
	float		g_fGunStrength = 0.f;		// �ڵ�����
	int				g_iCalStrength = 0;	// 1 : �ո�������У��������ȣ����Ƚ�����							         // 0 : ����������
	float			g_fGunRotation = g_fMaxRotation;	// �ڵĳ���
	float		fForceX;
	float		fForceY;
//
void		GameInit();
void		GameRun( float fDeltaTime );
void		GameEnd();
void	ProcessBumbHit(const int iHitState, const char *szBumbName, const char *szTargetName);//����ը�����������������ײ��ը��Ч����
int		IsGameWin();
void    Drawparalora(char * DandanGun, float fOldPosX, float fOldPosY, float g_fGunRotation, int g_fGunStrength);

//==============================================================================
//
// ����ĳ�������Ϊ��GameMainLoop����Ϊ��ѭ��������������ÿ֡ˢ����Ļͼ��֮�󣬶��ᱻ����һ�Ρ�


//==============================================================================
//
// ��Ϸ��ѭ�����˺���������ͣ�ĵ��ã�����ÿˢ��һ����Ļ���˺�����������һ��
// ���Դ�����Ϸ�Ŀ�ʼ�������С������ȸ���״̬. 
// ��������fDeltaTime : �ϴε��ñ��������˴ε��ñ�������ʱ��������λ����
void GameMainLoop( float fDeltaTime )
{
	switch( g_iGameState )
	{
		// ��ʼ����Ϸ�������һ���������
	case 1:
		{
			GameInit();
			g_iGameState	=	2; // ��ʼ��֮�󣬽���Ϸ״̬����Ϊ������
		}
		break;

		// ��Ϸ�����У����������Ϸ�߼�
	case 2:
		{
			// TODO �޸Ĵ˴���Ϸѭ�������������ȷ��Ϸ�߼�
			if(!IsGameWin())
			{
				GameRun( fDeltaTime );
			}
			else
			{
				// ��Ϸ������������Ϸ���㺯����������Ϸ״̬�޸�Ϊ����״̬
				g_iGameState	=	0;
				GameEnd();
			}
		}
		break;

		// ��Ϸ����/�ȴ����ո����ʼ
	case 0:
	default:
		break;
	};
}

//==============================================================================
//
// ÿ�ֿ�ʼǰ���г�ʼ���������һ���������
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
	//	// ��X�����ϣ�Ŀ����0-45��Χ������ƶ�
	//	armyNPC[iLoop].fPosX = dRandomRange(0, 45);
	//	armyNPC[iLoop].fPosY = dGetSpritePositionY(armyNPC[iLoop].g_szTargetName);
	//	dSpriteMoveTo(armyNPC[iLoop].g_szTargetName, armyNPC[iLoop].fPosX, armyNPC[iLoop].fPosY,40.f,1);
	//	dSetSpriteVisible(armyNPC[iLoop].g_szTargetName, 1);
	//	// �ָ���ʼ����/ͼƬ
	//	dAnimateSpritePlayAnimation(armyNPC[iLoop].g_szTargetName, "DandanTargetAnimation1", 0);
	//}
	for (int i = 0; i < Army_COUNT; i++)
	{
		dSetSpriteCollisionReceive(armyNPC[i].g_szTargetName, 1);//����Ŀ�꾫��Ϊ������ײ
		dSetSpriteCollisionSend(armyNPC[i].g_szTargetName, 1); //�����ڵ�����Ϊ������ײ
	}
	dSetSpriteCollisionReceive("DandanBlock1", 1);  //����ʯ�龫��Ϊ������ײ
	dSetSpriteCollisionReceive("DandanPlatform", 1); //���õ��澫��Ϊ������ײ
	for (int i = 0; i < BLOOD; i++)
		dSetSpriteMountOwned(dMakeSpriteName("blood", armyNPC[i].g_iTargetblood), 1);
}

//==============================================================================
//
// ÿ����Ϸ������
void GameRun( float fDeltaTime )
{	
	if (0 == g_iFireState)
	{
		if (g_iCalStrength)
		{
			g_fGunStrength += 50.f * fDeltaTime;  // 50ÿ��
			if (g_fGunStrength > 200.f)
				g_fGunStrength = 200.f;
		}
		g_fGunRotation += (g_fKeyDown - g_fKeyUp) * 15.f * fDeltaTime; // 15��ÿ��
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
			// ���³�ʼ��
			g_iFireState = 0;
			g_fGunRotation = g_fMaxRotation;
			g_fGunStrength = 0.f;
			// Ŀ��ÿ�غ�ˮƽ�ƶ�һ�Σ�
			// ��ѭ������g_szTargetName������þ���ı����д���
			// g_iTargetbloodС��3����Ҫ��
			// ��Ҫ����2���£�0-45֮�����X���꣬Y���겻�䡣
			// Ȼ��ʹ��dSpriteMoveTo�ƶ����顣��ο�GameInit����
			int		iLoop = 0;
			float 	fPosX = 0, fPosY = 0;
			for (iLoop = 0; iLoop < Army_COUNT; iLoop++)
			{
				if (armyNPC[iLoop].g_iTargetblood >= 3)
					continue;
				// ÿ�غ���X�����ϣ�Ŀ����0-45��Χ������ƶ�һ��
				//fPosX = dRandomRange(0, 45);
				//fPosY = dGetSpritePositionY(g_szTargetName[iLoop]);
				//dSpriteMoveTo(g_szTargetName[iLoop], fPosX, fPosY, 40.f, 1);
			}
		}
	}
// ��������ת�Ƕ�
// ������ֵ
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
// ������Ϸ����
void GameEnd()
{
	// �����ֿ�ʼ��Ϸ������ֱ������״̬
	g_iGameState = 1;
}
//==============================================================================
//
//�ú�������ը��������������鱬ըЧ����ʾ���������У�iHitState��ʾ���е�Ŀ�꣬0��ʾ�ɳ��߽磬1��ʾĿ����armyNPC��2��ʾ�ϰ��ը������ΪszBumbName
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
				dSetStaticSpriteImage(dMakeSpriteName("Targetblood",iLoop),dMakeSpriteName("blood",armyNPC[iLoop].g_iTargetblood), 10);//����Ŀ��Ѫ��ͼƬ
				dSpriteMountToSprite(dMakeSpriteName("blood", armyNPC[iLoop].g_iTargetblood), armyNPC[iLoop].g_szTargetName, 0, -10);
				//ɾ�����飬��Ѫ������ɾ��
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
		dPlayEffect("BumbExplode", 1.f, fPosX, fPosY, 0.f);//�ϰ�����Ч
	}
	dDeleteSprite(szBumbName);
}
//��Ϸʤ������
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
//�����߹켣
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
		// ����
		dDrawLine(fOldPosX, fOldPosY, fNewPosX, fNewPosY, 6.f, 0, 255 - fTime / fHalfTime * 255, 255 - fTime / fHalfTime * 255, 255, 255 - fTime / fHalfTime * 255);
		fOldPosX = fNewPosX;
		fOldPosY = fNewPosY;
	}
}
//==========================================================================
//
// ����ƶ�
// ���� fMouseX, fMouseY��Ϊ��굱ǰ����
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
// �����
// ���� iMouseType����갴��ֵ���� enum MouseTypes ����
// ���� fMouseX, fMouseY��Ϊ��굱ǰ����
void OnMouseClick( const int iMouseType, const float fMouseX, const float fMouseY )
{
	for(flag=0;flag<Army_COUNT/2;flag++)//ѭ��ȷ�����ĸ�����
	if (iMouseType == 0 && 2 == g_iGameState && (0 == g_iFireState&& armyNPC[flag].fPosX-10<fMouseX<armyNPC[flag].fPosX + 10) && (armyNPC[flag].fPosY-10<fMouseY < armyNPC[flag].fPosY + 10))
	{
		g_iCalStrength = 1;//��ʼ��������
		break;
		dSetSpriteVisible("dialogbox", 1);
		dSetTextString("Dialogboxtext", "�밴��1-10����ѡ���ĸ�����");
		for (int i = 0; i < Weaponcount; i++)
		{
			dSetTextValue(dMakeSpriteName("Weapon", i),bag[flag][i]);//��¡12�����־��飬��ʾÿһ����ֵ
			if (bag[flag][i] != 0)
			{
			dSetSpriteColorAlpha(dMakeSpriteName("Weapon", i), 255);//�����������
			}
		}
	}
}
//==========================================================================
//
// ��굯��
// ���� iMouseType����갴��ֵ���� enum MouseTypes ����
// ���� fMouseX, fMouseY��Ϊ��굱ǰ����
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
			szName = sWeapon[flag2].szName;//��ȡ��������
			dCloneSprite(dMakeSpriteName("Weapon",flag2), szName);//dCloneSprite��BumbTemplate�����ڵ�
			dSetSpritePosition(szName, fPosX, fPosY);
			dSetSpriteLinearVelocityPolar(szName, g_fGunStrength, g_fGunRotation);//dSetSpriteLinearVelocityPolar���ڵ������ٶ�
																			  //����Ҫģ�������ʵ���ڵ��������켣��һ�������ߣ�����������Ҫ���ڵ�һ�����µ�����
			dSetSpriteConstantForceY(szName, g_fBumbForceY);//�ڵ��ܳ�������������ͬ�����Է�����벻ͬ���ɹ�����
			flag2 = 0;
		}
}
//==========================================================================
//
// ���̰���
// ���� iKey�������µļ���ֵ�� enum KeyCodes �궨��
// ���� iAltPress, iShiftPress��iCtrlPress�������ϵĹ��ܼ�Alt��Ctrl��Shift��ǰ�Ƿ�Ҳ���ڰ���״̬(0δ���£�1����)
void OnKeyDown( const int iKey, const bool bAltPress, const bool bShiftPress, const bool bCtrlPress )
{	
	if (23<=iKey<=32)//��ȡ���µ�����
	{
		flag2 = iKey-22;//����0�ǵ�һ��������һ��10��
	}

}
//==========================================================================
//
// ���̵���
// ���� iKey������ļ���ֵ�� enum KeyCodes �궨��
void OnKeyUp(const int iKey)
{

}
//===========================================================================
//
// �����뾫����ײ
// ���� szSrcName��������ײ�ľ�������
// ���� szTarName������ײ�ľ�������
void OnSpriteColSprite( const char *szSrcName, const char *szTarName )
{
	if (2 == g_iGameState  && strstr(szSrcName, "DandanBumb"))
	{
		// �жϱ��ڵ������ĸ���Ŀ��
		if (strstr(szTarName, "DandanTarget"))
			ProcessBumbHit(1, szSrcName, szTarName);
		else
			ProcessBumbHit(2, szSrcName, "");
	}
}
//===========================================================================
//
// ����������߽���ײ
// ���� szName����ײ���߽�ľ�������
// ���� iColSide����ײ���ı߽� 0 ��ߣ�1 �ұߣ�2 �ϱߣ�3 �±�
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
			dSpriteMountToSprite("WinWenzi", dMakeSpriteName("Player", i), 0, 20);//��ʾ����λ�ã���������˵������
			dSetTextString("WinWenzi", "");/*��дʤ������*/
			break;
	}//ѡһ����˵��
	dLoadMap("Black.t2d");//�½�һ��ȫ�ڳ������ص����˵�
	dSetTextString("Wenzi", "������һ���밴T���ص����˵��밴F");
	//dSetSpriteColorAlpha(dMakeSpriteName("Level", LevelCount), 255);//���������ͼ������ٽ�����һ����һ��
	//dSetSpriteColorAlpha(dMakeSpriteName("Level", LevelCount+1), 100);
}
void YouLost()
{
	dSetSpriteVisible("YouLost", 1);
	Sleep(3000);
	dLoadMap(dMakeSpriteName("Level", LevelCount));//��ͼ������Level���ؿ���
}