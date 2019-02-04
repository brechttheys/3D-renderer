#include "math/transformation3d.h"
#include "math/transformation-matrices.h"

using namespace math;


Transformation3D math::transformations::translation(const Vector3D& v)
{
    Matrix4x4 tm = transformation_matrices::translation(v);
    Matrix4x4 itm = transformation_matrices::translation(-v);

    return Transformation3D(tm, itm);
}

Transformation3D math::transformations::scale(double sx, double sy, double sz)
{
	Matrix4x4 sm = transformation_matrices::scaling(sx, sy, sz);
	Matrix4x4 ism = transformation_matrices::scaling(1 / sx, 1 / sy, 1 / sz);

	return Transformation3D(sm, ism);
}

Transformation3D math::transformations::rotate_x(const Angle& angle)
{
	//rotation matrix
	Matrix4x4 rm = transformation_matrices::rotation_around_x(angle);
	//inverse matrix
	Matrix4x4 irm = transformation_matrices::rotation_around_x(-angle);
	return Transformation3D(rm, irm);
}

Transformation3D math::transformations::rotate_z(const Angle& angle)
{
	//rotation matrix en inverse
	Matrix4x4 rm = transformation_matrices::rotation_around_z(angle);
	Matrix4x4 irm = transformation_matrices::rotation_around_z(-angle);
	return Transformation3D(rm, irm);
}

Transformation3D math::transformations::rotate_y(const Angle& angle)
{
	Matrix4x4 rm = transformation_matrices::rotation_around_y(angle);
	Matrix4x4 irm = transformation_matrices::rotation_around_y(-angle);
	return Transformation3D(rm, irm);
}
