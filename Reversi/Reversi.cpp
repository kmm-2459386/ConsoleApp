//======================================
//	���o�[�V
//======================================
#include "Reversi.h"
#include "Vector2.h"
#include "Utility.h"
#include <stdio.h>  // printf()
#include <assert.h> // assert()
// �֐��v���g�^�C�v
bool inBoard(int x, int y);
const char* getTurnName(Cell turn);

const char* diskAA[] = {
	"�E",  // NONE
	"��",  // BLACK
	"�Z",  // WHITE
	"��",  // OUT
};

// �{�[�h�̎w��ʒu�ɑ��
void SetBoard(Reversi* reversi, int x, int y, Cell value)
{
	if (inBoard(x, y)) {
		reversi->board[y][x] = value;
	}
	//
	// ���������R�[�f�B���O���Ă��������B
	// inBoard()���m�F���ā@board��(x,y)���W��value���i�[���܂�
	// inBoard()�O�Ȃ�A�������܂���
	//
}
// �{�[�h�̎w��ʒu���擾
Cell GetBoard(Reversi* reversi, int x, int y)
{
	if (inBoard(x, y)) {
		return reversi->board[y][x];
	}
	return OUT;
	//
	// ���������R�[�f�B���O���Ă��������B
	// inBoard()���m�F���ā@board��(x,y)���W�̒l��Ԃ��܂��B
	// inBoard()�O�Ȃ�AOUT ��Ԃ��Ă��������B
	//
}
// ������
void Init(Reversi* reversi, Mode mode)
{
	reversi->mode = mode;
	for (int y = 0; y < BOARD_HEI; y++) {
		for (int x = 0; x < BOARD_WID; x++) {
			SetBoard(reversi, x, y, NONE);
		}
	}
	SetBoard(reversi, 3, 4, BLACK);
	SetBoard(reversi, 4, 3, BLACK);
	SetBoard(reversi, 3, 3, WHITE);
	SetBoard(reversi, 4, 4, WHITE);
	//
	// ���������R�[�f�B���O���Ă��������B
	// �{�[�h��������Ԃɂ��܂��B
	// �E�E�E�E�E�E�E�E
	// �E�E�E�E�E�E�E�E
	// �E�E�E�E�E�E�E�E
	// �E�E�E�Z���E�E�E
	// �E�E�E���Z�E�E�E
	// �E�E�E�E�E�E�E�E
	// �E�E�E�E�E�E�E�E
	// �E�E�E�E�E�E�E�E
	//
	reversi->turn = BLACK;
}
// ���̃^�[����
void NextTurn(Reversi* reversi)
{
	reversi->turn = GetOpponent(reversi->turn);
}
// �w��ʒu�ɐ΂������邩
bool CheckCanPlace(Reversi* reversi, Cell myself, Vector2 pos, bool turnOver)
{
	bool canPlace = false;
	// ���ɒu����Ă���?
	if (GetBoard(reversi, pos.x, pos.y) != NONE) {
		return false;
	}
	Cell opponent = GetOpponent(myself);
	// �W�����ɂ��āA
	for (int d = 0; d < DIR_MAX; d++) {
		Vector2 scan = pos;
		Vector2 dir = GetDirVector2((DIRECTION)d);
		// �ׂ͑���R�}?
		// 
		AddVector2(&scan, &dir);
		if (GetBoard(reversi, scan.x, scan.y) != opponent) {
			continue;
		}
		//
		// ����R�}�łȂ��Ȃ�܂Ői�߂�
		Cell tmp;
		//
		do {
			AddVector2(&scan, &dir);
			tmp = GetBoard(reversi, scan.x, scan.y);
		} while (tmp == opponent);
		// 
		// �����̃R�}�Ȃ�Aplace��
		if (tmp == myself) {
			canPlace = true;
			// �Ђ����肩����
			if (turnOver) {
				scan = pos;
				do {
					AddVector2(&scan, &dir);
					tmp = GetBoard(reversi, scan.x, scan.y);
					if (tmp == opponent) {
						SetBoard(reversi, scan.x, scan.y, myself);
					}
				} while (tmp == opponent);
			}
		}
		// �u�u����vturnOver���Ȃ��Ȃ�A�I��
		if (canPlace && turnOver == false) {
			break;
		}
	}
	return canPlace;
}
// �R�}�̐��𐔂���
int getDiskCount(Reversi* reversi, Cell myself)
{
	int count = 0;
	//
	for (int y = 0; y < BOARD_HEI; y++) {
		for (int x = 0; x < BOARD_WID; x++) {
			if (GetBoard(reversi, x, y) == myself) {
				count++;
			}
		}
	}
	//
	return count;
}
// �΂��ǂ����ɒu���邩?
bool CheckCanPlaceAll(Reversi* reversi, Cell myself)
{
	for (int y = 0; y < BOARD_HEI; y++) {
		for (int x = 0; x < BOARD_WID; x++) {
			Vector2 pos = { x,y };
			if (CheckCanPlace(reversi, myself, pos)) {
				return true;
			}
		}
	}
	return false;
}
// �u����ꏊ�����X�e�B���O
void ListCanPlaceAll(Reversi* reversi, Cell myself, Vector2List* list)
{
	Vector2 pos;
	for (pos.y = 0; pos.y < BOARD_HEI; pos.y++) {
		for (pos.x = 0; pos.x < BOARD_WID; pos.x++) {
			if (CheckCanPlace(reversi, myself, pos)) {
				AddVector2List(list, pos);
			}
		}
	}
}
// ��ʕ`��
void DrawScreen(Reversi* reversi, Vector2 pos, DrawStatus stat)
{
	ClearScreen();
	bool isCursor = (stat == IN_PLAY) && IsHumanPlayer(reversi);
	//
	for (int y = 0; y < BOARD_HEI; y++) {
		for (int x = 0; x < BOARD_WID; x++) {
			Cell tmp = GetBoard(reversi, x, y);
			printf("%s", diskAA[tmp]);
		}
		if (isCursor && pos.y == y) {
			printf("��");
		}
		putchar('\n');
	}
		for (int x = 0; x < BOARD_WID; x++) {
			if (isCursor && pos.x == x) {
				printf("��");
			}
			else{
				printf("�@");
		}
	}
	
	// ���������R�[�f�B���O���Ă��������B
	// �{�[�h�𑖍�����cell�ɂ���� diskAA[] ��\�����܂��B
	// isCorsor���^�Ȃ�Apos�ɑΉ��������A�E�Ɂ������Ɂ���\�����܂��B
	// �E�E�E�E�E�E�E�E�E
	// �E�E�E�E�E�E�E�E�E
	// �E�E�E�E�E�E�E�E�E
	// �E�E�E�Z���E�E�E�E��
	// �E�E�E���Z�E�E�E�E
	// �E�E�E�E�E�E�E�E�E
	// �E�E�E�E�E�E�E�E�E
	// �E�E�E�E�E�E�E�E�E
	// �@�@�@��
	//
	putchar('\n');

	switch (stat) {
	case IN_PLAY:
	case NO_PLACE:
		printf("%s�̃^�[���ł�\n", getTurnName(reversi->turn));
		if (stat == NO_PLACE) {
			printf("�łĂ�Ƃ��낪����܂���\n");
		}
		// 
		// ���������R�[�f�B���O���Ă��������B
		// �u�`�̃^�[���ł��v��\�����܂�
		// �X�� NO_PLACE�̂Ƃ���
		// �u�łĂ�Ƃ��낪����܂���v��\�����܂�
		//
		break;
	case RESULT:
	{
		int blackCount = getDiskCount(reversi, BLACK);
		int whiteCount = getDiskCount(reversi, WHITE);
		// 
		printf("%s%d-%s%d\n",
			getTurnName(BLACK), blackCount,
			getTurnName(WHITE), whiteCount
		);
		if (blackCount == whiteCount) {
			puts("���������ł�");
		}
		else {
			Cell winner = (blackCount > whiteCount) ? BLACK : WHITE;
			printf("%s�̏����ł�\n", getTurnName(winner));
		}
		// ���������R�[�f�B���O���Ă�������
		// �u��xx-��xx�v(xx�̓R�}��)��\�����܂�
		// �R�}���������Ȃ�
		// �u���������ł��v��\�����܂�
		// �����łȂ����
		// �u�`�̏����ł��v��\�����܂�
		// 
	}
	break;
	}
}

bool IsHumanPlayer(Reversi* reversi)
{
	switch (reversi->mode) {
	case MODE_1P:
		return reversi->turn == BLACK;
	case MODE_2P:
		return true;
	default:
	case MODE_WATCH:
		return false;
	}
}

const char* getTurnName(Cell turn)
{
	if (turn == BLACK) {
		return "��";
	}
	if (turn == WHITE) {
		return "��";
	}
	assert(false);
	return "";
}
// ����̐F���擾
Cell GetOpponent(Cell value)
{
	if (value == WHITE) {
		return BLACK;
	}
	if (value == BLACK) {
		return WHITE;
	}
	assert(false);
	return NONE;
}
// ���W���{�[�h����?
bool inBoard(int x, int y)
{
	return 0 <= x && x < BOARD_WID
		&& 0 <= y && y < BOARD_HEI;
}