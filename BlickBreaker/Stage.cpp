//======================================
//	ブロックくずし ステージ
//======================================
#include "Stage.h"
#include "Utility.h"
#include <stdio.h>

// 関数プロトタイプ
static void InitPaddle(Stage* stage);
static void InitField(Stage* stage);
static void SetField(Stage* stage, int x, int y, Field value);
static bool IsInField(int x, int y);
static bool IsBallPosition(Stage* stage, int x, int y);
static bool IsPaddlePosition(Stage* stage, int x, int y);
static void DrawHorizontalWall();

const char* AA_WALL = "■";
const char* AA_BALL = "●";
const char* AA_PADDLE = "回";
const char* AA_BLOCK = "□";
const char* AA_NONE = "　";

// ステージ初期化
void InitializeStage(Stage* stage)
{
	ResetBall(stage);
	InitPaddle(stage);
	InitField(stage);
}
// パドルの初期化
static void InitPaddle(Stage* stage)
{
	stage->paddleX = (FIELD_WIDTH - PADDLE_WIDTH) / 2;
	stage->paddleY = FIELD_HEIGHT - 3;
}
// フィールドの初期化
static void InitField(Stage* stage)
{
	for (int y = 0; y < FIELD_HEIGHT; y++) {
		Field blk = (y < FIELD_HEIGHT / 4) ? FIELD_BLOCK : FIELD_NONE;
		for (int x = 0; x < FIELD_WIDTH; x++) {
			SetField(stage, x, y, blk);
		}
	}
}
// ボールの初期化(リセット)
void ResetBall(Stage* stage)
{
	stage->ballX = GetRand(FIELD_WIDTH);
	stage->ballY = FIELD_HEIGHT / 3;
	stage->ballVelocityX = GetRand(2) == 0 ? 1 : -1;
	stage->ballVelocityY = 1;
}
void DrawScreen(Stage* stage, DrawMode mode)
{
	ClearScreen();
	DrawHorizontalWall();
	for (int y = 0; y <= FIELD_HEIGHT; y++) {
		printf(AA_WALL);
		for (int x = 0; x < FIELD_WIDTH; x++) {
			if (IsBallPosition(stage, x, y)) {
				printf(AA_BALL);
			}
			else if (IsPaddlePosition(stage, x, y)) {
				printf(AA_PADDLE);
			}
			else {
				Field blk = GetField(stage, x, y);
				printf(blk == FIELD_BLOCK ? AA_BLOCK : AA_NONE);
			}
		}
		printf(AA_WALL);
		putchar('\n');
	}
	DrawHorizontalWall();

	const char* msg = nullptr;
	int len;
	switch (mode) {
	case DM_PAUSE: msg = "ＰＡＵＳＥ"; len = 5; break;
	case DM_READY: msg = "ＲＥＡＤＹ"; len = 5; break;
	case DM_CLEAR: msg = "ＳＴＡＧＥ　ＣＬＥＡＲ"; len = 11; break;
	}
	if (msg != nullptr) {
		const int SCREEN_WIDTH = FIELD_WIDTH + 2;
		const int SCREEN_HEIGHT = FIELD_HEIGHT + 2;
		SaveCursor();
		PrintCursor((SCREEN_WIDTH - len) / 2 * 2 + 1, SCREEN_HEIGHT / 2 + 1);
		printf(msg);
		RestoreCursor();
	}
}
// ボール位置か?
static bool IsBallPosition(Stage* stage, int x, int y)
{
	return x == stage->ballX && y == stage->ballY;
}
// パドルの位置か?
static bool IsPaddlePosition(Stage* stage, int x, int y)
{
	if(y == stage->paddleY) {
		int dx = x - stage->paddleX;
		return 0 <= dx && dx < PADDLE_WIDTH;
	}
	return false;
}
// 水平壁の描画
static void DrawHorizontalWall()
{
	for (int x = 0; x < FIELD_WIDTH + 2; x++) {
		printf(AA_WALL);
	}
	putchar('\n');
}
// ボールの移動
void MoveBall(Stage* stage)
{
	stage->ballX += stage->ballVelocityX;
	stage->ballY += stage->ballVelocityY;

	// ボールが端にあるなら速度反転
	if (stage->ballX <= 0) {
		stage->ballVelocityX = 1;
	}
	else if (stage->ballX >= FIELD_WIDTH - 1) {
		stage->ballVelocityX = -1;
	}
	if (stage->ballY <= 0) {
		stage->ballVelocityY = 1;
	}
	else if (stage->ballY >= FIELD_HEIGHT - 1) {
		stage->ballVelocityY = -1;
	}
	// ボールがハドルに当たったら反射
	if (stage->ballY == stage->paddleY - 1) {
		if (stage->ballX >= stage->paddleX - 1
			&& stage->ballX <= stage->paddleX + PADDLE_WIDTH + 1) {
			stage->ballVelocityX = (stage->ballX < stage->paddleX + PADDLE_WIDTH / 2) ? -1 : 1;
			stage->ballVelocityY = -1;
		}
	}
	// ボールの上3コマのブロックを消す
	for (int x = stage->ballX - 1; x <= stage->ballX + 1; x++) {
		int y = stage->ballY - 1;
		if (GetField(stage, x, y) == FIELD_BLOCK) {
			SetField(stage, x, y, FIELD_NONE);
			stage->ballVelocityY = 1;
		}
	}
}
// ボールを落としてしまった?
bool IsBallMiss(Stage* stage)
{
	return stage->ballY >= FIELD_HEIGHT - 1;
}
// 面クリア?
bool IsClear(Stage* stage)
{
	for (int y = 0; y < FIELD_HEIGHT; y++) {
		for (int x = 0; x < FIELD_WIDTH; x++) {
			if (GetField(stage, x, y) == FIELD_BLOCK) {
				return false;
			}
		}
	}
	return true;
}
// パドル移動
void MovePaddle(Stage* stage, int addX)
{
	int x = stage->paddleX + addX;
	if (0 <= x && x + PADDLE_WIDTH - 1 < FIELD_WIDTH) {
		stage->paddleX = x;
	}
}
// フィールドのセッター
static void SetField(Stage* stage, int x, int y, Field value)
{
	if (IsInField(x, y)) {
		stage->field[y][x] = value;
	}
}
static Field GetField(Stage* stage, int x, int y)
{
	if (IsInField(x, y)) {
		return stage->field[y][x];
	}
	return FIELD_OUT;
}
// フィールド内か?
static bool IsInField(int x, int y)
{
	return 0 <= x && x < FIELD_WIDTH
		&& 0 <= y && y < FIELD_HEIGHT;
}