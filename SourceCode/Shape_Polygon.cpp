#include "Shape_Polygon.h"

#define Coefficient_Friction 0.8f //마찰계수

SHAPE_POLYGON::SHAPE_POLYGON()
{
	CenterPoint = nullptr;
	Area = 0.0f;
	Color = { 0.0f, };
	R_Angle = 0.0f;
	Scale = 1.0f;

	//↓다각형 끼리 충돌한 경우를 확인하기 위한 도형의 Vertex 중 최소/최대 값
	Max_x = nullptr;
	Min_x = nullptr;
	Max_y = nullptr;
	Min_y = nullptr;
}

SHAPE_POLYGON::~SHAPE_POLYGON()
{
	if (CenterPoint != nullptr) delete CenterPoint;
}

void SHAPE_POLYGON::Append_Vertex(D3POINT_f Point)
{
	Vertex.Append(Point);
}

void SHAPE_POLYGON::Set_Color(COLORREF_f Color)
{
	this->Color = Color;
}

//다각형의 중점(≒ 무게중심)
//공식 출처 :https://m.blog.naver.com/PostView.nhn?blogId=a630203&logNo=120213239668&proxyReferer=https%3A%2F%2Fwww.google.co.kr%2F
//CenterY = 1/(6*면적) * ∑{ (y_i + y_(i+1)) * (x_i * y_(i+1) - x_(i+1) * y_i) }
//CenterX = 1/(6*면적) * ∑{ (x_i + x_(i+1)) * (x_i * y_(i+1) - x_(i+1) * y_i) }
void SHAPE_POLYGON::Set_CenterPoint()
{
	NODE_D3POINT *index = Vertex.Get_StartNode();
	NODE_D3POINT *End = Vertex.Get_StartNode();

	if (index != nullptr)
	{
		float tArea = 0.0f;
		float Factor = 0.0f;//n번째 항 및 1/(6*면적)
		float CenterX = 0.0f;
		float CenterY = 0.0f;
		do
		{
			Factor = (index->Point.x * index->Over->Point.y) - (index->Over->Point.x * index->Point.y);
			tArea += Factor;

			CenterX += (index->Point.x + index->Over->Point.x) * Factor;
			CenterY += (index->Point.y + index->Over->Point.y) * Factor;
			index = index->Over;
		} while (index != End);

		tArea /= 2.0f;

		if (Area == 0.0f) Area = tArea;

		tArea *= 6.0f;

		Factor = 1.0f / tArea;

		CenterX *= Factor;
		CenterY *= Factor;

		if (CenterPoint == nullptr) CenterPoint = new D3POINT_f;
		*CenterPoint = { CenterX, CenterY, 0.0f };
	}
}

//다각형 면적
//공식 출처 :https://m.blog.naver.com/PostView.nhn?blogId=a630203&logNo=120213239668&proxyReferer=https%3A%2F%2Fwww.google.co.kr%2F
//Area = 1/2 * ∑(x_i * y_(i+1) - x_(i+1) * y_i)
void SHAPE_POLYGON::Set_Area()
{
	NODE_D3POINT *index = Vertex.Get_StartNode();
	NODE_D3POINT *End = Vertex.Get_StartNode();

	if (index != nullptr)
	{
		float Factor = 0.0f;//n번째 항
		do
		{
			Factor = (index->Point.x * index->Over->Point.y) - (index->Over->Point.x * index->Point.y);
			Area += Factor;
			index = index->Over;
		} while (index != End);
		Area /= 2.0f;
	}
}

//다각형의 중점(≒ 무게중심)
D3POINT_f* SHAPE_POLYGON::Get_CenterPoint()
{
	if (CenterPoint == nullptr)
		Set_CenterPoint();
	return CenterPoint;
}

//다각형의 면적
float SHAPE_POLYGON::Get_Area()
{
	if (Area == 0.0f)
		Set_Area();
	return Area;
}

NODE_D3POINT* SHAPE_POLYGON::Get_Max_x()
{
	return Max_x;
}

NODE_D3POINT* SHAPE_POLYGON::Get_Min_x()
{
	return Min_x;
}

NODE_D3POINT* SHAPE_POLYGON::Get_Max_y()
{
	return Max_y;
}

NODE_D3POINT* SHAPE_POLYGON::Get_Min_y()
{
	return Min_y;
}

COLORREF_f SHAPE_POLYGON::Get_Color()
{
	return Color;
}

//피벗: 도형 중앙
void SHAPE_POLYGON::Set_Pos(D3POINT_f Point)
{
	if (CenterPoint == nullptr) CenterPoint = Get_CenterPoint();
	NODE_D3POINT *index = Vertex.Get_StartNode();
	NODE_D3POINT *End = Vertex.Get_StartNode();
	if (index != nullptr)
	{
		Max_x = Min_x = index;
		Max_y = Min_y = index;

		D3POINT_f Dist;//이전 위치와의 거리차이
		Dist.x = Point.x - CenterPoint->x;
		Dist.y = Point.y - CenterPoint->y;
		Dist.z = Point.z - CenterPoint->z;
		do
		{
			index->Point.x += Dist.x;
			index->Point.y += Dist.y;
			index->Point.z += Dist.z;

			if (Max_x->Point.x < index->Point.x)
				Max_x = index;
			if (Min_x->Point.x > index->Point.x)
				Min_x = index;
			if (Max_y->Point.y < index->Point.y)
				Max_y = index;
			if (Min_y->Point.y > index->Point.y)
				Min_y = index;

			index = index->Over;
		} while (index != End);

		*CenterPoint = Point;
	}
}

//도형의 모든 Vertex가 Distance만큼 이동
void SHAPE_POLYGON::Move(D3POINT_f Distance)
{
	NODE_D3POINT *index = Vertex.Get_StartNode();
	NODE_D3POINT *End = Vertex.Get_StartNode();

	if (CenterPoint == nullptr) CenterPoint = Get_CenterPoint();

	if (index != nullptr)
	{
		Max_x = Min_x = index;
		Max_y = Min_y = index;
		do
		{
			index->Point.x += Distance.x;
			index->Point.y += Distance.y;
			index->Point.z += Distance.z;

			if (Max_x->Point.x < index->Point.x)
				Max_x = index;
			if (Min_x->Point.x > index->Point.x)
				Min_x = index;
			if (Max_y->Point.y < index->Point.y)
				Max_y = index;
			if (Min_y->Point.y > index->Point.y)
				Min_y = index;

			index = index->Over;
		} while (index != End);

		CenterPoint->x += Distance.x;
		CenterPoint->y += Distance.y;
		CenterPoint->z += Distance.z;
	}
}

//x-y평면 회전(정점 간의 출돌체크를 위해 도형의 정점을 직접 회전변환한다.)
void SHAPE_POLYGON::Rotation(float dAngle)
{
	R_Angle = dAngle;
	NODE_D3POINT *index = Vertex.Get_StartNode();
	NODE_D3POINT *End = Vertex.Get_StartNode();
	if (index != nullptr)
	{
		Max_x = Min_x = index;
		Max_y = Min_y = index;

		float x_ = 0.0f, y_ = 0.0f;
		if (CenterPoint == nullptr) CenterPoint = Get_CenterPoint();

		do
		{
			x_ = index->Point.x;
			y_ = index->Point.y;
			if (CenterPoint != nullptr)
			{//도형의 중심을 원점(0,0)으로 평행이동
				x_ -= CenterPoint->x;
				y_ -= CenterPoint->y;
			}
			//회전변환(좌표계 회전x)
			index->Point.x = x_ * cosf(R_Angle * PI / 180.0f) - y_ * sinf(R_Angle * PI / 180.0f);
			index->Point.y = x_ * sinf(R_Angle * PI / 180.0f) + y_ * cosf(R_Angle * PI / 180.0f);
			if (CenterPoint != nullptr)
			{//도형의 중심을 원래위치로 평행이동
				index->Point.x += CenterPoint->x;
				index->Point.y += CenterPoint->y;
			}

			if (Max_x->Point.x < index->Point.x)
				Max_x = index;
			if (Min_x->Point.x > index->Point.x)
				Min_x = index;
			if (Max_y->Point.y < index->Point.y)
				Max_y = index;
			if (Min_y->Point.y > index->Point.y)
				Min_y = index;

			index = index->Over;
		} while (index != End);
	}
}

//x-y평면 확대축소(균등) (정점 간의 출돌체크를 위해 도형의 정점을 직접 확대축소변환한다.)
void SHAPE_POLYGON::Set_Scale(float Scale)
{
	this->Scale = Scale;
	NODE_D3POINT *index = Vertex.Get_StartNode();
	NODE_D3POINT *End = Vertex.Get_StartNode();
	if (index != nullptr)
	{
		Max_x = Min_x = index;
		Max_y = Min_y = index;

		if (CenterPoint == nullptr) CenterPoint = Get_CenterPoint();
		do
		{
			if (CenterPoint != nullptr)
			{//도형의 중심을 원점(0,0)으로 평행이동
				index->Point.x -= CenterPoint->x;
				index->Point.y -= CenterPoint->y;
			}
			//확대축소 변환
			index->Point.x *= Scale;
			index->Point.y *= Scale;
			if (CenterPoint != nullptr)
			{//도형의 중심을 원래위치로 평행이동
				index->Point.x += CenterPoint->x;
				index->Point.y += CenterPoint->y;
			}

			if (Max_x->Point.x < index->Point.x)
				Max_x = index;
			if (Min_x->Point.x > index->Point.x)
				Min_x = index;
			if (Max_y->Point.y < index->Point.y)
				Max_y = index;
			if (Min_y->Point.y > index->Point.y)
				Min_y = index;
			index = index->Over;
		} while (index != End);
	}
}

//다각형's 그리기
void SHAPE_POLYGON::Draw_Polygon()
{
	NODE_D3POINT *index = Vertex.Get_StartNode();
	NODE_D3POINT *End = Vertex.Get_StartNode();
	if (index != nullptr)
	{
		if (CenterPoint == nullptr) CenterPoint = Get_CenterPoint();
		glPushMatrix();
		glColor3f(Color.R, Color.G, Color.B);
		glBegin(GL_POLYGON);
		do
		{
			glVertex2f(index->Point.x, index->Point.y);
			index = index->Over;
		} while (index != End);
		glEnd();
		glPopMatrix();
	}
}

//다각형 자르기(교점 갯수 판단)
SHAPE_POLYGON* SHAPE_POLYGON::Slice(D3POINT_f Line_P1, D3POINT_f Line_P2)
{
	SHAPE_POLYGON *SliceShape = nullptr;

	//교점갯수
	size_t Count_intersect = 0;

	//교점
	NODE_D3POINT *IP[2] = { nullptr, };

	NODE_D3POINT *index = Vertex.Get_StartNode();
	NODE_D3POINT *End = Vertex.Get_StartNode();

	if (index != nullptr)
	{
		//교점의 이전 혹은 이후의 Point 노드
		NODE_D3POINT *SplitNode[2] = { nullptr, };
		do
		{
			if (IP[Count_intersect] = GetIntersectPoint(index->Point, index->Over->Point, Line_P1, Line_P2))
			{//교점 구하기
				SplitNode[Count_intersect] = index;
				Count_intersect++;
			}
			if (Count_intersect == 2)//교점 갯수가 2개일 때 ≒ 도형이 잘린 상태
			{
				SliceShape = new SHAPE_POLYGON;
				Vertex.Split(SplitNode[0], SplitNode[1], IP[0], IP[1], &SliceShape->Vertex);
				break;
			}
			index = index->Over;
		} while (index != End);

		if (IP[0] != nullptr) delete IP[0];
		if (IP[1] != nullptr) delete IP[1];
	}

	return SliceShape;
}

//2017.10.11. 수직선 / 수평선 충돌체크 (dx,dy) 방식
//2017.10.15. 충돌 후 운동처리 (속도 *= -탄성계수)
//2017.10.16. 수직선 / 수평선 끝점 충돌 에러
//2017.10.20. 수직선 / 수평선 충돌체크 (교점(충돌작용점)) 방식
//2017.10.21. 충돌 후 운동처리 (직선과의 위치에 따른 선속도, 현재 회전방향과 작용점에 따른 각속도)
//2017.10.22. 충돌 후 수직선 끝점 충돌 에러 (임시방편으로 회전방향을 반전시키고 y축으로의 좌표조정)
/*
작용점에 따른 각속도(회전토크의 변화)
작용점이 물체의 무게중심과 얼마나 가까이 있느냐에 따라 충돌 후의 회전속도가 다르다.
가까이 있을 수록(dist = sqrt(pow(dx,2)+pow(dy,2))가 작을 수록) 충돌 후의 회전속도가 적고,
멀리 있을 수록(dist = sqrt(pow(dx,2)+pow(dy,2))가 클 수록) 충돌 후의 회전속도가 크다. (회전관성 모멘트)
*/
//무게중심과 중력에 따른 도형 회전속도 구현 필요
//(물체가 무게중심과 작용점 사이의 삼각구도 밸런스에 맞추어 회전해야, 좀 더 그럴듯한 회전관성이 된다.)
//수식화가 어렵고, 회전 동역학 내용을 공부할 필요가 있음.
void SHAPE_POLYGON::CollisionByLine(D3POINT_f Line_P1, D3POINT_f Line_P2, D3POINT_f &Velocity, float &R_Angle)
{
	//교점(IP : Intersect Point)
	NODE_D3POINT *IP =nullptr;

	NODE_D3POINT *index = Vertex.Get_StartNode();
	NODE_D3POINT *End = Vertex.Get_StartNode();

	if (index != nullptr)
	{
		do
		{
			//교점 구하기(충돌 작용점)
			if (IP = GetIntersectPoint(index->Point, index->Over->Point, Line_P1, Line_P2))
			{
				//수직선일 경우
				if (Line_P1.x == Line_P2.x)
				{
					D3POINT_f LeftPoint = (index->Point.x <= IP->Point.x) ? index->Point : index->Over->Point;
					D3POINT_f RightPoint = (index->Point.x >= IP->Point.x) ? index->Point : index->Over->Point;

					//물체의 무게중심이 작용점보다 왼쪽에 있을 경우 ≒ (물체가 수직선의 왼쪽에 있는 상태)
					if (CenterPoint->x < IP->Point.x)
					{
						//수직선의 오른쪽에 있는 점까지의 거리(충돌했을 때의 교차거리)만큼 좌표조정
						float dist = DistPointWithLine(RightPoint, Line_P1, Line_P2);
						float dx_Line = Line_P2.x - Line_P1.x;
						float dy_Line = Line_P2.y - Line_P1.y;
						D3POINT_f dist_divXY = { dist * (dy_Line / (fabsf(dx_Line) + fabsf(dy_Line))), dist * (dx_Line / (fabsf(dx_Line) + fabsf(dy_Line))), 0.0f };
						if (CenterPoint->y < Line_P1.y) dist_divXY = { 0.0f , Line_P1.y - IP->Point.y, 0.0f };
						Move(dist_divXY);

						//왼쪽으로 움직이고 있는 상태에서 물체의 오른쪽에 수직선이 충돌할 경우 현재 x속도값을 증가
						Velocity.x *= (Velocity.x > 0) ? -nFriction : 1.3;
						//물체의 무게중심이 작용점보다 위에 있을 경우
						if (CenterPoint->y < IP->Point.y)
						{
							//아래로 이동중일 때 충돌할 경우 y속도 가속, 위로 이동중일 충돌할 경우 y속도 감속
							Velocity.y += (Velocity.y > 0) ? Velocity.y * 0.3 : -Velocity.y * 0.3;
							//시계반대방향으로 회전중일 때 작용점에 따라 회전토크를 다르게 하여 현재 회전속도에 부여, 시계방향일 경우 현재 회전속도 증가
							R_Angle *= (R_Angle < 0) ? ((IP->Point.x - CenterPoint->x) / (Max_x->Point.x - CenterPoint->x)) *(-nFriction) : 1.1;
							//회전방향반전과 위치 조정(임시방편)
							if (Line_P1.y > CenterPoint->y)
							{
								Velocity.y *= (Velocity.y > 0) ? -1 : 1;
								R_Angle *= (R_Angle > 0) ? -1 : 1;
							}
						}
						//물체의 무게중심이 작용점보다 아래에 있을 경우
						else if (CenterPoint->y > IP->Point.y)
						{
							//아래로 이동중일 때 충돌할 경우 y속도 반전, 위로 이동중일 충돌할 경우 y속도 감속
							////							  속도0으로 + 반전된속도 * 탄성계수
							Velocity.y += (Velocity.y > 0) ? -Velocity.y -Velocity.y * nFriction : -Velocity.y * 0.3;
							//시계방향으로 회전중일 때 작용점에 따라 회전토크를 다르게 하여 현재 회전속도에 부여, 시계반대방향일 경우 현재 회전속도 증가
							R_Angle *= (R_Angle > 0) ? ((IP->Point.x - CenterPoint->x) / (Max_x->Point.x - CenterPoint->x)) *(-nFriction) : 1.1;
							//회전방향반전과 위치 조정(임시방편)
							if (Line_P1.y > CenterPoint->y)
							{
								Velocity.y *= (Velocity.y > 0) ? -1 : 1;
								R_Angle *= (R_Angle > 0) ? -1 : 1;
							}
						}
					}
					else if (CenterPoint->x > IP->Point.x)//물체의 무게중심이 작용점보다 오른쪽에 있을 경우 ≒ (물체가 수직선의 오른쪽에 있는 상태)
					{
						//수직선의 왼쪽에 있는 점까지의 거리(충돌했을 때의 교차거리)만큼 좌표조정
						float dist = DistPointWithLine(LeftPoint, Line_P1, Line_P2);
						float dx_Line = Line_P2.x - Line_P1.x;
						float dy_Line = Line_P2.y - Line_P1.y;
						D3POINT_f dist_divXY = { dist * (dy_Line / (fabsf(dx_Line) + fabsf(dy_Line))), dist * (dx_Line / (fabsf(dx_Line) + fabsf(dy_Line))), 0.0f };
						if (CenterPoint->y < Line_P1.y) dist_divXY = { 0.0f , Line_P1.y - IP->Point.y, 0.0f };
						Move(dist_divXY);

						//오른쪽으로 움직이고 있는 상태에서 물체의 왼쪽에 수직선이 충돌할 경우 현재 x속도값을 증가
						Velocity.x *= (Velocity.x < 0) ? -nFriction : 1.3;
						//물체의 무게중심이 작용점보다 위에 있을 경우
						if (CenterPoint->y < IP->Point.y)
						{
							//아래로 이동중일 때 충돌할 경우 y속도 가속, 위로 이동중일 충돌할 경우 y속도 감속
							Velocity.y += (Velocity.y > 0) ? Velocity.y * 0.3 : -Velocity.y * 0.3;
							//시계방향으로 회전중일 때 작용점에 따라 회전토크를 다르게 하여 현재 회전속도에 부여, 시계반대방향일 경우 현재 회전속도 증가
							R_Angle *= (R_Angle > 0) ? ((CenterPoint->x - IP->Point.x) / (CenterPoint->x - Min_x->Point.x)) *(-nFriction) : 1.1;
							//회전방향반전과 위치 조정(임시방편)
							if (Line_P1.y > CenterPoint->y)
							{
								Velocity.y *= (Velocity.y > 0) ? -1 : 1;
								R_Angle *= (R_Angle < 0) ? -1 : 1;
							}
						}
						//물체의 무게중심이 작용점보다 아래에 있을 경우
						else if (CenterPoint->y > IP->Point.y)
						{
							//아래로 이동중일 때 충돌할 경우 y속도 반전, 위로 이동중일 충돌할 경우 y속도 감속
							////							  속도0으로 + 반전된속도 * 탄성계수
							Velocity.y += (Velocity.y > 0) ? -Velocity.y - Velocity.y * nFriction : -Velocity.y * 0.1;
							//시계반대방향으로 회전중일 때 작용점에 따라 회전토크를 다르게 하여 현재 회전속도에 부여, 시계방향일 경우 현재 회전속도 증가
							R_Angle *= (R_Angle < 0) ? ((CenterPoint->x - IP->Point.x) / (CenterPoint->x - Min_x->Point.x)) *(-nFriction) : 1.1;
							//회전방향반전과 위치 조정(임시방편)
							if (Line_P1.y > CenterPoint->y)
							{
								Velocity.y *= (Velocity.y > 0) ? -1 : 1;
								R_Angle *= (R_Angle < 0) ? -1 : 1;
							}
						}
					}
				}
				//수평선일 경우
				else if (Line_P1.y == Line_P2.y)
				{
					D3POINT_f UpPoint = (index->Point.y <= IP->Point.y) ? index->Point : index->Over->Point;
					D3POINT_f DownPoint = (index->Point.y >= IP->Point.y) ? index->Point : index->Over->Point;
					//물체의 무게중심이 작용점보다 위에 있을 경우 ≒ (물체가 수평선의 위쪽에 있는 상태)
					if (CenterPoint->y < IP->Point.y)
					{
						//수평선의 아래쪽에 있는 점까지의 거리(충돌했을 때의 교차거리)만큼 좌표조정
						float dist = -DistPointWithLine(DownPoint, Line_P1, Line_P2);
						float dx_Line = Line_P2.x - Line_P1.x;
						float dy_Line = Line_P2.y - Line_P1.y;
						D3POINT_f dist_divXY = { dist * (dy_Line / (fabsf(dx_Line) + fabsf(dy_Line))), dist * (dx_Line / (fabsf(dx_Line) + fabsf(dy_Line))), 0.0f };
						Move(dist_divXY);

						//위쪽으로 움직이고 있는 상태에서 물체의 아래쪽에 수평선이 충돌할 경우 현재 y속도값을 증가
						Velocity.y *= (Velocity.y > 0) ? -nFriction : 1.3;
						//물체의 무게중심이 작용점보다 왼쪽에 있을 경우
						if (CenterPoint->x < IP->Point.x)
						{
							//현재 이동하는 x속도 크기 감소(회전에 따른 이동 x)
							Velocity.x *= nFriction / 2;
							//시계방향으로 회전중일 때 작용점에 따라 회전토크를 다르게 하여 현재 회전속도에 부여, 시계반대방향일 경우 현재 회전속도 증가
							R_Angle *= (R_Angle > 0) ? ((IP->Point.x - CenterPoint->x) / (Max_x->Point.x - CenterPoint->x)) *(-nFriction) : 1.1;
						}
						//물체의 무게중심이 작용점보다 오른쪽에 있을 경우
						else if (CenterPoint->x > IP->Point.x)
						{
							//현재 이동하는 x속도 크기 감소(회전에 따른 이동 x)
							Velocity.x *= nFriction / 2;
							//시계반대방향으로 회전중일 때 작용점에 따라 회전토크를 다르게 하여 현재 회전속도에 부여, 시계방향일 경우 현재 회전속도 증가
							R_Angle *= (R_Angle < 0) ? ((IP->Point.x - CenterPoint->x) / (Max_x->Point.x - CenterPoint->x)) *(-nFriction) : 1.1;
						}
					}
					else if (CenterPoint->y < IP->Point.y)//물체의 무게중심이 작용점보다 아래쪽에 있을 경우 ≒ (물체가 수직선의 아래쪽에 있는 상태)
					{
						//수평선의 위쪽에 있는 점까지의 거리(충돌했을 때의 교차거리)만큼 좌표조정
						float dist = -DistPointWithLine(UpPoint, Line_P1, Line_P2);
						float dx_Line = Line_P2.x - Line_P1.x;
						float dy_Line = Line_P2.y - Line_P1.y;
						D3POINT_f dist_divXY = { dist * (dy_Line / (fabsf(dx_Line) + fabsf(dy_Line))), dist * (dx_Line / (fabsf(dx_Line) + fabsf(dy_Line))), 0.0f };
						Move(dist_divXY);

						//아래쪽으로 움직이고 있는 상태에서 물체의 위쪽에 수평선이 충돌할 경우 현재 y속도값을 증가
						Velocity.y *= (Velocity.y < 0) ? -nFriction : 1.3;
						//물체의 무게중심이 작용점보다 왼쪽에 있을 경우
						if (CenterPoint->x < IP->Point.x)
						{
							//현재 이동하는 x속도 크기 감소(회전에 따른 이동 x)
							Velocity.x *= nFriction / 2;
							//시계반대방향으로 회전중일 때 작용점에 따라 회전토크를 다르게 하여 현재 회전속도에 부여, 시계방향일 경우 현재 회전속도 증가
							R_Angle *= (R_Angle < 0) ? ((IP->Point.x - CenterPoint->x) / (Max_x->Point.x - CenterPoint->x)) *(-nFriction) : 1.1;
						}
						//물체의 무게중심이 작용점보다 오른쪽에 있을 경우
						else if (CenterPoint->x > IP->Point.x)
						{
							//현재 이동하는 x속도 크기 감소(회전에 따른 이동 x)
							Velocity.x *= nFriction / 2;
							//시계방향으로 회전중일 때 작용점에 따라 회전토크를 다르게 하여 현재 회전속도에 부여, 시계반대방향일 경우 현재 회전속도 증가
							R_Angle *= (R_Angle > 0) ? ((IP->Point.x - CenterPoint->x) / (Max_x->Point.x - CenterPoint->x)) *(-nFriction) : 1.1;
						}
					}
				}

				delete IP;
				break;
			}
			index = index->Over;
		} while (index != End);
	}
}

bool SHAPE_POLYGON::CollisionByPolygon(SHAPE_POLYGON &OtherPolygon, D3POINT_f velocity)
{
	//미구현
	return false;
}

//클리핑 영역 그리기 (알파블렌딩 구현 <- z버퍼 x, 클리핑 영역 바탕색이 먼저 그려져야 적용.)
void SHAPE_POLYGON::Draw_ClipRegion(D3POINT_f Region[4], const COLORREF_f & RegionColor, const float & Alpha)
{
	NODE_D3POINT *index = Vertex.Get_StartNode();
	NODE_D3POINT *End = Vertex.Get_StartNode();

	//교점
	NODE_D3POINT *IP = { nullptr, };

	if (index != nullptr)
	{
		LIST_D3POINT ClipRegion;
		D3POINT_f Top_Line_P1 = Region[2];
		D3POINT_f Top_Line_P2 = Region[3];
		do
		{
			//영역 내 정점 따로 저장
			if (Region[0].x >= index->Point.x && index->Point.x >= Region[1].x
				&& DistPointWithLine(index->Point, Top_Line_P1, Top_Line_P2) >= 0.001f)
				ClipRegion.Append(index->Point);
			if (IP = GetIntersectPoint(index->Point, index->Over->Point, Top_Line_P1, Top_Line_P2))
			{//교점 구하기
				ClipRegion.Append(IP->Point);
				delete IP;
			}
			index = index->Over;
		} while (index != End);

		index = ClipRegion.Get_StartNode();
		End = ClipRegion.Get_StartNode();
		//저장된 정점에 맞추어 폴리곤 그리기(정점이 2개일 때(선분일 때)는 그리지 않음, 도형만 그림)
		if (ClipRegion.Size() > 2 && index != nullptr)
		{
			COLORREF_f AlphaBlend;
			AlphaBlend.R = Alpha * RegionColor.R + (1 - Alpha) * Color.R;
			AlphaBlend.G = Alpha * RegionColor.G + (1 - Alpha) * Color.G;
			AlphaBlend.B = Alpha * RegionColor.B + (1 - Alpha) * Color.B;
			glBegin(GL_POLYGON);
			glColor3f(AlphaBlend.R, AlphaBlend.G, AlphaBlend.B);
			do
			{
				glVertex2f(index->Point.x, index->Point.y);
				index = index->Over;
			} while (index != End);
			glEnd();
		}
	}
}