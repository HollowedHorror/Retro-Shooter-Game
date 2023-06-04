/// \file BarDisplay.cpp
/// \brief Code for the BarDisplay object class CBarDisplay.
/*

#include "BarDisplay.h"
#include "SpriteRenderer.h"

CBarDisplay::CBarDisplay()
{
	
}

LSpriteDesc2D CBarDisplay::Draw(eSprite sprite, int width, int i, Vector2 pos)//Itterates and displays i of sprite at vector2
{
	LSpriteDesc2D desc;
	desc.m_vPos.x = pos.x;
	desc.m_vPos.y = pos.y;
	int width = m_pRenderer->GetWidth(sprite);

	for (int j = 0; j <= i; j++)
	{
		m_pRenderer->Draw(&desc);
		desc.m_vPos.x += width + 1.0;
	}
}

/*
void CBarDisplay::Release()
{
	delete m_pRenderer;
	m_pRenderer = nullptr; //for safety
}
*/