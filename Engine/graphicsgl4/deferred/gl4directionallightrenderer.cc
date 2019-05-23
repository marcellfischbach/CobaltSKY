
#include <graphicsgl4/deferred/gl4directionallightrenderer.hh>
#include <graphicsgl4/deferred/gl4defaultcollector.hh>
#include <graphicsgl4/deferred/gl4gbuffer.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/graphics/irendertarget.hh>
#include <cobalt/graphics/ishader.hh>
#include <cobalt/graphics/ishaderattribute.hh>
#include <cobalt/graphics/itexture2d.hh>
#include <cobalt/graphics/itexture2darray.hh>
#include <cobalt/graphics/isampler.hh>
#include <cobalt/graphics/cscamera.hh>
#include <cobalt/graphics/csdirectionallight.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/core/cssettings.hh>
#include <cobalt/entity/csentity.hh>
#include <cobalt/entity/csgeometrydata.hh>
#include <cobalt/entity/csrenderstate.hh>
#include <cobalt/math/csclipper.hh>



cs::DirectionalLightRendererGL4::DirectionalLightRendererGL4(cs::iGraphics *renderer)
	: cs::LightRendererGL4(renderer)
	, m_colorBuffer(0)
	, m_colorBufferBlur(0)
	, m_depthBuffer(0)
{
	InitializeLightProgram(&m_programNoShadow, cs::ResourceLocator("${shaders}/deferred/DirectionalLight.asset"));
  m_attrLightDirectionNoShadow = m_programNoShadow.program->Get()->GetAttribute(cs::ShaderAttributeID("LightDirection"));

	InitializeLightProgram(&m_programPSSM, cs::ResourceLocator("${shaders}/deferred/DirectionalLightPSSM.asset"));
  m_attrLightDirectionPSSM = m_programPSSM.program->Get()->GetAttribute(cs::ShaderAttributeID("LightDirection"));
  m_attrShadowMap = m_programPSSM.program->Get()->GetAttribute(cs::ShaderAttributeID("ShadowMap"));

	//
	// load and init the shadow map renderer
  m_shadowMapRenderer.shader = cs::ResourceManager::Get()->GetOrLoad<cs::ShaderWrapper>(cs::ResourceLocator("${shaders}/deferred/DirectionalLightShadowPSSM.asset"));
  if (m_shadowMapRenderer.shader && m_shadowMapRenderer.shader->IsValid())
	{
    m_shadowMapRenderer.attrDepth = m_shadowMapRenderer.shader->Get()->GetAttribute(cs::ShaderAttributeID("Depth"));
    m_shadowMapRenderer.attrDistances = m_shadowMapRenderer.shader->Get()->GetAttribute(cs::ShaderAttributeID("Distances"));
    m_shadowMapRenderer.attrShadowColorMap = m_shadowMapRenderer.shader->Get()->GetAttribute(cs::ShaderAttributeID("ShadowColorMap"));
    m_shadowMapRenderer.attrShadowMap = m_shadowMapRenderer.shader->Get()->GetAttribute(cs::ShaderAttributeID("ShadowMap"));
    m_shadowMapRenderer.attrShadowMatsProjView = m_shadowMapRenderer.shader->Get()->GetAttribute(cs::ShaderAttributeID("ShadowMatsProjView"));
	}

  m_shadowMapBlurHori.shader = cs::ResourceManager::Get()->GetOrLoad<cs::ShaderWrapper>(cs::ResourceLocator("${shaders}/deferred/ShadowMapBlurHori.asset"));
  if (m_shadowMapBlurHori.shader)
  {
    m_shadowMapBlurHori.attrColor0 = m_shadowMapBlurHori.shader->Get()->GetAttribute(cs::ShaderAttributeID("Color0"));
  }

  m_shadowMapBlurVert.shader = cs::ResourceManager::Get()->GetOrLoad<cs::ShaderWrapper>(cs::ResourceLocator("${shaders}/deferred/ShadowMapBlurVert.asset"));
  if (m_shadowMapBlurVert.shader)
  {
    m_shadowMapBlurVert.attrColor0 = m_shadowMapBlurVert.shader->Get()->GetAttribute(cs::ShaderAttributeID("Color0"));
  }


	m_distances.x = cs::Settings::Get()->GetFloatValue("graphics.pssm.distances", 0, 5.0f);
	m_distances.y = cs::Settings::Get()->GetFloatValue("graphics.pssm.distances", 1, 25.0f);
	m_distances.z = cs::Settings::Get()->GetFloatValue("graphics.pssm.distances", 2, 120.0f); 
	m_shadowBufferSize = cs::Settings::Get()->GetIntValue("graphics.pssm.bufferSize", 0, 1024);

  printf("PSSM.Distances: %f %f %f\n", m_distances.x, m_distances.y, m_distances.z);
  printf("PSSM.MapSize : %d\n", m_shadowBufferSize);

	cs::ePixelFormat shadowBufferFormat = cs::ePF_R16G16F;
  m_colorBuffer = new cs::Texture2DArrayWrapper(renderer->CreateTexture2DArray(shadowBufferFormat, m_shadowBufferSize, m_shadowBufferSize, 3, false));
  m_depthBuffer = new cs::Texture2DArrayWrapper(renderer->CreateTexture2DArray(cs::ePF_D24S8, m_shadowBufferSize, m_shadowBufferSize, 3, false));

	cs::iSampler *colorSmplr = renderer->CreateSampler();
  colorSmplr->SetBorderColor(cs::Vector4f(1, 1, 1, 1));
  colorSmplr->SetFilter(cs::eFM_MinMagLinear);
  colorSmplr->SetAddressU(cs::eTAM_ClampBorder);
  colorSmplr->SetAddressV(cs::eTAM_ClampBorder);
  colorSmplr->SetAddressW(cs::eTAM_ClampBorder);
  cs::SamplerWrapper* colorSampler = new cs::SamplerWrapper(colorSmplr);

	m_colorBuffer->Get()->SetSampler(colorSampler);
	m_depthBuffer->Get()->SetSampler(m_depthSampler);


	m_shadowBuffer = static_cast<cs::iRenderTarget*>(renderer->CreateRenderTarget());
	m_shadowBuffer->Initialize(m_shadowBufferSize, m_shadowBufferSize);
	m_shadowBuffer->AddColorTexture(m_colorBuffer);
	m_shadowBuffer->SetDepthTexture(m_depthBuffer);
	if (!m_shadowBuffer->Finilize())
	{
		printf("Unable to finalize shadow buffer object.\n");
	}

  unsigned screenResolutionWidth = cs::Settings::Get()->GetIntValue("video.resolution", 0, 1366);
  unsigned screenResolutionHeight = cs::Settings::Get()->GetIntValue("video.resolution", 1, 768);

  unsigned shadowMapWidth = screenResolutionWidth / 2.0f;
  unsigned shadowMapHeight = screenResolutionHeight /2.0f;

  m_shadowMapRenderer.shadowMap = new cs::Texture2DWrapper(m_renderer->CreateTexture2D(cs::ePF_R8G8B8A8U, shadowMapWidth, shadowMapHeight, false));
  m_shadowMapRenderer.shadowMap->Get()->SetSampler(colorSampler);

  m_shadowMapRenderer.shadowRenderTarget = m_renderer->CreateRenderTarget();
  m_shadowMapRenderer.shadowRenderTarget->Initialize(shadowMapWidth, shadowMapHeight);
  m_shadowMapRenderer.shadowRenderTarget->AddColorTexture(m_shadowMapRenderer.shadowMap);
  m_shadowMapRenderer.shadowRenderTarget->SetDepthBuffer(shadowMapWidth, shadowMapHeight);
  if (!m_shadowMapRenderer.shadowRenderTarget->Finilize())
  {
    printf("Unable to finalize shadow map render target.\n");
  }

  m_shadowMapRenderer.shadowMapPingPong = new cs::Texture2DWrapper(m_renderer->CreateTexture2D(cs::ePF_R8G8B8A8U, shadowMapWidth, shadowMapHeight, false));
  m_shadowMapRenderer.shadowMapPingPong->Get()->SetSampler(colorSampler);

  m_shadowMapRenderer.shadowRenderTargetPingPong = m_renderer->CreateRenderTarget();
  m_shadowMapRenderer.shadowRenderTargetPingPong->Initialize(shadowMapWidth, shadowMapHeight);
  m_shadowMapRenderer.shadowRenderTargetPingPong->AddColorTexture(m_shadowMapRenderer.shadowMapPingPong);
  m_shadowMapRenderer.shadowRenderTargetPingPong->SetDepthBuffer(shadowMapWidth, shadowMapHeight);
  if (!m_shadowMapRenderer.shadowRenderTargetPingPong->Finilize())
  {
    printf("Unable to finalize shadow map render target ping-pong.\n");
  }


  colorSampler->Release();
}

  cs::DirectionalLightRendererGL4::~DirectionalLightRendererGL4()
{
	CS_RELEASE(m_colorBuffer);
	CS_RELEASE(m_depthBuffer);
	CS_RELEASE(m_colorBufferBlur);
}




void cs::DirectionalLightRendererGL4::Render(cs::Entity *root, cs::Camera *camera, cs::Light *light, cs::GBufferGL4 *gbuffer, cs::iRenderTarget *target)
{
	cs::eBlendMode blendModeSrcColor, blendModeSrcAlpha, blendModeDstColor, blendModeDstAlpha;
	m_renderer->GetBlendMode(blendModeSrcColor, blendModeSrcAlpha, blendModeDstColor, blendModeDstAlpha);
	bool blendEnabled = m_renderer->IsBlendEnabled();

	cs::DirectionalLight *directionalLight = static_cast<cs::DirectionalLight*>(light);

	bool shadow = directionalLight->IsCastingShadow();
	if (shadow)
	{
		RenderShadow(root, camera, directionalLight);
    RenderShadowMap(directionalLight, gbuffer);
    BlurShadowMap();
	}



	// now the final image will be assembled
	m_renderer->SetRenderTarget(target);
	m_renderer->SetViewport(target);
	m_renderer->SetBlendEnabled(blendEnabled);
	m_renderer->SetBlendMode(blendModeSrcColor, blendModeSrcAlpha, blendModeDstColor, blendModeDstAlpha);

	// from now on we will only render to the single color buffer
	m_renderer->SetRenderDestination(cs::eRD_Color0);

	LightProgram &prog = shadow ? m_programPSSM : m_programNoShadow;
  m_renderer->SetShader(prog.program->Get());
	m_renderer->InvalidateTextures();
	// bind the gbuffer this is used by the light program
	BindGBuffer(prog.gbuffer, gbuffer);
	BindLight(prog, light);

	if (shadow)
	{
		BindDirectionalLightPSSM(directionalLight);
	}
	else
	{
		BindDirectionalLightNoShadow(directionalLight);
	}

	m_renderer->RenderFullScreenFrame();
}


void cs::DirectionalLightRendererGL4::BindDirectionalLightNoShadow(cs::DirectionalLight *directionalLight)
{
	if (m_attrLightDirectionNoShadow)
	{
		m_attrLightDirectionNoShadow->Set(directionalLight->GetDirection());
	}

}



void cs::DirectionalLightRendererGL4::BindDirectionalLightPSSM(cs::DirectionalLight *directionalLight)
{
	if (m_attrLightDirectionPSSM)
	{
		m_attrLightDirectionPSSM->Set(directionalLight->GetDirection());
	}
	if (m_attrShadowMap)
	{
    cs::eTextureUnit tu = m_renderer->BindTexture(m_shadowMapRenderer.shadowMap->Get());
    m_attrShadowMap->Set(tu);
	}
}



void cs::DirectionalLightRendererGL4::UpdateProjectionMatrices()
{
	float min = FLT_MAX;
	float max = -FLT_MAX;
	const cs::Matrix4f &view = m_shadowCamAll;
	for (csSize i = 0; i < cs::eRQ_COUNT; ++i)
	{
		cs::Collection<cs::RenderState*> &queue = m_renderStates[i];
		for (csSize j = 0; j < queue.length; ++j)
		{
			cs::RenderState *renderState = queue[j];
			if (renderState)
			{
				const cs::BoundingBox &bbox = renderState->GetBoundingBox();
				for (unsigned k = 0; k < 8; ++k)
				{
					cs::Vector3f p;
					cs::Matrix4f::Transform(view, bbox.GetPoints()[k], p);
					if (p.y < min)
					{
						min = p.y;
					}
					if (p.y > max)
					{
						max = p.y;
					}
				}
			}
		}
	}
	for (unsigned i = 0; i < 3; ++i)
	{
		m_min[i].y = min;
		m_shadowNearFar[i].Set(m_min[i].y, m_max[i].y);
		m_renderer->GetOrthographicProjection(m_min[i].x, m_max[i].x, m_min[i].z, m_max[i].z, m_min[i].y, m_max[i].y, m_shadowProj[i]);
		cs::Matrix4f::Mult(m_shadowProj[i], m_shadowCam[i], m_shadowProjView[i]);

	}
}

cs::Clipper *cs::DirectionalLightRendererGL4::CreateClipper()
{
	cs::Vector3f topLeft(m_min[2].x, 0.0, m_max[2].z);
	cs::Vector3f topRight(m_max[2].x, 0.0, m_max[2].z);
	cs::Vector3f bottomLeft(m_min[2].x, 0.0, m_min[2].z);
	cs::Vector3f bottomRight(m_max[2].x, 0.0, m_min[2].z);

	const cs::Matrix4f &camInv = m_shadowCamInv[2];
	cs::Vector3f tl, tr, bl, br;
	cs::Matrix4f::Transform(camInv, topLeft, tl);
	cs::Matrix4f::Transform(camInv, topRight, tr);
	cs::Matrix4f::Transform(camInv, bottomLeft, bl);
	cs::Matrix4f::Transform(camInv, bottomRight, br);

	cs::Vector3f r;
	camInv.GetXAxis(r);
	cs::Vector3f l(r);
	l *= -1.0f;

	cs::Vector3f t;
	camInv.GetZAxis(t);
	cs::Vector3f b(t);
	b *= -1.0f;


	cs::PlaneClipper *clipper = new cs::PlaneClipper();
	clipper->AddPlane(cs::Plane(tl, r));
	clipper->AddPlane(cs::Plane(tl, b));
	clipper->AddPlane(cs::Plane(br, l));
	clipper->AddPlane(cs::Plane(br, t));
	return clipper;
}

void cs::DirectionalLightRendererGL4::RenderShadow(cs::Entity *root, cs::Camera *camera, const cs::DirectionalLight *light)
{
  m_renderer->PushRenderStates();

	//CalcPSSMMatrices(light, camera);
  CalcPSSMMatricesAlternative(light, camera);

	CalcShadowIntensity(light);

	cs::ScanConfig config;
  
	config.ScanShadowCasters = true;
	config.ScanNonShadowCasters = false;
	config.MainCameraPosition = camera->GetEye();
	// collect the shadow casting objects
	cs::DefaultCollectorGL4 collector(m_renderStates, 0);
	for (csSize i = 0; i < cs::eRQ_COUNT; ++i)
	{
		m_renderStates[i].Clear();
	}

	cs::Clipper *clipper = CreateClipper();
	root->Scan(clipper, m_renderer, &collector, config);
	delete clipper;


	UpdateProjectionMatrices();

	// setup the rendering 
	m_renderer->SetRenderTarget(m_shadowBuffer);
	m_renderer->SetViewport(m_shadowBuffer);
	m_renderer->SetRenderDestination(cs::eRD_Color0);
	m_renderer->SetDepthMask(true);
	m_renderer->SetDepthTest(true);
	m_renderer->SetDepthFunc(cs::eCM_LessOrEqual);
	m_renderer->SetColorMask(true, true, false, false);
	//m_renderer->SetColorMask(false, false, false, false);


	cs::eFaceSide current = m_renderer->GetCullFace();
	m_renderer->SetCullFace(cs::eFS_Back);

	m_renderer->Clear(true);
	m_renderer->SetShadowMatrices(m_shadowProjView, m_shadowProj, m_shadowCam, m_shadowNearFar, 3);
	m_renderer->SetBlendEnabled(false);

	for (csSize i = 0; i < cs::eRQ_COUNT; ++i)
	{
		cs::Collection<cs::RenderState*> &queue = m_renderStates[i];
		for (csSize j = 0; j < queue.length; ++j)
		{
			cs::RenderState *renderState = queue[j];
			if (renderState)
			{
				renderState->Render(m_renderer, cs::eRP_ShadowPSSM);
			}
		}
	}
	m_renderer->SetCullFace(cs::eFS_Back);
	m_renderer->SetColorMask(true, true, true, true);

  m_renderer->PopRenderStates();
}

void cs::DirectionalLightRendererGL4::RenderShadowMap(const cs::DirectionalLight *light, cs::GBufferGL4 *gBuffer)
{
  // now the final image will be assembled
  m_renderer->SetRenderTarget(m_shadowMapRenderer.shadowRenderTarget);
  m_renderer->SetViewport(m_shadowMapRenderer.shadowRenderTarget);
  m_renderer->SetBlendEnabled(false);
  m_renderer->SetColorMask(true, true, true, true);

  // from now on we will only render to the single color buffer
  m_renderer->SetRenderDestination(cs::eRD_Color0);

  m_renderer->SetShader(m_shadowMapRenderer.shader->Get());
  m_renderer->InvalidateTextures();

  if (m_shadowMapRenderer.attrDistances)
  {
    m_shadowMapRenderer.attrDistances->Set(m_distances);
  }

  if (m_shadowMapRenderer.attrShadowMatsProjView)
  {
    m_shadowMapRenderer.attrShadowMatsProjView->Set(m_shadowProjView, 3);
  }
  if (m_shadowMapRenderer.attrDepth)
  {
    cs::eTextureUnit tu = m_renderer->BindTexture(gBuffer->GetDepth()->Get());
    m_shadowMapRenderer.attrDepth->Set(tu);
  }
  if (m_shadowMapRenderer.attrShadowMap)
  {
    cs::eTextureUnit tu = m_renderer->BindTexture(m_depthBuffer->Get());
    m_shadowMapRenderer.attrShadowMap->Set(tu);
  }
  
  if (m_shadowMapRenderer.attrShadowColorMap)
  {
    cs::eTextureUnit tu = m_renderer->BindTexture(m_colorBuffer->Get());
    m_shadowMapRenderer.attrShadowColorMap->Set(tu);
  }
  /*
  */
  m_renderer->RenderFullScreenFrame();
}

void cs::DirectionalLightRendererGL4::BlurShadowMap()
{
  //
  // Blur the shadow map first in horizontal direction
  m_renderer->SetRenderTarget(m_shadowMapRenderer.shadowRenderTargetPingPong);
  m_renderer->SetViewport(m_shadowMapRenderer.shadowRenderTargetPingPong);
  m_renderer->SetBlendEnabled(false);
  m_renderer->SetColorMask(true, true, true, true);
  m_renderer->SetRenderDestination(cs::eRD_Color0);
  m_renderer->SetShader(m_shadowMapBlurHori.shader->Get());
  m_renderer->InvalidateTextures();

  if (m_shadowMapBlurHori.attrColor0)
  {
    cs::eTextureUnit tu = m_renderer->BindTexture(m_shadowMapRenderer.shadowMap->Get());
    m_shadowMapBlurHori.attrColor0->Set(tu);
  }
  m_renderer->RenderFullScreenFrame();



  //
  // Nor blur the shadow map in vertical direction
  m_renderer->SetRenderTarget(m_shadowMapRenderer.shadowRenderTarget);
  m_renderer->SetViewport(m_shadowMapRenderer.shadowRenderTarget);
  m_renderer->SetBlendEnabled(false);
  m_renderer->SetColorMask(true, true, true, true);
  m_renderer->SetRenderDestination(cs::eRD_Color0);
  m_renderer->SetShader(m_shadowMapBlurVert.shader->Get());
  m_renderer->InvalidateTextures();

  if (m_shadowMapBlurVert.attrColor0)
  {
    cs::eTextureUnit tu = m_renderer->BindTexture(m_shadowMapRenderer.shadowMapPingPong->Get());
    m_shadowMapBlurVert.attrColor0->Set(tu);
  }
  m_renderer->RenderFullScreenFrame();

}

void cs::DirectionalLightRendererGL4::CalcPSSMMatrices(const cs::DirectionalLight *light, const cs::Camera *camera)
{
	float dists[] = { 0.0f, m_distances.x, m_distances.y, m_distances.z };
	cs::Vector3f points[8];

	for (csSize i = 0; i < 3; ++i)
	{
		camera->GetPlanePoints(0, &points[0]);
		camera->GetPlanePoints(dists[i + 1], &points[4]);
		CalcMatrix(light->GetDirection(), camera->GetEye(), 8, points, m_shadowCam[i], m_shadowCamInv[i], m_min[i], m_max[i]);
	}

	camera->GetPlanePoints(0, &points[0]);
	camera->GetPlanePoints(m_distances.z, &points[4]);
	CalcMatrix(light->GetDirection(), camera->GetEye(), 8, points, m_shadowCamAll, m_shadowCamInvAll, m_minAll, m_maxAll);
}

void cs::DirectionalLightRendererGL4::CalcMatrix(const cs::Vector3f &dir, const cs::Vector3f &cameraPos, csSize numPoints, cs::Vector3f *points, cs::Matrix4f &cam, cs::Matrix4f &camInv, cs::Vector3f &min, cs::Vector3f &max) const
{
  /*
	cs::Vector3f spot;
	for (csSize i = 0; i < numPoints; i++)
	{
		cs::Vector3f::Add(spot, points[i], spot);
	}
	cs::Vector3f::Div(spot, (float)numPoints, spot);

	cs::Vector3f eye;
	cs::Vector3f::Mul(dir, -100.0f, eye);
	cs::Vector3f::Add(eye, spot, eye);
  */
	cs::Vector3f up(0.0f, 0.0f, 1.0f);
	if (dir.z >= 0.9999999)
	{
		up.Set(1.0f, 0.0f, 0.0f);
	}
	else if (dir.z <= -0.9999999)
	{
		up.Set(-1.0f, 0.0f, 0.0f);
	}

  cs::Vector3f at = cameraPos + dir;

	cam.SetLookAt(cameraPos, at, up);
	camInv.SetLookAtInv(cameraPos, at, up);

	min.Set(FLT_MAX, FLT_MAX, FLT_MAX);
	max.Set(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	cs::Vector3f t;
	for (csSize i = 0; i < numPoints; i++)
	{
		cs::Vector3f &p = points[i];
		cs::Matrix4f::Transform(cam, p, t);
		if (min.x > t.x)
		{
			min.x = t.x;
		}
		if (min.y > t.y)
		{
			min.y = t.y;
		}
		if (min.z > t.z)
		{
			min.z = t.z;
		}
		if (max.x < t.x)
		{
			max.x = t.x;
		}
		if (max.y < t.y)
		{
			max.y = t.y;
		}
		if (max.z < t.z)
		{
			max.z = t.z;
		}
	}
}




void cs::DirectionalLightRendererGL4::CalcPSSMMatricesAlternative(const cs::DirectionalLight *light, const cs::Camera *camera)
{
  float dists[] = { 0.0f, m_distances.x, m_distances.y, m_distances.z };
  cs::Vector3f points[8];

  for (csSize i = 0; i < 3; ++i)
  {
    camera->GetPlanePoints(0, &points[0]);
    camera->GetPlanePoints(dists[i + 1], &points[4]);
    CalcMatrixAlternative(light->GetDirection(), camera->GetEye(), 8, points, m_shadowCam[i], m_shadowCamInv[i], m_min[i], m_max[i]);
  }

  camera->GetPlanePoints(0, &points[0]);
  camera->GetPlanePoints(m_distances.z, &points[4]);
  CalcMatrixAlternative(light->GetDirection(), camera->GetEye(), 8, points, m_shadowCamAll, m_shadowCamInvAll, m_minAll, m_maxAll);
}

void cs::DirectionalLightRendererGL4::CalcMatrixAlternative(const cs::Vector3f &dir, const cs::Vector3f &cameraPos, csSize numPoints, cs::Vector3f *points, cs::Matrix4f &cam, cs::Matrix4f &camInv, cs::Vector3f &min, cs::Vector3f &max) const
{
  cs::Vector3f up(0.0f, 0.0f, 1.0f);
  if (dir.z >= 0.9999999)
  {
    up.Set(1.0f, 0.0f, 0.0f);
  }
  else if (dir.z <= -0.9999999)
  {
    up.Set(-1.0f, 0.0f, 0.0f);
  }


  cs::Vector3f center(0.0f, 0.0f, 0.0f);
  for (size_t i = 0; i < numPoints; ++i)
  {
    center += points[i];
  }
  center /= (float)numPoints;

  float maxDistSqr = 0.0f;
  for (size_t i = 0; i < numPoints; ++i)
  {
    float distSqr = (points[i] - center).Dot();
    if (distSqr > maxDistSqr)
    {
      maxDistSqr = distSqr;
    }
  }

  cs::Vector3f at = center + dir;
  cam.SetLookAt(center, at, up);
  camInv.SetLookAtInv(center, at, up);

  float radius = sqrt(maxDistSqr);
  float sizePerPixel = 2.0f * radius / (float)m_shadowBufferSize;

  cs::Vector3f c = cs::Matrix4f::Mult(cam, center, c);

  c.x -= fmodf(c.x, sizePerPixel);
  c.y -= fmodf(c.y, sizePerPixel);
  c.z -= fmodf(c.z, sizePerPixel);

  cs::Matrix4f::Mult(camInv, c, center);

  at = center + dir;
  cam.SetLookAt(center, at, up);
  camInv.SetLookAtInv(center, at, up);


  min.x = -radius;
  min.y = -radius;
  min.z = -radius;

  max.x = radius;
  max.y = radius;
  max.z = radius;


}

