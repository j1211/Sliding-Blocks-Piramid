//��ŉ����p
#include "DxLib.h"
#include <iostream>
#include <algorithm>
#include <string>
#define rep(i, n) for(i = 0; i < n; i++)
using namespace std;

const int WinX = 900;
const int WinY = 800;

//�`��p
const int Sx = 100;
const int Sy = 100;
const int cellSize = 50;
const int swapFrame = 12;		//�X���b�v�̃A�j���[�V��������
int font30;						//�t�H���g
const int FontSize = 30;

//�f�[�^
const int N = 4;
int field[N][N];						//���̔Ֆ�. �X���b�v����, �h���g�P�A.
int remSwapFrame;						//�X���b�v���̍d�����c�艽�t���[�����邩. �X���b�v���Ă��Ȃ��Ƃ���0�������Ă���.
int swpY1, swpX1, swpY2, swpX2;			//�X���b�v���ɈӖ�������. (swpY1�s, swpX1���)(�u���b�N)��(swpY2�s, swpX2���)(��}�X)��������.
double swpDY1, swpDX1, swpDY2, swpDX2;	//�X���b�v���ɈӖ�������. �`����W (�u���b�N�̒��S���W).

//���W�Ƃ�
int toDrawLx(int x) { return Sx + cellSize * x; }
int toDrawLy(int y) { return Sy + cellSize * y; }
int toDrawCx(int x) { return Sx + cellSize * x + cellSize / 2; }
int toDrawCy(int y) { return Sy + cellSize * y + cellSize / 2; }
int toDrawRx(int x) { return Sx + cellSize * x + cellSize; }
int toDrawRy(int y) { return Sy + cellSize * y + cellSize; }

int toFontLx(int cx, int fontSize, int textLength) {
	return cx - fontSize * textLength / 4;
}
int toFontLy(int cy, int fontSize) {
	return cy - fontSize / 2;
}

int toCellX(int x) {
	if (x < Sx) return -1;
	return (x - Sx) / cellSize;
}

int toCellY(int y) {
	if (y < Sy) return -1;
	return (y - Sy) / cellSize;
}

//�񕉐��� -> ������
string toString(int x) {
	if (x == 0) return "0";
	string s;
	while (x > 0) {
		s += (char)((x % 10) + '0');
		x /= 10;
	}

	string ret;
	int i = s.length() - 1;
	while (i >= 0) { ret += s[i]; i--; }
	return ret;
}

//�}�E�X���͗p
namespace mouse {
	bool nowPush, prevPush;
	int x, y;

	void init() {
		nowPush = prevPush = false;
		y = 0, x = 0;
	}

	void update() {
		prevPush = nowPush;
		GetMousePoint(&x, &y);
		if (GetMouseInput() & MOUSE_INPUT_LEFT) nowPush = true;
		else nowPush = false;
	}

	bool isClick() {
		return !prevPush && nowPush;
	}
}

//�Ƃ肠�������������Ă������
void init() {
	int y, x;

	rep(y, N) {
		rep(x, N) {
			if (y == N - 1 && x == N - 1) field[y][x] = 0;
			else field[y][x] = y * N + x + 1;
		}
	}
	remSwapFrame = 0;

	font30 = CreateFontToHandle(NULL, 30, 4);
	mouse::init();
}

//�Ƃ肠�����`�悵�Ă������
void draw() {
	int y, x;
	int blockColor = GetColor(0, 200, 255);
	int edgeColor = GetColor(0, 0, 0);
	int strColor = GetColor(0, 0, 0);

	//�u���b�N
	rep(y, N) {
		rep(x, N) {
			int lx, ly, rx, ry;
			if (remSwapFrame > 0 && y == swpY1 && x == swpX1) {
				lx = swpDX1 - cellSize / 2;
				ly = swpDY1 - cellSize / 2;
				rx = swpDX1 + cellSize / 2;
				ry = swpDY1 + cellSize / 2;
			}
			else if (remSwapFrame > 0 && y == swpY2 && x == swpX2) {
				continue;
			}
			else {
				lx = toDrawLx(x);
				ly = toDrawLy(y);
				rx = toDrawRx(x);
				ry = toDrawRy(y);
				if (field[y][x] == 0) { continue; }
			}
			
			int fontLx = toFontLx((lx + rx) / 2, FontSize, field[y][x] < 10 ? 1 : 2);
			int fontLy = toFontLy((ly + ry) / 2, FontSize);

			DrawBox(lx, ly, rx, ry, blockColor, TRUE);
			DrawLine(lx, ly, rx, ly, edgeColor, 1);
			DrawLine(rx, ly, rx, ry, edgeColor, 1);
			DrawLine(rx, ry, lx, ry, edgeColor, 1);
			DrawLine(lx, ry, lx, ly, edgeColor, 1);
			DrawFormatStringToHandle(fontLx, fontLy, strColor, font30, toString(field[y][x]).c_str());
		}
	}

	//�O�����͂� (�g)
	int Ex = Sx + cellSize * N;
	int Ey = Sy + cellSize * N;
	DrawLine(Sx, Sy, Ex, Sy, 0, 2);
	DrawLine(Ex, Sy, Ex, Ey, 0, 2);
	DrawLine(Ex, Ey, Sx, Ey, 0, 2);
	DrawLine(Sx, Ey, Sx, Sy, 0, 2);
}

//�Ƃ肠�����`�悵�Ă�����i���̂Q, ����̓}�X�̎d�؂�͈ړ����Ȃ��āA�������ړ�����^�C�v�j
void draw2() {
	int y, x;
	int strColor = GetColor(0, 0, 0);

	//�d�؂�
	rep(y, N) {
		rep(x, N) {
			int lx = toDrawLx(x);
			int ly = toDrawLy(y);
			int rx = toDrawRx(x);
			int ry = toDrawRy(y);
			DrawLine(lx, ly, rx, ly, 0, 2);
			DrawLine(rx, ly, rx, ry, 0, 2);
			DrawLine(rx, ry, lx, ry, 0, 2);
			DrawLine(lx, ry, lx, ly, 0, 2);
		}
	}

	//����
	rep(y, N) {
		rep(x, N) {
			int lx, ly, rx, ry;
			if (remSwapFrame > 0 && y == swpY1 && x == swpX1) {
				lx = swpDX1 - cellSize / 2;
				ly = swpDY1 - cellSize / 2;
				rx = swpDX1 + cellSize / 2;
				ry = swpDY1 + cellSize / 2;
			}
			else if (remSwapFrame > 0 && y == swpY2 && x == swpX2) {
				continue;
			}
			else {
				lx = toDrawLx(x);
				ly = toDrawLy(y);
				rx = toDrawRx(x);
				ry = toDrawRy(y);
				if (field[y][x] == 0) { continue; }
			}

			int fontLx = toFontLx((lx + rx) / 2, FontSize, field[y][x] < 10 ? 1 : 2);
			int fontLy = toFontLy((ly + ry) / 2, FontSize);
			DrawFormatStringToHandle(fontLx, fontLy, strColor, font30, toString(field[y][x]).c_str());
		}
	}
}

//��������Ƃ��ɐݒ肷��F�X
void updateSwapState(int x1, int y1, int x2, int y2) {
	swpX1 = x1;
	swpY1 = y1;
	swpX2 = x2;
	swpY2 = y2;
	swpDX1 = toDrawCx(x1);
	swpDY1 = toDrawCy(y1);
	swpDX2 = toDrawCx(x2);
	swpDY2 = toDrawCy(y2);
	remSwapFrame = swapFrame;
}

//��������1�t���[��������̈ړ�. �t���[���������炷.
void moveForSwap() {
	if (remSwapFrame <= 0) { return; }

	double gx = swpDX2;
	double gy = swpDY2;

	if (remSwapFrame > 1) {
		swpDX1 += (gx - swpDX1) * 0.15;
		swpDY1 += (gy - swpDY1) * 0.15;
	}
	else {
		swpDX1 = gx;
		swpDY1 = gy;
		swap(field[swpY1][swpX1], field[swpY2][swpX2]);
	}

	remSwapFrame--;
}

//�}�E�X�ŐF�X������
void mouseEvent() {
	if (remSwapFrame > 0) { return; }			//�d���^�C��
	if (mouse::isClick() == false) { return; }	//�N���b�N���ĂȂ�
	
	int cellX = toCellX(mouse::x);
	int cellY = toCellY(mouse::y);
	if (cellX < 0 || cellX >= N || cellY < 0 || cellY >= N) { return; }	//�͈͊O

	if (field[cellY][cellX] == 0) { return; }	//�󔒃}�X�͑I��s��

	int dy[4] = { -1, 0, 1, 0 };
	int dx[4] = { 0, 1, 0, -1 };
	int dir, ny, nx;
	for (dir = 0; dir < 4; dir++) {
		ny = cellY + dy[dir];
		nx = cellX + dx[dir];
		if (ny < 0 || ny >= N || nx < 0 || nx >= N) continue;
		if (field[ny][nx] == 0) break;
	}
	if (dir == 4) { return; }	//0(��)���ׂ荇���Ă��Ȃ�

	//�����\
	updateSwapState(cellX, cellY, nx, ny);
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR arg, int) {
	SetGraphMode(WinX, WinY, 32);
	SetBackgroundColor(255, 255, 255);
	ChangeWindowMode(TRUE);
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);

	init();
	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 && !CheckHitKey(KEY_INPUT_ESCAPE)) {
		mouse::update();
		mouseEvent();
		moveForSwap();
		draw();
	}

	DxLib_End();
	return 0;
}
