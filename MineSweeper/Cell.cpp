//======================================
//	マインスィーパー セル
//======================================
#include "Cell.h"
// セットフップ(初期化)状態にする
void SetupCell(Cell* cell)
{
	cell->bomb = false;
	cell->hide = true;
	cell->flag = false;
	cell->adjacentBombs = 0;
}
// bombか?
bool IsBomb(Cell* cell)
{
	return cell->bomb;
}
// bomb をセット
void SetBomb(Cell* cell, bool value)
{
	cell->bomb = value;
}
// hideか?
bool IsHide(Cell* cell)
{
	return cell->hide;
}
// hide をセット
void SetHide(Cell* cell, bool value)
{
	cell->hide = value;
}
// flagか?
bool IsFlag(Cell* cell)
{
	return cell->flag;
}
// flag を反転
void FlipFlag(Cell* cell)
{
	cell->flag != cell->flag;
}
// adjacentBombsを取得
int GetAdjacentBombs(Cell* cell)
{
	return cell->adjacentBombs;
}
// adjacentBombsをセット
void SetAdjacentBombs(Cell* cell, int value)
{
	cell->adjacentBombs = value;
}
// 開示する
void OpenCell(Cell* cell)
{
	cell->hide = false;
	cell->flag = false;
}
