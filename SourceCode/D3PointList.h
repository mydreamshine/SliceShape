#pragma once
#include "add_DataType.h"
#include "stdafx.h"

typedef struct NODE_D3POINT
{
	D3POINT_f Point;
	NODE_D3POINT *Over;
	NODE_D3POINT *Under;
}NODE_D3POINT;

class LIST_D3POINT
{
private:
	NODE_D3POINT *Top;
	NODE_D3POINT *Bottom;
	size_t Count_Node;

	NODE_D3POINT* Pop(int index);
public:
	LIST_D3POINT();
	~LIST_D3POINT();

	void Set_BottomTop(NODE_D3POINT *Bottom, NODE_D3POINT *Top);

	void Append(D3POINT_f Point);
	void Insert(D3POINT_f Point, int index);

	void Clear();

	void Split(NODE_D3POINT *SplitNode_Start, NODE_D3POINT *SplitNode_End, NODE_D3POINT *start_IP, NODE_D3POINT *end_IP, LIST_D3POINT *NewList);

	NODE_D3POINT* Get_StartNode();

	size_t Size();
};

//두 직선의 교점
NODE_D3POINT* GetIntersectPoint(const D3POINT_f& AP1, const D3POINT_f& AP2, const D3POINT_f& BP1, const D3POINT_f& BP2);

//점과 직선의 거리
float DistPointWithLine(const D3POINT_f &Point, D3POINT_f Line_P1, D3POINT_f Line_P2);


//원 그리기
void Draw_Circle(float CPointX, float CPointY, float r, COLORREF_f Color);

//줄어드는 선
class ReducedLine
{
private:
	D3POINT_f P1, P2;
	float Speed;
	float LineWidth;
public:
	ReducedLine();
	~ReducedLine() {};

	void Set_Speed(float Speed);
	void Set_P1(D3POINT_f Point);
	void Set_P2(D3POINT_f Point);
	void Set_LineWidth(float Width);

	D3POINT_f Get_P1();
	D3POINT_f Get_P2();
	float Get_dx();//P2-P1
	float Get_dy();//P2-P1

	void Reduced();

	void Draw();
};