#include <stdio.h>

#define SIZEX	10		/*�t�B�[���h�̉������̑傫��*/
#define SIZEY	10		/*�t�B�[���h�̏c�����̑傫��*/
#define WALL	-1		/*�ʍs�֎~����*/
#define START	-2		/*�X�^�[�g�n�_*/
#define GOAL	-3		/*�S�[���n�_*/
#define LOOPLIM	1000	/*A*�ɂ�����ő僋�[�v��*/

/*�m�[�h�X�e�[�^�X*/
typedef enum Status{ OPEN, CLOSED } Status;

/*A*�̎��s����(�����������ǂ���)*/
typedef enum Rslt { CLEAR, ERR1, ERR2, ERR3 } Rslt;

/*�t�B�[���h��̍��W*/
typedef struct Point {
	int x;				/*0 �� x �� SIZEX-1*/
	int y;				/*0 �� y �� SIZEY-1*/
}Point;

/*�m�[�h��\���\����*/
typedef struct Node {
	Point point;		/*�m�[�h�̍��W*/
	Status status;		/*�m�[�h��OPEN��CLOSED��*/
	int cost;			/*���R�X�g*/
	int heuristic;		/*����R�X�g*/
	Node *pParent;		/*�e�m�[�h�ւ̃|�C���^*/
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
