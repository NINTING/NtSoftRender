


#include"Geometry.h"
#include"math.h"
#include"tgaImage.h"
#include<algorithm>
using namespace Geometry;
/*
void Geometry::Line(NtVector3 v0, NtVector3 v1, NtImage<Uint32>*img, NtColor color)
{
	int x0 = v0.x(), y0 = v0.y(), x1 = v1.x(), y1 = v1.y();

	float dx = x1 - x0;
	float dy = y1 - y0;
	bool steel = false;

	//��dy>dx,�ݶ�>1�����������ֿն�
	//x���������yͨ����ֵ�õ���dy>dx,��y��y'֮��ľ����󣬲����˿ն�
	//�Ƚ�dx��dy,��һ��������һ�����򽫱���������Ϊ���һ��
	if (std::abs(dy) > std::abs(dx))
	{
		std::swap(x0, y0);
		std::swap(x1, y1);
		steel = true;
	}
	if (x0 > x1)
	{
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	dx = x1 - x0;
	dy = y1 - y0;
	float k = std::abs(dy) * 2;
	float error = 0;
	int y = y0;
	for (int x = x0; x <= x1; x++)
	{

		if (steel)
			img->Set(y, x, color.GetColor32());
		else
			img->Set(x, y, color.GetColor32());
		error += k;
		if (error > dx)
		{
			y += y1 > y0 ? 1 : -1;
			error -= 2 * dx;
		}
	}
}


void Geometry::Trigle(NtVector3 v0, NtVector3 v1, NtVector3 v2, NtImage<Uint32>*img, NtColor color)
{
	//�涨v0����͵㣬v2����ߵ�
	if (v0.y() > v1.y())std::swap(v0, v1);
	if (v1.y() > v2.y())std::swap(v1, v2);
	if (v0.y() > v1.y())std::swap(v0, v1);
	Line(v1, v2, img, color);
	Line(v0, v2, img, color);
	Line(v0, v1, img, color);
	color = NtColor::Blue;
	int dy1 = v2.y() - v0.y(); 
	int dy2 = v1.y() - v0.y() + 1;//��ֹdy=0
	bool top = false;
	for (int y = v0.y(); y <= v2.y(); y++)
	{
		if (y > v1.y()) {
			top = true;
			dy2 = v2.y() - v1.y() + 1;
			color = NtColor::Green;
		}
		float t0 = (y - v0.y()) / (float)dy1;
		float tmpdy = top ? (y - v1.y()) : (y - v0.y());
		float t1 = tmpdy / (float)dy2;

		int xl = t0 * (v2.x() - v0.x()) + v0.x();
		int tmpdx = top ? (v2.x() - v1.x()) : (v1.x() - v0.x());
		int tmpx = top ? v1.x() : v0.x();
		int xr = t1 * tmpdx + tmpx;

		if (xl > xr)std::swap(xl, xr);
		for (int x = xl; x <= xr; x++)
			img->Set(x, y, color.GetColor32());
	}
}
static NtVector3 Barycentric(NtVector3 *  v, const NtVector3& P)
{
	
	NtVector3 tmp = Cross(NtVector3(v[1].x() - v[0].x(), v[2].x() - v[0].x(),v[0].x()- P.x()), NtVector3(v[1].y() - v[0].y(), v[2].y() - v[0].y(),  v[0].y()-P.y()));
	//���z����Ϊ0�����������˻������ط���Ϊ��������
	if (std::abs(tmp.z()) > 1e-2)
	{
		float u = tmp.x() / tmp.z();
		float v = tmp.y() / tmp.z();

		return NtVector3(1 - u - v, u, v);
	}
	return NtVector3(-1, 1, 1);
}

*/