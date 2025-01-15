#include <iostream>
#include <memory.h>

#include "ClientNetwork.h"

int main()
{
    auto clNet = std::make_unique<XAL::ClientNetwork>();
    clNet->OnAttach();

    clNet->Send("hello");

    while (clNet->IsConnected()) {
        clNet->OnUpdate();
    }
}