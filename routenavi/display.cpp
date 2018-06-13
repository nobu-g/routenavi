#include "routenavi.h"

extern Node wall;
extern Node *pNodelist[][SIZEX];

/*�w���v��\��*/
void Help(void)
{
	printf("\
�ŒZ�o�H��T�����܂��B\n\
\n\
�T������}�b�v�t�@�C�����ƃI�v�V��������͂��Ă��������B\n\
[routenavi.exe] [�t�@�C����] [�I�v�V����]\n\
\n\
�t�@�C����\n\
�}�b�v�t�@�C����      ���̃t�@�C���ɂ���}�b�v��T������\n\
auto                  �t�B�[���h�����������[�h : �����_���Ƀ}�b�v���쐬����\n\
help                  �w���v��\������\n\
\n\
�I�v�V����\n\
�Ȃ�                  ��{���[�h : �T��������̌��ʂ݂̂�����\n\
-s                    �X�e�b�v���s���[�h : �T���̗����Enter�L�[�ŏ��ɕ\������\n\
-s1                   1��ʃX�e�b�v���s���[�h : 1��ʂ̒��ŃX�e�b�v���s���[�h���s��\n");
}

/*�t�B�[���h�ƃ��[�h��\��*/
void DispField()
{
	int i, j;

	if(is_auto == 1)
		printf("�t�B�[���h����������\n");
	switch (mode) {
	case 0:		printf("�ʏ탂�[�h\n\n");					break;
	case 1:		printf("�X�e�b�v���s���[�h\n\n");			break;
	case 2:		printf("1��ʃX�e�b�v���s���[�h\n\n");		break;
	default:	printf("�G���[:����`�̎��s���[�h\n\n");	break;
	}

	printf("�t�B�[���h\n");
	for (i = 0; i < SIZEY; i++) {
		for (j = 0; j < SIZEX; j++) {
			switch (field[i][j]) {
			case 1:		printf("\x1b[36m[1]");	break;	/*�V�A��*/
			case 2:		printf("\x1b[32m[2]");	break;	/*��*/
			case 3:		printf("\x1b[32m[3]");	break;	/*��*/
			case 4:		printf("\x1b[33m[4]");	break;	/*��*/
			case 5:		printf("\x1b[33m[5]");	break;	/*��*/
			case 6:		printf("\x1b[33m[6]");	break;	/*��*/
			case 7:		printf("\x1b[31m[7]");	break;	/*��*/
			case 8:		printf("\x1b[31m[8]");	break;	/*��*/
			case 9:		printf("\x1b[31m[9]");	break;	/*��*/
			case WALL:	printf("\x1b[37m[x]");	break;	/*��*/
			case START:	printf("\x1b[35m[S]");	break;	/*�}�[���^*/
			case GOAL:	printf("\x1b[35m[G]");	break;	/*�}�[���^*/
			default:	printf("\x1b[0m[?]");	break;	/*�W��*/
			}
		}
		printf("\n");
	}
	printf("\x1b[0m\n");
}

/*�T���ߒ���\��*/
void DispProc(int cnt)
{
	int i, j;

	/*1��ʃX�e�b�v���s���[�h�̎��J�[�\����߂�*/
	if (mode == 2 && cnt > 0)
		printf("\r\x1b[%dA", SIZEY + 2);

	if (mode != 0)
		printf("�X�e�b�v%d\n", cnt + 1);
	else
		printf("�ړ��R�X�g\n");

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

/*�T�����̃G���[��\��*/
void DispErr(Rslt error)
{
	switch (error) {
	case ERR1:	printf("�T���Ɏ��s���܂����B\n");						break;
	case ERR2:	printf("�q�[�v�̈悩��̃������m�ۂɎ��s���܂����B\n");	break;
	case ERR3:	printf("�v�Z�ʂ��������܂��B\n");						break;
	default:	printf("����`�̃G���[\n");								break;
	}
}

/*�T�����ʂ�\��*/
void DispResult()
{
	int i, j;
	Node *pNode = pNodelist[goal.y][goal.x]->pParent;	/*�S�[�����O�̃|�C���^�ŏ�����*/
	int route[SIZEY][SIZEX] = {};						/*�ʂ郋�[�g��1����ȊO��0*/

	/*�S�[�������瓹��H���Ă���*/
	while (pNode) {
		route[pNode->point.y][pNode->point.x] = 1;
		pNode = pNode->pParent;
	}

	/*�ʏ탂�[�h�̎��A�I�[�v�������e�m�[�h�ւ̃R�X�g��\��*/
	if (mode == 0)
		DispProc(0);

	printf("�T������\n");
	/*���[�g��̃Z���̓}�[���^�ŕ\��*/
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
