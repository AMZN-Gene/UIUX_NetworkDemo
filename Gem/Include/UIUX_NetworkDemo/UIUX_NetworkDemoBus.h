
#pragma once

#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>

namespace UIUX_NetworkDemo
{
    class UIUX_NetworkDemoRequests
    {
    public:
        AZ_RTTI(UIUX_NetworkDemoRequests, "{19ddab4d-64cd-4dcb-b67e-0b2162f814c8}");
        virtual ~UIUX_NetworkDemoRequests() = default;
        // Put your public methods here
    };

    class UIUX_NetworkDemoBusTraits
        : public AZ::EBusTraits
    {
    public:
        //////////////////////////////////////////////////////////////////////////
        // EBusTraits overrides
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
        //////////////////////////////////////////////////////////////////////////
    };

    using UIUX_NetworkDemoRequestBus = AZ::EBus<UIUX_NetworkDemoRequests, UIUX_NetworkDemoBusTraits>;
    using UIUX_NetworkDemoInterface = AZ::Interface<UIUX_NetworkDemoRequests>;

} // namespace UIUX_NetworkDemo
