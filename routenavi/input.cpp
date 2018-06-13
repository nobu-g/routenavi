#include "routenavi.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

/*ユーザーの入力を処理*/
int Input(int argc, char *argv[])
{
	switch (argc) {
	/*パラメータが足りない*/
	case 1:
		printf("ファイル名を指定してください。\n");
		return -1;
	/*オプション指定なし*/
	case 2:
		if (strcmp(argv[1], "auto") == 0) {			/*フィールド自動生成モード*/
			CreateField();							/*ランダムでフィールドを作成*/
			return 0;
		}
		else if (strcmp(argv[1], "help") == 0) {
			Help();									/*ヘルプを表示*/
			return -1;
		}
		else
			return Read(argv[1]);					/*フィールドデータの読み込み*/
	/*オプション指定あり*/
	case 3:
		if (strcmp(argv[1], "auto") == 0)			/*フィールド自動生成モード*/
			CreateField();							/*ランダムでフィールドを作成*/
		else if (Read(argv[1]) != 0)
			return -1;

		if (argv[2][0] != '-') {
			printf("オプションはハイフン'-'から始めてください。\n");
			return -1;
		}
		else if (strcmp(argv[2], "-s") == 0) {
			mode = 1;
			return 0;
		}
		else if (strcmp(argv[2], "-s1") == 0) {
			mode = 2;
			return 0;
		}
		else {
			printf("そのオプションは定義されていません。\n");
			return -1;
		}
	/*パラメータが多すぎる*/
	default:
		printf("コマンドライン引数が多すぎます。\n");
		return -1;
	}
}

/*外部ファイルからフィールドデータを読み込み、二次元配列fieldに記録*/
int Read(char *filename)
{
	FILE *fp;
	int i, j;
	char buff[SIZEX * 2 + 1];

	if ((fp = fopen(filename, "r")) == NULL) {
		printf("指定されたファイルが見つかりません。\n");
		return -1;
	}

	for (i = 0; i < SIZEY; i++) {
		fgets(buff, sizeof(buff), fp);

		for (j = 0; j < SIZEX; j++) {
			/*数字の場合を先に処理*/
			if (isdigit(buff[j * 2])) {
				if ((field[i][j] = atoi(buff + (j * 2))) == 0) {
					printf("フィールドデータに'0'は使用できません。\n");
					return -1;
				}
			}
			else
				switch (buff[j * 2]) {
				case 'N':
					field[i][j] = WALL;
					break;
				case 'S':
					field[i][j] = START;
					start = { j, i };
					break;
				case 'G':
					field[i][j] = GOAL;
					goal = { j, i };
					break;
				default:
					printf("マップファイルに予期しないデータ'%c'が含まれます。\n", buff[j * 2]);
					return -1;
				}
		}
	}
	if (start.x == -1 || goal.x == -1) {
		printf("スタート地点またはゴール地点が見つかりません。\n");
		return -1;
	}

	fclose(fp);
	return 0;
}


/*ランダムでフィールドを作成する*/
/*壁のみ出現確率2倍!*/
void CreateField(void)
{
	int i, j;
	int r;		/*乱数:0～10*/

	is_auto = 1;

	srand(time(NULL));

	for (i = 0; i < SIZEY; i++)
		for (j = 0; j < SIZEX; j++) {
			r = rand() % 11;

			if (r == 0 || r == 10)
				field[i][j] = WALL;
			else
				field[i][j] = r;
		}

	/*スタート地点を作成*/
	start.x = rand() % SIZEX;
	start.y = rand() % SIZEY;
	field[start.y][start.x] = START;

	/*ゴール地点を作成*/
	do {
		goal.x = rand() % SIZEX;
		goal.y = rand() % SIZEY;
	} while (goal.x == start.x && goal.y == start.y);
	field[goal.y][goal.x] = GOAL;
}
