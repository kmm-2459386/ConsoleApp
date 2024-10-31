//======================================
//	マインスィーパー ステージ
//======================================
#include "Stage.h"
#include "Cell.h"
#include "Utility.h"
#include <stdio.h>  // printf()
#include <assert.h>

// 関数プロトタイプ
static void SetupField(Stage* stage);
static bool IsCursorPosition(Stage* stage, int x, int y);
static void UpdateField(Stage* stage);
static void OpenFieldAll(Stage* stage);
static Cell* GetCursorCell(Stage* stage);
static bool IsInField(int x, int y);
static bool IsClear(Stage* stage);
static void Beep();

// ステージ初期化
void InitializeStage(Stage* stage)
{
	SetupField(stage);
	UpdateField(stage);
	stage->cursorX = 0;
	stage->cursorY = 0;
	stage->isExplosion = false;
	stage->isClear = false;
}
// フィールドのセットアップ
static void SetupField(Stage* stage)
{
	// 全部リセット
	for (int y = 0; y < FIELD_HEIGHT; y++) {
		for (int x = 0; x < FIELD_WIDTH; x++) {
			Cell* cell = GetFieldCell(stage, x, y);
			SetupCell(cell);
		}
	}
	// ランダムにBOMB配置
	int count = BOMB_COUNT;
	while (count > 0) {
		int x = GetRand(FIELD_WIDTH);
		int y = GetRand(FIELD_HEIGHT);
		Cell* cell = GetFieldCell(stage, x, y);
		if (IsBomb(cell) == false) {
			SetBomb(cell, true);
			count--;
		}
	}
}
// 隣接ボムの数を更新
static void UpdateField(Stage* stage)
{
	for (int y = 0; y < FIELD_HEIGHT; y++) {
		for (int x = 0; x < FIELD_WIDTH; x++) {
			Cell* cell = GetFieldCell(stage, x, y);
			int n = GetAdjacentBombsCount(stage, x, y);
			SetAdjacentBombs(cell, n);
		}
	}
}
// フィールドを開示する
static void OpenFieldAll(Stage* stage)
{
	for (int y = 0; y < FIELD_HEIGHT; y++) {
		for (int x = 0; x < FIELD_WIDTH; x++) {
			Cell* cell = GetFieldCell(stage, x, y);
			OpenCell(cell);
		}
	}
}
// 隣接ボムの数を取得
int GetAdjacentBombsCount(Stage* stage, int x, int y)
{
	int count = 0;
	for (int dy = -1; dy <= 1; dy++) {
		for (int dx = -1; dx <= 1; dx++) {
			if (dx == 0 && dy == 0) {
				continue;
			}
			Cell* cell = GetFieldCell(stage, x + dx, y+dy);
			if (cell != nullptr && IsBomb(cell)) {
				count++;
			}
		}
	}
	return count;
}
// 自動開示
void AutoEraseMines(Stage* stage, int x, int y)
{
	Cell* cell = GetFieldCell(stage, x, y);
	if (cell == nullptr || IsBomb(cell) || IsHide(cell) == false) {
		return;
	}
	SetHide(cell, false);
	if (GetAdjacentBombs(cell) == 0) {
		//隣接マスを開いてみる
		for (int dy = -1; dy <= 1; dy++) {
			for (int dx = -1; dx <= 1; dx++) {
				if (dx == 0 && dy == 0) {
					continue;
				}
				AutoEraseMines(stage, x + dx, y + dy);
			}
		}
	}
}

const char* AA_CURSOR = "＠";  // カーソル
const char* AA_EXPLOSION = "※"; // 爆発
const char* AA_FLAG = "▲";  // フラグ
const char* AA_BOMB = "●";  // 爆弾
const char* AA_MINE = "■";  // 未開放
const char* AA_NUMBERS[] = {   // 開放済
	"・",
	"１",
	"２",
	"３",
	"４",
	"５",
	"６",
	"７",
	"８",
	"９",
};
// スクリーン描画
void DrawScreen(Stage* stage)
{
	int hideCount = 0;
	int bombCount = 0;
	int flagCount = 0;

	ClearScreen();
	for (int y = 0; y < FIELD_HEIGHT; y++) {
		for (int x = 0; x < FIELD_WIDTH; x++) {
			Cell* cell = GetFieldCell(stage, x, y);
			if (IsCursorPosition(stage, x, y) && stage->isClear == false) {
				const char* tmp = stage->isExplosion ? AA_EXPLOSION : AA_CURSOR;
				printf(tmp);
			}
			else {
				if (IsFlag(cell)) {
					printf(AA_FLAG);
				}
				else if (IsHide(cell)) {
					printf(AA_MINE);
				}
				else if (IsBomb(cell)) {
					printf(AA_BOMB);
				}
				else {
					int n = GetAdjacentBombs(cell);
					assert(0 <= n && n <= 8);
					printf(AA_NUMBERS[n]);
				}
			}
			if (IsBomb(cell)) bombCount++;
			if (IsHide(cell)) hideCount++;
			if (IsFlag(cell)) flagCount++;
		}
		putchar('\n');
	}
	putchar('\n');
	printf("フラグ／ボム:%3d/%3d\n", flagCount, bombCount);
	int open1 = FIELD_WIDTH * FIELD_HEIGHT - hideCount; //開放済の数
	int open2 = FIELD_WIDTH * FIELD_HEIGHT - bombCount; //開放すべき数
	printf("開放　　　　:%3d/%3d\n", open1, open2);
}
// カーソル位置か?
static bool IsCursorPosition(Stage* stage, int x, int y)
{
	return x == stage->cursorX && y == stage->cursorY;
}

// カーソル移動
void MoveCursor(Stage* stage, int addX, int addY)
{
	int x = stage->cursorX + addX;
	int y = stage->cursorY + addY;
	if (IsInField(x, y)) {
		stage->cursorX = x;
		stage->cursorY = y;
	}
}
// カーソルのflag反転
void FlipCursorFlag(Stage* stage)
{
	Cell* cell = GetCursorCell(stage);
	FlipFlag(cell);
}
// カーソルのセルを開放(終了フラグを返す)
bool OpenCursorCell(Stage* stage)
{
	Cell* cell = GetCursorCell(stage);
	SetHide(cell, false);
	if (IsBomb(cell)) {
		Beep();
		stage->isExplosion = true;
		OpenFieldAll(stage);
	}
	else if (IsClear(stage)) {
		Beep();
		stage->isClear = true;
	}
	// 自動開示
	for (int dy = -1; dy <= 1; dy++) {
		for (int dx = -1; dx <= 1; dx++) {
			if (dx == 0 && dy == 0) {
				continue;
			}
			int x = stage->cursorX + dx;
			int y = stage->cursorY + dy;
			AutoEraseMines(stage, x, y);
		}
	}
	return stage->isExplosion || stage->isClear;
}
// クリアか?
static bool IsClear(Stage* stage)
{
	for (int y = 0; y < FIELD_HEIGHT; y++) {
		for (int x = 0; x < FIELD_WIDTH; x++) {
			Cell* cell = GetFieldCell(stage, x, y);
			//ボムでないところが相手いないなら、非クリア
			if (IsBomb(cell) == false && IsHide(cell)) {
				return false;
			}
		}
	}
	return true;
}
// カーソル位置のセルを取得
static Cell* GetCursorCell(Stage* stage)
{
	return GetFieldCell(stage, stage->cursorX, stage->cursorY);
}
// フィールドのセル取得
Cell* GetFieldCell(Stage* stage, int x, int y)
{
	if (IsInField(x, y)) {
		return &stage->field[y][x];
	}
	return nullptr;
}
// フィールド内か?
bool IsInField(int x, int y)
{
	return 0 <= x && x < FIELD_WIDTH
		&& 0 <= y && y < FIELD_HEIGHT;
}
// 勝った?
bool IsWin(Stage* stage)
{
	return stage->isClear;
}
// 負けた?
bool IsLose(Stage* stage)
{
	return stage->isExplosion;
}
// ビープ音を鳴らす
static void Beep()
{
	putchar('\a');
}