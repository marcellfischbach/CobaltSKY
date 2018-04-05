
#include <graphicsgl4/deferred/gl4directionallightrenderer.hh>
#include <graphicsgl4/deferred/gl4defaultcollector.hh>
#include <graphicsgl4/deferred/gl4gbuffer.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/graphics/irendertarget.hh>
#include <cobalt/graphics/isampler.hh>
#include <cobalt/graphics/ishader.hh>
#include <cobalt/graphics/ishaderattribute.hh>
#include <cobalt/graphics/itexture2d.hh>
#include <cobalt/graphics/itexture2darray.hh>
#include <cobalt/graphics/cscamera.hh>
#include <cobalt/graphics/csdirectionallight.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/core/cssettings.hh>
#include <cobalt/entity/csentity.hh>
#include <cobalt/entity/csgeometrydata.hh>
#include <cobalt/entity/csrenderstate.hh>
#include <cobalt/math/csclipper.hh>



csDirectionalLightRendererGL4::csDirectionalLightRendererGL4(iGraphics *renderer)
	: csLightRendererGL4(renderer)
	, m_colorBuffer(0)
	, m_colorBufferBlur(0)
	, m_depthBuffer(0)
{
	InitializeLightProgram(&m_programNoShadow, csResourceLocator("${shaders}/deferred/DirectionalLight.xasset"));
	m_attrLightDirectionNoShadow = m_programNoShadow.program->GetAttribute(csShaderAttributeID("LightDirection"));

	InitializeLightProgram(&m_programPSSM, csResourceLocator("${shaders}/deferred/DirectionalLightPSSM.xasset"));
	m_attrLightDirectionPSSM = m_programPSSM.program->GetAttribute(csShaderAttributeID("LightDirection"));
	m_attrShadowMap = m_programPSSM.program->GetAttribute(csShaderAttributeID("ShadowMap"));

	//
	// load and init the shadow map renderer
	m_shadowMapRenderer.shader = csResourceManager::Get()->GetOrLoad<iShader>(csResourceLocator("${shaders}/deferred/DirectionalLightShadowPSSM.xasset"));
	if (m_shadowMapRenderer.shader)
	{
		m_shadowMapRenderer.attrDepth = m_shadowMapRenderer.shader->GetAttribute(csShaderAttributeID("Depth"));
		m_shadowMapRenderer.attrDistances = m_shadowMapRenderer.shader->GetAttribute(csShaderAttributeID("Distances"));
		m_shadowMapRenderer.attrShadowColorMap = m_shadowMapRenderer.shader->GetAttribute(csShaderAttributeID("ShadowColorMap"));
		m_shadowMapRenderer.attrShadowMap = m_shadowMapRenderer.shader->GetAttribute(csShaderAttributeID("ShadowMap"));
		m_shadowMapRenderer.attrShadowMatsProjView = m_shadowMapRenderer.shader->GetAttribute(csShaderAttributeID("ShadowMatsProjView"));
	}

  m_shadowMapBlurHori.shader = csResourceManager::Get()->GetOrLoad<iShader>(csResourceLocator("${shaders}/deferred/ShadowMapBlurHori.xasset"));
  if (m_shadowMapBlurHori.shader)
  {
    m_shadowMapBlurHori.attrColor0 = m_shadowMapBlurHori.shader->GetAttribute(csShaderAttributeID("Color0"));
  }

  m_shadowMapBlurVert.shader = csResourceManager::Get()->GetOrLoad<iShader>(csResourceLocator("${shaders}/deferred/ShadowMapBlurVert.xasset"));
  if (m_shadowMapBlurVert.shader)
  {
    m_shadowMapBlurVert.attrColor0 = m_shadowMapBlurVert.shader->GetAttribute(csShaderAttributeID("Color0"));
  }


	m_distances.x = csSettings::Get()->GetFloatValue("graphics.pssm.distances", 0, 5.0f);
	m_distances.y = csSettings::Get()->GetFloatValue("graphics.pssm.distances", 1, 25.0f);
	m_distances.z = csSettings::Get()->GetFloatValue("graphics.pssm.distances", 2, 120.0f); 
	m_shadowBufferSize = csSettings::Get()->GetIntValue("graphics.pssm.bufferSize", 0, 1024);

  printf("PSSM.Distances: %f %f %f\n", m_distances.x, m_distances.y, m_distances.z);
  printf("PSSM.MapSize : %d\n", m_shadowBufferSize);

	csPixelFormat shadowBufferFormat = ePF_R16G16F;
	m_colorBuffer = renderer->CreateTexture2DArray(shadowBufferFormat, m_shadowBufferSize, m_shadowBufferSize, 3, false);
	m_depthBuffer = renderer->CreateTexture2DArray(ePF_D24S8, m_shadowBufferSize, m_shadowBufferSize, 3, false);

	iSampler *colorSampler = renderer->CreateSampler();
	colorSampler->SetBorderColor(csVector4f(1, 1, 1, 1));
	colorSampler->SetFilter(eFM_MinMagLinear);
	colorSampler->SetAddressU(eTAM_ClampBorder);
	colorSampler->SetAddressV(eTAM_ClampBorder);
	colorSampler->SetAddressW(eTAM_ClampBorder);

	m_colorBuffer->SetSampler(colorSampler);
	m_depthBuffer->SetSampler(m_depthSampler);


	m_shadowBuffer = static_cast<iRenderTarget*>(renderer->CreateRenderTarget());
	m_shadowBuffer->Initialize(m_shadowBufferSize, m_shadowBufferSize);
	m_shadowBuffer->AddColorTexture(m_colorBuffer);
	m_shadowBuffer->SetDepthTexture(m_depthBuffer);
	if (!m_shadowBuffer->Finilize())
	{
		printf("Unable to finalize shadow buffer object.\n");
	}

  unsigned screenResolutionWidth = csSettings::Get()->GetIntValue("video.resolution", 0, 1366);
  unsigned screenResolutionHeight = csSettings::Get()->GetIntValue("video.resolution", 1, 768);

  unsigned shadowMapWidth = screenResolutionWidth / 2.0f;
  unsigned shadowMapHeight = screenResolutionHeight /2.0f;

  m_shadowMapRenderer.shadowMap = m_renderer->CreateTexture2D(ePF_R8G8B8A8U, shadowMapWidth, shadowMapHeight, false);
  m_shadowMapRenderer.shadowMap->SetSampler(colorSampler);

  m_shadowMapRenderer.shadowRenderTarget = m_renderer->CreateRenderTarget();
  m_shadowMapRenderer.shadowRenderTarget->Initialize(shadowMapWidth, shadowMapHeight);
  m_shadowMapRenderer.shadowRenderTarget->AddColorTexture(m_shadowMapRenderer.shadowMap);
  m_shadowMapRenderer.shadowRenderTarget->SetDepthBuffer(shadowMapWidth, shadowMapHeight);
  if (!m_shadowMapRenderer.shadowRenderTarget->Finilize())
  {
    printf("Unable to finalize shadow map render target.\n");
  }

  m_shadowMapRenderer.shadowMapPingPong = m_renderer->CreateTexture2D(ePF_R8G8B8A8U, shadowMapWidth, shadowMapHeight, false);
  m_shadowMapRenderer.shadowMapPingPong->SetSampler(colorSampler);

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

  csDirectionalLightRendererGL4::~csDirectionalLightRendererGL4()
{
	CS_RELEASE(m_colorBuffer);
	CS_RELEASE(m_depthBuffer);
	CS_RELEASE(m_colorBufferBlur);
}




void csDirectionalLightRendererGL4::Render(csEntity *root, csCamera *camera, csLight *light, csGBufferGL4 *gbuffer, iRenderTarget *target)
{
	csBlendMode blendModeSrcColor, blendModeSrcAlpha, blendModeDstColor, blendModeDstAlpha;
	m_renderer->GetBlendMode(blendModeSrcColor, blendModeSrcAlpha, blendModeDstColor, blendModeDstAlpha);
	bool blendEnabled = m_renderer->IsBlendEnabled();

	csDirectionalLight *directionalLight = static_cast<csDirectionalLight*>(light);

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
	m_renderer->SetRenderDestination(eRD_Color0);

	LightProgram &prog = shadow ? m_programPSSM : m_programNoShadow;
	m_renderer->SetShader(prog.program);
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


void csDirectionalLightRendererGL4::BindDirectionalLightNoShadow(csDirectionalLight *directionalLight)
{
	if (m_attrLightDirectionNoShadow)
	{
		m_attrLightDirectionNoShadow->Set(directionalLight->GetDirection());
	}

}



void csDirectionalLightRendererGL4::BindDirectionalLightPSSM(csDirectionalLight *directionalLight)
{
	if (m_attrLightDirectionPSSM)
	{
		m_attrLightDirectionPSSM->Set(directionalLight->GetDirection());
	}
	if (m_attrShadowMap)
	{
    csTextureUnit tu = m_renderer->BindTexture(m_shadowMapRenderer.shadowMap);
    m_attrShadowMap->Set(tu);
	}
}



void csDirectionalLightRendererGL4::UpdateProjectionMatrices()
{
	float min = FLT_MAX;
	float max = -FLT_MAX;
	const csMatrix4f &view = m_shadowCamAll;
	for (csSize i = 0; i < eRQ_COUNT; ++i)
	{
		csCollection<csRenderState*> &queue = m_renderStates[i];
		for (csSize j = 0; j < queue.length; ++j)
		{
			csRenderState *renderState = queue[j];
			if (renderState)
			{
				const csBoundingBox &bbox = renderState->GetBoundingBox();
				for (unsigned k = 0; k < 8; ++k)
				{
					csVector3f p;
					csMatrix4f::Transform(view, bbox.GetPoints()[k], p);
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
		csMatrix4f::Mult(m_shadowProj[i], m_shadowCam[i], m_shadowProjView[i]);

	}
}

csClipper *csDirectionalLightRendererGL4::CreateClipper()
{
	csVector3f topLeft(m_min[2].x, 0.0, m_max[2].z);
	csVector3f topRight(m_max[2].x, 0.0, m_max[2].z);
	csVector3f bottomLeft(m_min[2].x, 0.0, m_min[2].z);
	csVector3f bottomRight(m_max[2].x, 0.0, m_min[2].z);

	const csMatrix4f &camInv = m_shadowCamInv[2];
	csVector3f tl, tr, bl, br;
	csMatrix4f::Transform(camInv, topLeft, tl);
	csMatrix4f::Transform(camInv, topRight, tr);
	csMatrix4f::Transform(camInv, bottomLeft, bl);
	csMatrix4f::Transform(camInv, bottomRight, br);

	csVector3f r;
	camInv.GetXAxis(r);
	csVector3f l(r);
	l *= -1.0f;

	csVector3f t;
	camInv.GetZAxis(t);
	csVector3f b(t);
	b *= -1.0f;


	csPlaneClipper *clipper = new csPlaneClipper();
	clipper->AddPlane(csPlane(tl, r));
	clipper->AddPlane(csPlane(tl, b));
	clipper->AddPlane(csPlane(br, l));
	clipper->AddPlane(csPlane(br, t));
	return clipper;
}

void csDirectionalLightRendererGL4::RenderShadow(csEntity *root, csCamera *camera, const csDirectionalLight *light)
{
  m_renderer->PushRenderStates();

	//CalcPSSMMatrices(light, camera);
  CalcPSSMMatricesAlternative(light, camera);

	CalcShadowIntensity(light);

	csScanConfig config;
  
	config.ScanShadowCasters = true;
	config.ScanNonShadowCasters = false;
	config.MainCameraPosition = camera->GetEye();
	// collect the shadow casting objects
	csDefaultCollectorGL4 collector(m_renderStates, 0);
	for (csSize i = 0; i < eRQ_COUNT; ++i)
	{
		m_renderStates[i].Clear();
	}

	csClipper *clipper = CreateClipper();
	root->Scan(clipper, m_renderer, &collector, config);
	delete clipper;


	UpdateProjectionMatrices();

	// setup the rendering 
	m_renderer->SetRenderTarget(m_shadowBuffer);
	m_renderer->SetViewport(m_shadowBuffer);
	m_renderer->SetRenderDestination(eRD_Color0);
	m_renderer->SetDepthMask(true);
	m_renderer->SetDepthTest(true);
	m_renderer->SetDepthFunc(eCM_LessOrEqual);
	m_renderer->SetColorMask(true, true, false, false);
	//m_renderer->SetColorMask(false, false, false, false);


	csFaceSide current = m_renderer->GetCullFace();
	m_renderer->SetCullFace(eFS_Back);

	m_renderer->Clear(true);
	m_renderer->SetShadowMatrices(m_shadowProjView, m_shadowProj, m_shadowCam, m_shadowNearFar, 3);
	m_renderer->SetBlendEnabled(false);

	for (csSize i = 0; i < eRQ_COUNT; ++i)
	{
		csCollection<csRenderState*> &queue = m_renderStates[i];
		for (csSize j = 0; j < queue.length; ++j)
		{
			csRenderState *renderState = queue[j];
			if (renderState)
			{
				renderState->Render(m_renderer, eRP_ShadowPSSM);
			}
		}
	}
	m_renderer->SetCullFace(eFS_Back);
	m_renderer->SetColorMask(true, true, true, true);

  m_renderer->PopRenderStates();
}

void csDirectionalLightRendererGL4::RenderShadowMap(const csDirectionalLight *light, csGBufferGL4 *gBuffer)
{
  // now the final image will be assembled
  m_renderer->SetRenderTarget(m_shadowMapRenderer.shadowRenderTarget);
  m_renderer->SetViewport(m_shadowMapRenderer.shadowRenderTarget);
  m_renderer->SetBlendEnabled(false);
  m_renderer->SetColorMask(true, true, true, true);

  // from now on we will only render to the single color buffer
  m_renderer->SetRenderDestination(eRD_Color0);

  m_renderer->SetShader(m_shadowMapRenderer.shader);
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
    csTextureUnit tu = m_renderer->BindTexture(gBuffer->GetDepth());
    m_shadowMapRenderer.attrDepth->Set(tu);
  }
  if (m_shadowMapRenderer.attrShadowMap)
  {
    csTextureUnit tu = m_renderer->BindTexture(m_depthBuffer);
    m_shadowMapRenderer.attrShadowMap->Set(tu);
  }
  
  if (m_shadowMapRenderer.attrShadowColorMap)
  {
    csTextureUnit tu = m_renderer->BindTexture(m_colorBuffer);
    m_shadowMapRenderer.attrShadowColorMap->Set(tu);
  }
  /*
  */
  m_renderer->RenderFullScreenFrame();
}

void csDirectionalLightRendererGL4::BlurShadowMap()
{
  //
  // Blur the shadow map first in horizontal direction
  m_renderer->SetRenderTarget(m_shadowMapRenderer.shadowRenderTargetPingPong);
  m_renderer->SetViewport(m_shadowMapRenderer.shadowRenderTargetPingPong);
  m_renderer->SetBlendEnabled(false);
  m_renderer->SetColorMask(true, true, true, true);
  m_renderer->SetRenderDestination(eRD_Color0);
  m_renderer->SetShader(m_shadowMapBlurHori.shader);
  m_renderer->InvalidateTextures();

  if (m_shadowMapBlurHori.attrColor0)
  {
    csTextureUnit tu = m_renderer->BindTexture(m_shadowMapRenderer.shadowMap);
    m_shadowMapBlurHori.attrColor0->Set(tu);
  }
  m_renderer->RenderFullScreenFrame();



  //
  // Nor blur the shadow map in vertical direction
  m_renderer->SetRenderTarget(m_shadowMapRenderer.shadowRenderTarget);
  m_renderer->SetViewport(m_shadowMapRenderer.shadowRenderTarget);
  m_renderer->SetBlendEnabled(false);
  m_renderer->SetColorMask(true, true, true, true);
  m_renderer->SetRenderDestination(eRD_Color0);
  m_renderer->SetShader(m_shadowMapBlurVert.shader);
  m_renderer->InvalidateTextures();

  if (m_shadowMapBlurVert.attrColor0)
  {
    csTextureUnit tu = m_renderer->BindTexture(m_shadowMapRenderer.shadowMapPingPong);
    m_shadowMapBlurVert.attrColor0->Set(tu);
  }
  m_renderer->RenderFullScreenFrame();

}

void csDirectionalLightRendererGL4::CalcPSSMMatrices(const csDirectionalLight *light, const csCamera *camera)
{
	float dists[] = { 0.0f, m_distances.x, m_distances.y, m_distances.z };
	csVector3f points[8];

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

void csDirectionalLightRendererGL4::CalcMatrix(const csVector3f &dir, const csVector3f &cameraPos, csSize numPoints, csVector3f *points, csMatrix4f &cam, csMatrix4f &camInv, csVector3f &min, csVector3f &max) const
{
  /*
	csVector3f spot;
	for (csSize i = 0; i < numPoints; i++)
	{
		csVector3f::Add(spot, points[i], spot);
	}
	csVector3f::Div(spot, (float)numPoints, spot);

	csVector3f eye;
	csVector3f::Mul(dir, -100.0f, eye);
	csVector3f::Add(eye, spot, eye);
  */
	csVector3f up(0.0f, 0.0f, 1.0f);
	if (dir.z >= 0.9999999)
	{
		up.Set(1.0f, 0.0f, 0.0f);
	}
	else if (dir.z <= -0.9999999)
	{
		up.Set(-1.0f, 0.0f, 0.0f);
	}

  csVector3f at = cameraPos + dir;

	cam.SetLookAt(cameraPos, at, up);
	camInv.SetLookAtInv(cameraPos, at, up);

	min.Set(FLT_MAX, FLT_MAX, FLT_MAX);
	max.Set(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	csVector3f t;
	for (csSize i = 0; i < numPoints; i++)
	{
		csVector3f &p = points[i];
		csMatrix4f::Transform(cam, p, t);
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




void csDirectionalLightRendererGL4::CalcPSSMMatricesAlternative(const csDirectionalLight *light, const csCamera *camera)
{
  float dists[] = { 0.0f, m_distances.x, m_distances.y, m_distances.z };
  csVector3f points[8];

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

void csDirectionalLightRendererGL4::CalcMatrixAlternative(const csVector3f &dir, const csVector3f &cameraPos, csSize numPoints, csVector3f *points, csMatrix4f &cam, csMatrix4f &camInv, csVector3f &min, csVector3f &max) const
{
  csVector3f up(0.0f, 0.0f, 1.0f);
  if (dir.z >= 0.9999999)
  {
    up.Set(1.0f, 0.0f, 0.0f);
  }
  else if (dir.z <= -0.9999999)
  {
    up.Set(-1.0f, 0.0f, 0.0f);
  }


  csVector3f center(0.0f, 0.0f, 0.0f);
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

  csVector3f at = center + dir;
  cam.SetLookAt(center, at, up);
  camInv.SetLookAtInv(center, at, up);

  float radius = sqrt(maxDistSqr);
  float sizePerPixel = 2.0f * radius / (float)m_shadowBufferSize;

  csVector3f c = csMatrix4f::Mult(cam, center, c);

  c.x -= fmodf(c.x, sizePerPixel);
  c.y -= fmodf(c.y, sizePerPixel);
  c.z -= fmodf(c.z, sizePerPixel);

  csMatrix4f::Mult(camInv, c, center);

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

