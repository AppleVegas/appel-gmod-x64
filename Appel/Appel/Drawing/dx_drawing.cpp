#include "dx_drawing.h"
#include <vector>
#include "../../resource.h"
#include "../appel.h"
struct DXVertex
{
    FLOAT x, y, z, rhw;
    D3DCOLOR color;
};
void init(IDirect3DDevice9* dxDevice) {
    dxDevice->SetPixelShader(NULL);
    dxDevice->SetVertexShader(NULL);
    dxDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    dxDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    dxDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
    dxDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    dxDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    dxDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
    dxDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    dxDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    dxDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
    dxDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
    dxDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
    dxDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    dxDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    dxDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    dxDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    dxDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    dxDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
    dxDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    dxDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    dxDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, false);
}

CDraw Draw;

void CDraw::Reset()
{
    D3DVIEWPORT9 screen;
    pDevice->GetViewport(&screen);

    Screen.Width = screen.Width;
    Screen.Height = screen.Height;
    Screen.x_center = Screen.Width / 2;
    Screen.y_center = Screen.Height / 2;
}

void CDraw::Line(float x1, float y1, float x2, float y2, float width, bool antialias, DWORD color)
{
    ID3DXLine* m_Line;

    D3DXCreateLine(pDevice, &m_Line);
    D3DXVECTOR2 line[] = { D3DXVECTOR2(x1, y1), D3DXVECTOR2(x2, y2) };
    m_Line->SetWidth(width);
    if (antialias) m_Line->SetAntialias(1);
    m_Line->Begin();
    m_Line->Draw(line, 2, color);
    m_Line->End();
    m_Line->Release();
}

void CDraw::Circle(float x, float y, float radius, int rotate, int type, bool smoothing, int resolution, DWORD color)
{
    std::vector<vertex> circle(resolution + 2);
    float angle = rotate * D3DX_PI / 180;
    float pi;

    if (type == full) pi = D3DX_PI;        // Full circle
    if (type == half) pi = D3DX_PI / 2;      // 1/2 circle
    if (type == quarter) pi = D3DX_PI / 4;   // 1/4 circle

    for (int i = 0; i < resolution + 2; i++)
    {
        circle[i].x = (float)(x - radius * cos(i * (2 * pi / resolution)));
        circle[i].y = (float)(y - radius * sin(i * (2 * pi / resolution)));
        circle[i].z = 0;
        circle[i].rhw = 1;
        circle[i].color = color;
    }

    // Rotate matrix
    int _res = resolution + 2;
    for (int i = 0; i < _res; i++)
    {
        circle[i].x = x + cos(angle) * (circle[i].x - x) - sin(angle) * (circle[i].y - y);
        circle[i].y = y + sin(angle) * (circle[i].x - x) + cos(angle) * (circle[i].y - y);
    }

    pDevice->CreateVertexBuffer((resolution + 2) * sizeof(vertex), D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &g_pVB, NULL);

    VOID* pVertices;
    g_pVB->Lock(0, (resolution + 2) * sizeof(vertex), (void**)&pVertices, 0);
    memcpy(pVertices, &circle[0], (resolution + 2) * sizeof(vertex));
    g_pVB->Unlock();


    pDevice->SetTexture(0, NULL);
    pDevice->SetPixelShader(NULL);
    if (smoothing)
    {
        pDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
        pDevice->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, TRUE);
    }
    pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    pDevice->SetStreamSource(0, g_pVB, 0, sizeof(vertex));
    pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

    pDevice->DrawPrimitive(D3DPT_LINESTRIP, 0, resolution);
    if (g_pVB != NULL) g_pVB->Release();
}

void CDraw::CircleFilled(float x, float y, float rad, float rotate, int type, int resolution, DWORD color)
{
    std::vector<vertex> circle(resolution + 2);
    float angle = rotate * D3DX_PI / 180;
    float pi;

    if (type == full) pi = D3DX_PI;        // Full circle
    if (type == half) pi = D3DX_PI / 2;      // 1/2 circle
    if (type == quarter) pi = D3DX_PI / 4;   // 1/4 circle

    circle[0].x = x;
    circle[0].y = y;
    circle[0].z = 0;
    circle[0].rhw = 1;
    circle[0].color = color;

    for (int i = 1; i < resolution + 2; i++)
    {
        circle[i].x = (float)(x - rad * cos(pi * ((i - 1) / (resolution / 2.0f))));
        circle[i].y = (float)(y - rad * sin(pi * ((i - 1) / (resolution / 2.0f))));
        circle[i].z = 0;
        circle[i].rhw = 1;
        circle[i].color = color;
    }

    // Rotate matrix
    int _res = resolution + 2;
    for (int i = 0; i < _res; i++)
    {
        circle[i].x = x + cos(angle) * (circle[i].x - x) - sin(angle) * (circle[i].y - y);
        circle[i].y = y + sin(angle) * (circle[i].x - x) + cos(angle) * (circle[i].y - y);
    }

    pDevice->CreateVertexBuffer((resolution + 2) * sizeof(vertex), D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &g_pVB, NULL);

    VOID* pVertices;
    g_pVB->Lock(0, (resolution + 2) * sizeof(vertex), (void**)&pVertices, 0);
    memcpy(pVertices, &circle[0], (resolution + 2) * sizeof(vertex));
    g_pVB->Unlock();

    pDevice->SetTexture(0, NULL);
    pDevice->SetPixelShader(NULL);
    pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    pDevice->SetStreamSource(0, g_pVB, 0, sizeof(vertex));
    pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
    pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, resolution);
    if (g_pVB != NULL) g_pVB->Release();
}

void CDraw::Box(float x, float y, float w, float h, float linewidth, DWORD color)
{
    if (linewidth == 0 || linewidth == 1)
    {
        BoxFilled(x, y, w, 1, color);             // Top
        BoxFilled(x, y + h - 1, w, 1, color);         // Bottom
        BoxFilled(x, y + 1, 1, h - 2 * 1, color);       // Left
        BoxFilled(x + w - 1, y + 1, 1, h - 2 * 1, color);   // Right
    }
    else
    {
        BoxFilled(x, y, w, linewidth, color);                                     // Top
        BoxFilled(x, y + h - linewidth, w, linewidth, color);                         // Bottom
        BoxFilled(x, y + linewidth, linewidth, h - 2 * linewidth, color);               // Left
        BoxFilled(x + w - linewidth, y + linewidth, linewidth, h - 2 * linewidth, color);   // Right
    }
}

void CDraw::BoxBordered(float x, float y, float w, float h, float border_width, DWORD color, DWORD color_border)
{
    BoxFilled(x, y, w, h, color);
    Box(x - border_width, y - border_width, w + 2 * border_width, h + border_width, border_width, color_border);
}

void CDraw::BoxFilled(float x, float y, float w, float h, DWORD color)
{
    vertex V[4];

    V[0].color = V[1].color = V[2].color = V[3].color = color;

    V[0].z = V[1].z = V[2].z = V[3].z = 0;
    V[0].rhw = V[1].rhw = V[2].rhw = V[3].rhw = 0;

    V[0].x = x;
    V[0].y = y;
    V[1].x = x + w;
    V[1].y = y;
    V[2].x = x + w;
    V[2].y = y + h;
    V[3].x = x;
    V[3].y = y + h;

    unsigned short indexes[] = { 0, 1, 3, 1, 2, 3 };

    pDevice->CreateVertexBuffer(4 * sizeof(vertex), D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &g_pVB, NULL);
    pDevice->CreateIndexBuffer(2 * sizeof(short), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &g_pIB, NULL);

    VOID* pVertices;
    g_pVB->Lock(0, sizeof(V), (void**)&pVertices, 0);
    memcpy(pVertices, V, sizeof(V));
    g_pVB->Unlock();

    VOID* pIndex;
    g_pIB->Lock(0, sizeof(indexes), (void**)&pIndex, 0);
    memcpy(pIndex, indexes, sizeof(indexes));
    g_pIB->Unlock();

    pDevice->SetTexture(0, NULL);
    pDevice->SetPixelShader(NULL);
    pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    pDevice->SetStreamSource(0, g_pVB, 0, sizeof(vertex));
    pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
    pDevice->SetIndices(g_pIB);

    pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

    g_pVB->Release();
    g_pIB->Release();
}

void CDraw::BoxRounded(float x, float y, float w, float h, float radius, bool smoothing, DWORD color, DWORD bcolor)
{
    BoxFilled(x + radius, y + radius, w - 2 * radius - 1, h - 2 * radius - 1, color);   // Center rect.
    BoxFilled(x + radius, y + 1, w - 2 * radius - 1, radius - 1, color);            // Top rect.
    BoxFilled(x + radius, y + h - radius - 1, w - 2 * radius - 1, radius, color);     // Bottom rect.
    BoxFilled(x + 1, y + radius, radius - 1, h - 2 * radius - 1, color);            // Left rect.
    BoxFilled(x + w - radius - 1, y + radius, radius, h - 2 * radius - 1, color);     // Right rect.

    // Smoothing method
    if (smoothing)
    {
        CircleFilled(x + radius, y + radius, radius - 1, 0, quarter, 16, color);             // Top-left corner
        CircleFilled(x + w - radius - 1, y + radius, radius - 1, 90, quarter, 16, color);        // Top-right corner
        CircleFilled(x + w - radius - 1, y + h - radius - 1, radius - 1, 180, quarter, 16, color);   // Bottom-right corner
        CircleFilled(x + radius, y + h - radius - 1, radius - 1, 270, quarter, 16, color);       // Bottom-left corner

        Circle(x + radius + 1, y + radius + 1, radius, 0, quarter, true, 16, bcolor);          // Top-left corner
        Circle(x + w - radius - 2, y + radius + 1, radius, 90, quarter, true, 16, bcolor);       // Top-right corner
        Circle(x + w - radius - 2, y + h - radius - 2, radius, 180, quarter, true, 16, bcolor);    // Bottom-right corner
        Circle(x + radius + 1, y + h - radius - 2, radius, 270, quarter, true, 16, bcolor);      // Bottom-left corner

        Line(x + radius, y + 1, x + w - radius - 1, y + 1, 1, false, bcolor);       // Top line
        Line(x + radius, y + h - 2, x + w - radius - 1, y + h - 2, 1, false, bcolor);   // Bottom line
        Line(x + 1, y + radius, x + 1, y + h - radius - 1, 1, false, bcolor);       // Left line
        Line(x + w - 2, y + radius, x + w - 2, y + h - radius - 1, 1, false, bcolor);   // Right line
    }
    else
    {
        CircleFilled(x + radius, y + radius, radius, 0, quarter, 16, color);             // Top-left corner
        CircleFilled(x + w - radius - 1, y + radius, radius, 90, quarter, 16, color);        // Top-right corner
        CircleFilled(x + w - radius - 1, y + h - radius - 1, radius, 180, quarter, 16, color);   // Bottom-right corner
        CircleFilled(x + radius, y + h - radius - 1, radius, 270, quarter, 16, color);       // Bottom-left corner
    }
}

void CDraw::Text(char* text, float x, float y, text_alignment orientation, text_alignment_v v_orientation, int font, bool bordered, DWORD color, DWORD bcolor, int bordersize)
{
    RECT rect;

    DWORD format;

    
    switch (orientation)
    {
    case lefted:
        switch (v_orientation)
        {
        case up:
            format = DT_LEFT | DT_NOCLIP | DT_TOP | DT_SINGLELINE;
            break;
        case down:
            format = DT_LEFT | DT_NOCLIP | DT_BOTTOM | DT_SINGLELINE;
            break;
        case center:
            format = DT_LEFT | DT_NOCLIP | DT_VCENTER | DT_SINGLELINE;
            break;
        }
        break;
    case centered:
        switch (v_orientation)
        {
        case up:
            format = DT_CENTER | DT_NOCLIP | DT_TOP | DT_SINGLELINE;
            break;
        case down:
            format = DT_CENTER | DT_NOCLIP | DT_BOTTOM | DT_SINGLELINE;
            break;
        case center:
            format = DT_CENTER | DT_NOCLIP | DT_VCENTER | DT_SINGLELINE;
            break;
        }
        break;
    case righted:
        switch (v_orientation)
        {
        case up:
            format = DT_RIGHT | DT_NOCLIP | DT_TOP | DT_SINGLELINE;
            break;
        case down:
            format = DT_RIGHT | DT_NOCLIP | DT_BOTTOM | DT_SINGLELINE;
            break;
        case center:
            format = DT_RIGHT | DT_NOCLIP | DT_VCENTER | DT_SINGLELINE;
            break;
        }
        break;
    }
    
    if (bordered)
    {
        SetRect(&rect, x - bordersize, y, x - bordersize, y);
        pFont[font]->DrawTextA(NULL, text, -1, &rect, format, bcolor);
        SetRect(&rect, x + bordersize, y, x + bordersize, y);
        pFont[font]->DrawTextA(NULL, text, -1, &rect, format, bcolor);
        SetRect(&rect, x, y - bordersize, x, y - bordersize);
        pFont[font]->DrawTextA(NULL, text, -1, &rect, format, bcolor);
        SetRect(&rect, x, y + bordersize, x, y + bordersize);
        pFont[font]->DrawTextA(NULL, text, -1, &rect, format, bcolor);

        SetRect(&rect, x - bordersize, y - bordersize, x - bordersize, y - bordersize);
        pFont[font]->DrawTextA(NULL, text, -1, &rect, format, bcolor);
        SetRect(&rect, x - bordersize, y + bordersize, x - bordersize, y + bordersize);
        pFont[font]->DrawTextA(NULL, text, -1, &rect, format, bcolor);
        SetRect(&rect, x + bordersize, y - bordersize, x + bordersize, y - bordersize);
        pFont[font]->DrawTextA(NULL, text, -1, &rect, format, bcolor);
        SetRect(&rect, x + bordersize, y + bordersize, x + bordersize, y + bordersize);
        pFont[font]->DrawTextA(NULL, text, -1, &rect, format, bcolor);
    }
    SetRect(&rect, x, y, x, y);
    pFont[font]->DrawTextA(NULL, text, -1, &rect, format, color);
}

void CDraw::ColoredText(int count, const char* *strings, float x, float y, text_alignment orientation, text_alignment_v v_orientation, int font, bool bordered, DWORD colors[], DWORD bcolor, int bordersize)
{
    RECT rect;
    DWORD format;
    DWORD rectformat;

    switch (orientation)
    {
    case lefted:
        switch (v_orientation)
        {
        case up:
            format = DT_LEFT | DT_NOCLIP | DT_TOP | DT_SINGLELINE;
            rectformat = DT_LEFT | DT_CALCRECT | DT_TOP | DT_SINGLELINE;
            break;
        case down:
            format = DT_LEFT | DT_NOCLIP | DT_BOTTOM | DT_SINGLELINE;
            rectformat = DT_LEFT | DT_CALCRECT | DT_BOTTOM | DT_SINGLELINE;
            break;
        case center:
            format = DT_LEFT | DT_NOCLIP | DT_VCENTER | DT_SINGLELINE;
            rectformat = DT_LEFT | DT_CALCRECT | DT_VCENTER | DT_SINGLELINE;
            break;
        }
        break;
    case centered:
        switch (v_orientation)
        {
        case up:
            format = DT_CENTER | DT_NOCLIP | DT_TOP | DT_SINGLELINE;
            rectformat = DT_CENTER | DT_CALCRECT | DT_TOP | DT_SINGLELINE;
            break;
        case down:
            format = DT_CENTER | DT_NOCLIP | DT_BOTTOM | DT_SINGLELINE;
            rectformat = DT_CENTER | DT_CALCRECT | DT_BOTTOM | DT_SINGLELINE;
            break;
        case center:
            format = DT_CENTER | DT_NOCLIP | DT_VCENTER | DT_SINGLELINE;
            rectformat = DT_CENTER | DT_CALCRECT | DT_VCENTER | DT_SINGLELINE;
            break;
        }
        break;
    case righted:
        switch (v_orientation)
        {
        case up:
            format = DT_RIGHT | DT_NOCLIP | DT_TOP | DT_SINGLELINE;
            rectformat = DT_RIGHT | DT_CALCRECT | DT_TOP | DT_SINGLELINE;
            break;
        case down:
            format = DT_RIGHT | DT_NOCLIP | DT_BOTTOM | DT_SINGLELINE;
            rectformat = DT_RIGHT | DT_CALCRECT | DT_BOTTOM | DT_SINGLELINE;
            break;
        case center:
            format = DT_RIGHT | DT_NOCLIP | DT_VCENTER | DT_SINGLELINE;
            rectformat = DT_RIGHT | DT_CALCRECT | DT_VCENTER | DT_SINGLELINE;
            break;
        }
        break;
    }
    
    char text[512] = {};

    for (int i = 0; i <= count; i++)
    {
        std::strcat(text, strings[i]);
    }
    //printf(text);
    //printf("\n");
    if (bordered)
    {
        SetRect(&rect, x - bordersize, y, x - bordersize, y);
        pFont[font]->DrawTextA(NULL, text, -1, &rect, format, bcolor);
        SetRect(&rect, x + bordersize, y, x + bordersize, y);
        pFont[font]->DrawTextA(NULL, text, -1, &rect, format, bcolor);
        SetRect(&rect, x, y - bordersize, x, y - bordersize);
        pFont[font]->DrawTextA(NULL, text, -1, &rect, format, bcolor);
        SetRect(&rect, x, y + bordersize, x, y + bordersize);
        pFont[font]->DrawTextA(NULL, text, -1, &rect, format, bcolor);

        SetRect(&rect, x - bordersize, y - bordersize, x - bordersize, y - bordersize);
        pFont[font]->DrawTextA(NULL, text, -1, &rect, format, bcolor);
        SetRect(&rect, x - bordersize, y + bordersize, x - bordersize, y + bordersize);
        pFont[font]->DrawTextA(NULL, text, -1, &rect, format, bcolor);
        SetRect(&rect, x + bordersize, y - bordersize, x + bordersize, y - bordersize);
        pFont[font]->DrawTextA(NULL, text, -1, &rect, format, bcolor);
        SetRect(&rect, x + bordersize, y + bordersize, x + bordersize, y + bordersize);
        pFont[font]->DrawTextA(NULL, text, -1, &rect, format, bcolor);
    }

    SetRect(&rect, x, y, x, y);
    pFont[font]->DrawTextA(NULL, text, -1, &rect, rectformat, 0);
    //pFont[font]->DrawTextA(NULL, text, -1, &rect, format, WHITE(255));
    SetRect(&rect, rect.left, rect.top, rect.left, rect.top);
    for (int i = 0; i <= count; ++i)
    {
        pFont[font]->DrawTextA(NULL, strings[i], -1, &rect, DT_LEFT | DT_CALCRECT | DT_TOP | DT_SINGLELINE, 0);
        pFont[font]->DrawTextA(NULL, strings[i], -1, &rect, DT_LEFT | DT_NOCLIP | DT_TOP | DT_SINGLELINE, colors[i]);
        rect.left = rect.right; // offset for next character.
    }
}

void CDraw::Message(char* text, float x, float y, int font, int orientation)
{
    RECT rect = { x, y, x, y };

    switch (orientation)
    {
    case lefted:
        pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_CALCRECT | DT_LEFT, BLACK(255));

        BoxRounded(x - 5, rect.top - 5, rect.right - x + 10, rect.bottom - rect.top + 10, 5, true, DARKGRAY(150), SKYBLUE(255));

        SetRect(&rect, x, y, x, y);
        pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_LEFT | DT_NOCLIP, ORANGE(255));
        break;
    case centered:
        pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_CALCRECT | DT_CENTER, BLACK(255));

        BoxRounded(rect.left - 5, rect.top - 5, rect.right - rect.left + 10, rect.bottom - rect.top + 10, 5, true, DARKGRAY(150), SKYBLUE(255));

        SetRect(&rect, x, y, x, y);
        pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, ORANGE(255));
        break;
    case righted:
        pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_CALCRECT | DT_RIGHT, BLACK(255));

        BoxRounded(rect.left - 5, rect.top - 5, rect.right - rect.left + 10, rect.bottom - rect.top + 10, 5, true, DARKGRAY(150), SKYBLUE(255));

        SetRect(&rect, x, y, x, y);
        pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_RIGHT | DT_NOCLIP, ORANGE(255));
        break;
    }
}

void CDraw::Sprite(LPDIRECT3DTEXTURE9 tex, float x, float y, float resolution, float scale, float rotation)
{
    float screen_pos_x = x;
    float screen_pos_y = y;

    // Texture being used is 64x64:
    D3DXVECTOR2 spriteCentre = D3DXVECTOR2(resolution / 2, resolution / 2);

    // Screen position of the sprite
    D3DXVECTOR2 trans = D3DXVECTOR2(screen_pos_x, screen_pos_y);

    // Build our matrix to rotate, scale and position our sprite
    D3DXMATRIX mat;

    D3DXVECTOR2 scaling(scale, scale);

    // out, scaling centre, scaling rotation, scaling, rotation centre, rotation, translation
    D3DXMatrixTransformation2D(&mat, NULL, 0.0, &scaling, &spriteCentre, rotation, &trans);

    //pDevice->SetRenderState(D3DRS_ZENABLE, false);
    pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
    pDevice->SetPixelShader(NULL);
    sSprite->Begin(NULL);
    sSprite->SetTransform(&mat); // Tell the sprite about the matrix
    sSprite->Draw(tex, NULL, NULL, NULL, 0xFFFFFFFF);
    sSprite->End();
}

bool CDraw::Font()
{
    for (int i = 0; i < FontNr; i++)
        if (pFont[i]) return false;
    return true;
}

void CDraw::AddFont(char* Caption, float size, bool bold, bool italic)
{
    wchar_t wtext[30];
    mbstowcs(wtext, Caption, strlen(Caption) + 1);//Plus null
    LPWSTR ptr = wtext;
    D3DXCreateFont(pDevice, size, 0, (bold) ? FW_BOLD : FW_NORMAL, 1, (italic) ? 1 : 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH, ptr, &pFont[++FontNr]);
}

void CDraw::AddPricedown(float size, bool bold, bool italic)
{
    HINSTANCE hResInstance = (HINSTANCE)Appel::Variables::mainModule;
    HRSRC res = FindResource(hResInstance, MAKEINTRESOURCE(FONT_PRICEDOWN), RT_FONT);

    if (res)
    {
        HGLOBAL mem = LoadResource(hResInstance, res);
        void* data = LockResource(mem);
        size_t len = SizeofResource(hResInstance, res);

        h_Pricedown = AddFontMemResourceEx(
            data,       // font resource
            len,       // number of bytes in font resource 
            NULL,          // Reserved. Must be 0.
            &nFonts      // number of fonts installed
        );

    }

    AddFont((char*)"Pricedown Rus", size, bold, italic);
}

void CDraw::AddChalet(float size, bool bold, bool italic)
{
    HINSTANCE hResInstance = (HINSTANCE)Appel::Variables::mainModule;
    HRSRC res = FindResource(hResInstance, MAKEINTRESOURCE(FONT_CHALET), RT_FONT);

    if (res)
    {
        HGLOBAL mem = LoadResource(hResInstance, res);
        void* data = LockResource(mem);
        size_t len = SizeofResource(hResInstance, res);

        h_Chalet = AddFontMemResourceEx(
            data,       // font resource
            len,       // number of bytes in font resource 
            NULL,          // Reserved. Must be 0.
            &nFonts      // number of fonts installed
        );
    }

    AddFont((char*)"ChaletComprime-CologneSixty", size, bold, italic);
}

void CDraw::FontReset()
{
    for (int i = 0; i < FontNr; i++) pFont[i]->OnLostDevice();
    for (int i = 0; i < FontNr; i++) pFont[i]->OnResetDevice();

    if (h_Pricedown)
        RemoveFontMemResourceEx(h_Pricedown);

    if (h_Chalet)
        RemoveFontMemResourceEx(h_Chalet);
}

void CDraw::OnLostDevice()
{
    for (int i = 0; i < FontNr; i++) pFont[i]->OnLostDevice();
}

