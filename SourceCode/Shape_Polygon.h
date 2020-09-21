#pragma once
#include <float.h>
#include "D3PointList.h"

#define GRAVITY 9.8//�߷�
#define nConstV 0.1//�߷°��
#define nFriction 0.8//ź�����

class SHAPE_POLYGON
{
private:
	D3POINT_f *CenterPoint;
	float Area;
	float R_Angle;
	float Scale;

	COLORREF_f Color;

	//�ٰ����鳢�� �浹üũ �� ����� ����(�̱���)
	NODE_D3POINT *Max_x, *Min_x, *Max_y, *Min_y;
public:
	LIST_D3POINT Vertex;
	SHAPE_POLYGON();
	~SHAPE_POLYGON();

	void Append_Vertex(D3POINT_f Point);

	void Set_CenterPoint();
	void Set_Area();
	void Set_Pos(D3POINT_f Point);//�ǹ�: ���� �߾�
	void Set_Scale(float Scale);
	void Set_Color(COLORREF_f Color);

	D3POINT_f* Get_CenterPoint();
	float Get_Area();
	NODE_D3POINT* Get_Max_x();
	NODE_D3POINT* Get_Min_x();
	NODE_D3POINT* Get_Max_y();
	NODE_D3POINT* Get_Min_y();
	COLORREF_f Get_Color();

	void Move(D3POINT_f Distance);
	void Rotation(float dAngle);
	void Draw_Polygon();

	//���� �ڸ���(�߸� ���� ��ȯ)
	SHAPE_POLYGON* Slice(D3POINT_f Line_P1, D3POINT_f Line_P2);

	//�浹ó��
	void CollisionByLine(D3POINT_f Line_P1, D3POINT_f Line_P2, D3POINT_f &Velocity, float &R_Angle);
	//�ٰ����鳢�� �浹ó�� <-�̱���
	bool CollisionByPolygon(SHAPE_POLYGON &OtherPolygon, D3POINT_f velocity);

	//Ŭ���� ���� �׸��� (���ĺ��� ���� <- z���� x, Ŭ���� ���� �������� ���� �׷����� ����.)
	void Draw_ClipRegion(D3POINT_f Region[4], const COLORREF_f &RegionColor, const float &Alpha);
};