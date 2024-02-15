#include "Player.h"
#include"../Utility/InputControl.h"
#include"DxLib.h"

// �R���̍ő�l
#define MAX_FUEL 20000

Player::Player() : is_active(false), location(0.0f), box_size(0.0f),
angle(0.0f), speed(0.0f), hp(0.0f), fuel(0.0f),nico(0.0f),niconow(0.0f),nicomax(false),nico_se(NULL), barrier_count(0), barrier(nullptr)
{
	for (int i = 0; i < 3; i++)
	{
		image[i] = NULL;
	}
}

Player::~Player()
{

}

//����������
void Player::Initialize()
{
	is_active = true;
	location = Vector2D(320.0f, 380.0f);
	box_size = Vector2D(31.0f, 60.0f);
	angle = 0.0f;
	speed = 5.0f;
	hp = 1000;
	nico = 0;
	niconow = 0;
	nicomax = false;
	fuel = MAX_FUEL;
	barrier_count = 3;

	//�摜�̓ǂݍ���
	int result = LoadDivGraph("Resource/images/car.bmp", 3, 3, 1, 63, 120, image);
	image[3] = LoadGraph("Resource/images/car1pol.bmp");

	//nico�Q�[�WMax���̉����̓ǂݍ���
	nico_se = LoadSoundMem("Resource/se/nico.wav");

	//�G���[�`�F�b�N
	if (result == -1)
	{
		throw("Resource/images/car.bmp������܂���\n");
	}

	if (image[3] == -1)
	{
		throw("Resource/images/car1pol.bmp������܂���\n");
	}

	if (nico_se == -1)
	{
		throw("Resource/se/nico.wav������܂���\n");
	}
}

//�X�V����
void Player::Update()
{
	//����s��Ԃł���΁A���g����]������
	if (!is_active)
	{
		angle += DX_PI_F / 24.0f;
		speed = 3.0f;
		if (angle >= DX_PI_F * 4.0f)
		{
			is_active = true;
		}
		return;
	}

	// ���΃Q�[�W���g�p������Ȃ���
	if (niconow != 2) {
		//�R���̏���
		fuel -= speed;
	}

	//�ړ�����
	Movement();

	//����������
	Acceleration();

	if (InputControl::GetButtonDown(XINPUT_BUTTON_START))
	{
		is_active = false;
	}

	//�o���A����
	if (InputControl::GetButtonDown(XINPUT_BUTTON_B) && barrier_count > 0)
	{
		if (barrier == nullptr)
		{
			barrier_count--;
			barrier = new Barrier;
		}
	}

	//�o���A����������Ă�����A�X�V���s��
	if (barrier != nullptr)
	{
		//�o���A���Ԃ��o�߂������H���Ă�����A�폜����
		if (barrier->IsFinished(this->speed))
		{
			delete barrier;
			barrier = nullptr;
		}
	}

	//nico�Q�[�W�����܂��Ă���Ƃ��AY�������Ɣ���
	if (GetNicoNow() == 1)
	{
		if (InputControl::GetButtonDown(XINPUT_BUTTON_Y))
		{
			DecreaseNicoNow(1);
		}
	}
}

//�`�揈��
void Player::Draw()
{
	//�o���A����������Ă�����A�`����s��
	if (barrier != nullptr)
	{
		barrier->Draw(this->location);
	}

	//�v���C���[�摜�̕`��
	if (niconow == 2)
	{
		DrawRotaGraphF(location.x, location.y, 1.0, angle, image[3], TRUE);
	}
	else
	{
		if (hp <= 200.0f)
		{
			DrawRotaGraphF(location.x, location.y, 1.0, angle, image[2], TRUE);
		}
		else if (hp <= 500.0f)
		{
			DrawRotaGraphF(location.x, location.y, 1.0, angle, image[0], TRUE);
		}
		else
		{
			DrawRotaGraphF(location.x, location.y, 1.0, angle, image[1], TRUE);
		}
	}
}

//�I������
void Player::Finalize()
{
	//�ǂݍ��񂾉摜���폜
	DeleteGraph(image[0]);
	DeleteGraph(image[1]);
	DeleteGraph(image[2]);
	DeleteGraph(image[3]);

	//�o���A����������Ă�����A�폜����
	if (barrier != nullptr)
	{
		delete barrier;
	}
}

//��Ԑݒ菈��
void Player::SetActive(bool flg)
{
	this->is_active = flg;
}

//�̗͌�������
void Player::DecreaseHp(float value)
{
	this->hp += value;
}

//nico��������
void Player::DecreaseNico(float value)
{
	this->nico += value;
	// nico�Q�[�W��1000�ȏ�̎�
	if (nico >= 1000) {
		this->nico = 1000; // nico��1000�ɂ���B
		nicomax = true; // �g�p�o����悤�ɓ`����
		// �����ɏ΂���1��炷
		PlaySoundMem(nico_se, DX_PLAYTYPE_BACK, TRUE);
	}
	else {
		nicomax = false; // �g�p�o���Ȃ��悤�ɓ`����
	}
}

//nico�����ǂ����̊m�F(0:������  1:�ҋ@��   2:������)
void Player::DecreaseNicoNow(float value)
{
	this->niconow += value;
}


//�ʒu���擾����
Vector2D Player::GetLocation() const
{
	return this->location;
}

//�����蔻��̑傫���擾����
Vector2D Player::GetBoxSize() const
{
	return this->box_size;
}

//�����擾����
float Player::GetSpeed() const
{
	return this->speed;
}

//�R���擾����
float Player::GetFuel() const
{
	return this->fuel;
}

//�̗͎擾����
float Player::GetHp() const
{
	return this->hp;
}

//nico�擾����
float Player::GetNico() const
{
	return this->nico;
}

//nico�擾����
float Player::GetNicoNow() const
{
	return this->niconow;
}

//nico�͖��^�����H���擾
bool Player::GetNicoMax() const
{
	if (nico == 1000)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//�o���A�����擾����
int Player::GetBarriarCount() const
{
	return this->barrier_count;
}

//�o���A�͗L�����H���擾
bool Player::IsBarrier() const
{
	return (barrier != nullptr);
}

//�ړ�����
void Player::Movement()
{
	Vector2D move = Vector2D(0.0f);
	angle = 0.0f;

	//�\���ړ�����
	if (InputControl::GetButton(XINPUT_BUTTON_DPAD_LEFT))
	{
		move += Vector2D(-1.0f, 0.0f);
		angle = -DX_PI_F / 18;
	}
	if (InputControl::GetButton(XINPUT_BUTTON_DPAD_RIGHT))
	{
		move += Vector2D(1.0f, 0.0f);
		angle = DX_PI_F / 18;
	}
	if (InputControl::GetButton(XINPUT_BUTTON_DPAD_UP))
	{
		move += Vector2D( 0.0f,-1.0f);
	}
	if (InputControl::GetButton(XINPUT_BUTTON_DPAD_DOWN))
	{
		move += Vector2D(0.0f, 1.0f);
	}

	move *= speed / 3.f;
	location += move;

	//��ʊO�ɍs���Ȃ��悤�ɐ�������
	if ((location.x < box_size.x) || (location.x >= 640.0f - 180.f) ||
		(location.y < box_size.y) || (location.y >= 480.0f - box_size.y))
	{
		location -= move;
	}
}

//����������
void Player::Acceleration()
{
	//LB�{�^���������ꂽ��A��������
	if (InputControl::GetButtonDown(XINPUT_BUTTON_LEFT_SHOULDER) && speed > 3.0f)
	{
		speed -= 1.0f;
	}
	//RB�{�^���������ꂽ��A��������
	if (InputControl::GetButtonDown(XINPUT_BUTTON_RIGHT_SHOULDER) && speed < 10.0f)
	{
		speed += 1.0f;
	}
}

//�R�����₷
void Player::AddFuel(float add) {
	fuel += add;
	if (fuel >= MAX_FUEL) {
		fuel = MAX_FUEL;
	}
}