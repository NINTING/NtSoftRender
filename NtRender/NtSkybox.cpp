#include"NtCube.h"
#include"Nt.h"
#include"NtRender.h"
#include"NtSkybox.h"
#include"Effect.h"
#include"Geometry.h"

NtSkybox::NtSkybox()
{
	unitsphere =  Geometry::Sphere(1, 15,15);
}


void NtSkybox::draw(NtSofterRender* render, const NtCamera& camera)
{
	SkyboxShader *ss = new SkyboxShader();
	ss->w = NtMatrix4x4();
	ss->v = camera.GetViewMatrix();
	ss->p = camera.GetProjMatrix();

	ss->eyePosW = camera.GetPos();
	ss->skybox = skyimg;
	render->SetRenderTarget(ZwriteOff);
	render->SetRenderTarget(BackBufferOn);
	render->SetCullState(NoCull);
	render->SetRenderState(GroundShading);
//	render->SetRenderState(Wireframe);
	render->SetZTestStat(LessEqual);
	render->SetVertexBuffer(unitsphere.GetVertexsBuffer());
	render->SetIndexBuffer(unitsphere.GetIndicesBuffer());
	render->SetShader(ss);
	render->Draw(); 
	render->SetZTestStat(Less);
}


