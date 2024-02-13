#include "Item.h"
#include "DxLib.h"

Item::Item() {

}

Item::~Item() {

}

void Item::Initialize() {
	//�o��������X���W�p�^�[�����擾
	float random_x = (float)(GetRand(4) * 105 + 40);
	//�����ʒu�̐ݒ�
	location = Vector2D(random_x, -50.0f);
	box_size = Vector2D(20.0f, 40.0f);
	angle = 0.0f;
	speed = 6.0f;

	//�摜�̓ǂݍ���
	image = LoadGraph("Resource/images/kan.bmp");

	//�G���[�`�F�b�N
	if (image == -1)
	{
		throw("Resource/images/kan.bmp������܂���\n");
	}
}

void Item::Update(float speed) {
	//�ʒu���Ɉړ��ʂ����Z����
	location += Vector2D(0.0f, this->speed + speed - 6);
}

void Item::Draw() const {
	//�A�C�e���摜�̕`��
	DrawRotaGraphF(location.x, location.y, 1.0, 0.0, image, TRUE);
}

void Item::Finalize() {

}

//�ʒu�����擾
Vector2D Item::GetLocation() const
{
	return location;
}

//�����蔻��̑傫�����擾
Vector2D Item::GetBoxSize() const
{
	return box_size;
}