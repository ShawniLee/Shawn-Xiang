#pragma once
///////////////////////////////////////////////////////////////////////
//
// �������
//
///////////////////////////////////////////////////////////////////
#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_
//
#include <Windows.h>
#include "CommonAPI.h"
#include "LessonX.h"
////////////////////////////////////////////////////////////////////
// ��������д����


extern	Treasure *g_pHead;                 // ���������
/*extern  int	GetListSize();	*/						// ���ص�ǰ����ڵ�����
extern Treasure 	*FindNode(int listNum);		// �������ֻ�ȡSprite
													// ��ȡ�������еı���
extern float	map_fSpeedX;
extern float	map_fSpeedY;
extern char		map[5];
extern int		pause;
extern int		g_iScore;
extern int		FpNum;
//
extern void	AddToList(Treasure *pWeapon);		// ���һ��Sprite��������
extern void	DeleteNode(int listNum);	// ��������ɾ��Sprite. 
extern void	DeleteList();						// ɾ������Sprite.
	// ����������ͷ�ļ���ֵ
								//int		 g_listSize = 0;		// �����еĽڵ���

#endif // _LIST_H_