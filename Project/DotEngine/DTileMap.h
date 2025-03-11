#ifndef ENGINE_DTILEMAP_H
#define ENGINE_DTILEMAP_H

#include "DRenderComponent.h"

class DStructuredBuffer;

struct tTileInfo
{
    int ImgIdx;
    int padding[3];
};

class DTileMap :
    public DRenderComponent
{
public:
    CLONE(DTileMap);
    DTileMap();
    DTileMap(const DTileMap& _Origin);
    ~DTileMap();

public:
    virtual void Init() override;
    virtual void FinalTick() override;
    virtual void Render() override;

public:
    void SetRowCol(UINT _Row, UINT _Col);
    void SetTileSize(Vec2 _Size);
    void SetAtlasTexture(Ptr<DTexture> _Atlas);
    void SetAtlasTileSize(Vec2 _TileSize);

    // Existing tile methods
    void SetTile(int _Row, int _Col, UINT _ImgIdx);
    int GetTile(int _Row, int _Col) const;
    void ClearTile(int _Row, int _Col);

    // New methods for editor integration
    void SetTileIndex(int _Row, int _Col, Vec2 _TileCoord);
    Vec2 GetTileIndex(int _Row, int _Col) const;
    int TileCoordToImgIdx(Vec2 _TileCoord) const;
    Vec2 ImgIdxToTileCoord(int _ImgIdx) const;

public:
    Vec2 GetRowCol() { return m_RowCol; }
    Vec2 GetTileSize() { return m_TileSize; }
    Ptr<DTexture> GetAtlasTexture() { return m_TileAtlas; }
    Vec2 GetAtlasTileSize() { return m_AtlasTileSize; }
    int GetRow() const { return m_Row; }
    int GetCol() const { return m_Col; }

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    int Save(const wstring& _FilePath);
    int Load(const wstring& _FilePath);

private:
    void ChangeTileMapSize();

private:
    int m_Row;                          // Number of rows in TileMap
    int m_Col;                          // Number of columns on TileMap
    Vec2 m_RowCol;                      // TileMap rows, columns in Vec2
    Vec2 m_TileSize;                    // Size of 1 Tile
    Ptr<DTexture> m_TileAtlas;          // Atlas texture that holds individual images of the tiles
    Vec2 m_AtlasResolution;             // Atlas resolution
    Vec2 m_AtlasTileSize;               // Size of 1 tile within Atlas Texture
    Vec2 m_AtlasTileSliceUV;            // Size of 1 tile converted to UV within Atlas texture (SliceUV)
    int m_AtlasMaxRow;                  // Maximum number of rows of tiles held by Atlas Texture
    int m_AtlasMaxCol;                  // Maximum number of columns of tiles held by Atlas Texture
    vector<tTileInfo> m_vecTileInfo;    // Information for each tile
    DStructuredBuffer* m_Buffer;        // Structured buffer to transfer each tile information to 't' register
};

#endif // !ENGINE_DTILEMAP_H