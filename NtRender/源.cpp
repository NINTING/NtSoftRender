
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
SDL_Window* gWindow = NULL;
SDL_Renderer* gRender = NULL;
SDL_Texture* gTexture = NULL;

#define DegToRad(x) M_PI/180.0*(x)
const double MS_PER_FRAME = 1000.0/10;

NtVector4 AmbientLight(0.2f, 0.2f, 0.2f,1.f);
std::shared_ptr<Light> MainLight = CreateDirectionalLight(NtVector3(1.f, 1.f, 1.f), NtVector3(0.f, 0.f, 1.f));

/* Camera */
NtVector3 Eye(0,0,1.5);
NtVector3 Foucus(0,0,0);
NtVector3 Up(0, 1, 0);
NtCamera Camera;
Model head,cube;

int WindowWidth = 640;
int WindowHeight = 480;

std::shared_ptr<NtSofterRender> render = NtSofterRender::Instance(WindowWidth, WindowHeight);


std::shared_ptr<NtWindow> window = NtWindow::Instance(WindowWidth, WindowHeight);



void WindowInit()
{
	NtViewport vp;
	vp.width = WindowWidth;
	vp.height = WindowHeight;
	vp.MinDepth = 0.001;
	vp.MaxDepth = 1;
	vp.TopLeftX = vp.TopLeftY = 0;
	render->SetViewport(vp);
	NtImage image = NtImage (window->GetWindowWidth(), window->GetWindowHeight(),4);
	window->FillWindow(image);
	window->PresentWindow();
}


void CubeAssemble()
{

	cube.Assemble(render.get());
	CGroudShader*cgs = new CGroudShader();
	render->SetShader(cgs);
}


void HeadAssemble()
{


	NtMatrix4x4 world = NtMatrix4x4();

	
	TGroudShader*tgs = new TGroudShader();
	PhoneShader*pgs = new PhoneShader();
	pgs->diffuseTex = head.GetDiffuseTex();
	pgs->w = world;
	pgs->v = Camera.GetViewMatrix();
	pgs->p = Camera.GetProjMatrix();
	pgs->EyePosW = Camera.GetPos();
	pgs->DirectionalLights.push_back(*MainLight->GetLightConstant());
	pgs->mat = head.GetMaterial();
	pgs->normalTex = head.GetNormalTex();
	pgs->specularTex = head.GetSpecularTex();
	pgs->Ambient = AmbientLight;

	render->SetVertexBuffer(head.GetVertexsBuffer());
	render->SetIndexBuffer(head.GetIndicesBuffer());
	render->SetShader(pgs);

	/*
	Model cube("cube.obj");
	render->SetVertexBuffer(head.GetVertexsBuffer());
	render->SetIndexBuffer(head.GetIndicesBuffer());

	NtMatrix4x4 world = NtMatrix4x4();
	render->SetWorldMatrix(world);

	std::shared_ptr< NtImage<Uint32>>difftexture = std::make_shared<NtImage<Uint32>>();
	NtUtility::Read_Tga_file("african_head_diffuse.tga",difftexture.get());
	render->AddTexture(0, difftexture);
	*/
}

void Assemble()
{
	//sCubeAssemble();
	HeadAssemble();
}

void Present_image(NtImage*img)
{
	window->FillWindow(*img);

	window->PresentWindow();
}

void ModelInit()
{
	head.init("african_head.obj");
	head.SetTexture("african_head_diffuse.tga");
	head.SetNotmalTexture("african_head_nm.tga");
	head.SetSpecularTexture("african_head_spec.tga");
	Present_image(head.GetSpecularTex().get());
	Material HeadMat;
	HeadMat.diffTextureId = 0;
	HeadMat.Roughness = 0.5;
	HeadMat.name = "Head";
	
	HeadMat.MaterialId = 0;
	head.SetMaterial(HeadMat);
	cube.init("cube.obj");
	Material CubeMat;
	CubeMat.name = "Cube";
	CubeMat.MaterialId = 0;
	cube.SetMaterial(CubeMat);
}

void ScenceInit()
{
	

	Camera.LookAt(Eye, Foucus, Up);
	Camera.SetViewFrustum(DegToRad(90), window->AspectRadio(), 0.1, 1000);
	render->SetProjMatrix(Camera.GetProjMatrix());

}



void UpdateScence(float dt)
{
	
	Camera.Update();
	render->SetViewMatrix(Camera.GetViewMatrix());
	render->SetCameraPos(Camera.GetPos());
}


bool ProcessInput(float dt)
{
	SDL_Event event;
	static bool isMouseLeftDown = false;
	int lastX, lastY;
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
					Camera.Walk(3 * dt);
					break;
				case SDLK_s:
					{
						Camera.Walk(-3 * dt);
						break;
					}
				case SDLK_a:
					{
						Camera.Strafe(-3 * dt);
						break;
					}
				case SDLK_d:
					{
						Camera.Strafe(3 * dt);
						break;
					}
				case SDLK_v:
					{
						render->SetRenderState(NtSofterRender::Wireframe);
						break;
					}
				case SDLK_g:
					{
						render->SetRenderState(NtSofterRender::GroundShading);
						break;
					}
				case SDLK_b:
				{
					if(render->GetBackCull())
						render->SetBackCull(false);
					else
						render->SetBackCull(true);
					break;
				}
				default:
					break;
				}
			}
			case SDL_MOUSEMOTION:
				//printf("Êó±êÒÆ¶¯ %d\n", isMouseLeftDown);
				if (isMouseLeftDown) {
					int px = event.motion.xrel;
					int py = event.motion.yrel;

					Camera.Pitch(DegToRad(py*0.1));
					Camera.RotateY(DegToRad(px*0.1));
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


	window->FillWindow(render->Present());
	
	window->PresentWindow();
	
}


int main(int argc, char*argv[])
{
	
 	int frameCount = 0;
	int acculateElapse = 0;
	float fps = 0;
	//image.FillImage(Red32);
	SDL_Event event;
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


