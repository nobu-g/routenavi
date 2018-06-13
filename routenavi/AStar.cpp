#include "routenavi.h"
#include "stdlib.h"
#include "conio.h"

Node *pNodelist[SIZEY][SIZEX] = {};	/*�쐬�������W[y][x]�̃m�[�h�ւ̃|�C���^*/
Node *pOpenlist[SIZEY*SIZEX];		/*�I�[�v�����Ă���m�[�h*/
int open_nodes = 0;					/*�I�[�v�����Ă���m�[�h��*/
Node wall;							/*�ǂ�\���_�~�[�̃m�[�h*/

/*A*�A���S���Y���Ōo�H�T��*/
Rslt Astar(void)
{
	int node_no;						/*���ݒn�̃m�[�h�̃I�[�v�����X�g�ɂ�����Y����*/
	Node *pNode;						/*���ݒn�̃m�[�h*/
	int cnt = 0;						/*���[�v�J�E���^*/

	if (CreateStartNode() != 0)			/*�X�^�[�g�n�_�̃m�[�h���쐬*/
		return ERR2;	

	while (1) {
		node_no = SelectNextNode();
		
		if (node_no == -1)				/*�I�[�v�����X�g���󂾂�����*/
			return ERR1;

		pNode = pOpenlist[node_no];

		if (OpenAround(pNode) != 0)		/*������I�[�v������*/
			return ERR2;

		CloseNode(node_no);				/*���ݒn�̃m�[�h���N���[�Y*/

		if (mode == 1 || mode == 2) {	/*�X�e�b�v���s���[�h�̎�*/
			DispProc(cnt);
			getch();					/*�����ꂩ�̃L�[���������܂ő҂�*/
		}

		if (IsEnd() == 1)				/*�J���Ă���m�[�h���S�[���Ȃ�ΏI��*/
			return CLEAR;
		
		if (cnt > LOOPLIM)				/*�v�Z�ʂ���������ꍇ�͏I��*/
			return ERR3;

		cnt++;
	}
}

/*�X�^�[�g�n�_�̃m�[�h���쐬*/
int CreateStartNode(void)
{
	pNodelist[start.y][start.x] = (Node *)malloc(sizeof(Node));
	if (pNodelist[start.y][start.x] == NULL)
		return -1;

	/*����������*/
	pNodelist[start.y][start.x]->point = start;
	pNodelist[start.y][start.x]->status = OPEN;
	pNodelist[start.y][start.x]->cost = 0;
	pNodelist[start.y][start.x]->heuristic = DistanceToGoal(start);
	pNodelist[start.y][start.x]->pParent = NULL;

	/*�X�^�[�g�m�[�h���I�[�v�����X�g�ɒǉ�*/
	AddOpenlist(pNodelist[start.y][start.x]);		

	return 0;
}

/*�S�[���܂ł̓��̂�����߂�(�΂߈ړ��������Ȃ�)*/
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

/*�I�[�v�����X�g�̒�����ŏ��X�R�A�̃m�[�h��I��*/
int SelectNextNode(void)
{
	int i;
	int min;		/*�X�R�A�̍ŏ��l*/
	int i_min;		/*�ŏ��̃X�R�A�����m�[�h�̓Y����*/
	int score;		/*���R�X�g+����R�X�g*/

	/*�I�[�v�����X�g���󂾂����ꍇ*/
	if (open_nodes <= 0)
		return -1;

	/*�����l��ݒ�*/
	min = pOpenlist[0]->cost + pOpenlist[0]->heuristic;
	i_min = 0;

	for (i = 1; i < open_nodes; i++) {
		score = pOpenlist[i]->cost + pOpenlist[i]->heuristic;
		if (score > min)
			continue;
		/*�X�R�A�������ꍇ�A���R�X�g��D��*/
		else if (score == min && pOpenlist[i]->cost >= pOpenlist[i_min]->cost)
			continue;
		min = score;
		i_min = i;
	}

	return i_min;
}

/*�w�肳�ꂽ�m�[�h�̏㉺���E���I�[�v������*/
int OpenAround(Node *pNode)
{
	int x, y;

	x = pNode->point.x;
	y = pNode->point.y;

	/*�E�����I�[�v��*/
	if (x + 1 < SIZEX) {
		if (field[y][x + 1] == WALL)
			pNodelist[y][x + 1] = &wall;
		else if (OpenNode(pNode, x + 1, y) != 0)
			return -1;
	}
	/*�������I�[�v��*/
	if (y + 1 < SIZEY) {
		if (field[y + 1][x] == WALL)
			pNodelist[y + 1][x] = &wall;
		else if (OpenNode(pNode, x, y + 1) != 0)
			return -1;
	}
	/*�������I�[�v��*/
	if (x - 1 >= 0) {
		if (field[y][x - 1] == WALL)
			pNodelist[y][x - 1] = &wall;
		else if (OpenNode(pNode, x - 1, y) != 0)
			return -1;
	}
	/*�㑤���I�[�v��*/
	if (y - 1 >= 0) {
		if (field[y - 1][x] == WALL)
			pNodelist[y - 1][x] = &wall;
		else if (OpenNode(pNode, x, y - 1) != 0)
			return -1;
	}

	return 0;
}

/*�w�肳�ꂽ���W�̃m�[�h���I�[�v������(�m�[�h���쐬����)*/
int OpenNode(Node *pParent, int x, int y)
{
	int child_cost;

	/*�܂��m�[�h���쐬����Ă��Ȃ�������*/
	if (pNodelist[y][x] == NULL) {
		/*�m�[�h���쐬*/
		pNodelist[y][x] = (Node *)malloc(sizeof(Node));
		if (pNodelist[y][x] == NULL)
			return -1;

		/*�m�[�h��������*/
		pNodelist[y][x]->point = { x, y };
		pNodelist[y][x]->status = OPEN;
		pNodelist[y][x]->heuristic = DistanceToGoal({ x, y });
		pNodelist[y][x]->pParent = pParent;

		/*�X�^�[�g�܂��̓S�[���n�_�̃R�X�g��0�Ōv�Z*/
		if (field[y][x] == START || field[y][x] == GOAL)
			pNodelist[y][x]->cost = pParent->cost;
		else
			pNodelist[y][x]->cost = pParent->cost + field[y][x];
		
		AddOpenlist(pNodelist[y][x]);		/*�I�[�v�����X�g�ɒǉ�*/
	}
	else {
		/*�X�^�[�g�܂��̓S�[���n�_�̃R�X�g��0�Ōv�Z*/
		if (field[y][x] == START || field[y][x] == GOAL)
			child_cost = pParent->cost;
		else
			child_cost = pParent->cost + field[y][x];

		/*���ɂ��郋�[�g�����R�X�g�̒Ⴂ���[�g�𔭌�������*/
		if (child_cost < pNodelist[y][x]->cost) {
			pNodelist[y][x]->cost = child_cost;				/*���R�X�g���X�V*/
			pNodelist[y][x]->pParent = pParent;				/*�e�m�[�h��ύX*/

			/*���ۂɂ͋N���蓾�Ȃ����A�i�����Ƃ����m�[�h���N���[�Y���Ă�����*/
			if (pNodelist[y][x]->status == CLOSED) {
				pNodelist[y][x]->status = OPEN;
				AddOpenlist(pNodelist[y][x]);
			}
		}
	}

	return 0;
}

/*�m�[�h���I�[�v�����X�g�ɒǉ�*/
void AddOpenlist(Node *p)
{
	pOpenlist[open_nodes] = p;
	open_nodes++;
}

/*�m�[�h���N���[�Y���A�I�[�v�����X�g����폜*/
void CloseNode(int node_no)
{
	pOpenlist[node_no]->status = CLOSED;

	pOpenlist[node_no] = pOpenlist[open_nodes - 1];
	open_nodes--;
}

/*�J���Ă���m�[�h�̒��ɃS�[�����܂܂�邩*/
int IsEnd(void)
{
	int i;

	for (i = 0; i < open_nodes; i++)
		if (pOpenlist[i]->point.x == goal.x && pOpenlist[i]->point.y == goal.y)
			return 1;

	return 0;
}

/*�m�[�h�̃��������J��*/
void Free(void)
{
	int i, j;

	for (i = 0; i < SIZEY; i++)
		for (j = 0; j < SIZEX; j++)
			if(pNodelist[i][j] != &wall)
				free(pNodelist[i][j]);
}
