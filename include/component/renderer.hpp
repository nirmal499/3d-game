#pragma once

class RawModel;

class Renderer
{
    public:
        /* It will be called every frame */
        void Prepare();
        void Render(RawModel* model);
};