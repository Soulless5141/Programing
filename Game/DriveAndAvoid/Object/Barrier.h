#pragma once

#include"../Utility/Vector2D.h"
class Barrier
{
private:
	int image;         //バリア画像
	float life_span;   //バリアの寿命

public:
	Barrier();
	~Barrier();

	void Draw(const Vector2D& location);   //描画処理
	bool IsFinished(float speed);          //寿命が尽きたか？

};

