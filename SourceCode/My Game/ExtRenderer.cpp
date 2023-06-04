/// \file ExtRenderer.cpp
//With help from Josh Wilson
// and the basics here https://www.unknowncheats.me/forum/d3d-tutorials-and-source/126704-drawing-function-directx-9-a.html
// I was able to modify and implement primitive shapes

//#include <vector>
#include "ExtRenderer.h"
//#include "SpriteDesc.h"
//#include "SpriteRenderer.h"

//Constructor
ExtRenderer::ExtRenderer(eSpriteMode mode) : LSpriteRenderer(mode)
{
	LSpriteRenderer::LSpriteRenderer(mode);
}

void ExtRenderer::DrawBoxFilled(int x, int y, int length, int height, Vector4 color)
{
	//Use m_pPrimitiveBatch to draw a rectangle
	VertexPositionColor vertices[4];
	vertices[0].position = Vector3(x, y, 0);
	vertices[0].color = color;
	vertices[1].position = Vector3(x + length, y, 0);
	vertices[1].color = color;
	vertices[2].position = Vector3(x + length, y + height, 0);
	vertices[2].color = color;
	vertices[3].position = Vector3(x, y + height, 0);
	vertices[3].color = color;

	//Draw a quad using two called to DrawTriangle
	m_pPrimitiveBatch->DrawTriangle(vertices[0], vertices[1], vertices[2]);
	m_pPrimitiveBatch->DrawTriangle(vertices[0], vertices[2], vertices[3]);
}

void ExtRenderer::BeginFrameExt() {
    LRenderer3D::BeginFrame();
    m_pSpriteBatch->Begin(m_pCommandList);
    m_pPrimitiveBatch->Begin(m_pCommandList);
	m_pTextSpriteBatch->Begin(m_pCommandList);
    m_fCurZ = 10000.0f;  //initial depth for unbatched sprite rendering
} //BeginFrame

void ExtRenderer::EndFrameExt() {
    m_pSpriteBatch->End();
    m_pPrimitiveBatch->End();
	m_pTextSpriteBatch->End();
    LRenderer3D::EndFrame();
} //EndFrame
