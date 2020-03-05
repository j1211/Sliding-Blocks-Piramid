//手で解く用
#include "DxLib.h"
#include <iostream>
#include <algorithm>
#include <string>
#define rep(i, n) for(i = 0; i < n; i++)
using namespace std;

const int WinX = 900;
const int WinY = 800;

//描画用
const int Sx = 100;
const int Sy = 100;
const int cellSize = 50;
const int swapFrame = 12;		//スワップのアニメーション時間
int font30;						//フォント
const int FontSize = 30;

//データ
const int N = 4;
int field[N][N];						//今の盤面. スワップ中は, ドントケア.
int remSwapFrame;						//スワップ中の硬直が残り何フレームあるか. スワップしていないときは0が入っている.
int swpY1, swpX1, swpY2, swpX2;			//スワップ中に意味を持つ. (swpY1行, swpX1列目)(ブロック)と(swpY2行, swpX2列目)(空マス)を交換中.
double swpDY1, swpDX1, swpDY2, swpDX2;	//スワップ中に意味を持つ. 描画座標 (ブロックの中心座標).

//座標とか
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

//非負整数 -> 文字列
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

//マウス入力用
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

//とりあえず初期化してくれるやつ
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

//とりあえず描画してくれるやつ
void draw() {
	int y, x;
	int blockColor = GetColor(0, 200, 255);
	int edgeColor = GetColor(0, 0, 0);
	int strColor = GetColor(0, 0, 0);

	//ブロック
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

	//外側を囲む (枠)
	int Ex = Sx + cellSize * N;
	int Ey = Sy + cellSize * N;
	DrawLine(Sx, Sy, Ex, Sy, 0, 2);
	DrawLine(Ex, Sy, Ex, Ey, 0, 2);
	DrawLine(Ex, Ey, Sx, Ey, 0, 2);
	DrawLine(Sx, Ey, Sx, Sy, 0, 2);
}

//とりあえず描画してくれるやつ（その２, これはマスの仕切りは移動しなくて、数字が移動するタイプ）
void draw2() {
	int y, x;
	int strColor = GetColor(0, 0, 0);

	//仕切り
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

	//数字
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

//交換するときに設定する色々
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

//交換中の1フレームあたりの移動. フレーム数も減らす.
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

//マウスで色々するやつ
void mouseEvent() {
	if (remSwapFrame > 0) { return; }			//硬直タイム
	if (mouse::isClick() == false) { return; }	//クリックしてない
	
	int cellX = toCellX(mouse::x);
	int cellY = toCellY(mouse::y);
	if (cellX < 0 || cellX >= N || cellY < 0 || cellY >= N) { return; }	//範囲外

	if (field[cellY][cellX] == 0) { return; }	//空白マスは選択不可

	int dy[4] = { -1, 0, 1, 0 };
	int dx[4] = { 0, 1, 0, -1 };
	int dir, ny, nx;
	for (dir = 0; dir < 4; dir++) {
		ny = cellY + dy[dir];
		nx = cellX + dx[dir];
		if (ny < 0 || ny >= N || nx < 0 || nx >= N) continue;
		if (field[ny][nx] == 0) break;
	}
	if (dir == 4) { return; }	//0(空白)が隣り合っていない

	//交換可能
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
