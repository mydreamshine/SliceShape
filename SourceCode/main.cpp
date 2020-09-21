#include "INPUT-OUTPUT.h"

/*
커맨드
c: 초기화 후 도형 한개 랜덤 생성
Space Bar: 도형 한개 연속 생성
*/

void main(int argc, char *argv[])
{
	srand((unsigned)time(NULL));
	char input = NULL;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(Win_WIDTH, Win_HEIGHT);

	glutCreateWindow("Slice Shape");

	//키보드 입력
	glutKeyboardFunc(processNormalKeys);
	glutKeyboardUpFunc(processNormalKeysUp);
	//마우스 입력(버튼)
	glutMouseFunc(Mouse);
	//마우스 이동
	glutMotionFunc(Motion);

	//출력
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);

	//타이머 설정
	glutTimerFunc(1000/FPS, TimerFunction, 1);

	glutMainLoop();
}