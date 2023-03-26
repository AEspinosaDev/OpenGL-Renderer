#pragma once

#include <Core/Renderer.h>



    int main()
    {
        Renderer* r = new Renderer('test', 1200, 900,AntialiasingType::MSAAx16);
        r->Run();


    }