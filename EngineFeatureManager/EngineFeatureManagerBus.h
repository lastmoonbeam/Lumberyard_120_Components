#pragma once

#include <AzCore/Component/ComponentBus.h>
#include <AzCore/Math/Vector3.h>

namespace GameProject
{
	class EngineFeatureManagerInterface : public AZ::ComponentBus
	{
	public:
		static const AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;

		virtual	void ApplyPresetBroadcast(AZStd::string_view pressetName) = 0;
		virtual	void ApplyPreset(AZStd::string_view pressetName) = 0;
		virtual void UpdateAll(bool show) = 0;
	};
	using EngineFeatureManagerBus = AZ::EBus<EngineFeatureManagerInterface>;
};