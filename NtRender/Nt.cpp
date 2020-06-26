#include"Nt.h"
#include"Ntmath.h"
#include"cstdio"
#include"Geometry.h"
#define NOMINMAX
#include<Windows.h>


 NtVector3 axisX(1, 0, 0);
 NtVector3 axisY(0, 1, 0);
 NtVector3 axisZ(0, 0, 1);

/*
========================
|	NtMatrix          |
========================

NtMatrix::NtMatrix()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (i == j)
				raw[i][j] = 1;
			else
			raw[i][j] = 0;
		}
	}

}
NtMatrix::NtMatrix(float Raw[4][4])
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			raw[i][j] = Raw[i][j];
		}
}
void NtMatrix::swap(NtMatrix &rhs)
{
	//匹配最优swap
	using std::swap;
	swap(this->raw, rhs);
}
NtMatrix::NtMatrix(float x0, float y0, float z0, float w0,
	float x1, float y1, float z1, float w1,
	float x2, float y2, float z2, float w2,
	float x3, float y3, float z3, float w3)
{
	raw[0][0] = x0; raw[0][1] = y0; raw[0][2] = z0; raw[0][3] = w0;
	raw[1][0] = x1; raw[1][1] = y1; raw[1][2] = z1; raw[1][3] = w1;
	raw[2][0] = x2; raw[2][1] = y2; raw[2][2] = z2; raw[2][3] = w2;
	raw[3][0] = x3; raw[3][1] = y3; raw[3][2] = z3; raw[3][3] = w3;
}
NtMatrix::NtMatrix(const NtMatrix&rhs)
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			this->raw[i][j] = rhs[i][j];
		}
	}
}

NtMatrix& NtMatrix::operator =(const NtMatrix&rhs)
{
	NtMatrix tmp(rhs);
	this->swap(tmp);
	return *this;
}

NtMatrix NtMatrix::operator*(const NtMatrix&rhs)const
{
	float ret[4][4] = {0};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) 
				ret[i][j] += raw[i][k] * rhs[k][j];
		}
	}
	return NtMatrix(ret);
}
void NtMatrix::operator*=(const NtMatrix&rhs)
{
	*this=(*this)*rhs;
}

void NtMatrix::Print()const
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			printf("%lf ", raw[i][j]);
		}
		printf("\n");
	}
}


========================
|	NtVector2          |
========================


NtVector2 NtVector2::operator - (const NtVector2& rv)const{

	return NtVector2(x - rv.x(),y - rv.y());
}
NtVector2 NtVector2::operator + (const NtVector2& rv)const{
	return NtVector2(x + rv.x(), y + rv.y());
}
NtVector2::operator NtVector3() { return NtVector3(x, y, 0); }

========================
|	NtVector3          |
========================


NtVector3 NtVector3::operator - (const NtVector3& rv)const
{
	return NtVector3(x - rv.x(), y - rv.y(),z-rv.z());
}
NtVector3 NtVector3::operator + (const NtVector3& rv)const
{
	return NtVector3(x + rv.x(), y + rv.y(),z+rv.z());
}
void NtVector3::operator += (const NtVector3&rv)
{
	this->x  += rv.x();
	this->y += rv.y();
	this->z += rv.z();

}

float NtVector3::operator * (const NtVector3& rv)const
{
	return x * rv.x() + y * rv.y()+ z * rv.z();
}
void NtVector3::normalize()
{
	float n = std::sqrt(x*x + y * y + z * z);
	x /= n;
	y /= n;
	z /= n;
}

NtVector3 NtVector3::operator * (float t)const
{
	return NtVector3(x *t, y *t, z *t);
}
void NtVector3::operator *= (float t)
{
	*this = *(this)*t;
}

========================
|	NtVector4          |
========================

NtVector4 NtVector4::operator * (const NtMatrix4x4&rhs)const
{
	NtVector4 ret;
	ret.x() = x * rhs[0][0] + y * rhs[1][0] + z*rhs[2][0] + w * rhs[3][0];
	ret.y() = x * rhs[0][1] + y * rhs[1][1] + z*rhs[2][1] + w * rhs[3][1];
	ret.z() = x * rhs[0][2] + y * rhs[1][2] + z*rhs[2][2] + w * rhs[3][2];
	ret.w() = x * rhs[0][3] + y * rhs[1][3] + z*rhs[2][3] + w * rhs[3][3];
	return ret;
}
void NtVector4:: operator *= (const NtMatrix&rhs)
{
	*this = this->operator*(rhs);

}

void NtVector4::normalize()
{
	x /= w;
	y /= w;
	z /= w;	
	w = 1;
}

NtVector4 NtVector4::operator * (float t)const
{
	return NtVector4(x *t, y *t, z *t,w*t);
}
void NtVector4::operator *= (float t)
{
	this->x = this->x*t;
	this->y = this->y*t;
	this->z = this->z*t;
	this->w = this->w*t;
}

NtVector4 NtVector4::operator - (const NtVector4& rv)const
{
	return NtVector4(x - rv.x(), y - rv.y(), z - rv.z(),w-rv.w());
}
NtVector4 NtVector4::operator + (const NtVector4& rv)const
{
	return NtVector4(x + rv.x(), y + rv.y(), z + rv.z(),w+rv.w());
}

void NtVector4::Print()const
{
	printf("%lf %lf %lf %lf\n", x, y, z, w);
}
*/
/*
========================
|	 NtVertex          |
========================
*/

/*
NtVertex NtVertex::Interpolation(const NtVertex& rhs, float t)
{

}
*/



/*
========================	
|	NtWindow           |
========================
*/
void NtWindow::init(int Width, int height)
{
	SDL_Init(SDL_INIT_VIDEO);
	gWindow_ = SDL_CreateWindow("SDL", 100, 100, Width, height, SDL_WINDOW_SHOWN);

	gRender_ = SDL_CreateRenderer(gWindow_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	//创建texture,注意像素格式和访问方式
	gTexture_ = SDL_CreateTexture(gRender_, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, Width, height );
	
	
}


void NtWindow::PresentWindow()
{
	SDL_RenderCopy(gRender_, gTexture_, NULL, NULL);
	SDL_RenderPresent(gRender_);
}


void NtWindow::FillWindow(const Tex2D_UC &image)
{
	
	void* pix;
	int pitch;
	//SDL_PixelFormat* format;
	SDL_LockTexture(gTexture_, NULL, &pix, &pitch);
	//为了生成颜色,使用rgba8888的格式

	
	int size = image.GetWidth()*image.GetHeight();
	memcpy(pix, image.GetBuffer(), size*sizeof(Uint32));
	/*
	int ww = GetWindowWidth();
	int w = image.GetWidth();
	int h = image.GetHeight();
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{

			((Uint32*)pix)[j+i*ww] = RGBAToUint32(image.GetPixel(j,i));
		}
	}
	//for (int i = 0; i < size; i++)
		//memcpy((char*)pix+i*4, image.GetBuffer(i * 4),4 );
		//((Uint32*)pix)[i] =RGBAToUint32(image.GetPixel(i));
	*/
	SDL_UnlockTexture(gTexture_);
}


int NtWindow::GetWindowWidth()
{
	int w;
	SDL_GetWindowSize(gWindow_, &w, nullptr);
	return w;
}

int NtWindow::GetWindowHeight()
{
	int h;
	SDL_GetWindowSize(gWindow_, nullptr,&h);
	return h;
}

float  NtWindow::AspectRadio()
{
	int w;
	SDL_GetWindowSize(gWindow_, &w, nullptr);
	
	int h;
	SDL_GetWindowSize(gWindow_, nullptr, &h);
	return float(w) / h;
}

void NtWindow::Present_image(const Tex2D_UC &img)
{
	this->FillWindow(img);

	this->PresentWindow();
}
/*
========================
|	NtSofterRender     |
========================
*/

/*
========================
|	   NtCamera        |
========================
*/
NtMatrix4x4 NtMatrixViewport(const NtViewport&vp)
{
	NtMatrix4x4 ret;
	float halfW = vp.width *0.5;
	float halfH = vp.height*0.5;
	ret[0][0] = halfW;
	ret[1][1] = -halfH;
	ret[2][2] = 1;//vp.MaxDepth - vp.MinDepth;
	ret[3][0] = vp.TopLeftX + halfW;
	ret[3][1] = vp.TopLeftY + halfH;
	ret[3][2] = 0;//vp.MinDepth;
	ret[3][3] = 1;
	return ret;
}


void NtCamera::LookAt(const NtVector3& Pos, const NtVector3& Target, const NtVector3& Up)
{
	Pos_ = Pos;
	Look_ = Target - Pos_;
	Look_.normalize();
	Up_ = Up;
	ViewDirty_ = true;
}
void NtCamera::Update()
{
	if(!ViewDirty_)return
	Look_.normalize();
	Right_ = Cross(Up_, Look_);
	Right_.normalize();
	Up_ = Cross(Look_, Right_);
	View_[0][0] = Right_.x();
	View_[1][0] = Right_.y();
	View_[2][0] = Right_.z();
	View_[3][0] = -(Pos_*Right_);
	
	View_[0][1] = Up_.x();
	View_[1][1] = Up_.y();
	View_[2][1] = Up_.z();
	View_[3][1] = -(Pos_*Up_);
	
	View_[0][2] = Look_.x();
	View_[1][2] = Look_.y();
	View_[2][2] = Look_.z();
	View_[3][2] = -(Pos_ * Look_);
	
	View_[0][3] = 0;
	View_[1][3] = 0;
	View_[2][3] = 0;
	View_[3][3] = 1;
	ViewDirty_ = false;
}
void NtCamera::SetViewFrustum(float FovAngleY, float Aspect, float NearZ, float FarZ)
{
	VisualBody.frustum.nearZ = NearZ;
	VisualBody.frustum.farZ = FarZ;
	VisualBody.frustum.aspect = Aspect;
	VisualBody.frustum.fovY = FovAngleY;
	
	VisualBody.frustum.nearH = 2 * NearZ* tanf(FovAngleY*0.5);
	VisualBody.frustum.farH = 2 * FarZ *tanf(FovAngleY*0.5);
	Proj_ = NtMatrixPerspective(FovAngleY, Aspect, NearZ, FarZ);

}

void NtCamera::SetVisualBody(float Width, float Height, float NearZ, float FarZ)
{
	VisualBody.rectangle.Width = Width;
	VisualBody.rectangle.Heiht = Height;
	VisualBody.rectangle.Far = FarZ;
	VisualBody.rectangle.Near = NearZ;
	Proj_ = NtMatrixOrthogonal(Width, Height, NearZ, FarZ);
}
NtMatrix4x4 NtCamera::GetViewMatrix()const
{
	return View_;
}
NtMatrix4x4 NtCamera::GetProjMatrix()const
{
	return Proj_;
}

void NtCamera::Walk(float d)
{
	Pos_ += Look_ * d;
	ViewDirty_ = true;
}

void NtCamera::Strafe(float d)
{

	Pos_ += Right_ * d;
	ViewDirty_ = true;
}


void NtCamera::RotateY(float d)
{
	NtMatrix3x3 R = NtMatrixRotationAxis(axisY, d);

	Look_ = Look_ * R;
	Up_ = Up_ * R;
	Right_ = Right_ * R;
	ViewDirty_ = true;
}
void NtCamera::Pitch(float d)
{
	NtMatrix3x3 R = NtMatrixRotationAxis(Right_, d);

	Look_ =Look_* R;
	Up_ = Up_ * R;
	ViewDirty_ = true;
}

std::vector<NtVector4> NtCamera::NDCface({
	//left
	NtVector4(1.0f,0.f,0.f,1.f),
	//right
	NtVector4(-1.f,0.f,0.f,1.f),
	//top
	NtVector4(0.f,1.f,0.f,1.f),
	//bottom
	NtVector4(0.f ,-1.f,0.f,1.f),
	//near
	NtVector4(0.f,0.f,0.f,1.f),
	//far
	NtVector4(0.f,0.f,1.f,1.f),
	});