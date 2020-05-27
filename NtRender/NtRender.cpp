#include"NtRender.h"
#include"Nt.h"





void NtSofterRender::SetVertexBuffer(const std::vector<NtVertex>&vs)
{
	this->VertexBuffer_.assign(vs.begin(), vs.end());
}
void NtSofterRender::SetIndexBuffer(const std::vector<std::vector<Uint32>>&is)
{
	this->IndexBuffer_.assign(is.begin(), is.end());
}
void NtSofterRender::ClearPrimitive()
{
	/*
	for (int i = 0; i < Primitive.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			delete Primitive[i][j];
		}
	}
	*/
	Primitive.clear();

	//VertexOuts.clear();

}


/*顶点变换*/
void NtSofterRender::VertexProcessing()
{
	
	ClearPrimitive();
	size_t Vsize = VertexBuffer_.size();
	for (int i = 0; i < Vsize; i++)
	{  	
		
		VertexOuts.push_back(shader_->VertexShader(VertexBuffer_[i]));
	}

}
bool ClipSpaceCull(ptrBase tri[])
{

	for (int i = 0; i < 3; i++)
	{
		if (std::abs(tri[i]->PosH.x()) <= abs(tri[i]->PosH.w()) ||
			std::abs(tri[i]->PosH.y()) <= abs(tri[i]->PosH.w()) ||
			tri[i]->PosH.z() < tri[i]->PosH.w() || tri[i]->PosH.z() > 1e-7)
			return true;
	}

	return false;
}


void NtSofterRender::PrimitiveAssemblyAndClip()
{

	
	size_t Vsize = VertexOuts.size();
	size_t Isize = IndexBuffer_.size();
	for (int i = 0; i < Isize; i++)
	{
		ptrBase tmp[3];
		for (int j = 0; j < IndexBuffer_[i].size(); j++)
		{
			tmp[j] = VertexOuts[IndexBuffer_[i][j]];
		}
		/*裁剪*/
		if (!ClipSpaceCull(tmp)) 
		{
			//delete tmp[][]
			continue;
		}
		HomogeneousCoordinateCropping(tmp);
		
	}
	VertexOuts.clear();
}
void NtSofterRender::Rasterization()
{
	NtMatrix4x4 vpM = NtMatrixViewport(Viewport_);
	/*视口变换 背面剔除*/

	/*透视除法*/
	for (auto i = Primitive.begin(); i != Primitive.end(); i++)
	{
		std::vector<ptrBase>  Tri({ (*i)[0]->Copy(),(*i)[1]->Copy(),(*i)[2]->Copy() });
		
		float w0 = Tri[0]->PosH.w();
		float w1 = Tri[1]->PosH.w();
		float w2 = Tri[2]->PosH.w();
		//Tri[0].PosH.Print();

		Tri[0]->PosH.Perspective();
		Tri[1]->PosH.Perspective();
		Tri[2]->PosH.Perspective();

		//Tri[0]->PosH.Print();

		Tri[0]->PosH = Tri[0]->PosH * vpM;
		Tri[1]->PosH = Tri[1]->PosH *vpM;
		Tri[2]->PosH = Tri[2]->PosH *vpM;
		//为透视插值做准备
		Tri[0]->PosH.w(1 / w0);
		Tri[1]->PosH.w(1 / w1);
		Tri[2]->PosH.w(1 / w2);
		if (IsBackCull) {
			NtVector3 normal = Cross(NtVector4To3(Tri[1]->PosH - Tri[0]->PosH), NtVector4To3(Tri[2]->PosH - Tri[0]->PosH));
			//背面剔除  视角朝向为z轴，法向量z分量>0为背面
			if (normal.z() <= -1e-6) {
				continue;
			}
		}
		if (rs_ == RenderState::Wireframe)
		{
			Wirframe(Tri[0].get(), Tri[1].get(), Tri[2].get());
			continue;
		}

		
		//	double TestTime = GetCurrentTime();

		//	printf("渲染间隔\n");
		BarycentricTriangle(Tri[0].get(), Tri[1].get(), Tri[2].get());
		//	printf("%lf\n", (GetCurrentTime() - TestTime) / 1000.0);


	}
	//printf("渲染完毕");
}
void NtSofterRender::Wirframe(const NtVertexOutBaseFormat* v0, const NtVertexOutBaseFormat* v1, const  NtVertexOutBaseFormat* v2)
{
	Line(v0, v1);
	Line(v1, v2);
	Line(v2, v0);
}

void NtSofterRender::Draw()
{

	//View_.Print();



	VertexProcessing();

	PrimitiveAssemblyAndClip();

	Rasterization();

}

NtSofterRender::NtSofterRender(int w, int h)
{
	FrontBuffer_ = new NtImage(w, h,4);
	BackBuffer_ = new NtImage(w, h,4);
	ZBuffer_ = new float[w*h];
	IsBackCull = true;
	//ZBuffer_->FillImage(2000);
}


/*
NtSofterRender::VertexOut NtSofterRender::VertexOut::Interpolation(const VertexOut& rhs, float t)const
{
	VertexOut ret;
	ret.PosH = rhs.PosH*t + this->PosH * (1 - t);
	ret.PosW = rhs.PosW*t + this->PosW * (1 - t);
	ret.Color = rhs.Color*t + this->Color* (1 - t);
	return ret;
}
*/

bool Inside(const ptrBase A, const NtVector4& face)
{
	float d = 0;
	for (int j = 0; j < 4; j++)
		d += A->PosH[j] * face[j];
	if (d >= 0)
		return true;
	return false;
}
ptrBase Intersect(const ptrBase A,const ptrBase B, const NtVector4& face)
{
	float da = 0, db = 0;
	for (int j = 0; j < 4; j++)
		da += A->PosH[j] * face[j];
	for (int j = 0; j < 4; j++)
		db += B->PosH[j] * face[j];

	float t = da / (da - db);

	return A->LineInterpolation(B.get(), t);
}
bool AllInside(ptrBase Tri[])
{
	for (int i = 0; i < 3; i++)
	{
		if (!(std::abs(Tri[i]->PosH.x()) <= abs(Tri[i]->PosH.w()) &&
			std::abs(Tri[i]->PosH.y()) <= abs(Tri[i]->PosH.w()) &&
			Tri[i]->PosH.z() < Tri[i]->PosH.w() && Tri[i]->PosH.z() > 1e-7))
		{
			return false;

		}
	}
	return true;
}
void  NtSofterRender::HomogeneousCoordinateCropping(ptrBase  Tri[])
{

	
	if (AllInside(Tri)) {
			Primitive.push_back(std::vector<ptrBase>{Tri[0], Tri[1], Tri[2]});
		return;
	}
	std::vector<ptrBase>out;
	out.push_back(Tri[0]);
	out.push_back(Tri[1]);
	out.push_back(Tri[2]);
	for (int i = 0; i < 6; i++)
	{
		std::vector<ptrBase>in(out);
		out.clear();
		for (int j = 0; j < in.size(); j++)
		{
			ptrBase Curr = in[j];
			ptrBase Pre = in[(j + in.size() - 1) % in.size()];
			NtVector4 face = NtCamera::NDCface[i];
			if (Inside(Curr, face))//当前点在视锥内
			{
				if (!Inside(Pre, face))
				{
					ptrBase c =  Intersect(Pre, Curr, face);
				//	if (c->PosH.z() < 1e-6 || c->PosH.z() > c->PosH.w()){
				//		continue;
				//	}
					out.push_back(c);
				}
				out.push_back(Curr);
			}
			else      //当前点不在视锥内
			{
				if (Inside(Pre, face))
				{
					ptrBase c = Intersect(Pre, Curr, face);
					//if (c->PosH.z() < 1e-6 || c->PosH.z() > c->PosH.w())
					//{
					//	continue;
					//}
					out.push_back(c);
				}
			}

			 
		}

	}

	for (int i = 1; i < out.size() - 1 && out.size() >= 3; i++)
	{
		Primitive.push_back(std::vector<ptrBase>{out[0], out[i], out[i + 1]});
	}
}

static NtVector3 Barycentric(NtVector3 *  v, const NtVector2& P)
{

	NtVector3 tmp = Cross(NtVector3(v[1].x() - v[0].x(), v[2].x() - v[0].x(), v[0].x() - P.x()), NtVector3(v[1].y() - v[0].y(), v[2].y() - v[0].y(), v[0].y() - P.y()));
	//如果z分量为0，则三角形退化，返回分量为负的向量
	if (std::abs(tmp.z()) > 1e-2)
	{
		float u = tmp.x() / tmp.z();
		float v = tmp.y() / tmp.z();

		return NtVector3(1 - u - v, u, v);
	}
	return NtVector3(-1, 1, 1);
}

void NtSofterRender::BarycentricTriangle(const NtVertexOutBaseFormat* v0, const NtVertexOutBaseFormat* v1, const  NtVertexOutBaseFormat* v2)
{

	//顶点数据
	const NtVertexOutBaseFormat* Tri[3] = { v0,v1,v2 };
	NtVector3 TriPosH[3] = { NtVector4To3(v0->PosH),NtVector4To3(v1->PosH), NtVector4To3(v2->PosH) };
	/*构建包围盒，缩小遍历范围*/
	NtVector2 BoundBox[2];
	BoundBox[0] = NtVector2(BackBuffer_->GetWidth(), BackBuffer_->GetHeight());
	BoundBox[1] = NtVector2(0, 0);
	for (int i = 0; i < 3; i++)
	{
		BoundBox[0].x(std::min(BoundBox[0].x(), Tri[i]->PosH.x()));
		BoundBox[0].y(std::min(BoundBox[0].y(), Tri[i]->PosH.y()));
		BoundBox[1].x(std::max(BoundBox[1].x(), Tri[i]->PosH.x()));
		BoundBox[1].y(std::max(BoundBox[1].y(), Tri[i]->PosH.y()));
	}
	//边界框裁剪
	BoundBox[0].x(std::max(0.0f, BoundBox[0].x()));
	BoundBox[0].y(std::max(0.0f, BoundBox[0].y()));
	BoundBox[1].x(std::min((float)BackBuffer_->GetWidth() - 1, BoundBox[1].x()));
	BoundBox[1].y(std::min((float)BackBuffer_->GetHeight() - 1, BoundBox[1].y()));


	ptrBase pixelVertex = Tri[0]->Copy();

	int pixelnum = 0;
	/*遍历像素*/
	for (int y = ceilf(BoundBox[0].y()); y <= BoundBox[1].y(); y++)
	{


		for (int x = ceilf(BoundBox[0].x()); x <= BoundBox[1].x(); x++)
		{

			NtVector2 P = NtVector2(x, y);
			NtVector3 u = Barycentric(TriPosH, P);
			if (u.x() < 0 || u.y() < 0 || u.z() < 0)
				continue;

			//通过三角形重心坐标插值计算点P的Z分量
			pixelVertex->PerspectiveInterpolation(Tri[0], Tri[1], Tri[2], u[0],u[1],u[2]);
			//VertexOut vo = PerspectiveCorrectionInterpolation(Tri, u);

			pixelVertex->PosH.x(x), pixelVertex->PosH.y(y);

			NtColor color = shader_->PixelShader(pixelVertex.get());

			//printf("%d\n", color.GetColor32());

			OutputMerge(x, y, pixelVertex->PosH.w(), color);


		}

	}

}

void NtSofterRender::OutputMerge(int x, int y, float z, NtColor color)
{
	int w= BackBuffer_->GetWidth();

	//z'=1/z z越小，z’越大
	if (z > ZBuffer_[x+y* w])
	{
		ZBuffer_[x + y * w] = z;
		BackBuffer_->Set(x, y, color);
	}

}

NtImage NtSofterRender::Present()
{
	std::swap(BackBuffer_, FrontBuffer_);
	CleanZBuffer();
	BackBuffer_->CleanBuffer();
	return *FrontBuffer_;
}

void NtSofterRender::SetRenderState(RenderState rs)
{
	rs_ = rs;
}


void NtSofterRender::Line(const NtVertexOutBaseFormat* v0, const  NtVertexOutBaseFormat* v1)
{
	int x0 = v0->PosH.x(), y0 = v0->PosH.y(), x1 = v1->PosH.x(), y1 = v1->PosH.y();

	float dx = x1 - x0;
	float dy = y1 - y0;
	bool steel = false;

	//当dy>dx,梯度>1，线条将出现空洞
	//x逐个遍历，y通过插值得到，dy>dx,则y与y'之间的距离变大，产生了空洞
	//比较dx，dy,当一方大于另一方，则将遍历对象设为大的一方
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
	dx = float(x1 - x0);
	dy = float(y1 - y0);
	float k = std::abs(dy) * 2;
	float error = 0;
	int y = y0;
	for (int x = x0; x <= x1; x++)
	{
		if (steel)
			BackBuffer_->Set(y, x, White32);
		else
			BackBuffer_->Set(x, y, White32);
		error += k;
		if (error > dx)
		{
			y += y1 > y0 ? 1 : -1;
			error -= 2 * dx;
		}
	}
}
