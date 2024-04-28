#pragma once

#include <JuceHeader.h>
#include "AudioFIFO.h"
namespace Gui
{
	class AudioWaveform : public juce::Component, public juce::Timer
	{
	public:

		SingleChannelSampleFifo<juce::AudioBuffer<float>>& waveform;
		juce::AudioBuffer<float> pathBuffer;
		PathProducer<juce::Path> pathProducer;
		juce::Path waveformPath;
		float length = 200;
		float height = 200;
		AudioWaveform(ScscAudioProcessor& p): waveform(p.getSingleChannelSampleFifo())
		{
			pathBuffer.setSize(1, 1024);
			startTimerHz(60);

		}

		void paint(juce::Graphics& g) override
		{
			g.fillAll(juce::Colour(50, 50, 50));
			g.setColour(juce::Colours::grey);
			g.drawRoundedRectangle(getLocalBounds().toFloat(), 3, 3);

			g.setColour(juce::Colours::white);
			g.strokePath(waveformPath, juce::PathStrokeType(1.f));
		}

		void timerCallback() override
		{
			//waveformPath.clear();
			generateWaveform();
		}

		void generateWaveform()
		{
			juce::AudioBuffer<float> tempIncomingBuffer;
			while (waveform.getNumCompleteBuffersAvailable() > 0)
			{
				waveformPath.clear();
				if (waveform.getAudioBuffer(tempIncomingBuffer))
				{
					auto size = tempIncomingBuffer.getNumSamples();
					//juce::FloatVectorOperations::copy(pathBuffer.getWritePointer(0, 0),
					//	pathBuffer.getReadPointer(0, size),
					//	pathBuffer.getNumSamples() - size);
					//juce::FloatVectorOperations::copy(pathBuffer.getWritePointer(0, pathBuffer.getNumSamples() - size),
					//	tempIncomingBuffer.getReadPointer(0, 0),
					//	size);
					//pathProducer.generatePath(pathBuffer, getLocalBounds().toFloat());
					auto xxx = tempIncomingBuffer.getSample(1, 0);
					auto yyy = 0;
					xxx = juce::jmap<float>(xxx, -1, 1, 0, 200);
					yyy = juce::jmap<float>(yyy, 0, 1024, 0, 200);
					waveformPath.startNewSubPath(0, xxx);
					for (auto a = 1; a < size; ++a)
					{
						auto xxx = tempIncomingBuffer.getSample(1, a);
						auto yyy = a;
						xxx=juce::jmap<float>(xxx, -1, 1, 0, height);
						yyy=juce::jmap<float>(yyy, 0, 1024, 0, length);
						waveformPath.lineTo(a, xxx);
					}
					repaint();
				}
			}
			//while (pathProducer.getNumPathsAvailable() > 0)
			//{
			//	pathProducer.getPath(waveformPath);
			//}
			//for (size_t i = 0; i < 200; i++)
			//{
			//	waveformPath.lineTo(i, i);
			//}
		}
	private:
		/*AudioFIFO<float> AudioWaveformFifo;*/
		//SingleChannelSampleFifo<juce::AudioBuffer<float>>& waveform;
	};

}