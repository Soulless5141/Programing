#pragma once

#include"../Utility/Vector2D.h"

class Item
{
private:
	int image;            //�摜�f�[�^
	Vector2D location;    //�ʒu���W
	Vector2D box_size;    //�����蔻��̑傫��
	float speed;          //����

public:
	Item();
	~Item();

	void Initialize();  //����������
	void Update(float speed);      //�X�V����
	void Draw() const;  //�`�揈��
	void Finalize();    //�I��������

	Vector2D GetLocation() const;   //�ʒu���̎擾
	Vector2D GetBoxSize() const;    //�����蔻��̑傫�����擾
};

