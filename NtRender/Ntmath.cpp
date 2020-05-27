

#include"Ntmath.h"

/*
NtMatrix NtMatrixLookAt(NtVector3&EyePosition, NtVector3& FoucusPosition,  NtVector3&Up)
{
	NtVector3 lookAt = FoucusPosition - EyePosition;
	lookAt.normalize();
	NtVector3 Right =  Cross(Up, lookAt);
	Right.normalize();
	Up = Cross(lookAt, Right);
	return NtMatrix(Right.x, Up.x, lookAt.x, 0,
		Right.y, Up.y, lookAt.y, 0,
		Right.z, Up.z, lookAt.z, 0,
		-(EyePosition*Right), -(EyePosition*Up), -(EyePosition*lookAt), 1);
}
*/
NtMatrix4x4 NtMatrixPerspective(float FovAngleY, float Aspect, float NearZ, float FarZ)
{
	float OverTanFov = 1/tanf(FovAngleY*0.5);
	float dz = FarZ - NearZ;
	return NtMatrix4x4({ 1 / Aspect * OverTanFov, 0, 0, 0,
		0, OverTanFov, 0, 0,
		0, 0, FarZ / dz, 1,
		0, 0, -FarZ * NearZ / dz, 0 });
}


NtMatrix3x3 NtMatrixRotationAxis(const NtVector3&axis, float angle)
{
	float c = cosf(angle);
	float s = sinf(angle);
	float sx = s * axis.x();
	float sy = s * axis.y();
	float sz = s * axis.z();
	float xy = axis.x()*axis.y();
	float xz = axis.x()*axis.z();
	float yz = axis.z()*axis.y();
	float overc = 1 - c;
	return NtMatrix3x3({
		c + overc * axis.x()*axis.x(), overc*xy + sz, overc*xz - sy,
		overc*xy - sz, c + overc * axis.y()*axis.y(), overc*yz + sx,
		overc*xz + sy, overc *yz - sx, c + overc * axis.z()*axis.z() });
}



NtMatrix2x3 ComputerTangent(const NtMatrix2x2& TexCoordM, const NtMatrix2x3& SideM)
{
	NtMatrix2x2 invertM = TexCoordM.invert();
	return invertM * SideM;
}
