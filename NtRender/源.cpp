
#include"SDL.h"
#include<stdio.h>
#include<iostream>
#include"Nt.h"
#include"NtRender.h"
#include"Geometry.h"
#include"Model.h"
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
NtVector3 Light_dir(0, 0, 1);
/* Camera */
NtVector3 Eye(0,0,1);
NtVector3 Foucus(0,0,0);
NtVector3 Up(0, 1, 0);
NtCamera Camera;
std::shared_ptr<NtSofterRender> render = NtSofterRender::Instance(640, 480);


std::shared_ptr<NtWindow> window = NtWindow::Instance(640, 480);



void WindowInit()
{
	NtViewport vp;
	vp.width = 640;
	vp.height = 480;
	vp.MinDepth = 0.001;
	vp.MaxDepth = 1;
	vp.TopLeftX = vp.TopLeftY = 0;
	render->SetViewport(vp);
	NtImage<Uint32>image = NtImage<Uint32>(window->GetWindowWidth(), window->GetWindowHeight());
	window->FillWindow(image);
	window->PresentWindow();
}


void HeadAssemble()
{

	
	render->SetLightDir(Light_dir);
	Model head("african_head.obj");
	Model cube("cube.obj");
	render->SetVertexBuffer(head.GetVertexsBuffer());
	render->SetIndexBuffer(head.GetIndicesBuffer());
	
	NtMatrix4x4 world = NtMatrix4x4();
	render->SetWorldMatrix(world);
	std::shared_ptr< NtImage<Uint32>>difftexture = std::make_shared<NtImage<Uint32>>();
	NtUtility::Read_Tga_file("african_head_diffuse.tga",difftexture.get());
	render->AddTexture(0, difftexture);
	GroudShader* gs = new GroudShader();
	
	TGroudShader*tgs = new TGroudShader();
	render->SetShader(tgs);

}

void CubeAssemble()
{
	Model cube("cube.obj");
	render->SetVertexBuffer(cube.GetVertexsBuffer());
	render->SetIndexBuffer(cube.GetIndicesBuffer());
	NtMatrix4x4 world = NtMatrix4x4();
	render->SetWorldMatrix(world);
	CGroudShader*cgs = new CGroudShader();
}

void ModelInit()
{
	Model head("african_head.obj");
}

void ScenceInit()
{
	

	Light_dir.normalize();
	Camera.LookAt(Eye, Foucus, Up);
	Camera.SetViewFrustum(DegToRad(90), window->AspectRadio(), 0.1, 1000);
	render->SetProjMatrix(Camera.GetProjMatrix());
	Assemble();
}



void UpdateScence(float dt)
{
	
	Camera.Update();
	render->SetViewMatrix(Camera.GetViewMatrix());
	
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


