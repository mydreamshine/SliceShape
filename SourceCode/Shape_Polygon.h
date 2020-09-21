#pragma once
#include <float.h>
#include "D3PointList.h"

#define GRAVITY 9.8//중력
#define nConstV 0.1//중력계수
#define nFriction 0.8//탄성계수

class SHAPE_POLYGON
{
private:
	D3POINT_f *CenterPoint;
	float Area;
	float R_Angle;
	float Scale;

	COLORREF_f Color;

	//다각형들끼리 충돌체크 시 사용할 예정(미구현)
	NODE_D3POINT *Max_x, *Min_x, *Max_y, *Min_y;
public:
	LIST_D3POINT Vertex;
	SHAPE_POLYGON();
	~SHAPE_POLYGON();

	void Append_Vertex(D3POINT_f Point);

	void Set_CenterPoint();
	void Set_Area();
	void Set_Pos(D3POINT_f Point);//피벗: 도형 중앙
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

	//도형 자르기(잘린 도형 반환)
	SHAPE_POLYGON* Slice(D3POINT_f Line_P1, D3POINT_f Line_P2);

	//충돌처리
	void CollisionByLine(D3POINT_f Line_P1, D3POINT_f Line_P2, D3POINT_f &Velocity, float &R_Angle);
	//다각형들끼리 충돌처리 <-미구현
	bool CollisionByPolygon(SHAPE_POLYGON &OtherPolygon, D3POINT_f velocity);

	//클리핑 영역 그리기 (알파블렌딩 구현 <- z버퍼 x, 클리핑 영역 바탕색이 먼저 그려져야 적용.)
	void Draw_ClipRegion(D3POINT_f Region[4], const COLORREF_f &RegionColor, const float &Alpha);
};