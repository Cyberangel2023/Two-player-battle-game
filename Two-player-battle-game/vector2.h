#ifndef _VECTOR2_H_
#define _VECTOR2_H_

#include <cmath>

class Vector2
{
public:
	float x = 0;
	float y = 0;

public:
	Vector2() = default;
	~Vector2() = default;

	Vector2(float x, float y)
		: x(x), y(y) { }

	// 二维向量的加减乘除
	Vector2 operator+(const Vector2& vec) const
	{
		return Vector2(x + vec.x, y + vec.y);
	}

	void operator+=(const Vector2& vec)
	{
		x += vec.x, y += vec.y;
	}

	void operator-=(const Vector2& vec)
	{
		x -= vec.x, y -= vec.y;
	}

	Vector2 operator-(const Vector2& vec) const
	{
		return Vector2(x - vec.x, y - vec.y);
	}

	float operator*(const Vector2& vec)
	{
		return x * vec.x + y * vec.y;
	}

	Vector2 operator*(float val) const
	{
		return Vector2(x * val, y * val);
	}

	void operator*=(float val)
	{
		x *= val, y *= val;
	}

	// 获取长度
	float length()
	{
		return sqrt(x * x + y * y);
	}

	// 二维向量的单一元素改变
	Vector2 add_x(float val)
	{
		return Vector2(x + val, y);
	}

	Vector2 add_y(float val)
	{
		return Vector2(x, y + val);
	}

	// 计算标准长度
	Vector2 normalize()
	{
		float len = length();

		if (len == 0)
			return Vector2(0, 0);

		return Vector2(x / len, y / len);
	}
};

#endif // !_VECTOR2_H_