#pragma once
#include "Shape_Polygon.h"

#define MAX_SHAPE 20

typedef struct NODE_SHAPE_POLYGON
{
	SHAPE_POLYGON Polygon;

	//선형 관련
	D3POINT_f Velocity;

	//각도 관련
	float Angula_Velocity;//각속도

	bool Sliced;
	NODE_SHAPE_POLYGON *Over;
	NODE_SHAPE_POLYGON *Under;
}NODE_SHAPE_POLYGON;

class LIST_SHAPE_POLYGON
{
private:
	NODE_SHAPE_POLYGON *Top;
	NODE_SHAPE_POLYGON *Bottom;
	size_t Count_Node;

public:
	void Delete_AllNode();
	LIST_SHAPE_POLYGON();
	~LIST_SHAPE_POLYGON();

	size_t Size();

	void append(NODE_SHAPE_POLYGON *NewNode);

	void Move_AllNode();
	void Rotation_AllNode();
	void Draw_AllNode();
	void Draw_ClipRegion(D3POINT_f Region[4], const COLORREF_f &RegionColor, const float &Alpha);

	void Collison_AllNode();
	void Collsion_Line(D3POINT_f Line_P1, D3POINT_f Line_P2);


	//잘린 도형 갯수 반환
	size_t Slice_EachOther(D3POINT_f Line_P1, D3POINT_f Line_P2);

	//Slice플래그 해제
	void SlicedDeActive_AllNode();

	void Check_Outside(CUBE_f Boundary);

	NODE_SHAPE_POLYGON* operator[](int index);
};

//사각형 생성
NODE_SHAPE_POLYGON* CreateRect(CUBE_f Region);

//삼각형 생성
NODE_SHAPE_POLYGON* CreateTriangle(CUBE_f Region);

//오각형 생성
NODE_SHAPE_POLYGON* CreatePentagon(CUBE_f Region);

//원 생성(72각형: 정점이 72개)
NODE_SHAPE_POLYGON* CreateCircle(CUBE_f Region);