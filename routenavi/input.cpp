#include "routenavi.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

/*���[�U�[�̓��͂�����*/
int Input(int argc, char *argv[])
{
	switch (argc) {
	/*�p�����[�^������Ȃ�*/
	case 1:
		printf("�t�@�C�������w�肵�Ă��������B\n");
		return -1;
	/*�I�v�V�����w��Ȃ�*/
	case 2:
		if (strcmp(argv[1], "auto") == 0) {			/*�t�B�[���h�����������[�h*/
			CreateField();							/*�����_���Ńt�B�[���h���쐬*/
			return 0;
		}
		else if (strcmp(argv[1], "help") == 0) {
			Help();									/*�w���v��\��*/
			return -1;
		}
		else
			return Read(argv[1]);					/*�t�B�[���h�f�[�^�̓ǂݍ���*/
	/*�I�v�V�����w�肠��*/
	case 3:
		if (strcmp(argv[1], "auto") == 0)			/*�t�B�[���h�����������[�h*/
			CreateField();							/*�����_���Ńt�B�[���h���쐬*/
		else if (Read(argv[1]) != 0)
			return -1;

		if (argv[2][0] != '-') {
			printf("�I�v�V�����̓n�C�t��'-'����n�߂Ă��������B\n");
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
			printf("���̃I�v�V�����͒�`����Ă��܂���B\n");
			return -1;
		}
	/*�p�����[�^����������*/
	default:
		printf("�R�}���h���C���������������܂��B\n");
		return -1;
	}
}

/*�O���t�@�C������t�B�[���h�f�[�^��ǂݍ��݁A�񎟌��z��field�ɋL�^*/
int Read(char *filename)
{
	FILE *fp;
	int i, j;
	char buff[SIZEX * 2 + 1];

	if ((fp = fopen(filename, "r")) == NULL) {
		printf("�w�肳�ꂽ�t�@�C����������܂���B\n");
		return -1;
	}

	for (i = 0; i < SIZEY; i++) {
		fgets(buff, sizeof(buff), fp);

		for (j = 0; j < SIZEX; j++) {
			/*�����̏ꍇ���ɏ���*/
			if (isdigit(buff[j * 2])) {
				if ((field[i][j] = atoi(buff + (j * 2))) == 0) {
					printf("�t�B�[���h�f�[�^��'0'�͎g�p�ł��܂���B\n");
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
					printf("�}�b�v�t�@�C���ɗ\�����Ȃ��f�[�^'%c'���܂܂�܂��B\n", buff[j * 2]);
					return -1;
				}
		}
	}
	if (start.x == -1 || goal.x == -1) {
		printf("�X�^�[�g�n�_�܂��̓S�[���n�_��������܂���B\n");
		return -1;
	}

	fclose(fp);
	return 0;
}


/*�����_���Ńt�B�[���h���쐬����*/
/*�ǂ̂ݏo���m��2�{!*/
void CreateField(void)
{
	int i, j;
	int r;		/*����:0�`10*/

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

	/*�X�^�[�g�n�_���쐬*/
	start.x = rand() % SIZEX;
	start.y = rand() % SIZEY;
	field[start.y][start.x] = START;

	/*�S�[���n�_���쐬*/
	do {
		goal.x = rand() % SIZEX;
		goal.y = rand() % SIZEY;
	} while (goal.x == start.x && goal.y == start.y);
	field[goal.y][goal.x] = GOAL;
}
