#include <Stdio.h>
#include "Linked List.h"

Treasure *g_pHead = NULL;		// ����������ͷ�ļ���ֵ
int		 g_listSize = 0;			// �����еĽڵ���
									// ��ȡ�������еı���

//
/////////////////////////////////////////////////////////////////////////////
// ���������нڵ�����
int	GetListSize()
{
	return g_listSize;
}
/////////////////////////////////////////////////////////////////////////////
// �����������ơ��÷��Լ�ʱ����������һ���½ڵ㱣�浽���������һ��λ��
void AddToList(Treasure *pWeapon)
{
	Treasure* pTemp = (Treasure*)malloc(sizeof(Treasure));
	memcpy(pTemp->WeaponNum, pWeapon->WeaponNum,Weaponcount*sizeof(int));
	pTemp->pNext = NULL;
	Treasure* pNode = g_pHead;

	if (g_pHead == NULL) // ��ͷΪ�գ�˵���������һ���ڵ�
	{
		g_pHead = pTemp;
	}
	else {
		while (pNode->pNext != NULL)  // ѭ��������������һ���ڵ�
		{
			pNode = pNode->pNext;
		}
		pNode->pNext = pTemp;
	}
	g_listSize++;					//	�ڵ�����һ

}
/////////////////////////////////////////////////////////////////////////////////
//
// �������ֻ�ȡlistNum
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
// ��������ɾ��listNum
void DeleteNode(int listNum)
{
	Treasure	*pNode = g_pHead;
	Treasure  *pTemp = NULL;

	// �������Ϊ��
	if (pNode != NULL)
	{
		if (pNode->listNum == listNum)  // �����ͷ����Ҫ�ҵĽڵ�
		{
			g_pHead = g_pHead->pNext;
			free(pNode);
		}
		else
		{
			while (pNode->pNext->listNum == listNum) // �ýڵ����һ���ڵ㲻��Ҫ�ҵĽڵ�
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
// ɾ������listNum
void DeleteList()
{
	Treasure	*pNode = NULL;
	Treasure	*pNext = g_pHead;
	while (NULL != pNext)
	{
		pNode = pNext;
		pNext = pNext->pNext;

		// �ͷ��ڴ�
		free(pNode);
	};

	//
	g_listSize = 0;
	g_pHead = NULL;
}
