#include <stdio.h>

#define SIZEX	10		/*フィールドの横方向の大きさ*/
#define SIZEY	10		/*フィールドの縦方向の大きさ*/
#define WALL	-1		/*通行禁止部分*/
#define START	-2		/*スタート地点*/
#define GOAL	-3		/*ゴール地点*/
#define LOOPLIM	1000	/*A*における最大ループ回数*/

/*ノードステータス*/
typedef enum Status{ OPEN, CLOSED } Status;

/*A*の実行結果(成功したかどうか)*/
typedef enum Rslt { CLEAR, ERR1, ERR2, ERR3 } Rslt;

/*フィールド上の座標*/
typedef struct Point {
	int x;				/*0 ≦ x ≦ SIZEX-1*/
	int y;				/*0 ≦ y ≦ SIZEY-1*/
}Point;

/*ノードを表す構造体*/
typedef struct Node {
	Point point;		/*ノードの座標*/
	Status status;		/*ノードがOPENかCLOSEDか*/
	int cost;			/*実コスト*/
	int heuristic;		/*推定コスト*/
	Node *pParent;		/*親ノードへのポインタ*/
}Node;

extern int field[SIZEY][SIZEX];
extern int mode;
extern int is_auto;
extern Point start, goal;

int Read(char *filename);
void DispField(void);
void DispProc(int cnt);
void DispErr(Rslt error);
void DispResult(void);
int Input(int argc, char *argv[]);
void Help(void);
void CreateField(void);
Rslt Astar(void);
int CreateStartNode(void);
int DistanceToGoal(Point p);
int SelectNextNode(void);
int OpenAround(Node *pNode);
int OpenNode(Node *pParent, int x, int y);
void AddOpenlist(Node *p);
void CloseNode(int node_no);
int IsEnd(void);
void Free(void);
