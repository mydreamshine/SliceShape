typedef struct D3POINT_f
{
	float x, y, z;
}D3POINT_f;

typedef struct RECT_f
{
	float left, top, right, bottom;
}RECT_f;

typedef struct CUBE_f
{
	float left, top, right, bottom, MinZ, MaxZ;
}CUBE_f;

typedef struct COLORREF_f
{
	float R, G, B;
}COLORREF_f;

enum MOUSE_STATE
{
	LeftButton_Up,
	LeftButton_Down,
	MiddleButton_Up,
	MiddleButton_Down,
	RightButton_Up,
	RightButton_Down,

	Wheel_RollUp,
	Wheel_RollDown,
};