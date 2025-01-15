#include <iostream>
#include <memory.h>

#include "ServerNetwork.h"

int main()
{
    auto sNet = std::make_unique<XAL::ServerNetwork>(8192);

    while (true) {
        sNet->OnUpdate();
    }
}