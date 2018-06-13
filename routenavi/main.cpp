#include "routenavi.h"

int field[SIZEY][SIZEX];	/*フィールドデータ*/
int mode = 0;				/*0:基本モード  1:ステップ実行モード  2:1画面ステップ実行モード*/
int is_auto = 0;			/*フィールド自動生成モードかどうか*/
Point start = { -1, -1 };	/*スタートの座標(エラー発見のため(-1,-1)で初期化)*/
Point goal  = { -1, -1 };	/*ゴールの座標*/

int main(int argc, char *argv[]) {
	Rslt result;

#ifdef DEBUG
	argc = 2;
	argv[1] = "map1.txt";
	mode = 2;
#endif

	/*ユーザーの入力からデータを読み込み*/
	if (Input(argc, argv) != 0)
		return -1;

	/*フィールドを表示*/
	DispField();

	result = Astar();
	/*エラー処理*/
	if (result != CLEAR) {
		DispErr(result);
		return -1;
	}

	/*探索結果を表示*/
	DispResult();

	/*ノードのメモリを開放*/
	Free();

	return 0;
}
