#pragma once

class TexturedModel;

class Renderer
{
    public:
        /* It will be called every frame */
        void Prepare();
        void Render(TexturedModel* model);
};