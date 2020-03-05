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
const int cellSize = 65;
const int swapFrame = 12;		//スワップのアニメーション時間
int font30;						//フォント
const int FontSize = 30;

//データ
const int N = 3;
int field[N][2 * N + 1];				//今の盤面 field[面(セル)座標(y, x)] = 番号. スワップ中は, ドントケア.
int remSwapFrame;						//スワップ中の硬直が残り何フレームあるか. スワップしていないときは0が入っている.
int swpY1, swpX1, swpDir, swpY2, swpX2;	//スワップ中に意味を持つ. (swpY1行, swpX1列目)(ブロック)と(swpY2行, swpX2列目)(空マス)を交換中. これは面座標.
										//swpDirは(swpY1行, swpX1列目)のブロックの何番目の点が移動しているか？（上／下:0番, 左:1番, 右:2番), 移動方向を表す
double swpDY1, swpDX1, swpDY2, swpDX2;	//スワップ中に意味を持つ. 描画座標 (移動する1頂点の座標）

//座標とか
//セル(面) -> 頂点. x%2==0のとき(上,左,右). x%2==1のとき(下,左,右)の順で入れる.
void CellToPoint(int x, int y, int px[3], int py[3]) {
	if (x % 2 == 0) {
		//上
		px[0] = x / 2;
		py[0] = y;

		//左
		px[1] = x / 2;
		py[1] = y + 1;

		//右
		px[2] = x / 2 + 1;
		py[2] = y + 1;
		return;
	}

	//下
	px[0] = (x + 1) / 2;
	py[0] = y + 1;

	//左
	px[1] = x / 2;
	py[1] = y;

	//右
	px[2] = (x + 1) / 2;
	py[2] = y;
}

//頂点 -> 描画
int PointToDrawX(int x, int y) { return Sx + (N - y) * cellSize / 2 + x * cellSize; }
int PointToDrawY(int y) { return Sy + cellSize * y; }

//描画：重心 -> 描画：左上
int toFontLx(int cx, int fontSize, int textLength) {
	return cx - fontSize * textLength / 4;
}
int toFontLy(int cy, int fontSize) {
	return cy - 2 * fontSize / 3;
}

//描画 -> セル(面)
int toCellY(int y) {
	if (y < Sy) return -1;
	return (y - Sy) / cellSize;
}

int toCellX(int x, int y) {
	if (y < Sy) return -1;

	int sz = y - PointToDrawY(toCellY(y));
	int sx = Sx + cellSize * N / 2 - (y - Sy) / 2;
	x -= sx; 
	if (x < 0) return -1;

	int cellX = 2 * (x / cellSize);
	x %= cellSize;
	if (x >= sz) { cellX++; }
	return cellX;
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

	int id = 0;
	rep(y, N) {
		rep(x, 2 * y + 1) {
			field[y][x] = id;
			id++;
		}
	}
	remSwapFrame = 0;

	font30 = CreateFontToHandle(NULL, 30, 4);
	mouse::init();
}

//とりあえず描画してくれるやつ
void draw() {
	int y, x, i;
	int blockColor = GetColor(0, 200, 255);
	int edgeColor = GetColor(0, 0, 0);
	int strColor = GetColor(0, 0, 0);

	//面座標(y, x)それぞれについて…
	rep(y, N) {
		rep(x, 2 * y + 1) {
			//まずは頂点座標にする
			int px[3], py[3];
			CellToPoint(x, y, px, py);

			//次に描画座標にする
			int drawX[3], drawY[3];
			rep(i, 3) { drawX[i] = PointToDrawX(px[i], py[i]); drawY[i] = PointToDrawY(py[i]); }

			if (remSwapFrame > 0 && y == swpY1 && x == swpX1) {
				drawX[swpDir] = swpDX1;
				drawY[swpDir] = swpDY1;
			}
			else if (remSwapFrame > 0 && y == swpY2 && x == swpX2) {
				continue;
			}
			else {
				if (field[y][x] == 0) { continue; }
			}

			int baseX = (drawX[0] + drawX[1] + drawX[2]) / 3;
			int baseY = (drawY[0] + drawY[1] + drawY[2]) / 3;
			int fontLx = toFontLx(baseX, FontSize, field[y][x] < 10 ? 1 : 2);
			int fontLy = toFontLy(baseY, FontSize);

			DrawTriangle(drawX[0], drawY[0], drawX[1], drawY[1], drawX[2], drawY[2], blockColor, TRUE);
			rep(i, 3) DrawLine(drawX[i], drawY[i], drawX[(i + 1) % 3], drawY[(i + 1) % 3], edgeColor, 1);
			DrawFormatStringToHandle(fontLx, fontLy, strColor, font30, toString(field[y][x]).c_str());
		}
	}

	//外側を囲む (枠)
	int wakuX[3] = { PointToDrawX(0, 0), PointToDrawX(0, N), PointToDrawX(N, N) };
	int wakuY[3] = { PointToDrawY(0), PointToDrawY(N), PointToDrawY(N) };
	rep(i, 3) { DrawLine(wakuX[i], wakuY[i], wakuX[(i + 1) % 3], wakuY[(i + 1) % 3], edgeColor, 2); }
}

//とりあえず描画してくれるやつ（その２, これはマスの仕切りは移動しなくて、数字が移動するタイプ）
void draw2() {
	int y, x, i;
	int blockColor = GetColor(0, 200, 255);
	int edgeColor = GetColor(0, 0, 0);
	int strColor = GetColor(0, 0, 0);

	//面座標(y, x)それぞれについて…
	rep(y, N) {
		rep(x, 2 * y + 1) {
			//まずは頂点座標にする
			int px[3], py[3];
			CellToPoint(x, y, px, py);

			//次に描画座標にする
			int drawX[3], drawY[3];
			rep(i, 3) { drawX[i] = PointToDrawX(px[i], py[i]); drawY[i] = PointToDrawY(py[i]); }

			//先に枠を描く
			rep(i, 3) DrawLine(drawX[i], drawY[i], drawX[(i + 1) % 3], drawY[(i + 1) % 3], edgeColor, 1);

			if (remSwapFrame > 0 && y == swpY1 && x == swpX1) {
				drawX[swpDir] = swpDX1;
				drawY[swpDir] = swpDY1;
			}
			else if (remSwapFrame > 0 && y == swpY2 && x == swpX2) {
				continue;
			}
			else {
				if (field[y][x] == 0) { continue; }
			}

			int baseX = (drawX[0] + drawX[1] + drawX[2]) / 3;
			int baseY = (drawY[0] + drawY[1] + drawY[2]) / 3;
			int fontLx = toFontLx(baseX, FontSize, field[y][x] < 10 ? 1 : 2);
			int fontLy = toFontLy(baseY, FontSize);
			DrawFormatStringToHandle(fontLx, fontLy, strColor, font30, toString(field[y][x]).c_str());
		}
	}

	//外側を囲む (枠)
	int wakuX[3] = { PointToDrawX(0, 0), PointToDrawX(0, N), PointToDrawX(N, N) };
	int wakuY[3] = { PointToDrawY(0), PointToDrawY(N), PointToDrawY(N) };
	rep(i, 3) { DrawLine(wakuX[i], wakuY[i], wakuX[(i + 1) % 3], wakuY[(i + 1) % 3], edgeColor, 2); }
}

//交換するときに設定する色々
void updateSwapState(int x1, int y1, int x2, int y2, int dir) {
	swpX1 = x1;
	swpY1 = y1;
	swpX2 = x2;
	swpY2 = y2;
	swpDir = dir;

	int px[3], py[3];
	CellToPoint(x1, y1, px, py);
	swpDX1 = PointToDrawX(px[dir], py[dir]);
	swpDY1 = PointToDrawY(py[dir]);

	CellToPoint(x2, y2, px, py);
	int toDir[3] = { 0, 2, 1 };
	swpDX2 = PointToDrawX(px[toDir[dir]], py[toDir[dir]]);
	swpDY2 = PointToDrawY(py[toDir[dir]]);

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

	int cellX = toCellX(mouse::x, mouse::y);
	int cellY = toCellY(mouse::y);
	if (cellY < 0 || cellY >= N || cellX < 0 || cellX > 2 * cellY) { return; }	//範囲外

	if (field[cellY][cellX] == 0) { return; }	//空白マスは選択不可

	int dy[2][3] = { {1, 0,  0}, {-1, 0,  0} };
	int dx[2][3] = { {1, 1, -1}, {-1, 1, -1} };

	int dir, ny, nx;
	for (dir = 0; dir < 3; dir++) {
		ny = cellY + dy[cellX % 2][dir];
		nx = cellX + dx[cellX % 2][dir];
		if (ny < 0 || ny >= N || nx < 0 || nx > 2 * ny) continue;
		if (field[ny][nx] == 0) break;
	}
	if (dir == 3) { return; }	//0(空白)が隣り合っていない

	//交換可能
	updateSwapState(cellX, cellY, nx, ny, dir);
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
