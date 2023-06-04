#pragma once
#include "SpriteDesc.h"
#include "SpriteRenderer.h"

//#include <d3d12.h>
//#include <d3dx12.h>

/*
struct vertex
{
    FLOAT x, y, z, rhw;
    Vector4 color;
};
*/

class ExtRenderer : public LSpriteRenderer
{
public:
    ExtRenderer(eSpriteMode); ///< Constructor.

    void DrawBoxFilled(int x, int y, int w, int h, Vector4 color);

    void BeginFrameExt();
    void EndFrameExt();
private:
    //LPDIRECT3DDEVICE9 pDevice;
    //IASetVertexBuffers g_pVB;    // Buffer to hold vertices
};