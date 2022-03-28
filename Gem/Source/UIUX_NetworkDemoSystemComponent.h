
#pragma once

#include <AzCore/Component/Component.h>

#include <UIUX_NetworkDemo/UIUX_NetworkDemoBus.h>

#include <Multiplayer/IMultiplayerSpawner.h>

#include "Spawners/IPlayerSpawner.h"

namespace UIUX_NetworkDemo
{
    class UIUX_NetworkDemoSystemComponent
        : public AZ::Component
        , protected UIUX_NetworkDemoRequestBus::Handler
        , public Multiplayer::IMultiplayerSpawner
    {
    public:
        AZ_COMPONENT(UIUX_NetworkDemoSystemComponent, "{c9e6777b-3ee6-4e19-bf0e-a0f932263e1f}");

        static void Reflect(AZ::ReflectContext* context);

        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        UIUX_NetworkDemoSystemComponent();
        ~UIUX_NetworkDemoSystemComponent();

    protected:
        ////////////////////////////////////////////////////////////////////////
        // UIUX_NetworkDemoRequestBus interface implementation

        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        // AZ::Component interface implementation
        void Init() override;
        void Activate() override;
        void Deactivate() override;
        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        // IMultiplayerSpawner overrides
        Multiplayer::NetworkEntityHandle OnPlayerJoin(uint64_t userId, const Multiplayer::MultiplayerAgentDatum& agentDatum) override;
        void OnPlayerLeave(
            Multiplayer::ConstNetworkEntityHandle entityHandle,
            const Multiplayer::ReplicationSet& replicationSet,
            AzNetworking::DisconnectReason reason) override;
        ////////////////////////////////////////////////////////////////////////

        AZStd::unique_ptr<UIUX_NetworkDemo::IPlayerSpawner> m_playerSpawner;
    };
}
