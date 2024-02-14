#pragma once

#include"../Utility/Vector2D.h"

class Enemy
{
private:
	int type;           //�^�C�v
	int image;          //�摜
	float speed;        //����
	int ex_img[16];			// ���j�摜������f�[�^
	int ex_flg;			// ���j�t���O
	int ex_num;			// �A�j���[�V�����摜��i�߂�ϐ�
	int ex_img_check;	// ���ɉ摜�����邩�m�F����ϐ�
	int ex_se;			// se�����ϐ�
	Vector2D location;   //�ʒu���
	Vector2D box_size;   //�����蔻��̑傫��

public:
	Enemy(int type, int handle, Vector2D size);
	~Enemy();

	void Initialize();          //����������
	void Update(float speed);   //�X�V����
	void Draw() const;          //�摜����
	void Finalize();            //�I��������
	void AnimEx();				//���j�A�j���[�V�����֐�
	int GetExFlg();			//���j�t���O���Q�b�g����

	int GetType() const;            //�^�C�v�擾
	bool IsPlayExplosion();
	Vector2D GetLocation() const;   //�ʒu���̎擾
	Vector2D GetBoxSize() const;    //�����蔻��̑傫�����擾
};

