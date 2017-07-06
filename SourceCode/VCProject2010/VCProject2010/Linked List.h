#pragma once
///////////////////////////////////////////////////////////////////////
//
// 链表管理
//
///////////////////////////////////////////////////////////////////
#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_
//
#include <Windows.h>
#include "CommonAPI.h"
#include "LessonX.h"
////////////////////////////////////////////////////////////////////
// 在这里填写代码


extern	Treasure *g_pHead;                 // 链表管理函数
/*extern  int	GetListSize();	*/						// 返回当前链表节点数量
extern Treasure 	*FindNode(int listNum);		// 根据名字获取Sprite
													// 获取主函数中的变量
extern float	map_fSpeedX;
extern float	map_fSpeedY;
extern char		map[5];
extern int		pause;
extern int		g_iScore;
extern int		FpNum;
//
extern void	AddToList(Treasure *pWeapon);		// 添加一个Sprite到链表里
extern void	DeleteNode(int listNum);	// 根据名字删除Sprite. 
extern void	DeleteList();						// 删除所有Sprite.
	// 给精灵链表头文件赋值
								//int		 g_listSize = 0;		// 链表中的节点数

#endif // _LIST_H_