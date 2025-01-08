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

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    void ChangeTileMapSize();

private:
    int                     m_Row;              // TileMap 의 행 숫자
    int                     m_Col;              // TileMap 의 열 숫자
    Vec2                    m_TileSize;         // Tile 1개의 크기

    Ptr<DTexture>           m_TileAtlas;        // Tile 개별 이미지들으 보유하고 있는 아틀라스 텍스쳐
    Vec2                    m_AtlasResolution;  // Atlas 텍스쳐 해상도
    Vec2                    m_AtlasTileSize;    // Atlas 텍스쳐 내에서 타일 1개의 크기
    Vec2                    m_AtlasTileSliceUV; // Atlas 텍스쳐 내에서 타일 1개의 크기를 UV 로 변환한 크기(SliceUV)

    int                     m_AtlasMaxRow;      // Atlas 텍스쳐가 보유하고 있는 타일의 최대 행 숫자
    int                     m_AtlasMaxCol;      // Atlas 텍스쳐가 보유하고 있는 타일의 최대 열 숫자


    vector<tTileInfo>       m_vecTileInfo;      // 각각의 타일의 정보
    DStructuredBuffer* m_Buffer;           // 각각의 타일정보를 t 레지스터로 전달하기 위한 구조화버퍼

};

#endif // !ENGINE_DTILEMAP_H