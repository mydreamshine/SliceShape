#include "ShapeList.h"

//도형에 힘이 작용할 때 각속도
void Set_SpeedAngle(NODE_SHAPE_POLYGON *Node, D3POINT_f Force, D3POINT_f ForcePoint)
{
	float dx = ForcePoint.x - Node->Polygon.Get_CenterPoint()->x;
	float DirectX = dx / fabsf(dx);
	Node->Angula_Velocity = DirectX * 5.0f;
}

//도형에 힘이 가해졌을 때의 선속도
void Set_Velocity(NODE_SHAPE_POLYGON *Node, D3POINT_f Force)
{
	Node->Velocity.x += Force.x;
	Node->Velocity.y += Force.y;
	Node->Velocity.z += Force.z;
	//중력작용
	Node->Velocity.y += ((float)GRAVITY - Node->Velocity.y) * (float)nConstV;
}

void LIST_SHAPE_POLYGON::Delete_AllNode()
{
	NODE_SHAPE_POLYGON *DeleteNode = nullptr;
	NODE_SHAPE_POLYGON *index = Bottom;
	while (index != nullptr)
	{
		DeleteNode = index;
		index = index->Over;
		DeleteNode->Over = nullptr;
		DeleteNode->Under = nullptr;
		delete DeleteNode;
	}
	Top = nullptr;
	Bottom = nullptr;
	Count_Node = 0;
}

LIST_SHAPE_POLYGON::LIST_SHAPE_POLYGON()
{
	Top = nullptr;
	Bottom = nullptr;
	Count_Node = 0;
}

LIST_SHAPE_POLYGON::~LIST_SHAPE_POLYGON()
{
	Delete_AllNode();
}

void LIST_SHAPE_POLYGON::append(NODE_SHAPE_POLYGON *NewNode)
{
	if (Bottom == nullptr)
	{
		Bottom = NewNode;
		Top = NewNode;
	}
	else
	{
		Top->Over = NewNode;
		NewNode->Under = Top;
		Top = NewNode;
	}

	Count_Node++;
}

size_t LIST_SHAPE_POLYGON::Size()
{
	return Count_Node;
}

void LIST_SHAPE_POLYGON::Move_AllNode()
{
	NODE_SHAPE_POLYGON *index = Bottom;
	while (index != nullptr)
	{
		Set_Velocity(index, { 0.0f,0.0f,0.0f });
		index->Polygon.Move(index->Velocity);
		index = index->Over;
	}
}

void LIST_SHAPE_POLYGON::Rotation_AllNode()
{
	NODE_SHAPE_POLYGON *index = Bottom;
	while (index != nullptr)
	{
		if (fabsf(index->Angula_Velocity) <= 0.002f) index->Angula_Velocity = 0.0f;
		index->Polygon.Rotation(index->Angula_Velocity);
		index = index->Over;
	}
}

void LIST_SHAPE_POLYGON::Draw_AllNode()
{
	NODE_SHAPE_POLYGON *index = Bottom;
	while (index != nullptr)
	{
		index->Polygon.Draw_Polygon();
		index = index->Over;
	}
}

void LIST_SHAPE_POLYGON::Draw_ClipRegion(D3POINT_f Region[4], const COLORREF_f & RegionColor, const float & Alpha)
{
	NODE_SHAPE_POLYGON *index = Bottom;
	while (index != nullptr)
	{
		index->Polygon.Draw_ClipRegion(Region, RegionColor, Alpha);
		index = index->Over;
	}
}

void LIST_SHAPE_POLYGON::Collison_AllNode()
{
	NODE_SHAPE_POLYGON *index = Bottom;
	while (index != nullptr)
	{
		NODE_SHAPE_POLYGON *NextPolygon = index->Over;
		D3POINT_f velocity = { 0.0f, };
		NextPolygon = index->Over;
		while (NextPolygon != nullptr)
		{
			velocity.x = NextPolygon->Velocity.x - index->Velocity.x;
			velocity.y = NextPolygon->Velocity.x - index->Velocity.y;
			NextPolygon->Polygon.CollisionByPolygon(index->Polygon, velocity);
			NextPolygon = NextPolygon->Over;
		}
		index = index->Over;
	}
}

void LIST_SHAPE_POLYGON::Collsion_Line(D3POINT_f Line_P1, D3POINT_f Line_P2)
{
	NODE_SHAPE_POLYGON *index = Bottom;
	while (index != nullptr)
	{
		index->Polygon.CollisionByLine(Line_P1, Line_P2, index->Velocity, index->Angula_Velocity);
		index = index->Over;
	}
}

//잘린 도형 갯수 반환
size_t LIST_SHAPE_POLYGON::Slice_EachOther(D3POINT_f Line_P1, D3POINT_f Line_P2)
{
	NODE_SHAPE_POLYGON *index = Bottom;
	SHAPE_POLYGON *NewPolygon = nullptr;
	size_t Count_SlicePolygon = 0;
	while (index != nullptr)
	{
		if (index->Sliced != true)
		{
			if (NewPolygon = index->Polygon.Slice(Line_P1, Line_P2))
			{//새롭게 잘린 도형
				NODE_SHAPE_POLYGON *NewNode = new NODE_SHAPE_POLYGON;
				NewNode->Polygon = *NewPolygon;
				NewNode->Angula_Velocity = 0.0f;
				NewNode->Velocity = { 0.0f,0.0f,0.0f };
				NewNode->Polygon.Set_Color(index->Polygon.Get_Color());

				float DirectX = (Line_P2.x - Line_P1.x) / fabsf(Line_P2.x - Line_P1.x);
				D3POINT_f Force = { DirectX * 5.0f,15.0f,0.0f };//물체에 가해진 벡터
				D3POINT_f ForcePoint = Line_P1;//작용점

				Set_Velocity(NewNode, Force);
				Set_SpeedAngle(NewNode, Force, ForcePoint);

				NewNode->Polygon.Set_CenterPoint();
				NewNode->Sliced = true;
				NewNode->Under = nullptr;
				NewNode->Over = nullptr;
				
				append(NewNode);//리스트 추가

				Force.y *= -2.0f;//물체에 가해진 벡터y부호 반대(충격량 분배)
				ForcePoint = Line_P1;//작용점

				Set_Velocity(index, Force);
				Set_SpeedAngle(index, Force, ForcePoint);

				index->Polygon.Set_CenterPoint();
				index->Sliced = true;

				Count_SlicePolygon++;
			}
		}
		index = index->Over;
	}
	return Count_SlicePolygon;
}

//Slice 플래그 해제
void LIST_SHAPE_POLYGON::SlicedDeActive_AllNode()
{
	NODE_SHAPE_POLYGON *index = Bottom;
	while (index != nullptr)
	{
		index->Sliced = false;
		index = index->Over;
	}
}

//영역밖에 넘어갔을 시 해당 노드 제거
void LIST_SHAPE_POLYGON::Check_Outside(CUBE_f Boundary)
{
	NODE_SHAPE_POLYGON *index = Bottom;
	NODE_SHAPE_POLYGON *DeleteNode = nullptr;
	while (index != nullptr)
	{
		if (index->Polygon.Get_Max_x()->Point.x < Boundary.left
			|| index->Polygon.Get_Min_x()->Point.x > Boundary.right
			|| index->Polygon.Get_Max_y()->Point.y < Boundary.top
			|| index->Polygon.Get_Min_y()->Point.y > Boundary.bottom)
		{
			DeleteNode = index;
			if (Count_Node == 1)
			{
				Top = nullptr;
				Bottom = nullptr;
			}
			else if (DeleteNode == Bottom)
			{
				Bottom = Bottom->Over;
				Bottom->Under = nullptr;
			}
			else if (DeleteNode == Top)
			{
				Top = Top->Under;
				Top->Over = nullptr;
			}
			else
			{
				index->Under->Over = index->Over;
				index->Over->Under = index->Under;
			}
			index = index->Over;
			DeleteNode->Over = nullptr;
			DeleteNode->Under = nullptr;
			delete DeleteNode;
			Count_Node--;
		}
		else
			index = index->Over;
	}
}

//Node 접근
NODE_SHAPE_POLYGON* LIST_SHAPE_POLYGON::operator[](int index)
{
	//오버엑세싱 or 언더엑세싱 방지
	index = (index >= 0) ? index % (int)Count_Node : (int)Count_Node - (abs(index) % ((int)Count_Node + 1));
	NODE_SHAPE_POLYGON *indexNode = Bottom;

	for (int i = 0; i < index; i++)
		indexNode = indexNode->Over;
	return indexNode;
}

//도형 생성(사각형)
NODE_SHAPE_POLYGON* CreateRect(CUBE_f Region)
{
	NODE_SHAPE_POLYGON *NewShape = new NODE_SHAPE_POLYGON;
	NewShape->Polygon.Append_Vertex({-30.0f + 100.0f, (float)Region.bottom / 2 - 100 + 100.0f, 0.0f });//right-bottom
	NewShape->Polygon.Append_Vertex({ -30.0f + -100.0f, (float)Region.bottom / 2 - 100 + 100.0f, 0.0f });//left-bottom
	NewShape->Polygon.Append_Vertex({ -30.0f + - 100.0f, (float)Region.bottom / 2 - 100 - 100.0f, 0.0f });//left-top
	NewShape->Polygon.Append_Vertex({ -30.0f + 100.0f, (float)Region.bottom / 2 - 100 - 100.0f, 0.0f });//right-top
	NewShape->Polygon.Set_Color({ rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX });
	NewShape->Angula_Velocity = 0.0f;
	NewShape->Velocity = { 25.0f,-25.0f,0.0f };
	NewShape->Sliced = false;
	NewShape->Over = nullptr;
	NewShape->Under = nullptr;
	return NewShape;
}

//삼각형 생성
NODE_SHAPE_POLYGON* CreateTriangle(CUBE_f Region)
{
	NODE_SHAPE_POLYGON *NewShape = new NODE_SHAPE_POLYGON;
	NewShape->Polygon.Append_Vertex({ -30.0f + 100.0f, (float)Region.bottom / 2, 0.0f });//right-bottom
	NewShape->Polygon.Append_Vertex({ -30.0f + -100.0f, (float)Region.bottom / 2, 0.0f });//left-bottom
	NewShape->Polygon.Append_Vertex({ -30.0f, (float)Region.bottom / 2 - 200.0f, 0.0f });//center-top
	NewShape->Polygon.Set_Color({ rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX });
	NewShape->Angula_Velocity = 0.0f;
	NewShape->Velocity = { 25.0f,-25.0f,0.0f };
	NewShape->Sliced = false;
	NewShape->Over = nullptr;
	NewShape->Under = nullptr;
	return NewShape;
}

//오각형 생성
NODE_SHAPE_POLYGON* CreatePentagon(CUBE_f Region)
{
	NODE_SHAPE_POLYGON *NewShape = new NODE_SHAPE_POLYGON;
	D3POINT_f CenterPoint = { -30.0f,Region.bottom / 2, 0.0f };
	const float r = 100.0f;
	const float innerAngle = 360.0f / 5;
	for (size_t i = 0; i < 5; i++)
		NewShape->Polygon.Append_Vertex({ CenterPoint.x + r * cosf(innerAngle*i * (PI / 180.0f)), CenterPoint.y + r * sinf(innerAngle*i * (PI / 180.0f)), 0.0f });

	NewShape->Polygon.Set_Color({ rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX });
	NewShape->Angula_Velocity = 0.0f;
	NewShape->Velocity = { 25.0f,-25.0f,0.0f };
	NewShape->Sliced = false;
	NewShape->Over = nullptr;
	NewShape->Under = nullptr;
	return NewShape;
}

//원 생성(72각형: 정점이 72개)
NODE_SHAPE_POLYGON* CreateCircle(CUBE_f Region)
{
	NODE_SHAPE_POLYGON *NewShape = new NODE_SHAPE_POLYGON;
	D3POINT_f CenterPoint = { -30.0f,Region.bottom / 2, 0.0f };
	const float r = 100.0f;
	const float innerAngle = 360.0f / 72;
	for (size_t i = 0; i < 72; i++)
		NewShape->Polygon.Append_Vertex({ CenterPoint.x + r * cosf(innerAngle*i * (PI / 180.0f)), CenterPoint.y + r * sinf(innerAngle*i * (PI / 180.0f)), 0.0f });

	NewShape->Polygon.Set_Color({ rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX });
	NewShape->Angula_Velocity = 0.0f;
	NewShape->Velocity = { 25.0f,-25.0f,0.0f };
	NewShape->Sliced = false;
	NewShape->Over = nullptr;
	NewShape->Under = nullptr;
	return NewShape;
}
