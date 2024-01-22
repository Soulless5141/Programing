#pragma once

//2次元ベクトルクラス
class Vector2D
{
public:
	float x;
	float y;

public:
	Vector2D() : x(0.0f), y(0.0f)
	{

	}
	Vector2D(float scalar) : x(scalar), y(scalar)
	{

	}
	Vector2D(float mx, float my) : x(mx), y(my)
	{

	}

public:
	//演算子オーバーロード
	Vector2D& operator = (const Vector2D& location)
	{
		this->x = location.x;
		this->y = location.y;

		return *this;
	}

	const Vector2D operator + (const Vector2D& location)
	{
		float x = this->x + location.x;
		float y = this->y + location.y;

		return Vector2D(x, y);
	}
	const Vector2D operator - (const Vector2D& location)
	{
		float x = this->x - location.x;
		float y = this->y - location.y;

		return Vector2D(x, y);
	}
	const Vector2D operator * (const float& scalar)
	{
		float x = this->x * scalar;
		float y = this->y * scalar;

		return Vector2D(x, y);
	}
	const Vector2D operator * (const Vector2D& location)
	{
		float x = this->x * location.x;
		float y = this->y * location.y;

		return Vector2D(x, y);
	}

};
