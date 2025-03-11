#include "pch.h"
#include "TE_TileMapMod.h"
#include "TE_TileMapView.h"
#include "TE_TileSetView.h"
#include "DAssetMgr.h"
#include "DPathMgr.h"
#include "DDevice.h"

TE_TileMapMod::TE_TileMapMod()
    : m_NumRows(10)
    , m_NumColumns(10)
{
    // Initialize file name buffer
    memset(m_SaveFileName, 0, sizeof(m_SaveFileName));
    strcpy_s(m_SaveFileName, "NewTileMap");
}

TE_TileMapMod::~TE_TileMapMod()
{
}

void TE_TileMapMod::Init()
{
    // Get initial grid size from TileMapView
    //m_NumRows = GetTileMapView()->GetNumRows();
    //m_NumColumns = GetTileMapView()->GetNumColumns();

    m_NumRows = 10;
    m_NumColumns = 10;
}

void TE_TileMapMod::Update()
{
    // Grid size controls
    ImGui::Text("Grid Dimensions");

    // Row control
    ImGui::InputInt("Rows", &m_NumRows);
    m_NumRows = max(1, m_NumRows);

    // Column control
    ImGui::InputInt("Columns", &m_NumColumns);
    m_NumColumns = max(1, m_NumColumns);

    // Apply button for grid size changes
    if (ImGui::Button("Apply Grid Size"))
    {
        GetTileMapView()->SetGridSize(m_NumRows, m_NumColumns);
    }

    ImGui::Separator();

    // File operations
    ImGui::Text("File Operations");

    // File name input
    ImGui::InputText("File Name", m_SaveFileName, sizeof(m_SaveFileName));

    // Save/Load buttons
    if (ImGui::Button("Save TileMap"))
    {
        SaveTileMap();
    }

    ImGui::SameLine();

    if (ImGui::Button("Load TileMap"))
    {
        LoadTileMap();
    }
}

void TE_TileMapMod::SetGridSize(int _rows, int _columns)
{
    m_NumRows = max(1, _rows);
    m_NumColumns = max(1, _columns);
    GetTileMapView()->SetGridSize(m_NumRows, m_NumColumns);
}

Ptr<DTexture> TE_TileMapMod::RenderTileMapToTexture()
{
    OutputDebugString(L"=== Starting RenderTileMapToTexture ===\n");

    // Get tilemap dimensions
    int rows = GetTileMapView()->GetNumRows();
    int cols = GetTileMapView()->GetNumColumns();
    float tileWidth = GetTileMapView()->GetTileWidth();
    float tileHeight = GetTileMapView()->GetTileHeight();

    // Calculate texture dimensions
    UINT texWidth = static_cast<UINT>(cols * tileWidth);
    UINT texHeight = static_cast<UINT>(rows * tileHeight);

    if (texWidth == 0 || texHeight == 0)
    {
        MessageBox(nullptr, L"Invalid tilemap dimensions.", L"Error", MB_OK | MB_ICONERROR);
        return nullptr;
    }

    // Create render target texture
    Ptr<DTexture> renderTarget = new DTexture();
    HRESULT hr = renderTarget->Create(texWidth, texHeight, DXGI_FORMAT_R8G8B8A8_UNORM,
        D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
    if (FAILED(hr))
    {
        wchar_t errMsg[256];
        swprintf_s(errMsg, L"Failed to create render target texture. HRESULT: 0x%08X", hr);
        OutputDebugString(errMsg);
        return nullptr;
    }

    ID3D11DeviceContext* pContext = CONTEXT;

    // Save current render target and viewport
    ComPtr<ID3D11RenderTargetView> oldRTV;
    ComPtr<ID3D11DepthStencilView> oldDSV;
    pContext->OMGetRenderTargets(1, oldRTV.GetAddressOf(), oldDSV.GetAddressOf());

    D3D11_VIEWPORT oldViewport;
    UINT numViewports = 1;
    pContext->RSGetViewports(&numViewports, &oldViewport);

    // Set new render target
    pContext->OMSetRenderTargets(1, renderTarget->GetRTV().GetAddressOf(), nullptr);

    // Set viewport
    D3D11_VIEWPORT viewport = {};
    viewport.Width = static_cast<float>(texWidth);
    viewport.Height = static_cast<float>(texHeight);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    pContext->RSSetViewports(1, &viewport);

    // Clear render target
    float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f }; // Transparent background
    pContext->ClearRenderTargetView(renderTarget->GetRTV().Get(), clearColor);

    // Render tiles
    TileData* tileData = GetTileMapView()->GetTileData();
    Ptr<DTexture> tilesetTexture = GetTileSetView()->GetTilesetTexture();

    if (!tilesetTexture)
    {
        OutputDebugString(L"Error: Tileset texture is null\n");
        return nullptr;
    }

    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < cols; ++col)
        {
            int index = row * cols + col;
            if (tileData[index].IsOccupied)
            {
                float x = col * tileWidth;
                float y = row * tileHeight;

                Vec2 tileIndex = tileData[index].TileIndex;
                Vec2 tileSize = GetTileSetView()->GetTileSize();

                float u0 = tileIndex.x * tileSize.x / tilesetTexture->GetWidth();
                float v0 = tileIndex.y * tileSize.y / tilesetTexture->GetHeight();
                float u1 = (tileIndex.x + 1) * tileSize.x / tilesetTexture->GetWidth();
                float v1 = (tileIndex.y + 1) * tileSize.y / tilesetTexture->GetHeight();

                RenderSpriteTile(pContext, tilesetTexture, x, y, tileWidth, tileHeight, u0, v0, u1, v1);
            }
        }
    }

    // Restore previous states
    pContext->OMSetRenderTargets(1, oldRTV.GetAddressOf(), oldDSV.Get());
    pContext->RSSetViewports(1, &oldViewport);

    OutputDebugString(L"=== Finished RenderTileMapToTexture ===\n");

    return renderTarget;
}

void TE_TileMapMod::RenderSpriteTile(ID3D11DeviceContext* pContext,
    Ptr<DTexture> texture,
    float x, float y,
    float width, float height,
    float u0, float v0,
    float u1, float v1)
{
    // Define vertices with proper positions and UVs
    struct Vertex
    {
        float x, y, z; // Position
        float u, v;    // Texture coordinates
    };

    Vertex vertices[] = {
        {x,         y + height, 0.0f, u0, v1}, // Bottom-left
        {x + width, y + height, 0.0f, u1, v1}, // Bottom-right
        {x + width, y,          0.0f, u1, v0}, // Top-right
        {x,         y,          0.0f, u0, v0}  // Top-left
    };

    UINT indices[] = {
        0, 1, 2,
        0, 2, 3,
    };

    ComPtr<ID3D11Buffer> vertexBuffer;
    ComPtr<ID3D11Buffer> indexBuffer;

    D3D11_BUFFER_DESC vbDesc{};
    vbDesc.ByteWidth = sizeof(vertices);
    vbDesc.Usage = D3D11_USAGE_DEFAULT;
    vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA vbData{};
    vbData.pSysMem = vertices;

    HRESULT hr = DEVICE->CreateBuffer(&vbDesc, &vbData, vertexBuffer.GetAddressOf());
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed to create vertex buffer\n");
        return;
    }

    D3D11_BUFFER_DESC ibDesc{};
    ibDesc.ByteWidth = sizeof(indices);
    ibDesc.Usage = D3D11_USAGE_DEFAULT;
    ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA ibData{};
    ibData.pSysMem = indices;

    hr = DEVICE->CreateBuffer(&ibDesc, &ibData, indexBuffer.GetAddressOf());
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed to create index buffer\n");
        return;
    }

    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    pContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

    pContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

    pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Use TE_EditorMtrl material for rendering tileset textures.
    Ptr<DMaterial> material = DAssetMgr::GetInst()->FindAsset<DMaterial>(L"TE_EditorMtrl");
    if (!material)
    {
        OutputDebugString(L"Error: TE_EditorMtrl not found\n");
        return;
    }

    material->SetTexParam(TEX_0, texture);
    material->SetScalarParam(INT_0, 1); // Enable g_btex_0

    material->Binding();

    pContext->DrawIndexed(6, 0, 0);
}

void TE_TileMapMod::SaveTileMap()
{
    OutputDebugString(L"=== Starting SaveTileMap ===\n");

    // Ensure we have a valid tileset texture
    Ptr<DTexture> tileset = GetTileSetView()->GetTilesetTexture();
    if (!tileset)
    {
        MessageBox(nullptr, L"No tileset loaded. Cannot save tilemap.", L"Error", MB_OK | MB_ICONERROR);
        return;
    }

    // Generate the texture from our tilemap
    Ptr<DTexture> tileMapTexture = RenderTileMapToTexture();
    if (!tileMapTexture)
    {
        MessageBox(nullptr, L"Failed to render tilemap to texture.", L"Error", MB_OK | MB_ICONERROR);
        return;
    }

    OutputDebugString(L"Successfully rendered tilemap to texture\n");

    // Generate full path
    wstring strContentPath = DPathMgr::GetInst()->GetContentPath();
    wstring fullPath = strContentPath + L"TileMap\\";

    // Create directory if it doesn't exist
    CreateDirectory(fullPath.c_str(), NULL);

    // Convert char filename to wstring
    wstring wFilename;
    wFilename.assign(m_SaveFileName, m_SaveFileName + strlen(m_SaveFileName));
    fullPath += wFilename + L".png";

    OutputDebugString((L"Saving to path: " + fullPath + L"\n").c_str());

    // Get the resource from the texture
    ComPtr<ID3D11Resource> resource;
    tileMapTexture->GetSRV()->GetResource(resource.GetAddressOf());

    // Cast to ID3D11Texture2D
    ComPtr<ID3D11Texture2D> texture2D;
    HRESULT hr = resource.As(&texture2D);
    if (FAILED(hr))
    {
        wchar_t errMsg[256];
        swprintf_s(errMsg, L"Failed to get texture resource. HRESULT: 0x%08X", hr);
        OutputDebugString(errMsg);
        MessageBox(nullptr, errMsg, L"Error", MB_OK | MB_ICONERROR);
        return;
    }

    // Create a staging texture for CPU access
    D3D11_TEXTURE2D_DESC texDesc;
    texture2D->GetDesc(&texDesc);

    D3D11_TEXTURE2D_DESC stagingDesc = texDesc;
    stagingDesc.Usage = D3D11_USAGE_STAGING;
    stagingDesc.BindFlags = 0;
    stagingDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    stagingDesc.MiscFlags = 0;

    ComPtr<ID3D11Texture2D> stagingTexture;
    hr = DEVICE->CreateTexture2D(&stagingDesc, nullptr, stagingTexture.GetAddressOf());
    if (FAILED(hr))
    {
        wchar_t errMsg[256];
        swprintf_s(errMsg, L"Failed to create staging texture. HRESULT: 0x%08X", hr);
        OutputDebugString(errMsg);
        MessageBox(nullptr, errMsg, L"Error", MB_OK | MB_ICONERROR);
        return;
    }

    // Copy resource to staging texture
    CONTEXT->CopyResource(stagingTexture.Get(), texture2D.Get());

    // Use DirectXTex to capture and save the texture
    DirectX::ScratchImage image;
    hr = DirectX::CaptureTexture(DEVICE, CONTEXT, stagingTexture.Get(), image);
    if (FAILED(hr))
    {
        wchar_t errMsg[256];
        swprintf_s(errMsg, L"Failed to capture texture. HRESULT: 0x%08X", hr);
        OutputDebugString(errMsg);
        MessageBox(nullptr, errMsg, L"Error", MB_OK | MB_ICONERROR);
        return;
    }

    // Save the image to a PNG file
    hr = DirectX::SaveToWICFile(*image.GetImage(0, 0, 0), DirectX::WIC_FLAGS_NONE,
        GetWICCodec(DirectX::WIC_CODEC_PNG), fullPath.c_str());

    if (SUCCEEDED(hr))
    {
        OutputDebugString(L"Successfully saved PNG file\n");

        // Register the texture with asset manager
        Ptr<DTexture> savedTexture = new DTexture();
        if (SUCCEEDED(savedTexture->Load(fullPath)))
        {
            // Add to asset manager
            DAssetMgr::GetInst()->AddAsset(wFilename.c_str(), savedTexture);

            // Save additional metadata
            SaveTileMapMetadata(wFilename);

            MessageBox(nullptr, L"TileMap saved successfully.", L"Success", MB_OK | MB_ICONINFORMATION);
        }
        else
        {
            OutputDebugString(L"Warning: Failed to load saved texture as asset\n");
            MessageBox(nullptr, L"TileMap image saved, but failed to load as asset.",
                L"Warning", MB_OK | MB_ICONWARNING);
        }
    }
    else
    {
        wchar_t errMsg[256];
        swprintf_s(errMsg, L"Failed to save PNG file. HRESULT: 0x%08X", hr);
        OutputDebugString(errMsg);
        MessageBox(nullptr, errMsg, L"Error", MB_OK | MB_ICONERROR);
    }

    OutputDebugString(L"=== Finished SaveTileMap ===\n");
}

void TE_TileMapMod::SaveTileMapMetadata(const wstring& filename)
{
    // Save the additional metadata needed to recreate this tilemap
    wstring metadataPath = DPathMgr::GetInst()->GetContentPath() + L"TileMap\\" + filename + L".metadata";

    FILE* file = nullptr;
    _wfopen_s(&file, metadataPath.c_str(), L"wb");
    if (!file)
    {
        OutputDebugString(L"Failed to create metadata file\n");
        return;
    }

    // Save tilemap dimensions
    int rows = GetTileMapView()->GetNumRows();
    int cols = GetTileMapView()->GetNumColumns();
    fwrite(&rows, sizeof(int), 1, file);
    fwrite(&cols, sizeof(int), 1, file);

    // Save tile size
    float tileWidth = GetTileMapView()->GetTileWidth();
    float tileHeight = GetTileMapView()->GetTileHeight();
    fwrite(&tileWidth, sizeof(float), 1, file);
    fwrite(&tileHeight, sizeof(float), 1, file);

    // Save tileset information
    Ptr<DTexture> tileset = GetTileSetView()->GetTilesetTexture();
    wstring tilesetPath = tileset->GetRelativePath();
    size_t pathLen = tilesetPath.length();
    fwrite(&pathLen, sizeof(size_t), 1, file);
    fwrite(tilesetPath.c_str(), sizeof(wchar_t), pathLen, file);

    // Save all tile data with better error handling
    TileData* tileData = GetTileMapView()->GetTileData();
    for (int i = 0; i < rows * cols; ++i)
    {
        // Save whether this tile is occupied
        fwrite(&tileData[i].IsOccupied, sizeof(bool), 1, file);

        // If occupied, save the tile index
        if (tileData[i].IsOccupied)
        {
            fwrite(&tileData[i].TileIndex, sizeof(Vec2), 1, file);
        }
    }

    fclose(file);
    OutputDebugString(L"Successfully saved tilemap metadata\n");
}

void TE_TileMapMod::LoadTileMap()
{
    // Convert char filename to wstring
    wstring wFilename;
    wFilename.assign(m_SaveFileName, m_SaveFileName + strlen(m_SaveFileName));

    // Generate metadata path
    wstring strContentPath = DPathMgr::GetInst()->GetContentPath();
    wstring metadataPath = strContentPath + L"TileMap\\" + wFilename + L".meta";

    // Check if metadata file exists
    FILE* file = nullptr;
    _wfopen_s(&file, metadataPath.c_str(), L"rb");
    if (!file)
    {
        MessageBox(nullptr, L"TileMap metadata file not found.", L"Error", MB_OK | MB_ICONERROR);
        return;
    }

    // Load grid dimensions
    int rows, cols;
    fread(&rows, sizeof(int), 1, file);
    fread(&cols, sizeof(int), 1, file);

    // Load tile size
    float tileWidth, tileHeight;
    fread(&tileWidth, sizeof(float), 1, file);
    fread(&tileHeight, sizeof(float), 1, file);

    // Load tileset name
    int nameLength;
    fread(&nameLength, sizeof(int), 1, file);
    wchar_t* tilesetNameBuffer = new wchar_t[nameLength + 1];
    fread(tilesetNameBuffer, sizeof(wchar_t), nameLength, file);
    tilesetNameBuffer[nameLength] = L'\0';
    wstring tilesetName = tilesetNameBuffer;
    delete[] tilesetNameBuffer;

    // Load tileset
    Ptr<DTexture> tileset = DAssetMgr::GetInst()->FindAsset<DTexture>(tilesetName);
    if (!tileset)
    {
        fclose(file);
        MessageBox(nullptr, L"Tileset texture not found.", L"Error", MB_OK | MB_ICONERROR);
        return;
    }

    // Update editor views
    GetTileMapView()->SetGridSize(rows, cols);
    GetTileMapView()->SetTileSize(tileWidth, tileHeight);
    GetTileSetView()->SetTilesetTexture(tileset);

    // Load tile data
    for (int i = 0; i < rows * cols; ++i)
    {
        bool isOccupied;
        fread(&isOccupied, sizeof(bool), 1, file);

        if (isOccupied)
        {
            Vec2 tileIndex;
            fread(&tileIndex, sizeof(Vec2), 1, file);

            int row = i / cols;
            int col = i % cols;
            GetTileMapView()->PlaceTile(row, col, tileIndex);
        }
    }

    fclose(file);

    // Load the texture as well (optional, mostly for preview)
    wstring texturePath = strContentPath + L"TileMap\\" + wFilename + L".png";
    Ptr<DTexture> tileMapTexture = new DTexture();
    if (SUCCEEDED(tileMapTexture->Load(texturePath)))
    {
        DAssetMgr::GetInst()->AddAsset(wFilename.c_str(), tileMapTexture);
    }

    // Show success message
    MessageBox(nullptr, L"TileMap loaded successfully.", L"Success", MB_OK | MB_ICONINFORMATION);
}
