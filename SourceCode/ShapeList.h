#pragma once
#include "Shape_Polygon.h"

#define MAX_SHAPE 20

typedef struct NODE_SHAPE_POLYGON
{
	SHAPE_POLYGON Polygon;

	//���� ����
	D3POINT_f Velocity;

	//���� ����
	float Angula_Velocity;//���ӵ�

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


	//�߸� ���� ���� ��ȯ
	size_t Slice_EachOther(D3POINT_f Line_P1, D3POINT_f Line_P2);

	//Slice�÷��� ����
	void SlicedDeActive_AllNode();

	void Check_Outside(CUBE_f Boundary);

	NODE_SHAPE_POLYGON* operator[](int index);
};

//�簢�� ����
NODE_SHAPE_POLYGON* CreateRect(CUBE_f Region);

//�ﰢ�� ����
NODE_SHAPE_POLYGON* CreateTriangle(CUBE_f Region);

//������ ����
NODE_SHAPE_POLYGON* CreatePentagon(CUBE_f Region);

//�� ����(72����: ������ 72��)
NODE_SHAPE_POLYGON* CreateCircle(CUBE_f Region);