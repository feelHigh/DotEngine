#include "pch.h"
#include "DTileMap.h"

// Device Headers
#include "DStructuredBuffer.h"

// Manager Headers
#include "DAssetMgr.h"

// Component Headers
#include "DTransform.h"

DTileMap::DTileMap()
    : DRenderComponent(COMPONENT_TYPE::TILEMAP)
    , m_Row(1)
    , m_Col(1)
    , m_RowCol(1, 1)
    , m_AtlasMaxRow(0)
    , m_AtlasMaxCol(0)
    , m_Buffer(nullptr)
{
    SetMesh(DAssetMgr::GetInst()->FindAsset<DMesh>(L"RectMesh"));
    SetMaterial(DAssetMgr::GetInst()->FindAsset<DMaterial>(L"TileMapMtrl"));
    m_Buffer = new DStructuredBuffer;
}

DTileMap::DTileMap(const DTileMap& _Origin)
    : DRenderComponent(_Origin)
    , m_Row(_Origin.m_Row)
    , m_Col(_Origin.m_Col)
    , m_RowCol(_Origin.m_RowCol)
    , m_TileSize(_Origin.m_TileSize)
    , m_TileAtlas(_Origin.m_TileAtlas)
    , m_AtlasResolution(_Origin.m_AtlasResolution)
    , m_AtlasTileSize(_Origin.m_AtlasTileSize)
    , m_AtlasTileSliceUV(_Origin.m_AtlasTileSliceUV)
    , m_AtlasMaxRow(_Origin.m_AtlasMaxRow)
    , m_AtlasMaxCol(_Origin.m_AtlasMaxCol)
    , m_vecTileInfo(_Origin.m_vecTileInfo)
    , m_Buffer(nullptr)
{
    m_Buffer = new DStructuredBuffer;
}

DTileMap::~DTileMap()
{
    delete m_Buffer;
}

void DTileMap::Init()
{
    // Set rows, columns to resize structured buffers
    SetRowCol(m_Row, m_Col);
}

void DTileMap::FinalTick()
{
}

void DTileMap::Render()
{
    // Tile information is bound to the 't' register through a structured buffer
    m_Buffer->SetData(m_vecTileInfo.data(), sizeof(tTileInfo) * m_Row * m_Col);
    m_Buffer->Binding(15);
    GetMaterial()->SetTexParam(TEX_0, m_TileAtlas);
    GetMaterial()->SetScalarParam(INT_1, m_AtlasMaxRow);
    GetMaterial()->SetScalarParam(INT_2, m_AtlasMaxCol);
    GetMaterial()->SetScalarParam(VEC2_1, Vec2(m_Col, m_Row));
    GetMaterial()->SetScalarParam(VEC2_0, m_AtlasTileSliceUV);
    GetMaterial()->Binding();
    Transform()->Binding();
    GetMesh()->Render();
}

void DTileMap::SetRowCol(UINT _Row, UINT _Col)
{
    m_Row = _Row;
    m_Col = _Col;

    // Exclusive for TileMapUI
    m_RowCol = Vec2((float)_Row, (float)_Col);

    ChangeTileMapSize();

    // Number of tiles
    UINT TileCount = m_Row * m_Col;

    // Resize if the number of pieces of data in the vector that stores tile information is different from the number of tiles
    if (m_vecTileInfo.size() != TileCount)
    {
        m_vecTileInfo.clear();
        m_vecTileInfo.resize(TileCount);

        // Initialize all tiles as empty
        /*for (size_t i = 0; i < m_vecTileInfo.size(); ++i)
        {
            m_vecTileInfo[i].ImgIdx = -1;
        }*/
    }
    // Resize if the structured buffer that receives tile information and sends it to the 't' register is smaller than the total tile data size
    if (m_Buffer->GetElementCount() < TileCount)
    {
        m_Buffer->Create(sizeof(tTileInfo), TileCount, SB_TYPE::SRV_ONLY, true);
    }
}

void DTileMap::SetTileSize(Vec2 _Size)
{
    m_TileSize = _Size;
    ChangeTileMapSize();
}

void DTileMap::ChangeTileMapSize()
{
    Vec2 vSize = m_TileSize * Vec2((float)m_Col, (float)m_Row);
    Transform()->SetRelativeScale(vSize.x, vSize.y, 1.f);
}

void DTileMap::SetAtlasTexture(Ptr<DTexture> _Atlas)
{
    m_TileAtlas = _Atlas;
    if (nullptr == m_TileAtlas)
        m_AtlasResolution = Vec2(0.f, 0.f);
    else
        m_AtlasResolution = Vec2((float)_Atlas->GetWidth(), (float)_Atlas->GetHeight());
    SetAtlasTileSize(m_AtlasTileSize);
}

void DTileMap::SetAtlasTileSize(Vec2 _TileSize)
{
    m_AtlasTileSize = _TileSize;

    if (nullptr != m_TileAtlas)
    {
        m_AtlasTileSliceUV = m_AtlasTileSize / m_AtlasResolution;
        m_AtlasMaxCol = int(m_AtlasResolution.x / m_AtlasTileSize.x);
        m_AtlasMaxRow = int(m_AtlasResolution.y / m_AtlasTileSize.y);
    }
}

void DTileMap::SetTile(int _Row, int _Col, UINT _ImgIdx)
{
    if (_Row < 0 || _Row >= m_Row || _Col < 0 || _Col >= m_Col)
        return;

    int idx = _Row * m_Col + _Col;

    m_vecTileInfo[idx].ImgIdx = _ImgIdx;
}

int DTileMap::GetTile(int _Row, int _Col) const
{
    if (_Row < 0 || _Row >= m_Row || _Col < 0 || _Col >= m_Col)
        return -1;

    int idx = _Row * m_Col + _Col;

    return m_vecTileInfo[idx].ImgIdx;
}

void DTileMap::ClearTile(int _Row, int _Col)
{
    if (_Row < 0 || _Row >= m_Row || _Col < 0 || _Col >= m_Col)
        return;

    int idx = _Row * m_Col + _Col;

    m_vecTileInfo[idx].ImgIdx = -1;
}

void DTileMap::SetTileIndex(int _Row, int _Col, Vec2 _TileCoord)
{
    int imgIdx = TileCoordToImgIdx(_TileCoord);

    SetTile(_Row, _Col, imgIdx);
}

Vec2 DTileMap::GetTileIndex(int _Row, int _Col) const
{
    int imgIdx = GetTile(_Row, _Col);

    return ImgIdxToTileCoord(imgIdx);
}

int DTileMap::TileCoordToImgIdx(Vec2 _TileCoord) const
{
    int row = (int)_TileCoord.y;
    int col = (int)_TileCoord.x;

    if (row < 0 || col < 0 || row >= m_AtlasMaxRow || col >= m_AtlasMaxCol)
        return -1;

    return row * m_AtlasMaxCol + col;
}

Vec2 DTileMap::ImgIdxToTileCoord(int _ImgIdx) const
{
    if (_ImgIdx < 0 || _ImgIdx >= (m_AtlasMaxRow * m_AtlasMaxCol))
        return Vec2(-1.f, -1.f);

    int row = _ImgIdx / m_AtlasMaxCol;
    int col = _ImgIdx % m_AtlasMaxCol;

    return Vec2((float)col, (float)row);
}

void DTileMap::SaveToFile(FILE* _File)
{
    SaveDataToFile(_File);
    fwrite(&m_Row, sizeof(int), 1, _File);
    fwrite(&m_Col, sizeof(int), 1, _File);
    fwrite(&m_RowCol, sizeof(Vec2), 1, _File);
    fwrite(&m_TileSize, sizeof(Vec2), 1, _File);
    fwrite(&m_AtlasTileSize, sizeof(Vec2), 1, _File);
    for (size_t i = 0; i < m_vecTileInfo.size(); ++i)
    {
        fwrite(&m_vecTileInfo[i], sizeof(tTileInfo), 1, _File);
    }
    // Atlas Texture
    SaveAssetRef(m_TileAtlas, _File);
}

void DTileMap::LoadFromFile(FILE* _File)
{
    LoadDataFromFile(_File);
    fread(&m_Row, sizeof(int), 1, _File);
    fread(&m_Col, sizeof(int), 1, _File);
    fread(&m_RowCol, sizeof(Vec2), 1, _File);
    SetRowCol(m_Row, m_Col);
    fread(&m_TileSize, sizeof(Vec2), 1, _File);
    fread(&m_AtlasTileSize, sizeof(Vec2), 1, _File);
    SetTileSize(m_TileSize);
    for (size_t i = 0; i < m_vecTileInfo.size(); ++i)
    {
        fread(&m_vecTileInfo[i], sizeof(tTileInfo), 1, _File);
    }
    // Atlas Texture
    LoadAssetRef(m_TileAtlas, _File);
    if (nullptr != m_TileAtlas)
    {
        SetAtlasTexture(m_TileAtlas);
    }
}

int DTileMap::Save(const wstring& _FilePath)
{
    FILE* File = nullptr;
    _wfopen_s(&File, _FilePath.c_str(), L"wb");
    if (nullptr == File)
        return E_FAIL;

    SaveToFile(File);

    fclose(File);
    return S_OK;
}

int DTileMap::Load(const wstring& _FilePath)
{
    FILE* File = nullptr;
    _wfopen_s(&File, _FilePath.c_str(), L"rb");
    if (nullptr == File)
        return E_FAIL;

    LoadFromFile(File);

    fclose(File);
    return S_OK;
}
