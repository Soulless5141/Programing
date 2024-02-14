#include "Enemy.h"
#include"DxLib.h"

Enemy::Enemy(int type, int handle, Vector2D size) :type(type), image(handle), speed(0.0f),
location(0.0f), box_size(size)
{
	for (int i = 0; i < 16; i++)
	{
		ex_img[i] = NULL;
	}
}

Enemy::~Enemy()
{

}

//����������
void Enemy::Initialize()
{
	//�o��������X���W�p�^�[�����擾
	float random_x = (float)(GetRand(4) * 105 + 40);
	//�����ʒu�̐ݒ�
	location = Vector2D(random_x, -50.0f);
	//�����蔻��̐ݒ�
	//box_size = Vector2D(31.0f, 60.0f);
	//�����̐ݒ�
	speed = (float)((this->type + 1) * 2) + 2.f;
	//���j�f�[�^�ǂݍ���
	LoadDivGraph("Resource/images/explosion.png", 16, 8, 2, 100, 100, ex_img);
}

void Enemy::Update(float speed)
{
	////�ʒu���Ɉړ��ʂ����Z����
	//location += Vector2D(0.0f, this->speed + speed - 5);

	if (ex_flg == 1)
	{
		//�ʒu���Ɉړ��ʂ����Z����
		location += Vector2D(0.0f, 3);
		ex_num++;
		if (ex_num >= 16)
		{
			ex_flg = 2;
		}
	}
	else
	{
		//�ʒu���Ɉړ��ʂ����Z����
		location += Vector2D(0.0f, this->speed + speed - 5);
	}
}

void Enemy::Draw() const
{
	if (ex_flg == 1)
	{
		DrawRotaGraphF(location.x, location.y, 1.0, 0.0, ex_img[ex_num], TRUE); // �A�j���[�V�����i���j
	}
	else
	{
		//�G�摜�̕`��
		DrawRotaGraphF(location.x, location.y, 1.0, 0.0, image, TRUE);
	}
}

void Enemy::Finalize()
{

}

//�G�̃^�C�v���擾
int Enemy::GetType() const
{
	return type;
}

//�ʒu�����擾
Vector2D Enemy::GetLocation() const
{
	return location;
}

//�����蔻��̑傫�����擾
Vector2D Enemy::GetBoxSize() const
{
	return box_size;
}

// ���j�̃A�j���[�V��������
void Enemy::AnimEx()
{
	ex_flg = 1;
	ex_num = 0;
}

// ���j�t���O���Q�b�g����
int Enemy::GetExFlg()
{
	return this->ex_flg;
}