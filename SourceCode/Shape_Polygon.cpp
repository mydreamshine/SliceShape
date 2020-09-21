#include "Shape_Polygon.h"

#define Coefficient_Friction 0.8f //�������

SHAPE_POLYGON::SHAPE_POLYGON()
{
	CenterPoint = nullptr;
	Area = 0.0f;
	Color = { 0.0f, };
	R_Angle = 0.0f;
	Scale = 1.0f;

	//��ٰ��� ���� �浹�� ��츦 Ȯ���ϱ� ���� ������ Vertex �� �ּ�/�ִ� ��
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

//�ٰ����� ����(�� �����߽�)
//���� ��ó :https://m.blog.naver.com/PostView.nhn?blogId=a630203&logNo=120213239668&proxyReferer=https%3A%2F%2Fwww.google.co.kr%2F
//CenterY = 1/(6*����) * ��{ (y_i + y_(i+1)) * (x_i * y_(i+1) - x_(i+1) * y_i) }
//CenterX = 1/(6*����) * ��{ (x_i + x_(i+1)) * (x_i * y_(i+1) - x_(i+1) * y_i) }
void SHAPE_POLYGON::Set_CenterPoint()
{
	NODE_D3POINT *index = Vertex.Get_StartNode();
	NODE_D3POINT *End = Vertex.Get_StartNode();

	if (index != nullptr)
	{
		float tArea = 0.0f;
		float Factor = 0.0f;//n��° �� �� 1/(6*����)
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

//�ٰ��� ����
//���� ��ó :https://m.blog.naver.com/PostView.nhn?blogId=a630203&logNo=120213239668&proxyReferer=https%3A%2F%2Fwww.google.co.kr%2F
//Area = 1/2 * ��(x_i * y_(i+1) - x_(i+1) * y_i)
void SHAPE_POLYGON::Set_Area()
{
	NODE_D3POINT *index = Vertex.Get_StartNode();
	NODE_D3POINT *End = Vertex.Get_StartNode();

	if (index != nullptr)
	{
		float Factor = 0.0f;//n��° ��
		do
		{
			Factor = (index->Point.x * index->Over->Point.y) - (index->Over->Point.x * index->Point.y);
			Area += Factor;
			index = index->Over;
		} while (index != End);
		Area /= 2.0f;
	}
}

//�ٰ����� ����(�� �����߽�)
D3POINT_f* SHAPE_POLYGON::Get_CenterPoint()
{
	if (CenterPoint == nullptr)
		Set_CenterPoint();
	return CenterPoint;
}

//�ٰ����� ����
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

//�ǹ�: ���� �߾�
void SHAPE_POLYGON::Set_Pos(D3POINT_f Point)
{
	if (CenterPoint == nullptr) CenterPoint = Get_CenterPoint();
	NODE_D3POINT *index = Vertex.Get_StartNode();
	NODE_D3POINT *End = Vertex.Get_StartNode();
	if (index != nullptr)
	{
		Max_x = Min_x = index;
		Max_y = Min_y = index;

		D3POINT_f Dist;//���� ��ġ���� �Ÿ�����
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

//������ ��� Vertex�� Distance��ŭ �̵�
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

//x-y��� ȸ��(���� ���� �⵹üũ�� ���� ������ ������ ���� ȸ����ȯ�Ѵ�.)
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
			{//������ �߽��� ����(0,0)���� �����̵�
				x_ -= CenterPoint->x;
				y_ -= CenterPoint->y;
			}
			//ȸ����ȯ(��ǥ�� ȸ��x)
			index->Point.x = x_ * cosf(R_Angle * PI / 180.0f) - y_ * sinf(R_Angle * PI / 180.0f);
			index->Point.y = x_ * sinf(R_Angle * PI / 180.0f) + y_ * cosf(R_Angle * PI / 180.0f);
			if (CenterPoint != nullptr)
			{//������ �߽��� ������ġ�� �����̵�
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

//x-y��� Ȯ�����(�յ�) (���� ���� �⵹üũ�� ���� ������ ������ ���� Ȯ����Һ�ȯ�Ѵ�.)
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
			{//������ �߽��� ����(0,0)���� �����̵�
				index->Point.x -= CenterPoint->x;
				index->Point.y -= CenterPoint->y;
			}
			//Ȯ����� ��ȯ
			index->Point.x *= Scale;
			index->Point.y *= Scale;
			if (CenterPoint != nullptr)
			{//������ �߽��� ������ġ�� �����̵�
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

//�ٰ���'s �׸���
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

//�ٰ��� �ڸ���(���� ���� �Ǵ�)
SHAPE_POLYGON* SHAPE_POLYGON::Slice(D3POINT_f Line_P1, D3POINT_f Line_P2)
{
	SHAPE_POLYGON *SliceShape = nullptr;

	//��������
	size_t Count_intersect = 0;

	//����
	NODE_D3POINT *IP[2] = { nullptr, };

	NODE_D3POINT *index = Vertex.Get_StartNode();
	NODE_D3POINT *End = Vertex.Get_StartNode();

	if (index != nullptr)
	{
		//������ ���� Ȥ�� ������ Point ���
		NODE_D3POINT *SplitNode[2] = { nullptr, };
		do
		{
			if (IP[Count_intersect] = GetIntersectPoint(index->Point, index->Over->Point, Line_P1, Line_P2))
			{//���� ���ϱ�
				SplitNode[Count_intersect] = index;
				Count_intersect++;
			}
			if (Count_intersect == 2)//���� ������ 2���� �� �� ������ �߸� ����
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

//2017.10.11. ������ / ���� �浹üũ (dx,dy) ���
//2017.10.15. �浹 �� �ó�� (�ӵ� *= -ź�����)
//2017.10.16. ������ / ���� ���� �浹 ����
//2017.10.20. ������ / ���� �浹üũ (����(�浹�ۿ���)) ���
//2017.10.21. �浹 �� �ó�� (�������� ��ġ�� ���� ���ӵ�, ���� ȸ������� �ۿ����� ���� ���ӵ�)
//2017.10.22. �浹 �� ������ ���� �浹 ���� (�ӽù������� ȸ�������� ������Ű�� y�������� ��ǥ����)
/*
�ۿ����� ���� ���ӵ�(ȸ����ũ�� ��ȭ)
�ۿ����� ��ü�� �����߽ɰ� �󸶳� ������ �ִ��Ŀ� ���� �浹 ���� ȸ���ӵ��� �ٸ���.
������ ���� ����(dist = sqrt(pow(dx,2)+pow(dy,2))�� ���� ����) �浹 ���� ȸ���ӵ��� ����,
�ָ� ���� ����(dist = sqrt(pow(dx,2)+pow(dy,2))�� Ŭ ����) �浹 ���� ȸ���ӵ��� ũ��. (ȸ������ ���Ʈ)
*/
//�����߽ɰ� �߷¿� ���� ���� ȸ���ӵ� ���� �ʿ�
//(��ü�� �����߽ɰ� �ۿ��� ������ �ﰢ���� �뷱���� ���߾� ȸ���ؾ�, �� �� �׷����� ȸ�������� �ȴ�.)
//����ȭ�� ��ư�, ȸ�� ������ ������ ������ �ʿ䰡 ����.
void SHAPE_POLYGON::CollisionByLine(D3POINT_f Line_P1, D3POINT_f Line_P2, D3POINT_f &Velocity, float &R_Angle)
{
	//����(IP : Intersect Point)
	NODE_D3POINT *IP =nullptr;

	NODE_D3POINT *index = Vertex.Get_StartNode();
	NODE_D3POINT *End = Vertex.Get_StartNode();

	if (index != nullptr)
	{
		do
		{
			//���� ���ϱ�(�浹 �ۿ���)
			if (IP = GetIntersectPoint(index->Point, index->Over->Point, Line_P1, Line_P2))
			{
				//�������� ���
				if (Line_P1.x == Line_P2.x)
				{
					D3POINT_f LeftPoint = (index->Point.x <= IP->Point.x) ? index->Point : index->Over->Point;
					D3POINT_f RightPoint = (index->Point.x >= IP->Point.x) ? index->Point : index->Over->Point;

					//��ü�� �����߽��� �ۿ������� ���ʿ� ���� ��� �� (��ü�� �������� ���ʿ� �ִ� ����)
					if (CenterPoint->x < IP->Point.x)
					{
						//�������� �����ʿ� �ִ� �������� �Ÿ�(�浹���� ���� �����Ÿ�)��ŭ ��ǥ����
						float dist = DistPointWithLine(RightPoint, Line_P1, Line_P2);
						float dx_Line = Line_P2.x - Line_P1.x;
						float dy_Line = Line_P2.y - Line_P1.y;
						D3POINT_f dist_divXY = { dist * (dy_Line / (fabsf(dx_Line) + fabsf(dy_Line))), dist * (dx_Line / (fabsf(dx_Line) + fabsf(dy_Line))), 0.0f };
						if (CenterPoint->y < Line_P1.y) dist_divXY = { 0.0f , Line_P1.y - IP->Point.y, 0.0f };
						Move(dist_divXY);

						//�������� �����̰� �ִ� ���¿��� ��ü�� �����ʿ� �������� �浹�� ��� ���� x�ӵ����� ����
						Velocity.x *= (Velocity.x > 0) ? -nFriction : 1.3;
						//��ü�� �����߽��� �ۿ������� ���� ���� ���
						if (CenterPoint->y < IP->Point.y)
						{
							//�Ʒ��� �̵����� �� �浹�� ��� y�ӵ� ����, ���� �̵����� �浹�� ��� y�ӵ� ����
							Velocity.y += (Velocity.y > 0) ? Velocity.y * 0.3 : -Velocity.y * 0.3;
							//�ð�ݴ�������� ȸ������ �� �ۿ����� ���� ȸ����ũ�� �ٸ��� �Ͽ� ���� ȸ���ӵ��� �ο�, �ð������ ��� ���� ȸ���ӵ� ����
							R_Angle *= (R_Angle < 0) ? ((IP->Point.x - CenterPoint->x) / (Max_x->Point.x - CenterPoint->x)) *(-nFriction) : 1.1;
							//ȸ����������� ��ġ ����(�ӽù���)
							if (Line_P1.y > CenterPoint->y)
							{
								Velocity.y *= (Velocity.y > 0) ? -1 : 1;
								R_Angle *= (R_Angle > 0) ? -1 : 1;
							}
						}
						//��ü�� �����߽��� �ۿ������� �Ʒ��� ���� ���
						else if (CenterPoint->y > IP->Point.y)
						{
							//�Ʒ��� �̵����� �� �浹�� ��� y�ӵ� ����, ���� �̵����� �浹�� ��� y�ӵ� ����
							////							  �ӵ�0���� + �����ȼӵ� * ź�����
							Velocity.y += (Velocity.y > 0) ? -Velocity.y -Velocity.y * nFriction : -Velocity.y * 0.3;
							//�ð�������� ȸ������ �� �ۿ����� ���� ȸ����ũ�� �ٸ��� �Ͽ� ���� ȸ���ӵ��� �ο�, �ð�ݴ������ ��� ���� ȸ���ӵ� ����
							R_Angle *= (R_Angle > 0) ? ((IP->Point.x - CenterPoint->x) / (Max_x->Point.x - CenterPoint->x)) *(-nFriction) : 1.1;
							//ȸ����������� ��ġ ����(�ӽù���)
							if (Line_P1.y > CenterPoint->y)
							{
								Velocity.y *= (Velocity.y > 0) ? -1 : 1;
								R_Angle *= (R_Angle > 0) ? -1 : 1;
							}
						}
					}
					else if (CenterPoint->x > IP->Point.x)//��ü�� �����߽��� �ۿ������� �����ʿ� ���� ��� �� (��ü�� �������� �����ʿ� �ִ� ����)
					{
						//�������� ���ʿ� �ִ� �������� �Ÿ�(�浹���� ���� �����Ÿ�)��ŭ ��ǥ����
						float dist = DistPointWithLine(LeftPoint, Line_P1, Line_P2);
						float dx_Line = Line_P2.x - Line_P1.x;
						float dy_Line = Line_P2.y - Line_P1.y;
						D3POINT_f dist_divXY = { dist * (dy_Line / (fabsf(dx_Line) + fabsf(dy_Line))), dist * (dx_Line / (fabsf(dx_Line) + fabsf(dy_Line))), 0.0f };
						if (CenterPoint->y < Line_P1.y) dist_divXY = { 0.0f , Line_P1.y - IP->Point.y, 0.0f };
						Move(dist_divXY);

						//���������� �����̰� �ִ� ���¿��� ��ü�� ���ʿ� �������� �浹�� ��� ���� x�ӵ����� ����
						Velocity.x *= (Velocity.x < 0) ? -nFriction : 1.3;
						//��ü�� �����߽��� �ۿ������� ���� ���� ���
						if (CenterPoint->y < IP->Point.y)
						{
							//�Ʒ��� �̵����� �� �浹�� ��� y�ӵ� ����, ���� �̵����� �浹�� ��� y�ӵ� ����
							Velocity.y += (Velocity.y > 0) ? Velocity.y * 0.3 : -Velocity.y * 0.3;
							//�ð�������� ȸ������ �� �ۿ����� ���� ȸ����ũ�� �ٸ��� �Ͽ� ���� ȸ���ӵ��� �ο�, �ð�ݴ������ ��� ���� ȸ���ӵ� ����
							R_Angle *= (R_Angle > 0) ? ((CenterPoint->x - IP->Point.x) / (CenterPoint->x - Min_x->Point.x)) *(-nFriction) : 1.1;
							//ȸ����������� ��ġ ����(�ӽù���)
							if (Line_P1.y > CenterPoint->y)
							{
								Velocity.y *= (Velocity.y > 0) ? -1 : 1;
								R_Angle *= (R_Angle < 0) ? -1 : 1;
							}
						}
						//��ü�� �����߽��� �ۿ������� �Ʒ��� ���� ���
						else if (CenterPoint->y > IP->Point.y)
						{
							//�Ʒ��� �̵����� �� �浹�� ��� y�ӵ� ����, ���� �̵����� �浹�� ��� y�ӵ� ����
							////							  �ӵ�0���� + �����ȼӵ� * ź�����
							Velocity.y += (Velocity.y > 0) ? -Velocity.y - Velocity.y * nFriction : -Velocity.y * 0.1;
							//�ð�ݴ�������� ȸ������ �� �ۿ����� ���� ȸ����ũ�� �ٸ��� �Ͽ� ���� ȸ���ӵ��� �ο�, �ð������ ��� ���� ȸ���ӵ� ����
							R_Angle *= (R_Angle < 0) ? ((CenterPoint->x - IP->Point.x) / (CenterPoint->x - Min_x->Point.x)) *(-nFriction) : 1.1;
							//ȸ����������� ��ġ ����(�ӽù���)
							if (Line_P1.y > CenterPoint->y)
							{
								Velocity.y *= (Velocity.y > 0) ? -1 : 1;
								R_Angle *= (R_Angle < 0) ? -1 : 1;
							}
						}
					}
				}
				//������ ���
				else if (Line_P1.y == Line_P2.y)
				{
					D3POINT_f UpPoint = (index->Point.y <= IP->Point.y) ? index->Point : index->Over->Point;
					D3POINT_f DownPoint = (index->Point.y >= IP->Point.y) ? index->Point : index->Over->Point;
					//��ü�� �����߽��� �ۿ������� ���� ���� ��� �� (��ü�� ������ ���ʿ� �ִ� ����)
					if (CenterPoint->y < IP->Point.y)
					{
						//������ �Ʒ��ʿ� �ִ� �������� �Ÿ�(�浹���� ���� �����Ÿ�)��ŭ ��ǥ����
						float dist = -DistPointWithLine(DownPoint, Line_P1, Line_P2);
						float dx_Line = Line_P2.x - Line_P1.x;
						float dy_Line = Line_P2.y - Line_P1.y;
						D3POINT_f dist_divXY = { dist * (dy_Line / (fabsf(dx_Line) + fabsf(dy_Line))), dist * (dx_Line / (fabsf(dx_Line) + fabsf(dy_Line))), 0.0f };
						Move(dist_divXY);

						//�������� �����̰� �ִ� ���¿��� ��ü�� �Ʒ��ʿ� ������ �浹�� ��� ���� y�ӵ����� ����
						Velocity.y *= (Velocity.y > 0) ? -nFriction : 1.3;
						//��ü�� �����߽��� �ۿ������� ���ʿ� ���� ���
						if (CenterPoint->x < IP->Point.x)
						{
							//���� �̵��ϴ� x�ӵ� ũ�� ����(ȸ���� ���� �̵� x)
							Velocity.x *= nFriction / 2;
							//�ð�������� ȸ������ �� �ۿ����� ���� ȸ����ũ�� �ٸ��� �Ͽ� ���� ȸ���ӵ��� �ο�, �ð�ݴ������ ��� ���� ȸ���ӵ� ����
							R_Angle *= (R_Angle > 0) ? ((IP->Point.x - CenterPoint->x) / (Max_x->Point.x - CenterPoint->x)) *(-nFriction) : 1.1;
						}
						//��ü�� �����߽��� �ۿ������� �����ʿ� ���� ���
						else if (CenterPoint->x > IP->Point.x)
						{
							//���� �̵��ϴ� x�ӵ� ũ�� ����(ȸ���� ���� �̵� x)
							Velocity.x *= nFriction / 2;
							//�ð�ݴ�������� ȸ������ �� �ۿ����� ���� ȸ����ũ�� �ٸ��� �Ͽ� ���� ȸ���ӵ��� �ο�, �ð������ ��� ���� ȸ���ӵ� ����
							R_Angle *= (R_Angle < 0) ? ((IP->Point.x - CenterPoint->x) / (Max_x->Point.x - CenterPoint->x)) *(-nFriction) : 1.1;
						}
					}
					else if (CenterPoint->y < IP->Point.y)//��ü�� �����߽��� �ۿ������� �Ʒ��ʿ� ���� ��� �� (��ü�� �������� �Ʒ��ʿ� �ִ� ����)
					{
						//������ ���ʿ� �ִ� �������� �Ÿ�(�浹���� ���� �����Ÿ�)��ŭ ��ǥ����
						float dist = -DistPointWithLine(UpPoint, Line_P1, Line_P2);
						float dx_Line = Line_P2.x - Line_P1.x;
						float dy_Line = Line_P2.y - Line_P1.y;
						D3POINT_f dist_divXY = { dist * (dy_Line / (fabsf(dx_Line) + fabsf(dy_Line))), dist * (dx_Line / (fabsf(dx_Line) + fabsf(dy_Line))), 0.0f };
						Move(dist_divXY);

						//�Ʒ������� �����̰� �ִ� ���¿��� ��ü�� ���ʿ� ������ �浹�� ��� ���� y�ӵ����� ����
						Velocity.y *= (Velocity.y < 0) ? -nFriction : 1.3;
						//��ü�� �����߽��� �ۿ������� ���ʿ� ���� ���
						if (CenterPoint->x < IP->Point.x)
						{
							//���� �̵��ϴ� x�ӵ� ũ�� ����(ȸ���� ���� �̵� x)
							Velocity.x *= nFriction / 2;
							//�ð�ݴ�������� ȸ������ �� �ۿ����� ���� ȸ����ũ�� �ٸ��� �Ͽ� ���� ȸ���ӵ��� �ο�, �ð������ ��� ���� ȸ���ӵ� ����
							R_Angle *= (R_Angle < 0) ? ((IP->Point.x - CenterPoint->x) / (Max_x->Point.x - CenterPoint->x)) *(-nFriction) : 1.1;
						}
						//��ü�� �����߽��� �ۿ������� �����ʿ� ���� ���
						else if (CenterPoint->x > IP->Point.x)
						{
							//���� �̵��ϴ� x�ӵ� ũ�� ����(ȸ���� ���� �̵� x)
							Velocity.x *= nFriction / 2;
							//�ð�������� ȸ������ �� �ۿ����� ���� ȸ����ũ�� �ٸ��� �Ͽ� ���� ȸ���ӵ��� �ο�, �ð�ݴ������ ��� ���� ȸ���ӵ� ����
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
	//�̱���
	return false;
}

//Ŭ���� ���� �׸��� (���ĺ��� ���� <- z���� x, Ŭ���� ���� �������� ���� �׷����� ����.)
void SHAPE_POLYGON::Draw_ClipRegion(D3POINT_f Region[4], const COLORREF_f & RegionColor, const float & Alpha)
{
	NODE_D3POINT *index = Vertex.Get_StartNode();
	NODE_D3POINT *End = Vertex.Get_StartNode();

	//����
	NODE_D3POINT *IP = { nullptr, };

	if (index != nullptr)
	{
		LIST_D3POINT ClipRegion;
		D3POINT_f Top_Line_P1 = Region[2];
		D3POINT_f Top_Line_P2 = Region[3];
		do
		{
			//���� �� ���� ���� ����
			if (Region[0].x >= index->Point.x && index->Point.x >= Region[1].x
				&& DistPointWithLine(index->Point, Top_Line_P1, Top_Line_P2) >= 0.001f)
				ClipRegion.Append(index->Point);
			if (IP = GetIntersectPoint(index->Point, index->Over->Point, Top_Line_P1, Top_Line_P2))
			{//���� ���ϱ�
				ClipRegion.Append(IP->Point);
				delete IP;
			}
			index = index->Over;
		} while (index != End);

		index = ClipRegion.Get_StartNode();
		End = ClipRegion.Get_StartNode();
		//����� ������ ���߾� ������ �׸���(������ 2���� ��(������ ��)�� �׸��� ����, ������ �׸�)
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