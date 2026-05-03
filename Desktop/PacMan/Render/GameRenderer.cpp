#include "GameRenderer.h"
#include "Core/Color.h"
#include "Core/Config.h"
#include "Math/Vector3f.h"
#include "Math/Matrix4f.h"
#include "Math/Transform.h"
#include "GameEntities/PacMan.h"
#include <sstream>

#define TRIGGER_STATE_ACTIVE		1
#define TRIGGER_STATE_NON_ACTIVE	0

namespace
{
	enum TextureAtlas
	{
		WALL_TEXTURE_ID = 1,
		PEN_DOOR_TEXTURE_ID = 10,
		PILL_POINT_TEXTURE_ID = 11,
		POWER_UP_TEXTURE_ID = 0,
		BLINKY_FIRST_TEXTURE_ID = 16,
		PINKY_FIRST_TEXTURE_ID = 20,
		INKY_FIRST_TEXTURE_ID = 6,
		PACMAN_POWERED_FIRST_TEXTURE_ID = 2,
		PACMAN_NORMAL_FIRST_TEXTURE_ID = 12,
		NUM_TEXTURES_IN_ATLAS = 24
	};
		
	struct TransformData
	{
		Matrix4f m_ViewProjMatrix;
		FLOAT m_TexSpaceImageWidth;
		Vector3f m_Padding0;
		Vector4f m_Padding1;
		Vector4f m_Padding2;
		Vector4f m_Padding3;
	};
}

GameRenderer::GameRenderer(std::shared_ptr<RenderSystem> renderSystem)
	: m_RenderSystem(renderSystem)
	, m_UpdateTriggerStates(false)
{
	MessageCenter::GetInstance().RegisterListener(this);
	SetupResources();

	m_TextColorBrush = m_RenderSystem->CreateTextColorBrush(Color::BLUE);

	m_TextFormat = m_RenderSystem->CreateTextFormat(L"Consolas", 20.0f);
	m_TextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
}

GameRenderer::~GameRenderer()
{
	MessageCenter::GetInstance().UnregisterListener(this);
}

void GameRenderer::Setup(std::shared_ptr<EntityManager> entityManager, const Vector2f& worldMinPoint, const Vector2f& worldMaxPoint)
{
	SetupGeometry(entityManager);
	SetupCamera(worldMinPoint, worldMaxPoint);
}

void GameRenderer::Update()
{
	UpdateDynamicUnits(m_EntityManager->GetMovingEntities());
	m_RenderSystem->WriteToBuffer(m_DynamicUnitDataBuffer, m_DynamicUnits.data(), m_DynamicUnits.size() * sizeof(InstanceData));

	if (m_UpdateTriggerStates)
	{
		m_RenderSystem->WriteToBuffer(m_TriggerStateBuffer, m_TriggerStates.data(), m_TriggerStates.size() * sizeof(int));
		m_UpdateTriggerStates = false;
	}
}

void GameRenderer::Draw()
{
	m_RenderSystem->BeginRenderFrame();
	m_RenderSystem->ClearRenderTarget(Color::BLACK);

	DrawEntities();
	DrawPacManLives();
	DrawPacManScore();

	m_RenderSystem->EndRenderFrame();
}

void GameRenderer::SetupResources()
{
	m_RenderSystem->LoadTexture(L"Assets/Atlas.dds", &m_AssetAtlasSRV);

	m_RenderStaticUnitVS = m_RenderSystem->LoadVertexShader(L"Shaders/RenderStaticUnitVS.hlsl", "Main", "vs_4_0");
	m_RenderStaticUnitPS = m_RenderSystem->LoadPixelShader(L"Shaders/RenderStaticUnitPS.hlsl", "Main", "ps_4_0");

	m_RenderDynamicUnitVS = m_RenderSystem->LoadVertexShader(L"Shaders/RenderDynamicUnitVS.hlsl", "Main", "vs_4_0");
	m_RenderDynamicUnitPS = m_RenderSystem->LoadPixelShader(L"Shaders/RenderDynamicUnitPS.hlsl", "Main", "ps_4_0");

	m_RenderTriggerVS = m_RenderSystem->LoadVertexShader(L"Shaders/RenderTriggerVS.hlsl", "Main", "vs_4_0");
	m_RenderTriggerPS = m_RenderSystem->LoadPixelShader(L"Shaders/RenderTriggerPS.hlsl", "Main", "ps_4_0");

	m_LinearSampler = m_RenderSystem->CreateLinearSampler();
}

void GameRenderer::SetupGeometry(std::shared_ptr<EntityManager> entityManager)
{
	m_EntityManager = entityManager;

	SetupPacMan();
	SetupStaticGeometry();
	SetupTriggerGeometry();
	SetupDynamicGeometry();
}

void GameRenderer::SetupStaticGeometry()
{
	const auto& entities = m_EntityManager->GetStaticEntities();
	
	std::vector<InstanceData> staticUnits;
	staticUnits.reserve(entities.size());
	
	for (const auto& entity : entities)
	{
		if (entity->GetType() == EntityType::WALL)
			staticUnits.emplace_back(entity->GetCenter(), entity->GetHalfSize(), WALL_TEXTURE_ID);
		else if (entity->GetType() == EntityType::PEN_DOOR)
			staticUnits.emplace_back(entity->GetCenter(), entity->GetHalfSize(), PEN_DOOR_TEXTURE_ID);
		else
			assert(false);
	}

	m_StaticUnitDataSRV = nullptr;
	m_NumStaticUnits = staticUnits.size();

	if (m_NumStaticUnits > 0)
	{
		m_RenderSystem->CreateStructuredBuffer(m_NumStaticUnits, sizeof(InstanceData),
			D3D11_USAGE_IMMUTABLE, 0, staticUnits.data(), &m_StaticUnitDataSRV, nullptr);
	}
}

void GameRenderer::SetupTriggerGeometry()
{
	const auto& entities = m_EntityManager->GetTriggers();

	std::vector<InstanceData> triggers;
	triggers.reserve(entities.size());
	m_TriggerStates.reserve(entities.size());

	for (std::size_t i = 0; i < entities.size(); ++i)
	{
		const auto& entity = entities[i];
		if (entity->GetType() == EntityType::PILL_POINT)
		{
			triggers.emplace_back(entity->GetCenter(), entity->GetHalfSize(), PILL_POINT_TEXTURE_ID);
			
			m_TriggerStates.emplace_back(TRIGGER_STATE_ACTIVE);
			m_TriggerIndexMap.emplace(entity.get(), i);
		}
		else if (entity->GetType() == EntityType::POWER_UP)
		{
			triggers.emplace_back(entity->GetCenter(), entity->GetHalfSize(), POWER_UP_TEXTURE_ID);
			
			m_TriggerStates.emplace_back(TRIGGER_STATE_ACTIVE);
			m_TriggerIndexMap.emplace(entity.get(), i);
		}
		else if (entity->GetType() == EntityType::TELEPORT)
		{
			// Do not render teleport
		}
		else
		{
			assert(false);
		}
	}

	m_TriggerDataSRV = nullptr;
	m_TriggerStateBuffer = nullptr;
	m_TriggerStateSRV = nullptr;
		
	if (!triggers.empty())
	{
		m_RenderSystem->CreateStructuredBuffer(triggers.size(), sizeof(InstanceData),
			D3D11_USAGE_IMMUTABLE, 0, triggers.data(), &m_TriggerDataSRV, nullptr);

		m_TriggerStateBuffer = m_RenderSystem->CreateStructuredBuffer(m_TriggerStates.size(), sizeof(int),
			D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE, m_TriggerStates.data(), &m_TriggerStateSRV, nullptr);
	}
	m_UpdateTriggerStates = false;
}

void GameRenderer::SetupDynamicGeometry()
{
	const auto& entities = m_EntityManager->GetMovingEntities();
	m_DynamicUnits.resize(entities.size());
	
	UpdateDynamicUnits(entities);

	m_DynamicUnitDataBuffer = nullptr;
	m_DynamicUnitDataSRV = nullptr;
	
	if (m_DynamicUnits.size() > 0)
	{
		m_DynamicUnitDataBuffer = m_RenderSystem->CreateStructuredBuffer(m_DynamicUnits.size(), sizeof(InstanceData),
			D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE, m_DynamicUnits.data(), &m_DynamicUnitDataSRV, nullptr);
	}
}

void GameRenderer::SetupCamera(const Vector2f& worldMinPoint, const Vector2f& worldMaxPoint)
{
	Vector3f focusPos(0.5f * (worldMinPoint.m_X + worldMaxPoint.m_X), 0.5f * (worldMinPoint.m_Y + worldMaxPoint.m_Y), 0.0f);
	Vector3f eyePos = focusPos - Vector3f(0.0f, 0.0f, 1.0f);

	Matrix4f viewMatrix = CreateLookAtMatrix(eyePos, focusPos, Vector3f::UP);
	Matrix4f projMatrix = CreateOrthoProjMatrix(worldMaxPoint.m_X - worldMinPoint.m_X, worldMaxPoint.m_Y - worldMinPoint.m_Y, -1.0f, 1.0f);

	TransformData transformData;
	transformData.m_ViewProjMatrix = viewMatrix * projMatrix;
	transformData.m_TexSpaceImageWidth = 1.0f / FLOAT(NUM_TEXTURES_IN_ATLAS);

	m_TransformDataBuffer = m_RenderSystem->CreateConstantBuffer(sizeof(TransformData), D3D11_USAGE_IMMUTABLE, 0, &transformData);

	m_LevelMapViewport.Width = (FLOAT)Config::MAP_WIDTH;
	m_LevelMapViewport.Height = (FLOAT)Config::MAP_HEIGHT;
	m_LevelMapViewport.TopLeftX = 0.0f;
	m_LevelMapViewport.TopLeftY = 0.0f;
	m_LevelMapViewport.MinDepth = 0.0f;
	m_LevelMapViewport.MaxDepth = 1.0f;
}

void GameRenderer::SetupPacMan()
{
	m_PacMan.reset();
	for (const auto& entity : m_EntityManager->GetMovingEntities())
	{
		if (entity->GetType() == EntityType::PACMAN)
		{
			m_PacMan = std::dynamic_pointer_cast<PacMan>(entity);
			break;
		}
	}
	assert(m_PacMan);

	UpdatePacManScoreString();
	UpdatePacManLivesString();
}

void GameRenderer::UpdateDynamicUnits(const EntityManager::MovingEntities& entities)
{
	assert(m_DynamicUnits.size() == entities.size());
	for (std::size_t i = 0; i < entities.size(); ++i)
	{
		auto entity = entities[i];

		m_DynamicUnits[i].m_WorldSpaceCenter = entity->GetCenter();
		m_DynamicUnits[i].m_HalfSize = entity->GetHalfSize();

		if (entity->GetType() == EntityType::PACMAN)
		{
			if (m_PacMan->GetBehaviorMode() == BehaviorMode::POWERED)
			{
				m_DynamicUnits[i].m_TextureId = PACMAN_POWERED_FIRST_TEXTURE_ID + entity->GetHeading();
			}
			else
			{
				assert(m_PacMan->GetBehaviorMode() == BehaviorMode::NORMAL);
				m_DynamicUnits[i].m_TextureId = PACMAN_NORMAL_FIRST_TEXTURE_ID + entity->GetHeading();
			}
		}
		else if (entity->GetType() == EntityType::BLINKY)
			m_DynamicUnits[i].m_TextureId = BLINKY_FIRST_TEXTURE_ID + entity->GetHeading();
		else if (entity->GetType() == EntityType::PINKY)
			m_DynamicUnits[i].m_TextureId = PINKY_FIRST_TEXTURE_ID + entity->GetHeading();
		else
			assert(false);
	}
}

void GameRenderer::UpdatePacManScoreString()
{
	std::wstringstream stream;
	stream << L"Score: " << m_PacMan->GetScore();
	m_PacManScoreString = stream.str();
}

void GameRenderer::UpdatePacManLivesString()
{
	std::wstringstream stream;
	stream << L"Lives: " << m_PacMan->GetLives();
	m_PacManLivesString = stream.str();
}

void GameRenderer::DrawEntities()
{
	static const UINT numVerticesPerInstance = 4;

	m_RenderSystem->SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	m_RenderSystem->SetInputLayout(m_NullInputLayout);
	m_RenderSystem->SetVertexBuffer(m_NullVertexBuffer, 0, 0);
	m_RenderSystem->SetIndexBuffer(m_NullIndexBuffer, true, 0);
	m_RenderSystem->SetViewport(&m_LevelMapViewport);

	m_RenderSystem->SetVertexShader(m_RenderStaticUnitVS);
	m_RenderSystem->SetVertexShaderConstantBuffer(0, m_TransformDataBuffer);
	m_RenderSystem->SetVertexShaderResource(0, m_StaticUnitDataSRV);
	m_RenderSystem->SetPixelShader(m_RenderStaticUnitPS);
	m_RenderSystem->SetPixelShaderResource(0, m_AssetAtlasSRV);
	m_RenderSystem->SetPixelShaderSampler(0, m_LinearSampler);
	m_RenderSystem->DrawInstanced(numVerticesPerInstance, m_NumStaticUnits, 0, 0);

	m_RenderSystem->SetVertexShader(m_RenderTriggerVS);
	m_RenderSystem->SetVertexShaderConstantBuffer(0, m_TransformDataBuffer);
	m_RenderSystem->SetVertexShaderResource(0, m_TriggerDataSRV);
	m_RenderSystem->SetVertexShaderResource(1, m_TriggerStateSRV);
	m_RenderSystem->SetPixelShader(m_RenderTriggerPS);
	m_RenderSystem->SetPixelShaderResource(0, m_AssetAtlasSRV);
	m_RenderSystem->SetPixelShaderSampler(0, m_LinearSampler);
	m_RenderSystem->DrawInstanced(numVerticesPerInstance, m_TriggerStates.size(), 0, 0);

	m_RenderSystem->SetVertexShader(m_RenderDynamicUnitVS);
	m_RenderSystem->SetVertexShaderConstantBuffer(0, m_TransformDataBuffer);
	m_RenderSystem->SetVertexShaderResource(0, m_DynamicUnitDataSRV);
	m_RenderSystem->SetPixelShader(m_RenderDynamicUnitPS);
	m_RenderSystem->SetPixelShaderResource(0, m_AssetAtlasSRV);
	m_RenderSystem->SetPixelShaderSampler(0, m_LinearSampler);
	m_RenderSystem->DrawInstanced(numVerticesPerInstance, m_DynamicUnits.size(), 0, 0);
}

void GameRenderer::DrawPacManScore()
{
	m_RenderSystem->DrawText(m_PacManScoreString.c_str(), m_TextFormat, m_TextColorBrush,
		(FLOAT)Config::MAP_TILE_SIZE_IN_PIXELS,
		(FLOAT)Config::MAP_HEIGHT,
		(FLOAT)Config::MAP_WIDTH * 0.5f,
		(FLOAT)Config::MAP_HEIGHT + (FLOAT)Config::SCORE_PANEL_HEIGHT);
}

void GameRenderer::DrawPacManLives()
{
	m_RenderSystem->DrawText(m_PacManLivesString.c_str(), m_TextFormat, m_TextColorBrush,
		(FLOAT)Config::MAP_WIDTH * 0.5f + (FLOAT)Config::MAP_TILE_SIZE_IN_PIXELS,
		(FLOAT)Config::MAP_HEIGHT,
		(FLOAT)Config::MAP_WIDTH,
		(FLOAT)Config::MAP_HEIGHT + (FLOAT)Config::SCORE_PANEL_HEIGHT);
}

void GameRenderer::OnMessageReceived(const Message& message)
{
	if (message.m_ID == PowerUpChanged)
	{
		auto pTrigger = (Trigger*)message.m_pSender;
		auto triggerIndex = m_TriggerIndexMap[pTrigger];
		m_TriggerStates[triggerIndex] = pTrigger->IsActive() ? TRIGGER_STATE_ACTIVE : TRIGGER_STATE_NON_ACTIVE;

		m_UpdateTriggerStates = true;
		return;
	}
	if (message.m_ID == PillPointChanged)
	{
		auto pTrigger = (Trigger*)message.m_pSender;
		auto triggerIndex = m_TriggerIndexMap[pTrigger];
		m_TriggerStates[triggerIndex] = pTrigger->IsActive() ? TRIGGER_STATE_ACTIVE : TRIGGER_STATE_NON_ACTIVE;

		m_UpdateTriggerStates = true;
		return;
	}
	if (message.m_ID == ScoreChanged)
	{
		UpdatePacManScoreString();
		return;
	}
	if (message.m_ID == EntityKilled)
	{
		auto pEntity = (MovingEntity*)message.m_pSender;
		if (pEntity->GetType() == EntityType::PACMAN)
		{
			UpdatePacManLivesString();
		}
		return;
	}
}
