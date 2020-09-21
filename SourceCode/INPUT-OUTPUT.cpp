#include "INPUT-OUTPUT.h"

//BackGroundColor (White)
COLORREF_f BGC = { 1.0f, 1.0f, 1.0f };
//뷰포트 영역
CUBE_f ViewPortRegion = { 0.0f, 0.0f, (float)Win_WIDTH, (float)Win_HEIGHT, 0.0f, 1.0f };

//마우스 상태
MOUSE_STATE MouseState = LeftButton_Up;

//게임 정보(Combo, )
GAME_INFO G_info;

//자르는 선
ReducedLine Cut_Line;

//도형 리스트
LIST_SHAPE_POLYGON ShapeList;
size_t Select = 0;//0: Triangle, 1: Rect, 2: Pentagon, 3: Circle

//바구니
#define BOX_MAX_SPEED 20
#define Box_Friction 0.05 //마찰계수
RECT_f Box = { 250.0f, 300.0f, 550.0f, 500.0f };
D3POINT_f WaterRegion[4] = { {0.0f,0.0f,0.0f}, };//0: Right-Bottom, 1: Left-Bottom, 2: Left-Top, 3: Right-Top
float WaterAlpha = 0.3;
bool Move[2] = { false, };//0: Leftmove, 1: Rightmove
float SpeedX = 0.0f;

//키보드 입력
void processNormalKeys(unsigned char input_Char, int Mouse_x, int Mouse_y)
{
	switch (input_Char)
	{
	case 'q':
		exit(0);
		break;
	case 'a'://바구니 이동(←)
		Move[0] = true;
		break;
	case 'd'://바구니 이동(→)
		Move[1] = true;
		break;
	case 'c':
		ShapeList.Delete_AllNode();
		Select = (size_t)(rand() % 4);
		if(Select == 0)//삼각형
			ShapeList.append(CreateTriangle(ViewPortRegion));
		else if (Select == 1)//사각형
			ShapeList.append(CreateRect(ViewPortRegion));
		else if (Select == 2)//오각형
			ShapeList.append(CreatePentagon(ViewPortRegion));
		else if (Select == 3)//원
			ShapeList.append(CreateCircle(ViewPortRegion));
		break;
	case 32://SpaceBar
		Select = (size_t)(rand() % 4);
		if (Select == 0)//삼각형
			ShapeList.append(CreateTriangle(ViewPortRegion));
		else if (Select == 1)//사각형
			ShapeList.append(CreateRect(ViewPortRegion));
		else if (Select == 2)//오각형
			ShapeList.append(CreatePentagon(ViewPortRegion));
		else if (Select == 3)//원
			ShapeList.append(CreateCircle(ViewPortRegion));
		break;
	}
}

//키보드 입력(키가 눌렸다가 때졌을 때)
void processNormalKeysUp(unsigned char input_Char, int Mouse_x, int Mouse_y)
{
	switch (input_Char)
	{
	case 'a'://바구니 이동정지(←)
		Move[0] = false;
		break;
	case 'd'://바구니 이동정지(→)
		Move[1] = false;
		break;
	}
}

//마우스 입력(버튼)
void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (MouseState == LeftButton_Up)
		{
			Cut_Line.Set_LineWidth(5.0f);
			Cut_Line.Set_Speed(30.0f);
			D3POINT_f mPoint = { (float)x, (float)y, 0.0f };
			Cut_Line.Set_P1(mPoint);
			Cut_Line.Set_P2(mPoint);
			MouseState = LeftButton_Down;
		}
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		if (MouseState == LeftButton_Down)
		{
			D3POINT_f mPoint = { 0.0f, 0.0f, 0.0f };
			Cut_Line.Set_P1(mPoint);
			Cut_Line.Set_P2(mPoint);
			ShapeList.SlicedDeActive_AllNode();
			MouseState = LeftButton_Up;
		}
	}
}
//마우스 이동
void Motion(int x, int y)
{
	if (MouseState == LeftButton_Down)
	{
		D3POINT_f mPoint = { (float)x, (float)y, 0.0f };
		Cut_Line.Set_P2(mPoint);
	}
}

//타이머 설정
void TimerFunction(int value)
{
	//드래그 직선
	if (MouseState == LeftButton_Down)
	{
		Cut_Line.Reduced();
		if(Cut_Line.Get_dx() == 0.0f && Cut_Line.Get_dy() == 0.0f)
			ShapeList.SlicedDeActive_AllNode();
	}

	//각 도형과 직선과의 교차판단 및 도형 자르기
	size_t Count_SlicedPolygon = ShapeList.Slice_EachOther(Cut_Line.Get_P1(), Cut_Line.Get_P2());

	//잘려진 도형 갯수만큼 콤보 증가
	if (Count_SlicedPolygon != 0)
	{
		G_info.Set_ComboColor({ 0.0f,0.0f,0.0f });
		G_info.Set_ComboPos(Cut_Line.Get_P2());
	}
	for (size_t i = 0; i < Count_SlicedPolygon; i++)
		G_info.Add_Combo();

	//콤보 유지 체크
	G_info.Check_Combointerval();

	//모든 도형에 대하여
	ShapeList.Move_AllNode();				//이동
	ShapeList.Rotation_AllNode();			//회전
	ShapeList.Check_Outside(ViewPortRegion);//화면영역 외 위치 확인 및 노드 삭제

	//바구니 이동
	if(Move[0]) SpeedX += ((-BOX_MAX_SPEED) - SpeedX) * Box_Friction;
	else SpeedX += (0 - SpeedX) * Box_Friction;
	if (Move[1]) SpeedX += (BOX_MAX_SPEED - SpeedX) * Box_Friction;
	else SpeedX += (0 - SpeedX) * Box_Friction;
	Box.left += SpeedX;
	Box.right += SpeedX;

	//물 영역
	WaterRegion[0] = { Box.right, Box.bottom, 0.0f };//right-bottom
	WaterRegion[1] = { Box.left, Box.bottom, 0.0f };//left-bottom
	WaterRegion[2] = { Box.left, Box.top + 40.0f - SpeedX * 4, 0.0f };//left-top
	WaterRegion[3] = { Box.right, Box.top + 40.0f + SpeedX * 4, 0.0f };//right-top

	//바구니 테두리
	D3POINT_f Line_UnderP1 = { Box.left, Box.bottom, 0.0f }, Line_UnderP2 = { Box.right, Box.bottom, 0.0f };
	D3POINT_f Line_LeftP1 = { Box.left, Box.top, 0.0f }, Line_LeftP2 = { Box.left, Box.bottom, 0.0f };
	D3POINT_f Line_RightP1 = { Box.right, Box.top, 0.0f }, Line_RightP2 = { Box.right, Box.bottom, 0.0f };

	//바구니 테두리와 모든 도형과의 충돌처리
	ShapeList.Collsion_Line(Line_UnderP1, Line_UnderP2);
	ShapeList.Collsion_Line(Line_LeftP1, Line_LeftP2);
	ShapeList.Collsion_Line(Line_RightP1, Line_RightP2);

	glutPostRedisplay();
	glutTimerFunc(1000 / FPS, TimerFunction, 1);
}

// 윈도우 출력 함수
void drawScene(void)
{
	glClearColor(BGC.R, BGC.G, BGC.B, 1.0f);	
	glClear(GL_COLOR_BUFFER_BIT);

	glLoadIdentity();
	glColor3f(0.7f, 0.7f, 0.7f);
	drawBitmapText("c: Create New Shape!", { ViewPortRegion.left + 20.0f, ViewPortRegion.top + 40.0f, 0.0f }, { 0.7f, 0.7f, 0.7f });
	drawBitmapText("SpaceBar: Continue New Shape!", { ViewPortRegion.left + 20.0f, ViewPortRegion.top + 70.0f, 0.0f }, { 0.7f, 0.7f, 0.7f });
	drawBitmapText("a: Move Left!", { ViewPortRegion.left + 20.0f, ViewPortRegion.top + 100.0f, 0.0f }, { 0.7f, 0.7f, 0.7f });
	drawBitmapText("d: Move Right!", { ViewPortRegion.left + 20.0f, ViewPortRegion.top + 130.0f, 0.0f }, { 0.7f, 0.7f, 0.7f });

	if (fabsf(SpeedX) <= 0.002f)
		SpeedX = 0.0f;
	if (SpeedX < 0.0f)
	{
		glColor3f(1.0f * (1.0f - fabsf(SpeedX) / BOX_MAX_SPEED), 1.0f * (1.0f - fabsf(SpeedX) / BOX_MAX_SPEED), 1.0f * (1.0f - fabsf(SpeedX) / BOX_MAX_SPEED));
		drawBitmapText("LEFT", { Box.left - 80.0f, Box.top + 100.0f, 0.0f }, { 1.0f * (1.0f - fabsf(SpeedX) / BOX_MAX_SPEED), 1.0f * (1.0f - fabsf(SpeedX) / BOX_MAX_SPEED), 1.0f * (1.0f - fabsf(SpeedX) / BOX_MAX_SPEED) });
	}
	if (SpeedX > 0.0f)
	{
		glColor3f(1.0f * (1.0f - fabsf(SpeedX) / BOX_MAX_SPEED), 1.0f * (1.0f - fabsf(SpeedX) / BOX_MAX_SPEED), 1.0f * (1.0f - fabsf(SpeedX) / BOX_MAX_SPEED));
		drawBitmapText("RIGHT", { Box.right + 20.0f, Box.top + 100.0f, 0.0f }, { 1.0f * (1.0f - fabsf(SpeedX) / BOX_MAX_SPEED), 1.0f * (1.0f - fabsf(SpeedX) / BOX_MAX_SPEED), 1.0f * (1.0f - fabsf(SpeedX) / BOX_MAX_SPEED) });
	}
	ShapeList.Draw_AllNode();

	Cut_Line.Draw();

	//물
	glPushMatrix();
	{
		COLORREF_f AlphaBlend;
		AlphaBlend.R = (1.0f - WaterAlpha) * 1.0f;
		AlphaBlend.G = (1.0f - WaterAlpha) * 1.0f;
		AlphaBlend.B = 1.0f;
		glColor3f(AlphaBlend.R, AlphaBlend.G, AlphaBlend.B);
		glBegin(GL_POLYGON);
		glVertex2f(WaterRegion[0].x, WaterRegion[0].y);
		glVertex2f(WaterRegion[1].x, WaterRegion[1].y);
		glVertex2f(WaterRegion[2].x, WaterRegion[2].y);
		glVertex2f(WaterRegion[3].x, WaterRegion[3].y);
		glEnd();
	}
	glPopMatrix();
	//바구니
	glPushMatrix();
	{
		glLineWidth(3.0f);
		glColor3f(0.0f, 0.0f, 0.0f);
		glBegin(GL_LINES);
		glVertex2f(Box.left, Box.top);
		glVertex2f(Box.left, Box.bottom);
		glVertex2f(Box.left, Box.bottom);
		glVertex2f(Box.right, Box.bottom);
		glVertex2f(Box.right, Box.top);
		glVertex2f(Box.right, Box.bottom);
		glEnd();
		glLineWidth(1.0f);
	}
	glPopMatrix();

	ShapeList.Draw_ClipRegion(WaterRegion, { 0.0f,0.0f,1.0f }, WaterAlpha);

	G_info.Draw_info(ViewPortRegion);

	glutSwapBuffers();
}

// 윈도우 갱신(크기 변경 시)
void Reshape(int w, int h)
{
	glViewport(0, 0, w, h);

	float WidthFactor = (float)w / (ViewPortRegion.right - ViewPortRegion.left);//너비비율(변경너비/기준너비)
	float HeightFactor = (float)h / (ViewPortRegion.bottom - ViewPortRegion.top);//높이비율(변경높이/기준높이)

	ViewPortRegion.left *= WidthFactor;
	ViewPortRegion.right *= WidthFactor;
	ViewPortRegion.top *= HeightFactor;
	ViewPortRegion.bottom *= HeightFactor;

	Box.bottom = ViewPortRegion.bottom - 50.0f;
	Box.top = Box.bottom - 200.0f;

	glMatrixMode(GL_PROJECTION); // 투영평면 좌표계로 변환(카메라 좌표계)
	glLoadIdentity();
	glOrtho(ViewPortRegion.left, ViewPortRegion.right, ViewPortRegion.bottom, ViewPortRegion.top, ViewPortRegion.MinZ, ViewPortRegion.MaxZ);
	glMatrixMode(GL_MODELVIEW);//월드공간 상의 모델's 좌표계로 변환
	glutPostRedisplay();
}