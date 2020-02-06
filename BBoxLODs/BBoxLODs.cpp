#include "GameProject_precompiled.h"
#pragma optimize("", on)

#include <AzCore/Component/Entity.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/RTTI/BehaviorContext.h>

/// EBUS
#include <AzCore/Component/TransformBus.h>
#include <AzCore/Component/ComponentApplicationBus.h>

#include <AzFramework/Entity/EntityContextBus.h>
#include <AzFramework/Entity/GameEntityContextBus.h>

#include <..\..\..\Gems\LmbrCentral\Code\include\LmbrCentral\Rendering\RenderNodeBus.h>

//#include <..\..\..\Gems\EMotionFX\Code\Source\Integration\Components\ActorComponent.h>
#include <..\..\..\Gems\EMotionFX\Code\EMotionFX\Source\ActorInstance.h>

#include <MCore/Source/AzCoreConversions.h>
#include <..\..\..\Gems\EMotionFX\Code\MCore\Source\AABB.h>
#include <..\..\..\Code\CryEngine\CryCommon\MathConversion.h>


//4bbox
#include <LyShine\ILyShine.h>
#include <LyShine\IDraw2d.h>
#include <Cry_Camera.h>

#include "BBoxLODs.h"

using namespace GameProject;


AZ::Aabb MCoreAABBToAzAABB(MCore::AABB aabb) 
{	
	AZ::Aabb azAabb;

	azAabb.SetMax(aabb.GetMax());
	azAabb.SetMin(aabb.GetMin());

	return azAabb;
}

void BBoxLODComponent::Reflect(AZ::ReflectContext * context)
{
	AZ::SerializeContext* serialize = azrtti_cast<AZ::SerializeContext*>(context);

	if (serialize)
	{
		serialize->Class<BBoxLODComponent, AZ::Component >()
			->Version(1)
			->Field("LODSquares", &BBoxLODComponent::m_lodSquares)
			->Field("debugBBoxBounds", &BBoxLODComponent::m_debugBBoxBounds)
			->Field("minbb", &BBoxLODComponent::m_minbb)
			->Field("maxbb", &BBoxLODComponent::m_maxbb)
			->Field("LODBoundingBoxFactor", &BBoxLODComponent::m_LODBoundingBoxFactor)
			


			
			;

		AZ::EditContext* edit = serialize->GetEditContext();
		if (edit)
		{
			edit->Class<BBoxLODComponent>("BBoxLODComponent", "")
				->ClassElement(AZ::Edit::ClassElements::EditorData, "")
				->Attribute(AZ::Edit::Attributes::Category, CATEGORY)
				->Attribute("Icon", "Editor/Icons/Components/ComponentPlaceholder.png")
				->Attribute("ViewportIcon", "Editor/Icons/Components/Viewport/ComponentPlaceholder.png")
				->Attribute("AutoExpand", true)
				->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("Game", 0x232b318c))
				->DataElement(AZ::Edit::UIHandlers::Default, &BBoxLODComponent::m_lodSquares, "m_lodSquares", "")
				->DataElement(AZ::Edit::UIHandlers::Default, &BBoxLODComponent::m_debugBBoxBounds, "m_debugBBoxBounds", "")
				->DataElement(AZ::Edit::UIHandlers::Default, &BBoxLODComponent::m_minbb, "m_minbb", "static Worldspace bounding box")
				->DataElement(AZ::Edit::UIHandlers::Default, &BBoxLODComponent::m_maxbb, "m_maxbb", "static Worldspace bounding box")
				->DataElement(AZ::Edit::UIHandlers::Default, &BBoxLODComponent::m_LODBoundingBoxFactor, "m_LODBoundingBoxFactor", "factor what allows to enlarge bbox square incompare with whole screen square")
				;
		}
	}
}

void BBoxLODComponent::Init()
{

}

void BBoxLODComponent::Activate()
{
	EMotionFX::Integration::ActorComponentNotificationBus::Handler::BusConnect(GetEntityId());
	AZ::TickBus::Handler::BusConnect();
	AzFramework::GameEntityContextEventBus::Handler::BusConnect();
}

void BBoxLODComponent::Deactivate()
{
	AZ::TickBus::Handler::BusDisconnect();
	EMotionFX::Integration::ActorComponentNotificationBus::Handler::BusDisconnect();
	AzFramework::GameEntityContextEventBus::Handler::BusDisconnect();
}

void BBoxLODComponent::OnTick(float deltaTime, AZ::ScriptTimePoint time)
{
	AZ_UNUSED(deltaTime);
	AZ_UNUSED(time);

	UpdateLODLevelByBoundingBox(m_actorInstance, m_lodSquares, GetEntityId());
}

void BBoxLODComponent::UpdateLODLevelByBoundingBox(EMotionFX::ActorInstance * actorInstance, const AZStd::vector<float>& squares, AZ::EntityId entityId)
{
	if (actorInstance)
	{
		// Compute BBox LOD
		if (gEnv->pSystem)
		{
			//const CCamera& camera = gEnv->pSystem->GetViewCamera(); // dunno but this is buggy camera
			const CCamera& camera = gEnv->p3DEngine->GetRenderingCamera(); // this is normal camera

			AZ::Aabb aabb = AZ::Aabb::CreateFromMinMax(m_minbb, m_maxbb);
			AZ::Transform tm = m_entity->GetTransform()->GetWorldTM();
			aabb.ApplyTransform(tm);
			//AZ::Aabb aabb = MCoreAABBToAzAABB(actorInstance->GetAABB()); // editor's crash here, so we use static bbox for test, reflect vars to component

			AABB cryAABB = AZAabbToLyAABB(aabb);
			int rectangle [4]; //x0y0 x1y1
			camera.CalcScreenBounds(rectangle, &cryAABB, gEnv->pRenderer->GetWidth(), gEnv->pRenderer->GetHeight());

			// TODO: so next step compare size of rectangle with screeen size and choose proper lod

			float screenSpaceBoundingBoxWidth = AZ::GetMax<float>(AZ::GetAbs((float)(rectangle[2] - rectangle[0])), 0.0f);
			float screenSpaceBoundingBoxHeight = AZ::GetMax<float>(AZ::GetAbs((float)(rectangle[3] - rectangle[1])), 0.0f);
			float screenSpaceBoundingBoxSquare = screenSpaceBoundingBoxWidth * screenSpaceBoundingBoxHeight * m_LODBoundingBoxFactor;
			float fullScreenSquare = (float)(gEnv->pRenderer->GetWidth() * gEnv->pRenderer->GetHeight());

			// clamp ssbb square by screen square
			screenSpaceBoundingBoxSquare = AZ::GetClamp<float>(screenSpaceBoundingBoxSquare, screenSpaceBoundingBoxSquare, fullScreenSquare);

			//float screenRatio = AZ::LerpInverse(0.0f, (float)gEnv->pRenderer->GetWidth(), screenSpaceBoundingBoxWidth);
			float screenRatio = AZ::LerpInverse(0.0f, fullScreenSquare, screenSpaceBoundingBoxSquare);




			const AZ::u32 lodByBBox = GetLODByBoundingBoxSquare(m_lodSquares, screenRatio);
			//AZ_Printf("BBoxLODComponent", "lodByDistance = %d" , lodByBBox);
			if (m_maxLOD >= lodByBBox)
				actorInstance->SetLODLevel(lodByBBox);



			if (m_debugBBoxBounds)
			{
				DrawRect(AZ::Vector2((float)rectangle[0], (float)rectangle[1]), AZ::Vector2((float)rectangle[2], (float)rectangle[3]));
				DrawLODLabel(AZ::Vector2((float)rectangle[0], (float)rectangle[1]), (float)lodByBBox, screenRatio);
			}
		}
	}
}

void BBoxLODComponent::DrawRect(AZ::Vector2 min, AZ::Vector2 max)
{
	gEnv->pLyShine->GetDraw2d()->BeginDraw2d(true);

	AZ::Vector2 verts[4];

	verts[0] = AZ::Vector2(min.GetX(), min.GetY());
	verts[1] = AZ::Vector2(max.GetX(), min.GetY());
	verts[2] = AZ::Vector2(max.GetX(), max.GetY());
	verts[3] = AZ::Vector2(min.GetX(), max.GetY());

	gEnv->pLyShine->GetDraw2d()->DrawLine(verts[0], verts[1], AZ::Color::CreateOne());
	gEnv->pLyShine->GetDraw2d()->DrawLine(verts[1], verts[2], AZ::Color::CreateOne());
	gEnv->pLyShine->GetDraw2d()->DrawLine(verts[2], verts[3], AZ::Color::CreateOne());
	gEnv->pLyShine->GetDraw2d()->DrawLine(verts[3], verts[0], AZ::Color::CreateOne());

	gEnv->pLyShine->GetDraw2d()->EndDraw2d();
}

void BBoxLODComponent::DrawLODLabel(AZ::Vector2 pos, float lod, float ratio)
{
	gEnv->pLyShine->GetDraw2d()->BeginDraw2d(true);
	AZStd::string text2label = AZStd::string::format("LOD %0.4f ratio %0.4f", lod, ratio);
	gEnv->pLyShine->GetDraw2d()->BeginDraw2d(true);
	gEnv->pLyShine->GetDraw2d()->DrawText(text2label.c_str(), pos, 25.0f);
	gEnv->pLyShine->GetDraw2d()->EndDraw2d();
}

void BBoxLODComponent::OnGameEntitiesStarted()
{
	//AZ_Printf("BBoxLODComponent", "OnGameEntitiesStarted");
	//AZ::TickBus::Handler::BusConnect();
}

void BBoxLODComponent::OnActorInstanceCreated(EMotionFX::ActorInstance * actorInstance)
{
	if (m_actorInstance != actorInstance)
	{
		m_actorInstance = actorInstance;
		m_maxLOD = m_actorInstance->GetActor()->GetNumLODLevels();

		//const AZ::u32 numLODs = m_actorInstance->GetActor()->GetNumLODLevels();
		//if (numLODs != m_lodSquares.size())
		//{
		//	GenerateDefaultRatioLODs(numLODs);
		//}
	}
}

void BBoxLODComponent::OnActorInstanceDestroyed(EMotionFX::ActorInstance * actorInstance)
{
	AZ_UNUSED(actorInstance);
	m_actorInstance = nullptr;
}

void BBoxLODComponent::GenerateDefaultRatioLODs(AZ::u32 numLodLevels)
{
	// Generate the default LOD to 10, 20, 30....
	m_lodSquares.resize(numLodLevels);
	for (AZ::u32 i = 0; i < numLodLevels; ++i)
	{
		m_lodSquares[i] = AZ::LerpInverse(0.0f, (float)numLodLevels, (float)i);
	}
}


AZ::u32 BBoxLODComponent::GetLODByBoundingBoxSquare(const AZStd::vector<float>& squares, float square)
{
	const size_t max = squares.size();
	for (size_t i = 0; i < max; ++i)
	{
		const float rSquare = squares[i];
		if (square > rSquare)
		{
			return static_cast<AZ::u32>(i);
		}
	}

	return static_cast<AZ::u32>(max - 1);
}
