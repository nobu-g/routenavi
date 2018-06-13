#include "routenavi.h"

int field[SIZEY][SIZEX];	/*�t�B�[���h�f�[�^*/
int mode = 0;				/*0:��{���[�h  1:�X�e�b�v���s���[�h  2:1��ʃX�e�b�v���s���[�h*/
int is_auto = 0;			/*�t�B�[���h�����������[�h���ǂ���*/
Point start = { -1, -1 };	/*�X�^�[�g�̍��W(�G���[�����̂���(-1,-1)�ŏ�����)*/
Point goal  = { -1, -1 };	/*�S�[���̍��W*/

int main(int argc, char *argv[]) {
	Rslt result;

#ifdef DEBUG
	argc = 2;
	argv[1] = "map1.txt";
	mode = 2;
#endif

	/*���[�U�[�̓��͂���f�[�^��ǂݍ���*/
	if (Input(argc, argv) != 0)
		return -1;

	/*�t�B�[���h��\��*/
	DispField();

	result = Astar();
	/*�G���[����*/
	if (result != CLEAR) {
		DispErr(result);
		return -1;
	}

	/*�T�����ʂ�\��*/
	DispResult();

	/*�m�[�h�̃��������J��*/
	Free();

	return 0;
}
