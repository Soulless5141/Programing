#pragma once

#include"../Utility/Vector2D.h"
#include"Barrier.h"

class Player
{
private:
	bool is_active;       //�L����Ԃ��H
	int image;            //�摜�f�[�^
	Vector2D location;    //�ʒu���W
	Vector2D box_size;    //�����蔻��̑傫��
	float angle;          //�p�x
	float speed;          //����
	float hp;             //�̗�
	float fuel;              //�R��
	float sp;            //sp�Q�[�W
	float spnow;
	int barrier_count;    //�o���A�̖���
	Barrier* barrier;     //�o���A

public:
	Player();
	~Player();

	void Initialize();  //����������
	void Update();      //�X�V����
	void Draw();        //�`�揈��
	void Finalize();    //�I��������

public:
	void SetActive(bool flg);    //�L���t���O�ݒ�
	void DecreaseHp(float value);  //�̗͌�������
	void DecreaseSp(float value);  //SP��������
	void DecreaseSpNow(float value);  //SP�����ǂ����̊m�F(0:������  1:�ҋ@��   2:������)
	Vector2D GetLocation() const;  //�ʒu���W�擾����
	Vector2D GetBoxSize() const;   //�����蔻��̑傫���擾
	float GetSpeed() const;          //�����擾����
	float GetFuel() const;           //�R���擾
	float GetHp() const;           //�̗͎擾
	float GetSp() const;           //SP�擾
	float GetSpNow() const;        //SP��Ԏ擾
	int GetBarriarCount() const;   //�o���A�̖����擾
	bool IsBarrier() const;          //�o���A�L�����H���擾
	void AddFuel(float add);		//�R���𑝂₷����
	
private:
	void Movement();      //�ړ�����
	void Acceleration();   //��������
};

