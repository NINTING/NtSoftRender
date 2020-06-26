
#include"SDL.h"
#include<stdio.h>
#include<iostream>
#include"Nt.h"
#include"NtRender.h"
#include"Geometry.h"
#include"Model.h"
#include"LightHelper.h"
#include"Effect.h"
#include"tgaImage.h"
#include"NtUtility.h"
#include<sstream>
#include<windows.h>
#include"ShaderAssemble.h"
#include"ShadowMap.h"

SDL_Window* gWindow = NULL;
SDL_Renderer* gRender = NULL;
SDL_Texture* gTexture = NULL;	

#define DegToRad(x) M_PI/180.0f*(x)
const double MS_PER_FRAME = 1000.0/10;

NtVector4 AmbientLight(1.0f, 1.0f, 1.0f,1.f);
std::shared_ptr<Light> MainLight = CreateDirectionalLight(NtVector3(1.f, 1.f, 1.f), NtVector3(0.f,0.f, -1.f));



/* Camera */
NtVector3 Eye(0,0,2.5);	
NtVector3 Foucus(0,0,0);
NtVector3 Up(0, 1, 0);
NtCamera Camera;
NtCamera LightCamera;
Model head,cube;

int WindowWidth = 640;
int WindowHeight =480;

std::shared_ptr<NtSofterRender> render = NtSofterRender::Instance(WindowWidth, WindowHeight);
float AlphaScale = 1;

std::shared_ptr<NtWindow> window = NtWindow::Instance(WindowWidth, WindowHeight);

ShadowMap shadowmap(render.get(), WindowWidth, WindowHeight);

enum RenderEnum
{
	TPhone,
	Phone,
	STPhone,
	Blend,
	SSAO
}renderFlag ;
void WindowInit()
{
	NtViewport vp;
	vp.width =(float) WindowWidth;
	vp.height = (float)WindowHeight;
	vp.MinDepth = 0.001f;
	vp.MaxDepth = 1.f;
	vp.TopLeftX = vp.TopLeftY = 0.f;
	render->SetViewport(vp);
	Tex2D_UC  image = Tex2D_UC(window->GetWindowWidth(), window->GetWindowHeight());
	window->FillWindow(image);
	window->PresentWindow();
}


void CubeAssemble()
{
	renderFlag = TPhone;
	NtMatrix4x4 world = NtMatrix4x4();
	if (renderFlag == STPhone) {
		render->CleanBackAndDepthBuffer();
		shadowmap.SetMainLight(LightCamera);
		auto depthTex = shadowmap.BuildDepthMap(world, cube);
		//Present_image(depthTex.get());
	    render->CleanBackAndDepthBuffer();
		STPhoneAssemble(cube, Camera, render.get(), *MainLight.get(), AmbientLight, shadowmap);
	}
	else if (renderFlag == TPhone)
	{
		render->CleanBackAndDepthBuffer();
		TPhoneAssemble(cube, Camera, render.get(), *MainLight.get(), AmbientLight);
	}
	else if (renderFlag = Blend)
	{

		render->CleanBackAndDepthBuffer();
		BlendAssemble(cube, Camera, render.get(), *MainLight.get(), AmbientLight, AlphaScale);
	}
}




void HeadPhoneAssemble()
{
	renderFlag = SSAO;
	NtMatrix4x4 world = NtMatrix4x4();
	if (renderFlag == STPhone) {
		render->CleanBackAndDepthBuffer();
		shadowmap.SetMainLight(LightCamera);
		auto depthTex = shadowmap.BuildDepthMap(world, head);
		//Present_image(depthTex.get());
		render->CleanBackAndDepthBuffer();
		STPhoneAssemble(head, Camera, render.get(), *MainLight.get(), AmbientLight, shadowmap);
	}
	else if (renderFlag == TPhone)
	{
		render->CleanBackAndDepthBuffer();
		TPhoneAssemble(head, Camera, render.get(), *MainLight.get(), AmbientLight);

	}
	else if (renderFlag == SSAO)
	{
		render->CleanBackAndDepthBuffer();
		SSAOAssemble(head, Camera, render.get(), *MainLight.get(), AmbientLight);
	}

}

void Assemble()
{
	
	//Present_image(depthTex.get());
	HeadPhoneAssemble();
	//CubeAssemble();
}

void ModelInit()
{
	head.init("Asset\\african_head.obj");
	 

	head.SetMainTexture("Asset\\african_head_diffuse.tga");
	head.GetDiffuseTex()->flip_vertically();
	head.GetDiffuseTex()->flip_horizontally();
	//Present_image(Float4ImageToRGBAImage(*head.GetDiffuseTex()));

	//Present_image(NtUtility::Read_file("african_head_diffuse.tga"));
	//head.SetNormalTexture("african_head_nm.tga");
	//head.GetNormalTex()->flip_vertically();
	//head.GetNormalTex()->flip_horizontally();
	//Present_image(Float4ImageToRGBAImage(*head.GetNormalTex()));
	head.SetSpecularTexture("Asset\\african_head_spec.tga");
	//Present_image(Float4ImageToRGBAImage(*head.GetSpecularTex()));
	head.GetSpecularTex()->flip_vertically();
	head.GetSpecularTex()->flip_horizontally();

	head.SetTangentTexture("Asset\\african_head_nm_tangent.tga");
	head.GetTangentTex()->flip_vertically();
	head.GetTangentTex()->flip_horizontally();
	//Present_image(Float4ImageToRGBAImage(*head.GetTangentTex()));
	Material HeadMat;
	HeadMat.diffTextureId = 0;
	HeadMat.Roughness = 0.5;
	HeadMat.name = "Head";
	HeadMat.MaterialId = 0;
	head.SetMaterial(HeadMat);
	cube.init("Asset\\cube.obj");
	Material CubeMat;

	CubeMat.name = "Cube";
	CubeMat.MaterialId = 0;
	cube.SetMaterial(CubeMat);
}

void SkyboxInit()
{
	Tex2D_UC PositiveX = NtUtility::Read_file("riverside_west.BMP");
	Tex2D_UC NegativeX = NtUtility::Read_file("riverside_east.BMP");
	Tex2D_UC PositiveY = NtUtility::Read_file("riverside_up.BMP");
	Tex2D_UC NegativeY = NtUtility::Read_file("riverside_down.BMP");
	Tex2D_UC PositiveZ = NtUtility::Read_file("riverside_north.BMP");
	Tex2D_UC NegativeZ = NtUtility::Read_file("riverside_south.BMP");
	
}

void ScenceInit()
{
	//MainLight->GetDirection()*-2.f
	LightCamera.LookAt(-MainLight->GetDirection(), NtVector3(0, 0, 0), NtVector3(0, 1, 0));
	//只需要将正交投影视景体覆盖整个场景即可
	LightCamera.SetVisualBody(3.f,3.f,0.1f,10.f);
	Camera.LookAt(Eye, Foucus, Up);
	Camera.SetViewFrustum(DegToRad(90), window->AspectRadio(), 0.1f, 1000.f);	

}



void UpdateScence(float dt)
{
	
	Camera.Update();
	LightCamera.Update();
	MainLight->normalise();
}


bool ProcessInput(float dt)
{
	SDL_Event event;
	static bool isMouseLeftDown = false;
///	int lastX, lastY;
	while (SDL_PollEvent(&event)) {
		//printf("%d\n", event.type);
		// check for messages
		switch (event.type)
		{
			
			// exit if the window is closed
			case SDL_QUIT:
			
				return true;
			case SDL_KEYDOWN:
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					return true;
				case SDLK_w:
					Camera.Walk(0.1f);
					break;
				case SDLK_s:
					{
						Camera.Walk(-0.1f);
						break;
					}
				case SDLK_a:
					{
						Camera.Strafe(-0.1f);
						break;
					}
				case SDLK_d:
					{
						Camera.Strafe(0.1f);
						break;
					}
				case SDLK_v:
					{
						render->SetRenderState(Wireframe);
						break;
					}
				case SDLK_g:
					{
						render->SetRenderState(GroundShading);
						break;
					}
				case SDLK_b:
				{
					if(render->GetCullState()==Cullback)
						render->SetCullState(NoCull);
					else
						render->SetCullState(Cullback);
					break;
				}
				case SDLK_z:
				{
					renderFlag = STPhone;
					break;
				}
				case SDLK_x:
				{
					renderFlag = TPhone;
					break;
				}
				case SDLK_n:
				{
					renderFlag = SSAO;
					break;
				}
				case SDLK_UP:
					AlphaScale = min(1, AlphaScale + 0.1f);
					break;
				case SDLK_DOWN:
					AlphaScale = max(0, AlphaScale - 0.1f);
					break;

				default:
					break;
				}
			}
			case SDL_MOUSEMOTION:
				//printf("鼠标移动 %d\n", isMouseLeftDown);
				if (isMouseLeftDown) {
					int px = event.motion.xrel;
					int py = event.motion.yrel;

					Camera.Pitch(DegToRad(py*0.1f));
					Camera.RotateY(DegToRad(px*0.1f));
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
			{
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					isMouseLeftDown = true;
					//printf("isMouseLeftDown %d\n", isMouseLeftDown);
					
				}
				break;
			}
			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					//printf("isMouseLeftDown %d\n", isMouseLeftDown);
					isMouseLeftDown = false;
				}
				break;
			
		} // end switch
	}
	
	return false;
}


void Rnder()
{

	Assemble();
	render->Draw();

	window->Present_image(render->Present());
	
	//printf("suc\n");
}


int main(int argc, char*argv[])
{
	renderFlag = TPhone;
 	int frameCount = 0;
	double acculateElapse = 0;
	float fps = 0;
	//image.FillImage(Red32);

	double lastTime = GetCurrentTime();
	bool done = false;
	WindowInit();
	ScenceInit();
	ModelInit();		

	std::stringstream  ss;
	while (true) {
		double CurrTime = GetCurrentTime();
		double ElapseTime =  CurrTime - lastTime;
		frameCount++;
		acculateElapse += ElapseTime;
		if (acculateElapse > 1000)
		{
			fps = float(frameCount);
			frameCount = 0;
			acculateElapse -= 1000;
			ss << "NtRender   fps ";
			ss << fps;
			window->SetTitle(ss.str().c_str());
			ss.str("");
		}
		ElapseTime /= 1000;
	//	printf("%lf\n", ElapseTime);
		
		if (ProcessInput(ElapseTime))break;;

		
		UpdateScence(ElapseTime);

		Rnder();

		lastTime = CurrTime;
		
	}

	return 0;
} // end of message processing


