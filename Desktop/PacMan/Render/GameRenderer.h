#pragma once

#include "RenderSystem.h"
#include "Core/MessageCenter.h"
#include "GameEntities/EntityManager.h"
#include "GameEntities/Trigger.h"
#include "Math/Vector2f.h"
#include <memory>
#include <vector>
#include <unordered_map>
#include <string>
#include <cstdlib>
#include <cstdint>

class PacMan;

class GameRenderer : public MessageListener
{
public:
	GameRenderer(std::shared_ptr<RenderSystem> renderSystem);
	~GameRenderer();
		
	GameRenderer(const GameRenderer&) = delete;
	GameRenderer& operator= (const GameRenderer&) = delete;
	
	void Setup(std::shared_ptr<EntityManager> entityManager, const Vector2f& worldMinPoint, const Vector2f& worldMaxPoint);
	void Update();
	void Draw();

private:
	struct InstanceData
	{
		static const unsigned InvalidTextureId = ~0u;

		InstanceData()
			: m_TextureId(InvalidTextureId)
		{}
		InstanceData(const Vector2f& worldSpaceCenter, const Vector2f& halfSize, unsigned textureId)
			: m_WorldSpaceCenter(worldSpaceCenter)
			, m_HalfSize(halfSize)
			, m_TextureId(textureId)
		{}
		Vector2f m_WorldSpaceCenter;
		Vector2f m_HalfSize;
		unsigned m_TextureId;
	};

	void SetupResources();
	void SetupGeometry(std::shared_ptr<EntityManager> entityManager);
	void SetupStaticGeometry();
	void SetupTriggerGeometry();
	void SetupDynamicGeometry();
	void SetupCamera(const Vector2f& worldMinPoint, const Vector2f& worldMaxPoint);
	void SetupPacMan();

	void UpdateDynamicUnits(const EntityManager::MovingEntities& entities);
	
	void UpdatePacManScoreString();
	void UpdatePacManLivesString();

	void DrawEntities();
	void DrawPacManLives();
	void DrawPacManScore();

	void OnMessageReceived(const Message& message) override;

private:
	std::shared_ptr<RenderSystem> m_RenderSystem;
	std::shared_ptr<EntityManager> m_EntityManager;

	ComPtr<ID3D11ShaderResourceView> m_AssetAtlasSRV;
	ComPtr<ID3D11InputLayout> m_NullInputLayout;
	ComPtr<ID3D11Buffer> m_NullVertexBuffer;
	ComPtr<ID3D11Buffer> m_NullIndexBuffer;
	ComPtr<ID3D11Buffer> m_TransformDataBuffer;
	ComPtr<ID3D11SamplerState> m_LinearSampler;
	D3D11_VIEWPORT m_LevelMapViewport;

	ComPtr<ID3D11VertexShader> m_RenderStaticUnitVS;
	ComPtr<ID3D11PixelShader> m_RenderStaticUnitPS;
	ComPtr<ID3D11ShaderResourceView> m_StaticUnitDataSRV;
	std::size_t m_NumStaticUnits;

	ComPtr<ID3D11VertexShader> m_RenderTriggerVS;
	ComPtr<ID3D11PixelShader> m_RenderTriggerPS;
	ComPtr<ID3D11ShaderResourceView> m_TriggerDataSRV;
	ComPtr<ID3D11Buffer> m_TriggerStateBuffer;
	ComPtr<ID3D11ShaderResourceView> m_TriggerStateSRV;
	std::unordered_map<Trigger*, std::size_t> m_TriggerIndexMap;
	std::vector<int> m_TriggerStates;
	bool m_UpdateTriggerStates;

	ComPtr<ID3D11VertexShader> m_RenderDynamicUnitVS;
	ComPtr<ID3D11PixelShader> m_RenderDynamicUnitPS;
	ComPtr<ID3D11Buffer> m_DynamicUnitDataBuffer;
	ComPtr<ID3D11ShaderResourceView> m_DynamicUnitDataSRV;
	std::vector<InstanceData> m_DynamicUnits;

	ComPtr<ID2D1SolidColorBrush> m_TextColorBrush;
	ComPtr<IDWriteTextFormat> m_TextFormat;

	std::shared_ptr<PacMan> m_PacMan;
	std::wstring m_PacManScoreString;
	std::wstring m_PacManLivesString;
};
