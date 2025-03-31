#pragma once

#include <Windows.h>

class GameTimer
{
   public:
    GameTimer();

    float GameTime() const;   // 초 단위
    float DeltaTime() const;  // 초 단위
    float TotalTime() const;  // 초 단위위

    void Reset();
    void Start();
    void Stop();
    void Tick();

   private:
    double mSecondsPerCount;
    double mDeltaTime;

    __int64 mBaseTime;     // 프로그램 시작 후 경과 시간
    __int64 mPausedTime;   // 멈춰있던 시간
    __int64 mStopTime;     // 멈추기 시작한 시간
    __int64 mPrevTime;     // 과거 측정된 시간
    __int64 mCurrentTime;  // 현재 측정된 시간

    bool mStopped;  // 정지 여부
};