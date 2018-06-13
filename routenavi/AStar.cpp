#include "routenavi.h"
#include "stdlib.h"
#include "conio.h"

Node *pNodelist[SIZEY][SIZEX] = {};	/*作成した座標[y][x]のノードへのポインタ*/
Node *pOpenlist[SIZEY*SIZEX];		/*オープンしているノード*/
int open_nodes = 0;					/*オープンしているノード数*/
Node wall;							/*壁を表すダミーのノード*/

/*A*アルゴリズムで経路探索*/
Rslt Astar(void)
{
	int node_no;						/*現在地のノードのオープンリストにおける添え字*/
	Node *pNode;						/*現在地のノード*/
	int cnt = 0;						/*ループカウンタ*/

	if (CreateStartNode() != 0)			/*スタート地点のノードを作成*/
		return ERR2;	

	while (1) {
		node_no = SelectNextNode();
		
		if (node_no == -1)				/*オープンリストが空だった時*/
			return ERR1;

		pNode = pOpenlist[node_no];

		if (OpenAround(pNode) != 0)		/*周りをオープンする*/
			return ERR2;

		CloseNode(node_no);				/*現在地のノードをクローズ*/

		if (mode == 1 || mode == 2) {	/*ステップ実行モードの時*/
			DispProc(cnt);
			getch();					/*いずれかのキーが押されるまで待つ*/
		}

		if (IsEnd() == 1)				/*開いているノードがゴールならば終了*/
			return CLEAR;
		
		if (cnt > LOOPLIM)				/*計算量が多すぎる場合は終了*/
			return ERR3;

		cnt++;
	}
}

/*スタート地点のノードを作成*/
int CreateStartNode(void)
{
	pNodelist[start.y][start.x] = (Node *)malloc(sizeof(Node));
	if (pNodelist[start.y][start.x] == NULL)
		return -1;

	/*初期化処理*/
	pNodelist[start.y][start.x]->point = start;
	pNodelist[start.y][start.x]->status = OPEN;
	pNodelist[start.y][start.x]->cost = 0;
	pNodelist[start.y][start.x]->heuristic = DistanceToGoal(start);
	pNodelist[start.y][start.x]->pParent = NULL;

	/*スタートノードをオープンリストに追加*/
	AddOpenlist(pNodelist[start.y][start.x]);		

	return 0;
}

/*ゴールまでの道のりを求める(斜め移動を許可しない)*/
int DistanceToGoal(Point p)
{
	int sum = 0;

	if (p.x > goal.x)
		sum += p.x - goal.x;
	else
		sum += goal.x - p.x;
	if (p.y > goal.y)
		sum += p.y - goal.y;
	else
		sum += goal.y - p.y;

	return sum;
}

/*オープンリストの中から最小スコアのノードを選ぶ*/
int SelectNextNode(void)
{
	int i;
	int min;		/*スコアの最小値*/
	int i_min;		/*最小のスコアを持つノードの添え字*/
	int score;		/*実コスト+推定コスト*/

	/*オープンリストが空だった場合*/
	if (open_nodes <= 0)
		return -1;

	/*初期値を設定*/
	min = pOpenlist[0]->cost + pOpenlist[0]->heuristic;
	i_min = 0;

	for (i = 1; i < open_nodes; i++) {
		score = pOpenlist[i]->cost + pOpenlist[i]->heuristic;
		if (score > min)
			continue;
		/*スコアが同じ場合、実コストを優先*/
		else if (score == min && pOpenlist[i]->cost >= pOpenlist[i_min]->cost)
			continue;
		min = score;
		i_min = i;
	}

	return i_min;
}

/*指定されたノードの上下左右をオープンする*/
int OpenAround(Node *pNode)
{
	int x, y;

	x = pNode->point.x;
	y = pNode->point.y;

	/*右側をオープン*/
	if (x + 1 < SIZEX) {
		if (field[y][x + 1] == WALL)
			pNodelist[y][x + 1] = &wall;
		else if (OpenNode(pNode, x + 1, y) != 0)
			return -1;
	}
	/*下側をオープン*/
	if (y + 1 < SIZEY) {
		if (field[y + 1][x] == WALL)
			pNodelist[y + 1][x] = &wall;
		else if (OpenNode(pNode, x, y + 1) != 0)
			return -1;
	}
	/*左側をオープン*/
	if (x - 1 >= 0) {
		if (field[y][x - 1] == WALL)
			pNodelist[y][x - 1] = &wall;
		else if (OpenNode(pNode, x - 1, y) != 0)
			return -1;
	}
	/*上側をオープン*/
	if (y - 1 >= 0) {
		if (field[y - 1][x] == WALL)
			pNodelist[y - 1][x] = &wall;
		else if (OpenNode(pNode, x, y - 1) != 0)
			return -1;
	}

	return 0;
}

/*指定された座標のノードをオープンする(ノードを作成する)*/
int OpenNode(Node *pParent, int x, int y)
{
	int child_cost;

	/*まだノードが作成されていなかった時*/
	if (pNodelist[y][x] == NULL) {
		/*ノードを作成*/
		pNodelist[y][x] = (Node *)malloc(sizeof(Node));
		if (pNodelist[y][x] == NULL)
			return -1;

		/*ノードを初期化*/
		pNodelist[y][x]->point = { x, y };
		pNodelist[y][x]->status = OPEN;
		pNodelist[y][x]->heuristic = DistanceToGoal({ x, y });
		pNodelist[y][x]->pParent = pParent;

		/*スタートまたはゴール地点のコストは0で計算*/
		if (field[y][x] == START || field[y][x] == GOAL)
			pNodelist[y][x]->cost = pParent->cost;
		else
			pNodelist[y][x]->cost = pParent->cost + field[y][x];
		
		AddOpenlist(pNodelist[y][x]);		/*オープンリストに追加*/
	}
	else {
		/*スタートまたはゴール地点のコストは0で計算*/
		if (field[y][x] == START || field[y][x] == GOAL)
			child_cost = pParent->cost;
		else
			child_cost = pParent->cost + field[y][x];

		/*既にあるルートよりもコストの低いルートを発見した時*/
		if (child_cost < pNodelist[y][x]->cost) {
			pNodelist[y][x]->cost = child_cost;				/*実コストを更新*/
			pNodelist[y][x]->pParent = pParent;				/*親ノードを変更*/

			/*実際には起こり得ないが、進もうとしたノードがクローズしていた時*/
			if (pNodelist[y][x]->status == CLOSED) {
				pNodelist[y][x]->status = OPEN;
				AddOpenlist(pNodelist[y][x]);
			}
		}
	}

	return 0;
}

/*ノードをオープンリストに追加*/
void AddOpenlist(Node *p)
{
	pOpenlist[open_nodes] = p;
	open_nodes++;
}

/*ノードをクローズし、オープンリストから削除*/
void CloseNode(int node_no)
{
	pOpenlist[node_no]->status = CLOSED;

	pOpenlist[node_no] = pOpenlist[open_nodes - 1];
	open_nodes--;
}

/*開いているノードの中にゴールが含まれるか*/
int IsEnd(void)
{
	int i;

	for (i = 0; i < open_nodes; i++)
		if (pOpenlist[i]->point.x == goal.x && pOpenlist[i]->point.y == goal.y)
			return 1;

	return 0;
}

/*ノードのメモリを開放*/
void Free(void)
{
	int i, j;

	for (i = 0; i < SIZEY; i++)
		for (j = 0; j < SIZEX; j++)
			if(pNodelist[i][j] != &wall)
				free(pNodelist[i][j]);
}
