#pragma once
#include <opencv_all.h>
class Myqueue
{
public:
	Myqueue(int queue_capacity);
	~Myqueue();
	bool                                isQueueEmpty()const;//�ж϶����Ƿ�Ϊ��
	bool                                isQueueFull()const;//�ж϶����Ƿ�Ϊ��
	void                                clear_queue();//��ն���
	int                                 queue_length()const;//�õ����г���
	bool                                InQueue(int  element);//���
	bool                                DeQueue();//����
	int                                 queue_traverse();//��������
	vector<int>                         queueResult();
private:
	int                                 m_iQueueCapacity;//���е�����
	int                                 m_iQueueLength;//���е�ʵ�ʳ���
	int                                 *m_pCircleQ;//Ϊ�������������
	int                                 m_iHead;//����ͷ������Ϊ�������ֻ����Ϊһ������
	int                                 m_iTail;//����β
	vector<int>                         m_vecValue;
};

