
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
#include "NtSkybox.h"

SDL_Window* gWindow = NULL;
SDL_Renderer* gRender = NULL;
SDL_Texture* gTexture = NULL;	

#define DegToRad(x) M_PI/180.0f*(x)
const double MS_PER_FRAME = 1000.0/10;

NtVector4 AmbientLight(0.2f, 0.2f, 0.2f,0.f);
std::shared_ptr<Light> MainLight = CreateDirectionalLight(NtVector3(1.f, 1.f, 1.f), NtVector3(0.f,0.f, -1.f));

/*Skybox*/
NtSkybox skyBox;


/* Camera */
NtVector3 Eye(0,0,2.5);	
NtVector3 Foucus(0,0,0);
NtVector3 Up(0, 1, 0);
NtCamera Camera;
NtCamera LightCamera;
Model head,cube,sphere,diablo,plane;

int WindowWidth = 620;
int WindowHeight = 480;

std::shared_ptr<NtSofterRender> render = NtSofterRender::Instance(WindowWidth, WindowHeight);
float AlphaScale = 1;

std::shared_ptr<NtWindow> window = NtWindow::Instance(WindowWidth, WindowHeight);

ShadowMap shadowmap(render.get(), WindowWidth, WindowHeight);

RenderState rs;
CullEnum ce;

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
	vp.width =(float) WindowWidth-1;
	vp.height = (float)WindowHeight-1;
	vp.MinDepth = 0.001f;
	vp.MaxDepth = 1.f;
	vp.TopLeftX = vp.TopLeftY = 0.f;
	render->SetViewport(vp);
	Tex2D_UC  image = Tex2D_UC(window->GetWindowWidth(), window->GetWindowHeight(),NtColor4(1,1,1,1));
	window->FillWindow(image);
	window->PresentWindow();
}


void CubeAssemble()
{
	renderFlag = STPhone;
	NtMatrix4x4 world = NtMatrix4x4();
	if (renderFlag == STPhone) {
		render->CleanBackAndDepthBuffer();
		shadowmap.SetMainLight(LightCamera);
		auto depthTex = shadowmap.BuildDepthMap({ cube });
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
void SphereAssemble()
{
	render->CleanBackAndDepthBuffer();
	TPhoneAssemble(skyBox.unitsphere, Camera, render.get(), *MainLight.get(), AmbientLight);
}



void HeadPhoneAssemble()
{
	renderFlag = TPhone;
	NtMatrix4x4 world = NtMatrix4x4();
	if (renderFlag == STPhone) {
		render->CleanBackAndDepthBuffer();
		shadowmap.SetMainLight(LightCamera);
		auto depthTex = shadowmap.BuildDepthMap({ head });
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
		shadowmap.SetMainLight(LightCamera);
		auto depthTex = shadowmap.BuildDepthMap({ head });
		SSAOAssemble(head, Camera, render.get(), *MainLight.get(), AmbientLight);
	}

}


void Assemble(Model& model)
{
	render->SetRenderState(rs);
	render->SetCullState(ce);
	//renderFlag = TPhone;
	NtMatrix4x4 world = NtMatrix4x4();
	if (renderFlag == STPhone) {
		render->CleanBackAndDepthBuffer();
		shadowmap.SetMainLight(LightCamera);
		
		auto depthTex = shadowmap.BuildDepthMap({ model });
		//Present_image(depthTex.get());
		render->CleanBackAndDepthBuffer();
		STPhoneAssemble(model, Camera, render.get(), *MainLight.get(), AmbientLight, shadowmap);
	}
	else if (renderFlag == TPhone)
	{
		render->CleanBackAndDepthBuffer();
		TPhoneAssemble(model, Camera, render.get(), *MainLight.get(), AmbientLight);

	}
	else if (renderFlag == SSAO)
	{
		render->CleanBackAndDepthBuffer();
		shadowmap.SetMainLight(LightCamera);
		auto depthTex = shadowmap.BuildDepthMap({ model });
		SSAOAssemble(model, Camera, render.get(), *MainLight.get(), AmbientLight);
	}

}

/*
void Assemble()
{
	
	//Present_image(depthTex.get());
	//HeadPhoneAssemble();
	NtMatrix4x4 world = NtMatrix4x4();
	
	shadowmap.SetMainLight(LightCamera);
	auto depthTex = shadowmap.BuildDepthMap(world, diablo);	
	
	render->CleanBackAndDepthBuffer();
	//window->Present_image(Float4ImageToRGBAImage(FlotnFToFloat4F(*depthTex.get())));
	STPhoneAssemble(diablo, Camera, render.get(), *MainLight.get(), AmbientLight, shadowmap);
//	TPhoneAssemble(diablo, Camera, render.get(), *MainLight.get(), AmbientLight);
	//CubeAssemble();
	//SphereAssemble();
}*/

void ModelInit()
{
	head.init("Asset\\african_head.obj");
	 

	head.SetMainTexture("Asset\\african_head_diffuse.tga");
	head.GetDiffuseTex()->flip_vertically();
	//head.GetDiffuseTex()->flip_horizontally();
	//Present_image(Float4ImageToRGBAImage(*head.GetDiffuseTex()));

	//Present_image(NtUtility::Read_file("african_head_diffuse.tga"));
	//head.SetNormalTexture("african_head_nm.tga");
	//head.GetNormalTex()->flip_vertically();
	//head.GetNormalTex()->flip_horizontally();
	//Present_image(Float4ImageToRGBAImage(*head.GetNormalTex()));
	head.SetSpecularTexture("Asset\\african_head_spec.tga");
	//Present_image(Float4ImageToRGBAImage(*head.GetSpecularTex()));
	head.GetSpecularTex()->flip_vertically();
	//head.GetSpecularTex()->flip_horizontally();

	head.SetTangentTexture("Asset\\african_head_nm_tangent.tga");
	head.GetTangentTex()->flip_vertically();
	//head.GetTangentTex()->flip_horizontally();
	//Present_image(Float4ImageToRGBAImage(*head.GetTangentTex()));
	Material HeadMat;
	HeadMat.diffTextureId = 0;
	HeadMat.Roughness = 0.5;
	HeadMat.name = "Head";
	HeadMat.MaterialId = 0;
	head.SetMaterial(HeadMat);
	/*Cube*/
	cube.init("Asset\\cube.obj");
	Material CubeMat;
	
	CubeMat.name = "Cube";
	CubeMat.MaterialId = 0;
	cube.SetMaterial(CubeMat);
	/*Diablo*/
	diablo.init("Asset\\diablo3_pose\\diablo3_pose.obj");
	diablo.SetMainTexture("Asset\\diablo3_pose\\diablo3_pose_diffuse.tga");
	diablo.GetDiffuseTex()->flip_vertically();
	//diablo.GetDiffuseTex()->flip_horizontally();
	diablo.SetSpecularTexture("Asset\\diablo3_pose\\diablo3_pose_spec.tga");
	diablo.GetSpecularTex()->flip_vertically();
	//diablo.GetSpecularTex()->flip_horizontally();
	diablo.SetTangentTexture("Asset\\diablo3_pose\\diablo3_pose_nm_tangent.tga");
	diablo.GetTangentTex()->flip_vertically();
	//diablo.GetTangentTex()->flip_horizontally();
	diablo.SetEmissionTexture("Asset\\diablo3_pose\\diablo3_pose_glow.tga");
	diablo.GetEmissionTex()->flip_vertically();
	//diablo.GetEmissionTex()->flip_horizontally();
	Material DiabloMat;
	DiabloMat.diffTextureId = 0;
	DiabloMat.Roughness = 0.5;
	DiabloMat.name = "Diablo";
	DiabloMat.MaterialId = 0;
	diablo.SetMaterial(DiabloMat);

	/*plane*/
	plane = Geometry::plane(5, 5);
	Material plane;
	DiabloMat.diffTextureId = 0;
	DiabloMat.Roughness = 0.5;
	DiabloMat.name = "plane";
	DiabloMat.MaterialId = 0;
	diablo.SetMaterial(DiabloMat);
	
}

void diabloInit()
{
	

}
void SkyboxInit()
{
	skyBox.skyimg = std::make_shared<NtCubeTex4F>();
	Tex2D_UC PositiveX = NtUtility::Read_file("Asset\\riverside_west.BMP");
	Tex2D_UC NegativeX = NtUtility::Read_file("Asset\\riverside_east.BMP");
	Tex2D_UC PositiveY = NtUtility::Read_file("Asset\\riverside_up.BMP");
	Tex2D_UC NegativeY = NtUtility::Read_file("Asset\\riverside_down.BMP");
	Tex2D_UC NegativeZ = NtUtility::Read_file("Asset\\riverside_north.BMP");
	Tex2D_UC PositiveZ = NtUtility::Read_file("Asset\\riverside_south.BMP");
	skyBox.skyimg->Setface(RGBAImageToFloat4Image(PositiveX), Cubeface::POSITIVE_X);
	skyBox.skyimg->Setface(RGBAImageToFloat4Image(NegativeX), Cubeface::NEGATIVE_X);
	skyBox.skyimg->Setface(RGBAImageToFloat4Image(PositiveY), Cubeface::POSITIVE_Y);
	skyBox.skyimg->Setface(RGBAImageToFloat4Image(NegativeY), Cubeface::NEGATIVE_Y);
	skyBox.skyimg->Setface(RGBAImageToFloat4Image(PositiveZ), Cubeface::POSITIVE_Z);
	skyBox.skyimg->Setface(RGBAImageToFloat4Image(NegativeZ), Cubeface::NEGATIVE_Z);
	 
}


void ScenceInit()
{
	//MainLight->GetDirection()*-2.f
	LightCamera.LookAt(-MainLight->GetDirection()*2.f, NtVector3(0, 0, 0), NtVector3(0, 1, 0));
	//只需要将正交投影视景体覆盖整个场景即可
	LightCamera.SetVisualBody(6.f,6.f,0.1f,10.f);
	Camera.LookAt(Eye, Foucus, Up);
	Camera.SetViewFrustum(DegToRad(60), window->AspectRadio(), 0.01f, 100.f);	

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
					rs = Wireframe;
						
						break;
					}
				case SDLK_g:
					{
						rs = GroundShading;
						
						break;
					}
				case SDLK_b:
				{
			
					if (render->GetCullState() == Cullback)
						ce = NoCull;

					else
						ce = Cullback;
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
				case SDLK_q:
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

void shadowEffectShow()
{
	plane.SetWorld(Transiation(0, 0, -1.8));
	diablo.SetWorld(Transiation(0, 0, 0));
	render->CleanBackAndDepthBuffer();
	shadowmap.SetMainLight(LightCamera);

	std::vector<Model>models{ diablo,plane };

	auto depthTex = shadowmap.BuildDepthMap(models);
	//window->Present_image(Float4ImageToRGBAImage(FlotnFToFloat4F(*depthTex.get())));
	render->CleanBackAndDepthBuffer();
	STPhoneAssemble(diablo, Camera, render.get(), *MainLight.get(), AmbientLight, shadowmap);
	render->Draw();

	STPhoneAssemble(plane, Camera, render.get(), *MainLight.get(), AmbientLight, shadowmap);
	render->Draw();
}

void Rnder()
{
	render->SetRenderState(rs);
	render->SetCullState(ce);
	//Assemble(plane);
	
	render->CleanBackAndDepthBuffer();
	BlendAssemble(diablo, Camera, render.get(), *MainLight.get(), AmbientLight, AlphaScale);
	render->Draw();
	//skyBox.draw(render.get(), Camera);
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
	//SkyboxInit();
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


