/*
  ==============================================================================

    FIFO.h
    Created: 4 Oct 2021 4:37:37pm
    Author:  tuiji da

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

template<typename T>
class FIFO
{
public:
    void prepare(int numChannels, int numSamples)
    {
        for (auto& buffer : buffers)
        {
            buffer.setSize(numChannels,
                numSamples,
                false,   //clear everything?
                true,    //including the extra space?
                true);   //avoid reallocating if you can?
            buffer.clear();
        }
    }

    void prepare(int numElements)
    {
        for (auto& buffer : buffers)
        {
            buffer.clear();
            buffer.resize(1, numElements);
        }
    }

    bool push(const T& b)  //入栈
    {
        auto write = fifo.write(1);
        if (write.blockSize1 > 0)
        {
            buffers[write.startIndex1] = b;
            return true;
        }

        return false;
    }

    bool pop(T& b)  //出栈
    {
        auto read = fifo.read(1);
        if (read.blockSize1 > 0)
        {
            b = buffers[read.startIndex1];
            return true;
        }

        return false;
    }

    int getNumAvailableForReading() const //可用数
    {
        return fifo.getNumReady();
    }

private:
    static const int Capacity{ 1024 };  //大小
    std::array<T, Capacity> buffers;    //存储
    juce::AbstractFifo fifo{ Capacity };//FIFO类只负责记录，不负责存储
};

template<typename BlockType>
class SingleChannelSampleFifo
{
public:
    SingleChannelSampleFifo(int ch) : channelToUse(ch) // ch 使用的通道数
    {
        prepared.set(false);
    }

    void update(const BlockType& buffer) //更新数据
    {
        bufferToFill = buffer;
        auto* channelPtr = buffer.getReadPointer(channelToUse);

        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            pushNextSampleIntoFifo(channelPtr[i]);
        }
    }

    void prepare(int bufferSize)  //准备
    {
        prepared.set(false);
        size.set(bufferSize);

        bufferToFill.setSize(2,             //channel
            bufferSize,    //num samples
            false,         //keepExistingContent
            true,          //clear extra space
            true);         //avoid reallocating
        audioBufferFifo.prepare(1, bufferSize);
        fifoIndex = 0;
        prepared.set(true);
    }
    //==============================================================================
    int getNumCompleteBuffersAvailable() const { return audioBufferFifo.getNumAvailableForReading(); } // 还剩多少数据
    bool isPrepared() const { return prepared.get(); }  // are you Prepared
    int getSize() const { return size.get(); } // get size 
    //==============================================================================
    bool getAudioBuffer(BlockType& buf) { return audioBufferFifo.pop(buf); }// get buffer

private:
    int channelToUse;
    int fifoIndex = 0;
    FIFO<BlockType> audioBufferFifo;
    BlockType bufferToFill;
    juce::Atomic<bool> prepared = false;
    juce::Atomic<int> size = 0;

    void pushNextSampleIntoFifo(float sample) //把拿到的数据再传出去
    {
        if (fifoIndex == bufferToFill.getNumSamples())
        {
            auto ok = audioBufferFifo.push(bufferToFill);

            juce::ignoreUnused(ok);

            fifoIndex = 0;
        }
        for (auto channel = 0; channel < bufferToFill.getNumChannels(); ++channel)
        {
            bufferToFill.setSample(channel, fifoIndex, sample);
        }
        ++fifoIndex;
    }
};

template<typename PathType>
class PathProducer
{
public:

    void generatePath(juce::AudioBuffer<float>& data, juce::Rectangle<float> bounds)
    {
        auto top = bounds.getY();
        auto bottom = bounds.getHeight();

        auto map = [bottom, top](float v)
            {
                return juce::jmap(v, -1.f, 1.f, bottom, top);
            };

        PathType p;

        auto y = map(data.getSample(0, 0));

        if (std::isnan(y) || std::isinf(y))
            y = bottom;

        p.startNewSubPath(0, y);

        for (int i = 1; i < data.getNumSamples(); i++)
        {
            auto y = map(data.getSample(0, i));

            if (!std::isnan(y) && !std::isinf(y))
            {
                p.lineTo(i, y);
            }
        }

        pathFifo.push(p);
    }

    int getNumPathsAvailable() const
    {
        return pathFifo.getNumAvailableForReading();
    }

    bool getPath(PathType& path)
    {
        return pathFifo.pop(path);
    }

private:
    FIFO<PathType> pathFifo;
};