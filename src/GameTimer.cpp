#include "GameTimer.h"

GameTimer::GameTimer()
    : mSecondsPerCount(0),
      mDeltaTime(-1.0),
      mBaseTime(0),
      mPausedTime(0),
      mPrevTime(0),
      mCurrentTime(0),
      mStopped(false)
{
    __int64 countsPerSec;
    QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&countsPerSec));
    mSecondsPerCount = 1.0 / static_cast<double>(countsPerSec);
}

void GameTimer::Tick()
{
    if (mStopped)
    {
        mDeltaTime = 0.0;
        return;
    }

    // 이번 프레임 시간 얻기
    __int64 currentTime;
    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currentTime));
    mCurrentTime = currentTime;

    // 이전 프레임 시간과 차이 구함
    mDeltaTime = (mCurrentTime - mPrevTime) * mSecondsPerCount;

    // 다음 프레임 준비
    mPrevTime = mCurrentTime;

    // 특수한 경우 음수가 될 수 있기에 음수 방지
    if (mDeltaTime < 0.0) mDeltaTime = 0.0;
}

float GameTimer::DeltaTime() const { return static_cast<float>(mDeltaTime); }

void GameTimer::Reset()
{
    __int64 currentTime;
    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currentTime));

    mBaseTime = currentTime;
    mPrevTime = currentTime;
    mStopTime = 0;
    mStopped = false;
}

void GameTimer::Stop()
{
    if (!mStopped)
    {
        __int64 currentTime;
        QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currentTime));

        // 중단 시점 시간 저장 및 타이머 중단 플래그 변경
        mStopTime = currentTime;
        mStopped = true;
    }
}

void GameTimer::Start()
{
    __int64 startTime;
    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&startTime));

    if (mStopped)
    {
        // 일시정지한 시간 누적
        mPausedTime += (startTime - mStopTime);

        // 기타 변수 초기화
        mPrevTime = startTime;
        mStopTime = 0;
        mStopped = false;
    }
}

float GameTimer::TotalTime() const
{
    if (mStopped)
    {
        return static_cast<double>(((mStopTime - mPausedTime) - mBaseTime) *
                                   mSecondsPerCount);
    }
    else
    {
        return static_cast<double>(((mCurrentTime - mPausedTime) - mBaseTime) *
                                   mSecondsPerCount);
    }
}
