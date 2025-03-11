#ifndef ENGINE_DSOUND_H
#define ENGINE_DSOUND_H

#include "DAsset.h"

class DSound :
    public DAsset
{
public:
    DSound();
    ~DSound();

public:
    // _iRoopCount: 0 (Infinite Repeat), _fVolume: 0 to 1 (Volume), _bOverlap: Sound Overlap: Possible
    int Play(int _iRoopCount, float _fVolume, bool _bOverlap);
    void RemoveChannel(FMOD::Channel* _pTargetChannel);
    void Stop();

    // 0 ~ 1
    void SetVolume(float _f, int _iChannelIdx);

public:
    virtual int Load(const wstring& _FilePath) override;
    virtual int Save(const wstring& _FilePath) override;

private:
    FMOD::Sound*            m_Sound;        // Sound Buffer
    list<FMOD::Channel*>	m_listChannel;  // List of channels on which the sound is playing

};

#endif // !ENGINE_DSOUND_H