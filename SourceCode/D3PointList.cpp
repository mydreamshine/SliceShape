#include "D3PointList.h"

void LIST_D3POINT::Clear()
{
	NODE_D3POINT *DeleteNode = nullptr;
	while (DeleteNode = Pop(0)) delete DeleteNode;
	Top = nullptr;
	Bottom = nullptr;
	Count_Node = 0;
}

LIST_D3POINT::LIST_D3POINT()
{
	Top = nullptr;
	Bottom = nullptr;
	Count_Node = 0;
}

LIST_D3POINT::~LIST_D3POINT()
{
	Clear();
}

NODE_D3POINT* LIST_D3POINT::Pop(int index)
{
	NODE_D3POINT *PopNode = nullptr;
	if (Count_Node > 0)
	{
		index = (index > 0) ? index % (Count_Node + 1) : index % Count_Node;

		if (index >= 0)
		{
			PopNode = Bottom;
			for (int i = 0; i < index; i++) PopNode = PopNode->Over;
		}
		else
		{
			PopNode = Top;
			for (int i = index; i < 0; i++) PopNode = PopNode->Under;
		}

		if (Count_Node == 1)
		{
			Bottom = nullptr;
			Top = nullptr;
		}
		else if (PopNode == Bottom)
		{
			Bottom = Bottom->Over;
			Bottom->Under = Top;
			Top->Over = Bottom;
		}
		else if (PopNode == Top)
		{
			Top = Top->Under;
			Top->Over = Bottom;
			Bottom->Under = Top;
		}
		else
		{
			PopNode->Under->Over = PopNode->Over;
			PopNode->Over->Under = PopNode->Under;
		}

		PopNode->Under = nullptr;
		PopNode->Over = nullptr;

		Count_Node--;
	}
	return PopNode;
}

void LIST_D3POINT::Set_BottomTop(NODE_D3POINT *Bottom, NODE_D3POINT *Top)
{
	this->Top = Top;
	this->Bottom = Bottom;
	NODE_D3POINT *indexNode = Bottom;
	size_t Count = 0;
	if (indexNode != nullptr)
	{
		do
		{
			Count++;
			indexNode = indexNode->Over;
		} while (indexNode != Bottom);
	}
	Count_Node = Count;
}

void LIST_D3POINT::Append(D3POINT_f Point)
{
	NODE_D3POINT *NewNode = new NODE_D3POINT;
	NewNode->Point = Point;

	if (Count_Node == 0)
	{
		Bottom = NewNode;
		Top = NewNode;
	}
	NewNode->Over = Bottom;
	NewNode->Under = Top;
	
	Bottom->Under = NewNode;
	Top->Over = NewNode;

	Top = Top->Over;

	Count_Node++;
}
void LIST_D3POINT::Insert(D3POINT_f Point, int index)
{
	NODE_D3POINT *indexNode = nullptr;
	NODE_D3POINT *NewNode = new NODE_D3POINT;
	NewNode->Point = Point;

	if (Count_Node == 0)
	{
		Bottom = NewNode;
		Top = NewNode;
		NewNode->Over = Bottom;
		NewNode->Under = Top;
	}
	else if (index >= (int)Count_Node)
	{
		NewNode->Over = Bottom;
		NewNode->Under = Top;
		Top->Over = NewNode;
		Bottom->Under = NewNode;
		Top = NewNode;
	}
	else if (index <= -(int)Count_Node)
	{
		NewNode->Over = Bottom;
		NewNode->Under = Top;
		Bottom->Under = NewNode;
		Top->Over = NewNode;
		Bottom = NewNode;
	}
	else
	{
		indexNode = Bottom;
		if (index >= 0)
		{
			for (int i = 0; i < index; i++) indexNode = indexNode->Over;
		}
		else
		{
			for (int i = index; i < 0; i++) indexNode = indexNode->Under;
		}

		if (indexNode == Bottom)
		{
			NewNode->Over = Bottom;
			NewNode->Under = Top;
			Bottom->Under = NewNode;
			Bottom = Bottom->Under;
			Top->Over = Bottom;
		}
		else if (indexNode == Top)
		{
			NewNode->Over = Top;
			NewNode->Under = Top->Under;
			Top->Under->Over = NewNode;
			Top->Under = NewNode;
		}
		else
		{
			NewNode->Over = indexNode;
			NewNode->Under = indexNode->Under;
			indexNode->Under->Over = NewNode;
			indexNode->Under = NewNode;
		}
	}

	Count_Node++;
}

//SplitNode: ������ ������ ���, start: ����1, end: ����2, NewList: ������ ������ ����� ������ PointList
void LIST_D3POINT::Split(NODE_D3POINT *SplitNode_Start, NODE_D3POINT *SplitNode_End, NODE_D3POINT *start_IP, NODE_D3POINT *end_IP, LIST_D3POINT *NewList)
{
	NODE_D3POINT *IP1_s = new NODE_D3POINT;
	NODE_D3POINT *IP2_s = new NODE_D3POINT;

	NODE_D3POINT *IP1_b = new NODE_D3POINT;
	NODE_D3POINT *IP2_b = new NODE_D3POINT;

	//Vertex ��ġ(�������� �� or �Ʒ�)
	bool UpsideLine[2] = { false, };

	//Vertex�迭 ������ �ð������ ��쿡�� ����.
	if ((start_IP->Point.y > end_IP->Point.y && start_IP->Point.x < end_IP->Point.x)//�ע��϶�
		|| (start_IP->Point.y < end_IP->Point.y && start_IP->Point.x < end_IP->Point.x))//���� ��� �٢��϶�
	{
		NODE_D3POINT *Temp = start_IP;
		start_IP = end_IP;
		end_IP = Temp;

		SplitNode_End = SplitNode_End->Over;
		Temp = SplitNode_End;
		SplitNode_End = SplitNode_Start;
		SplitNode_Start = Temp;
		Temp = nullptr;
		UpsideLine[1] = true;
	}
	else if ((start_IP->Point.y > end_IP->Point.y && start_IP->Point.x > end_IP->Point.x)//�٢��϶�
		|| (start_IP->Point.y < end_IP->Point.y && start_IP->Point.x > end_IP->Point.x))//���� �ϴ� �ע��϶�
	{
		SplitNode_Start = SplitNode_Start->Over;
		UpsideLine[0] = true;
	}
	else if (start_IP->Point.y == end_IP->Point.y || start_IP->Point.x == end_IP->Point.x)
	{
		SplitNode_Start = SplitNode_Start->Over;
		UpsideLine[0] = true;
	}
	else if (UpsideLine[0] == false && UpsideLine[1] == false)
	{
		NODE_D3POINT *Temp = start_IP;
		start_IP = end_IP;
		end_IP = Temp;

		SplitNode_End = SplitNode_End->Over;
		Temp = SplitNode_End;
		SplitNode_End = SplitNode_Start;
		SplitNode_Start = Temp;
		Temp = nullptr;
		UpsideLine[1] = true;
	}
	if (UpsideLine[0] == true || UpsideLine[1] == true)
	{
		IP1_b->Point = start_IP->Point;
		IP2_b->Point = end_IP->Point;
		IP1_s->Point = start_IP->Point;
		IP2_s->Point = end_IP->Point;

		//���� �� ���� ����
		IP1_b->Over = IP2_b;//bottom
		IP2_b->Over = SplitNode_End->Over;
		IP1_b->Under = SplitNode_Start->Under;
		IP2_b->Under = IP1_b;//top

		//���� �Ʒ� ���� ����
		IP1_s->Over = SplitNode_Start;//bottom
		IP2_s->Over = IP1_s;//top
		IP1_s->Under = IP2_s;//bottom
		IP2_s->Under = SplitNode_End;

		//���� ������ Node ���� �ٲٱ�
		SplitNode_Start->Under->Over = IP1_b;
		SplitNode_End->Over->Under = IP2_b;
		Set_BottomTop(IP2_b, IP1_b);//���� PointList�� ���۳��� ����� ����

		//�и��� ������ Node ���� �ٲٱ�
		SplitNode_Start->Under = IP1_s;
		SplitNode_End->Over = IP2_s;
		if (SplitNode_Start == SplitNode_End)
		{
			SplitNode_Start->Over = IP2_s;
			SplitNode_End->Under = IP1_s;
		}

		//�и��� PointList�� ���۳��� ����� ����
		NewList->Set_BottomTop(IP1_s, IP2_s);
	}
}

NODE_D3POINT* LIST_D3POINT::Get_StartNode()
{
	return Bottom;
}

size_t LIST_D3POINT::Size()
{
	return Count_Node;
}

//�� ������ ����
NODE_D3POINT* GetIntersectPoint(const D3POINT_f &LineA_P1, const D3POINT_f &LineA_P2,
	const D3POINT_f &LineB_P1, const D3POINT_f &LineB_P2)
{
	float Intersect_ratioA = 0.0f;
	float Intersect_ratioB = 0.0f;

	//�� ������ �̷�� ����纯���� ���� (Parallelogram : ����纯��)
	float Parallelogram = (LineB_P2.y - LineB_P1.y)*(LineA_P2.x - LineA_P1.x) - (LineB_P2.x - LineB_P1.x)*(LineA_P2.y - LineA_P1.y);
	if (Parallelogram == 0) return nullptr;

	//Line_A���� ����纯���� �ش��ϴ� ���� ����
	float RatioA_Par = (LineB_P2.x - LineB_P1.x)*(LineA_P1.y - LineB_P1.y) - (LineB_P2.y - LineB_P1.y)*(LineA_P1.x - LineB_P1.x);
	//Line_B���� ����纯���� �ش��ϴ� ���� ����
	float RatioB_Par = (LineA_P2.x - LineA_P1.x)*(LineA_P1.y - LineB_P1.y) - (LineA_P2.y - LineA_P1.y)*(LineA_P1.x - LineB_P1.x);

	//����纯�� �� ������ ���� ���
	if (RatioA_Par == 0.0f && RatioB_Par == 0.0f) return nullptr;

	//������ ���� ���� ��ġ ����
	Intersect_ratioA = RatioA_Par / Parallelogram;
	Intersect_ratioB = RatioB_Par / Parallelogram;

	//0 ~ 1 ����: ���� �߻�, 0,1 ���� ��: ���� X
	if (Intersect_ratioA < 0.0f || Intersect_ratioA > 1.0f || Intersect_ratioB < 0.0f || Intersect_ratioB > 1.0f) return nullptr;

	NODE_D3POINT *IP = new NODE_D3POINT;
	IP->Point.x = LineA_P1.x + Intersect_ratioA * (float)(LineA_P2.x - LineA_P1.x);
	IP->Point.y = LineA_P1.y + Intersect_ratioA * (float)(LineA_P2.y - LineA_P1.y);
	IP->Point.z = 0.0f;
	IP->Over = nullptr;
	IP->Under = nullptr;

	return IP;
}

//���� ������ �Ÿ�
float DistPointWithLine(const D3POINT_f &Point, D3POINT_f Line_P1, D3POINT_f Line_P2)
{
	//�������� ���� �ִ� ���� ����, �Ʒ��ִ� ���� ���
	if (Line_P1.x > Line_P2.x)
	{
		D3POINT_f Temp = Line_P1;
		Line_P1 = Line_P2;
		Line_P2 = Temp;
	}
	//������ ������ ax + by + z = 0;
	float D = (Line_P1.x - Point.x) * (Line_P2.y - Point.y) - (Line_P1.y - Point.y) * (Line_P2.x - Point.x);
	float DistLine = sqrtf(pow(Line_P2.x - Line_P1.x, 2) + pow(Line_P2.y - Line_P1.y, 2));
	return D / DistLine;
}