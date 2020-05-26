#pragma once

class Vector3d
{
public:
	Vector3d(long double x, long double y, long double z);
	Vector3d();
	~Vector3d();
	Vector3d(const Vector3d &);
	long double x() const;
	long double y() const;
	long double z() const;
	Vector3d operator+(const Vector3d & b) const;
	Vector3d operator-(const Vector3d & b) const;
	Vector3d operator*(long double val) const;
	Vector3d operator*(const Vector3d & b) const;

	Vector3d& operator+=(const Vector3d & b);
	Vector3d& operator-=(const Vector3d & b);
	Vector3d& operator*=(long double val);
	Vector3d& operator*=(const Vector3d & b);

	bool operator==(const Vector3d & b) const;
	long double length() const;
private:
	long double _x, _y, _z;
};

