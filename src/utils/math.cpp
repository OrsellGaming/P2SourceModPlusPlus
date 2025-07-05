/*******************************************************************
* @file   math.cpp
* @brief  Vector, Matrix, and other math related functions.
* @author SAR Team
*********************************************************************/

#include "math.hpp"

#include <cmath>
#include <cfloat>
#include <random>

#include "loggingsystem.hpp"

float Math::AngleNormalize(float angle)
{
	angle = fmodf(angle, 360.0f);
	if (angle > 180)
		angle -= 360;

	if (angle < -180)
		angle += 360;

	return angle;
}

float Math::VectorNormalize(Vector& vec)
{
	const auto radius = sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	const auto iRadius = 1.f / (radius + FLT_EPSILON);

	vec.x *= iRadius;
	vec.y *= iRadius;
	vec.z *= iRadius;

	return radius;
}

void Math::AngleVectors(const QAngle& angles, Vector* forward)
{
	float sp, sy, cp, cy;

	Math::SinCos(DEG_2RAD(angles.y), &sy, &cy);
	Math::SinCos(DEG_2RAD(angles.x), &sp, &cp);

	forward->x = cp * cy;
	forward->y = cp * sy;
	forward->z = -sp;
}

void Math::AngleVectors(const QAngle& angles, Vector* forward, Vector* right, Vector* up)
{
	float sr, sp, sy, cr, cp, cy;

	Math::SinCos(DEG_2RAD(angles.y), &sy, &cy);
	Math::SinCos(DEG_2RAD(angles.x), &sp, &cp);
	Math::SinCos(DEG_2RAD(angles.z), &sr, &cr);

	if (forward)
	{
		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	if (right)
	{
		right->x = -1 * sr * sp * cy + -1 * cr * -sy;
		right->y = -1 * sr * sp * sy + -1 * cr * cy;
		right->z = -1 * sr * cp;
	}

	if (up)
	{
		up->x = cr * sp * cy + -sr * -sy;
		up->y = cr * sp * sy + -sr * cy;
		up->z = cr * cp;
	}
}

Matrix Math::AngleMatrix(const QAngle& angles)
{
	float fsPitch, fcPitch;
	float fsYaw, fcYaw;
	float fsRoll, fcRoll;

	Math::SinCos(DEG_2RAD(angles.x), &fsPitch, &fcPitch);
	Math::SinCos(DEG_2RAD(angles.y), &fsYaw, &fcYaw);
	Math::SinCos(DEG_2RAD(angles.z), &fsRoll, &fcRoll);

	const double sPitch = fsPitch;
	const double cPitch = fcPitch;
	const double sYaw = fsYaw;
	const double cYaw = fcYaw;
	const double sRoll = fsRoll;
	const double cRoll = fcRoll;

	Matrix rot{3, 3, 0};
	rot(0, 0) = cYaw * cPitch;
	rot(0, 1) = cYaw * sPitch * sRoll - sYaw * cRoll;
	rot(0, 2) = cYaw * sPitch * cRoll + sYaw * sRoll;
	rot(1, 0) = sYaw * cPitch;
	rot(1, 1) = sYaw * sPitch * sRoll + cYaw * cRoll;
	rot(1, 2) = sYaw * sPitch * cRoll - cYaw * sRoll;
	rot(2, 0) = -sPitch;
	rot(2, 1) = cPitch * sRoll;
	rot(2, 2) = cPitch * cRoll;

	return rot;
}

void Math::VectorAngles(const Vector& forward, Vector& pseudoup, QAngle* angles)
{
	if (!angles)
		return;

	const Vector left = pseudoup.Cross(forward).Normalize();

	if (float xyDist = forward.Length2D(); xyDist > 0.001f)
	{
		angles->y = RAD_2DEG(atan2f(forward.y, forward.x));
		angles->x = RAD_2DEG(atan2f(-forward.z, xyDist));
		const float upZ = (left.y * forward.x) - (left.x * forward.y);
		angles->z = RAD_2DEG(atan2f(left.z, upZ));
	}
	else
	{
		angles->y = RAD_2DEG(atan2f(-left.x, left.y));
		angles->x = RAD_2DEG(atan2f(-forward.z, xyDist));
		angles->z = 0;
	}
}

float Math::RandomNumber(const float& min, const float& max)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<float> dist_pitch(min, std::nextafter(max, FLT_MAX));

	return dist_pitch(mt);
}

int Math::RandomNumber(const int& min, const int& max)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> dist_pitch(min, max);

	return dist_pitch(mt);
}


Matrix::Matrix(const int rows, const int cols, const double init)
	: rows(rows)
	, cols(cols)
{
	this->mat.resize(rows);
	for (auto& it : this->mat)
		it.resize(cols, init);
}

Matrix& Matrix::operator=(const Matrix& rhs)
{
	if (&rhs == this)
		return *this;

	const auto newRows = rhs.rows;
	auto newCols = rhs.cols;

	this->mat.resize(newRows);
	for (auto& it : this->mat)
		it.resize(newCols);

	for (int i = 0; i < newRows; i++)
		for (int j = 0; j < newCols; j++)
			mat[i][j] = rhs(i, j);

	this->rows = newRows;
	this->cols = newCols;

	return *this;
}

Matrix Matrix::operator+(const Matrix& rhs) const
{
	Matrix result(this->rows, this->cols, 0);

	for (int i = 0; i < this->rows; i++)
		for (int j = 0; j < this->cols; j++)
			result(i, j) = this->mat[i][j] + rhs(i, j);

	return result;
}

Matrix& Matrix::operator+=(const Matrix& rhs)
{
	const auto newRows = rhs.rows;
	const auto newCols = rhs.cols;

	for (int i = 0; i < newRows; i++)
		for (int j = 0; j < newCols; j++)
			this->mat[i][j] += rhs(i, j);

	return *this;
}

Matrix Matrix::operator*(const Matrix& rhs) const
{
	if (this->cols != rhs.rows)
	{
		Log(INFO, true, "Matrix Error: rows != cols");
		return {1, 1, 0};
	}

	Matrix result(this->rows, rhs.cols, 0);

	for (int i = 0; i < this->rows; i++)
	{
		for (int j = 0; j < rhs.cols; j++)
		{
			int sum = 0;
			for (int k = 0; k < this->rows; k++)
			{
				sum += this->mat[i][k] * rhs(k, j);
			}
			result(i, j) += sum;
		}
	}

	return result;
}

Matrix& Matrix::operator*=(const Matrix& rhs)
{
	const Matrix result = (*this) * rhs;
	(*this) = result;
	return *this;
}

Vector Matrix::operator*(const Vector& rhs) const
{
	if (this->cols != 3)
		{
		Log(INFO, true, "Matrix Error: rows != cols");
		return {0, 0, 0};
	}

	Vector result(0, 0, 0);

	for (int i = 0; i < this->rows; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			result[i] += this->mat[i][j] * rhs[j];
		}
	}

	return result;
}

inline Vector Matrix::operator*=(const Vector& rhs) const
{
	return (*this) * rhs;
}

void Matrix::Transpose()
{
	Matrix result(this->rows, this->cols, 0);

	for (int i = 0; i < this->rows; i++)
	{
		for (int j = 0; j < this->cols; j++)
		{
			result(i, j) = this( j, i );
		}
	}

	*this = result;
}

void Matrix::Print() const
{
	for (int i = 0; i < this->rows; i++)
	{
		for (int j = 0; j < this->cols; j++)
		{
			Log(INFO, false, "%d ", this->mat[i][j]);
		}
		Log(INFO, false, "\n");
	}
}
