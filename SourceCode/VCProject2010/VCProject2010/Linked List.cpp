#include <Stdio.h>
#include "Linked List.h"

Treasure *g_pHead = NULL;		// 给武器链表头文件赋值
int		 g_listSize = 0;			// 链表中的节点数
									// 获取主函数中的变量

//
/////////////////////////////////////////////////////////////////////////////
// 返回链表中节点数量
int	GetListSize()
{
	return g_listSize;
}
/////////////////////////////////////////////////////////////////////////////
// 根据武器名称、得分以及时间间隔，创建一个新节点保存到链表中最后一个位置
void AddToList(Treasure *pWeapon)
{
	Treasure* pTemp = (Treasure*)malloc(sizeof(Treasure));
	memcpy(pTemp->WeaponNum, pWeapon->WeaponNum,Weaponcount*sizeof(int));
	pTemp->pNext = NULL;
	Treasure* pNode = g_pHead;

	if (g_pHead == NULL) // 表头为空，说明加入的是一个节点
	{
		g_pHead = pTemp;
	}
	else {
		while (pNode->pNext != NULL)  // 循环读到链表的最后一个节点
		{
			pNode = pNode->pNext;
		}
		pNode->pNext = pTemp;
	}
	g_listSize++;					//	节点数加一

}
/////////////////////////////////////////////////////////////////////////////////
//
// 根据名字获取listNum
Treasure* FindNode(int listNum)
{
	Treasure	*pNode = g_pHead;
	while (NULL != pNode)
	{
		if (pNode->listNum == listNum)
			return pNode;

		pNode = pNode->pNext;
	}

	return NULL;
}


//==============================================================================
//
// 根据名字删除listNum
void DeleteNode(int listNum)
{
	Treasure	*pNode = g_pHead;
	Treasure  *pTemp = NULL;

	// 如果链表不为空
	if (pNode != NULL)
	{
		if (pNode->listNum == listNum)  // 如果表头就是要找的节点
		{
			g_pHead = g_pHead->pNext;
			free(pNode);
		}
		else
		{
			while (pNode->pNext->listNum == listNum) // 该节点的下一个节点不是要找的节点
			{
				pNode = pNode->pNext;
			}
			pTemp = pNode->pNext;
			pNode->pNext = pNode->pNext->pNext;
			free(pTemp);
		}
	}
	g_listSize--;
}

//==============================================================================
//
// 删除所有listNum
void DeleteList()
{
	Treasure	*pNode = NULL;
	Treasure	*pNext = g_pHead;
	while (NULL != pNext)
	{
		pNode = pNext;
		pNext = pNext->pNext;

		// 释放内存
		free(pNode);
	};

	//
	g_listSize = 0;
	g_pHead = NULL;
}
