#include "routenavi.h"

extern Node wall;
extern Node *pNodelist[][SIZEX];

/*ヘルプを表示*/
void Help(void)
{
	printf("\
最短経路を探索します。\n\
\n\
探索するマップファイル名とオプションを入力してください。\n\
[routenavi.exe] [ファイル名] [オプション]\n\
\n\
ファイル名\n\
マップファイル名      そのファイルにあるマップを探索する\n\
auto                  フィールド自動生成モード : ランダムにマップを作成する\n\
help                  ヘルプを表示する\n\
\n\
オプション\n\
なし                  基本モード : 探索完了後の結果のみを示す\n\
-s                    ステップ実行モード : 探索の流れをEnterキーで順に表示する\n\
-s1                   1画面ステップ実行モード : 1画面の中でステップ実行モードを行う\n");
}

/*フィールドとモードを表示*/
void DispField()
{
	int i, j;

	if(is_auto == 1)
		printf("フィールドを自動生成\n");
	switch (mode) {
	case 0:		printf("通常モード\n\n");					break;
	case 1:		printf("ステップ実行モード\n\n");			break;
	case 2:		printf("1画面ステップ実行モード\n\n");		break;
	default:	printf("エラー:未定義の実行モード\n\n");	break;
	}

	printf("フィールド\n");
	for (i = 0; i < SIZEY; i++) {
		for (j = 0; j < SIZEX; j++) {
			switch (field[i][j]) {
			case 1:		printf("\x1b[36m[1]");	break;	/*シアン*/
			case 2:		printf("\x1b[32m[2]");	break;	/*緑*/
			case 3:		printf("\x1b[32m[3]");	break;	/*緑*/
			case 4:		printf("\x1b[33m[4]");	break;	/*黄*/
			case 5:		printf("\x1b[33m[5]");	break;	/*黄*/
			case 6:		printf("\x1b[33m[6]");	break;	/*黄*/
			case 7:		printf("\x1b[31m[7]");	break;	/*赤*/
			case 8:		printf("\x1b[31m[8]");	break;	/*赤*/
			case 9:		printf("\x1b[31m[9]");	break;	/*赤*/
			case WALL:	printf("\x1b[37m[x]");	break;	/*白*/
			case START:	printf("\x1b[35m[S]");	break;	/*マゼンタ*/
			case GOAL:	printf("\x1b[35m[G]");	break;	/*マゼンタ*/
			default:	printf("\x1b[0m[?]");	break;	/*標準*/
			}
		}
		printf("\n");
	}
	printf("\x1b[0m\n");
}

/*探索過程を表示*/
void DispProc(int cnt)
{
	int i, j;

	/*1画面ステップ実行モードの時カーソルを戻す*/
	if (mode == 2 && cnt > 0)
		printf("\r\x1b[%dA", SIZEY + 2);

	if (mode != 0)
		printf("ステップ%d\n", cnt + 1);
	else
		printf("移動コスト\n");

	for (i = 0; i < SIZEY; i++) {
		for (j = 0; j < SIZEX; j++) {
			if (pNodelist[i][j] == &wall)
				printf("\x1b[36m[-1]\x1b[0m");
			else if (pNodelist[i][j] == NULL)
				printf("[  ]");
			else {
				printf("\x1b[%dm", (pNodelist[i][j]->status == OPEN) ? 33 : 36);
				printf("[%2d]", pNodelist[i][j]->cost);
				printf("\x1b[0m");
			}
		}
		printf("\n");
	}
	printf("\n");
}

/*探索時のエラーを表示*/
void DispErr(Rslt error)
{
	switch (error) {
	case ERR1:	printf("探索に失敗しました。\n");						break;
	case ERR2:	printf("ヒープ領域からのメモリ確保に失敗しました。\n");	break;
	case ERR3:	printf("計算量が多すぎます。\n");						break;
	default:	printf("未定義のエラー\n");								break;
	}
}

/*探索結果を表示*/
void DispResult()
{
	int i, j;
	Node *pNode = pNodelist[goal.y][goal.x]->pParent;	/*ゴール直前のポインタで初期化*/
	int route[SIZEY][SIZEX] = {};						/*通るルートは1それ以外は0*/

	/*ゴール側から道を辿っていく*/
	while (pNode) {
		route[pNode->point.y][pNode->point.x] = 1;
		pNode = pNode->pParent;
	}

	/*通常モードの時、オープンした各ノードへのコストを表示*/
	if (mode == 0)
		DispProc(0);

	printf("探索完了\n");
	/*ルート上のセルはマゼンタで表示*/
	for (i = 0; i < SIZEY; i++) {
		for (j = 0; j < SIZEX; j++) {
			switch (field[i][j]) {
			case 1:		printf("\x1b[%dm[1]", route[i][j] ? 35 : 36);	break;
			case 2:		printf("\x1b[%dm[2]", route[i][j] ? 35 : 32);	break;
			case 3:		printf("\x1b[%dm[3]", route[i][j] ? 35 : 32);	break;
			case 4:		printf("\x1b[%dm[4]", route[i][j] ? 35 : 33);	break;
			case 5:		printf("\x1b[%dm[5]", route[i][j] ? 35 : 33);	break;
			case 6:		printf("\x1b[%dm[6]", route[i][j] ? 35 : 33);	break;
			case 7:		printf("\x1b[%dm[7]", route[i][j] ? 35 : 31);	break;
			case 8:		printf("\x1b[%dm[8]", route[i][j] ? 35 : 31);	break;
			case 9:		printf("\x1b[%dm[9]", route[i][j] ? 35 : 31);	break;
			case WALL:	printf("\x1b[37m[x]");	break;
			case START:	printf("\x1b[35m[S]");	break;
			case GOAL:	printf("\x1b[35m[G]");	break;
			default:	printf("\x1b[0m[?]");	break;
			}
		}
		printf("\n");
	}
	printf("\x1b[0m\n");
}
