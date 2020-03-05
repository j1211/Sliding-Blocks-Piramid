//��ŉ����p (����̂ŃA�j���[�V�����Ȃ��I�j
#include "DxLib.h"
#include <iostream>
#include <algorithm>
#include <string>
#include <complex>
#include <cmath>
#define rep(i, n) for(i = 0; i < n; i++)
using namespace std;

typedef complex<double> Point;

const int WinX = 900;
const int WinY = 800;

//�`��p
const int Sx = 100;
const int Sy = 100;
const int cellSize = 60;
int font30;						//�t�H���g
const int FontSize = 30;

//�f�[�^
const int N = 4;
int field[N][N];				//���̔Ֆ� field[�Z�����W(y, x)] = �ԍ�.

namespace Math { const double sq3 = sqrt(3.0); }

//���W�Ƃ�
//�Z��(���S) -> �`��
int CellToDrawX(int x, int y) { return Sx + (N - y) * cellSize / 2 + x * cellSize; }
int CellToDrawY(int y) { return Sy + cellSize / Math::sq3 + cellSize * Math::sq3 * y / 2; }

//�`��F�Z�����S -> �`��F����
int toFontLx(int cx, int fontSize, int textLength) {
	return cx - fontSize * textLength / 4;
}
int toFontLy(int cy, int fontSize) {
	return cy - fontSize / 2;
}

//�`�� -> �Z��(��)
double cross(Point a, Point b) { return a.real() * b.imag() - a.imag() * b.real(); }

//�T�u�֐� �O�p�`(a, b, c)�̓����ɓ_p���܂܂�邩�H
bool isInTri(Point a, Point b, Point c, Point p) {
	double A = cross(b - a, p - a);
	double B = cross(c - b, p - b);
	double C = cross(a - c, p - c);
	if (A > 0 && B > 0 && C > 0) return true;
	if (A < 0 && B < 0 && C < 0) return true;
	return false;
}

//�T�u�֐�check6 (�Z���ԍ�(cx, cy)�̘Z�p�`�Z���Ƀ}�E�X�ŃN���b�N�������W(x, y)���܂܂�邩�j
bool check6(int cx, int cy, int x, int y) {
	double Cx = CellToDrawX(cx, cy);
	double Cy = CellToDrawY(cy);
	
	int i;
	Point p[6];
	double PI = 3.14159265358979;
	rep(i, 6) {
		p[i] = Point(Cx, Cy) + cellSize / Math::sq3 * exp(Point(0, 1) * (PI / 2 + i * PI / 3));
	}

	Point point = Point((double)x, (double)y);
	rep(i, 6) {
		if (isInTri(Point(Cx, Cy), p[i], p[(i + 1) % 6], point)) return true;
	}
	return false;
}

//�V�˓I�ȃA�C�f�A�H�F�e�Z�����S�ɁE������ƁA(x, y)�ɍł��߂��E�̔ԍ��𓚂���Ηǂ��I�I
//���A�ł��A�ǂ̃Z���ɂ��܂܂�Ȃ��p�^�[��������̂��B���Ⴀ1��͋𒼂ɔ��肷�銴�����˂��B
//N���������̂ŁA����1�ɍi��Ƃ���܂ł͑S�T�������Ⴂ�܂����B
void DrawToCell(int x, int y, int &cellX, int &cellY) {
	int cx, cy;
	int minDist2 = 1145141919;
	int kohoX, kohoY;

	rep(cx, N) {
		rep(cy, N) {
			int drawX = CellToDrawX(cx, cy);
			int drawY = CellToDrawY(cy);
			int dx = x - drawX;
			int dy = y - drawY;
			int dist2 = dx * dx + dy * dy;
			if (minDist2 > dist2) {
				minDist2 = dist2;
				kohoX = cx;
				kohoY = cy;
			}
		}
	}

	if (check6(kohoX, kohoY, x, y)) { cellX = kohoX; cellY = kohoY; return; }
	cellX = -1; cellY = -1;
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

	int id = 0;
	rep(y, N) {
		rep(x, y + 1) {
			field[y][x] = id;
			id++;
		}
	}

	font30 = CreateFontToHandle(NULL, 30, 4);
	mouse::init();
}

//�Ƃ肠�����`�悵�Ă�����i���̂Q, ����̓}�X�̎d�؂�͈ړ����Ȃ��āA�������ړ�����^�C�v�j
void draw2() {
	int y, x, i;
	int blockColor = GetColor(0, 200, 255);
	int edgeColor = GetColor(0, 0, 0);
	int strColor = GetColor(0, 0, 0);

	//�ʍ��W(y, x)���ꂼ��ɂ��āc
	rep(y, N) {
		rep(x, y + 1) {
			double Cx = CellToDrawX(x, y);
			double Cy = CellToDrawY(y);
			Point p[6];
			double PI = 3.14159265358979;
			rep(i, 6) {
				p[i] = Point(Cx, Cy) + cellSize / Math::sq3 * exp(Point(0, 1) * (PI / 2 + i * PI / 3));
			}

			rep(i, 6) {
				DrawLine((int)p[i].real(), (int)p[i].imag(), (int)p[(i + 1) % 6].real(), (int)p[(i + 1) % 6].imag(), edgeColor, 1);
			}
	
			if (field[y][x] > 0) {
				int fontLx = toFontLx((int)Cx, FontSize, field[y][x] < 10 ? 1 : 2);
				int fontLy = toFontLy((int)Cy, FontSize);
				DrawFormatStringToHandle(fontLx, fontLy, strColor, font30, toString(field[y][x]).c_str());
			}
		}
	}
}

//�}�E�X�ŐF�X������
void mouseEvent() {
	if (mouse::isClick() == false) { return; }	//�N���b�N���ĂȂ�
	
	int cellX, cellY;
	DrawToCell(mouse::x, mouse::y, cellX, cellY);

	if (cellY < 0 || cellY >= N || cellX < 0 || cellX > cellY) { return; }	//�͈͊O

	if (field[cellY][cellX] == 0) { return; }	//�󔒃}�X�͑I��s��

	int dy[6] = {-1, -1, 0, 1, 1,  0};
	int dx[6] = {-1,  0, 1, 1, 0, -1};

	int dir, ny, nx;
	for (dir = 0; dir < 6; dir++) {
		ny = cellY + dy[dir];
		nx = cellX + dx[dir];
		if (ny < 0 || ny >= N || nx < 0 || nx > ny) continue;
		if (field[ny][nx] == 0) break;
	}
	if (dir == 6) { return; }	//0(��)���ׂ荇���Ă��Ȃ�

	//�����\
	swap(field[cellY][cellX], field[ny][nx]);
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
		draw2();
	}

	DxLib_End();
	return 0;
}
