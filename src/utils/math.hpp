/*******************************************************************
* @file   math.cpp
* @brief  Vector, Matrix, and other math related functions.
* @author SAR Team
*********************************************************************/

#pragma once

#include <algorithm>
#include <random>
#include <cmath>

#define M_PI 3.14159265358979323846
#define M_PI_F (static_cast<float>(M_PI))
#define M_PI_D2 1.57079632679489661923  // pi/2
#define TAU 6.28318530717958647692
#define RAD_2DEG(x) (static_cast<float>(x) * static_cast<float>(180.f / M_PI_F))
#define DEG_2RAD(x) (static_cast<float>(x) * static_cast<float>(M_PI_F / 180.f))

struct Vector
{
	float x, y, z;
	Vector() : x(0), y(0), z(0) {}
	Vector(const float x_, const float y_, const float z_ = 0) : x(x_), y(y_), z(z_) {}

	float SquaredLength() const { return x * x + y * y + z * z; }
	float Length() const { return std::sqrt(x * x + y * y + z * z); }
	float Length2D() const { return std::sqrt(x * x + y * y); }
	float Dot(const Vector& vOther) const { return Vector::DotProduct(*this, vOther); }
	static float DotProduct(const Vector& a, const Vector& b) { return a.x * b.x + a.y * b.y + a.z * b.z;}

	Vector Cross(const Vector& v) const
	{
		Vector out;
		out.x = this->y * v.z - this->z * v.y;
		out.y = this->z * v.x - this->x * v.z;
		out.z = this->x * v.y - this->y * v.x;
		return out;
	}

	Vector Normalize() const { return *this / this->Length(); }

	Vector operator*(const float flt) const
	{
		Vector result;
		result.x = this->x * flt;
		result.y = this->y * flt;
		result.z = this->z * flt;
		return result;
	}

	Vector& operator*=(const float flt)
	{
		this->x = this->x * flt;
		this->y = this->y * flt;
		this->z = this->z * flt;
		return *this;
	}

	Vector operator/(const float flt) const
	{
		return *this * (1 / flt);
	}

	Vector& operator+=(const Vector& vec)
	{
		this->x = this->x + vec.x;
		this->y = this->y + vec.y;
		this->z = this->z + vec.z;
		return *this;
	}

	Vector operator+(const Vector vec) const
	{
		Vector result;
		result.x = this->x + vec.x;
		result.y = this->y + vec.y;
		result.z = this->z + vec.z;
		return result;
	}

	Vector& operator-=(const Vector& vec)
	{
		this->x -= vec.x;
		this->y -= vec.y;
		this->z -= vec.z;
		return *this;
	}

	Vector operator-(const Vector vec) const
	{
		Vector result;
		result.x = this->x - vec.x;
		result.y = this->y - vec.y;
		result.z = this->z - vec.z;
		return result;
	}

	Vector operator-() const { return Vector{0, 0, 0} - *this; }
	float& operator[](const int i) { return reinterpret_cast<float*>(this)[i]; }
	float operator[](const int i) const { return ((float*)this)[i]; }
	bool operator==(const Vector vec) const { return x == vec.x && y == vec.y && z == vec.z; }
	bool operator!=(const Vector vec) const { return !(*this == vec); }
};

template <typename T>
struct Vector2
{
	T x, y;
	Vector2() : x(0), y(0) {}
	Vector2(const T& x, const T& y) : x(x), y(y) {}

	float Length() const { return sqrt(x * x + y * y); }
	T Dot(const Vector2& other) const { return x * other.x + y * other.y; }
	T Cross(const Vector2& other) const { return x * other.y - y * other.x; }
	Vector2 Normalize() { return *this / this->Length(); }

	Vector2 operator+(const Vector2& other) const
	{
		Vector2 result;
		result.x = this->x + other.x;
		result.y = this->y + other.y;
		return result;
	}

	Vector2 operator+=(const Vector2& other)
	{
		this->x += other.x;
		this->y += other.y;
		return *this;
	}

	Vector2 operator-(const Vector2& other) const
	{
		Vector2 result;
		result.x = this->x - other.x;
		result.y = this->y - other.y;
		return result;
	}

	Vector2 operator-=(const Vector2& other)
	{
		this->x -= other.x;
		this->y -= other.y;
		return *this;
	}

	Vector2 operator*(float val) const
	{
		Vector2 result;
		result.x = this->x * val;
		result.y = this->y * val;
		return result;
	}

	Vector2 operator*=(float val)
	{
		this->x *= val;
		this->y *= val;
		return *this;
	}

	Vector2 operator/(float val) const
	{
		Vector2 result;
		result.x = x / val;
		result.y = y / val;
		return result;
	}

	Vector2 operator/=(float val)
	{
		this->x /= val;
		this->y /= val;
		return *this;
	}
};

template <typename T>
struct Bounds
{
	Vector2<T> vBegin, vEnd;

	Bounds()
		: vBegin(Vector2<T>(static_cast<T>(0), static_cast<T>(0)))
		, vEnd(Vector2<T>(static_cast<T>(0), static_cast<T>(0))) {}

	Bounds(const T& x0, const T& y0, const T& x1, const T& y1)
		: vBegin(Vector2<T>(x0, y0))
		, vEnd(Vector2<T>(x1, y1)) {}

	Bounds(const Vector2<T>& v0, const Vector2<T>& v1)
		: vBegin(v0)
		, vEnd(v1) {}

	Bounds Scale(const Vector2<T>& stretchPoint, const float amp)
	{
		this->vBegin = stretchPoint - (stretchPoint - this->vBegin) * amp;
		this->vEnd = stretchPoint + (this->vEnd - stretchPoint) * amp;
		return *this;
	}

	Bounds Scale(const float amp)
	{
		Scale(this->vBegin, amp);
		return *this;
	}

	Bounds operator+(const Bounds& other) const
	{
		Bounds<T> result;
		result.vBegin = this->vBegin + other.vBegin;
		result.vEnd = this->vEnd + other.vEnd;
		return result;
	}

	Bounds operator+=(const Bounds& other)
	{
		this->vBegin += other.vBegin;
		this->vEnd += other.vEnd;
		return *this;
	}

	Bounds operator-(const Bounds& other) const
	{
		Bounds<T> result;
		result.vBegin = this->vBegin - other.vBegin;
		result.vEnd = this->vEnd - other.vEnd;
		return result;
	}

	Bounds operator-=(const Bounds& other)
	{
		this->vBegin -= other.vBegin;
		this->vEnd -= other.vEnd;
		return *this;
	}
};


struct QAngle
{
	float x, y, z;
};

inline QAngle VectorToQAngle(const Vector& v)
{
	return {v.x, v.y, v.z};
}

inline Vector QAngleToVector(const QAngle& a)
{
	return {a.x, a.y, a.z};
}

struct VectorAligned : public Vector
{
	float w;

	VectorAligned() : w(0) {}
	VectorAligned(const float x_, const float y_, const float z_) : Vector(x_, y_, z_), w(0) {}
	VectorAligned(const Vector& v) : Vector(v.x, v.y, v.z), w(0) {}
};

class Matrix
{
public:
	Matrix(int rows, int cols, double init);

	double& operator()(const unsigned int x, const unsigned int y) { return mat[x][y]; }
	const double& operator()(const unsigned int x, const unsigned int y) const { return mat[x][y]; }

	Matrix& operator=(const Matrix& rhs);
	Matrix operator+(const Matrix& rhs) const;
	Matrix& operator+=(const Matrix& rhs);
	Matrix operator*(const Matrix& rhs) const;
	Matrix& operator*=(const Matrix& rhs);
	Vector operator*(const Vector& rhs) const;
	inline Vector operator*=(const Vector& rhs) const;

	void Transpose();

	void Print() const;

	int GetRows() const { return this->rows; }
	int GetCols() const { return this->cols; }

private:
	std::vector<std::vector<double>> mat;

	int rows;
	int cols;
};

struct Matrix3X4
{
	float matrix[3][4];

	Vector VectorTransform(const Vector& v) const
	{
		const float(*m)[4] = matrix;
		return Vector{
			m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3],
			m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3],
			m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3],
		};
	}
};

struct VMatrix
{
	float matrix[4][4];

	Vector VectorTransform(const Vector& vVec) const
	{
		return {
			this->matrix[0][0] * vVec.x +this->matrix[0][1] * vVec.y + this->matrix[0][2] * vVec.z,
			this->matrix[1][0] * vVec.x +this->matrix[1][1] * vVec.y + this->matrix[1][2] * vVec.z,
			this->matrix[2][0] * vVec.x +this->matrix[2][1] * vVec.y + this->matrix[2][2] * vVec.z};
	}

	Vector PointTransform(const Vector& vVec) const
	{
		return {
			this->matrix[0][0] * vVec.x + this->matrix[0][1] * vVec.y + this->matrix[0][2] * vVec.z + this->matrix[0][3],
			this->matrix[1][0] * vVec.x + this->matrix[1][1] * vVec.y + this->matrix[1][2] * vVec.z + this->matrix[1][3],
			this->matrix[2][0] * vVec.x + this->matrix[2][1] * vVec.y + this->matrix[2][2] * vVec.z + this->matrix[2][3]};
	}
};

namespace Math
{
	void	SinCos(float radians, float* sine, float* cosine);
	float	AngleNormalize(float angle);
	float	VectorNormalize(Vector& vec);
	void	VectorAdd(const Vector& a, const Vector& b, Vector& c);
	void	AngleVectors(const QAngle& angles, Vector* forward);
	void	AngleVectors(const QAngle& angles, Vector* forward, Vector* right, Vector* up);
	Matrix	AngleMatrix(const QAngle& angles);
	void	VectorAngles(const Vector& forward, Vector& pseudoup, QAngle* angles);
	void	VectorScale(Vector const& src, float b, Vector& dst);
	void	VectorCopy(const Vector& src, Vector& dst);
	float	RandomNumber(const float& min, const float& max);
	int		RandomNumber(const int& min, const int& max);
	double	Distance(const Vector& a, const Vector& b);
	double	Distance(const QAngle& a, const QAngle& b);
	void	Lerp(const Vector& oldPos, const Vector& newPos, float time, Vector& outPut);
	float	LerpAngle(float oldAngle, float newAngle, float time);
	void	LerpAngles(const QAngle& oldAngles, const QAngle& newAngles, float time, QAngle& outPut);
}

inline void Math::SinCos(float radians, float* sine, float* cosine)
{
#ifdef _WIN32
	_asm {
        fld DWORD PTR[radians]
        fsincos

        mov edx, DWORD PTR[cosine]
        mov eax, DWORD PTR[sine]

        fstp DWORD PTR[edx]
        fstp DWORD PTR[eax]
	}
#else
	double __cosr, __sinr;
	__asm("fsincos"
	      : "=t"(__cosr), "=u"(__sinr)
	      : "0"(radians));

	*sine = __sinr;
	*cosine = __cosr;
#endif
}

inline void Math::VectorAdd(const Vector& a, const Vector& b, Vector& c)
{
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
}
inline void Math::VectorScale(Vector const& src, float b, Vector& dst)
{
	dst.x = src.x * b;
	dst.y = src.y * b;
	dst.z = src.z * b;
}
inline void Math::VectorCopy(const Vector& src, Vector& dst)
{
	dst.x = src.x;
	dst.y = src.y;
	dst.z = src.z;
}
inline double Math::Distance(const Vector& a, const Vector& b)
{
	return std::sqrt(
		std::pow(b.x - a.x, 2)
		 + std::pow(b.y - a.y, 2)
		 + std::pow(b.z - a.z, 2)
	);
}
inline double Math::Distance(const QAngle& a, const QAngle& b)
{
	return std::sqrt(
		std::pow(b.x - a.x, 2)
		 + std::pow(b.y - a.y, 2)
		 + std::pow(b.z - a.z, 2)
	);
}

inline void Math::Lerp(const Vector& oldPos, const Vector& newPos, float time, Vector& outPut)
{
	time = std::min<float>(time, 1);
	time = std::max<float>(time, 0);

	outPut.x = (1 - time) * oldPos.x + time * newPos.x;
	outPut.y = (1 - time) * oldPos.y + time * newPos.y;
	outPut.z = (1 - time) * oldPos.z + time * newPos.z;
}

inline float Math::LerpAngle(const float oldAngle, const float newAngle, float time)
{
	time = std::min<float>(time, 1);
	time = std::max<float>(time, 0);

	auto delta = (newAngle - oldAngle);
	if (delta < -180.0f)
		delta += 360.0f;
	if (delta > 180.0f)
		delta -= 360.0f;

	auto result = oldAngle + delta * time;
	if (result < -180.0f)
		result += 360.0f;
	if (result > 180.0f)
		result -= 360.0f;

	return result;
}

inline void Math::LerpAngles(const QAngle& oldAngles, const QAngle& newAngles, const float time, QAngle& outPut)
{
	outPut.x = Math::LerpAngle(oldAngles.x, newAngles.x, time);
	outPut.y = Math::LerpAngle(oldAngles.y, newAngles.y, time);
	outPut.z = Math::LerpAngle(oldAngles.z, newAngles.z, time);
}
