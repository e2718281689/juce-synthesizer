#pragma once

#include <JuceHeader.h>
#include "AudioFIFO.h"
namespace Gui
{
	class AudioWaveform : public juce::Component, public juce::Timer, private juce::ComboBox::Listener
	{
	public:

		SingleChannelSampleFifo<juce::AudioBuffer<float>>& waveform;
		juce::AudioBuffer<float> pathBuffer;
		PathProducer<juce::Path> pathProducer;
		juce::Path waveformPath;
		float Width = 0;
		float Height = 0;
		float date_max = 1;
		AudioWaveform(ScscAudioProcessor& p): waveform(p.getSingleChannelSampleFifo())
		{

			addAndMakeVisible(ChannelIndexComboBox);

			for (const auto& item : myvector) {
				ChannelIndexComboBox.addItem(item.first, item.second);
				juce::Logger::outputDebugString("xxx" + juce::String(item.second));
			}
			ChannelIndexComboBox.setSelectedId(1, juce::dontSendNotification);
			show_mod = 1;
			ChannelIndexComboBox.addListener(this); 
			

			pathBuffer.setSize(1, 1024);
			startTimerHz(60);
		}
		~AudioWaveform()
		{
			ChannelIndexComboBox.removeListener(this);
		}
		void paint(juce::Graphics& g) override
		{
			g.fillAll(juce::Colour(50, 50, 50));
			g.setColour(juce::Colours::grey);
			g.drawRoundedRectangle(getLocalBounds().toFloat(), 3, 3);

			g.setColour(juce::Colours::white);
			g.strokePath(waveformPath, juce::PathStrokeType(1.f));
		}
		void resized()
		{
			Height = getHeight();
			Width = getWidth();

			ChannelIndexComboBox.setBounds(0, Height - 20, 200, 20);


		}

		void comboBoxChanged(juce::ComboBox* comboBox) override
		{
			if (comboBox == &this->ChannelIndexComboBox)
			{
				int selectedIndex = comboBox->getSelectedId(); //1 from
				show_mod = selectedIndex;
				juce::Logger::outputDebugString("show_mod=" + juce::String(selectedIndex));
			}
		}

		void timerCallback() override
		{
			//waveformPath.clear();
			generateWaveform();
		}

		float getShowDate(int index)
		{
			if(show_mod == 1)
			{
				return tempIncomingBuffer.getSample(0, index);
			}
			if(show_mod == 2)
			{
				return tempIncomingBuffer.getSample(1, index);
			}
			if(show_mod == 3)
			{
				return (tempIncomingBuffer.getSample(1, index) + tempIncomingBuffer.getSample(0, index)) * 0.5 ;
			}
		}
		void PositionMap(Point<float> &point, size_t size)
		{
			point.y = juce::jmap<float>(point.y, -date_max, date_max, 0, Height-ChannelIndexComboBox.getHeight());
			point.x = juce::jmap<float>(point.x, 0, size, 0, Width);

		}
		void generateWaveform()
		{
			
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
					Point<float> DrawPosition;
					DrawPosition.y = getShowDate(0);
					DrawPosition.x = 0;
					PositionMap(DrawPosition,size);
					waveformPath.startNewSubPath(DrawPosition);
					// y_position = juce::jmap<float>(y_position, -date_max, date_max, 0, Height-ChannelIndexComboBox.getHeight());
					// x_position = juce::jmap<float>(x_position, 0, size, 0, Width);
					// waveformPath.startNewSubPath(x_position, y_position);
					for (auto index = 1; index < size; ++index)
					{
						DrawPosition.y  = getShowDate(index);
						DrawPosition.x = index;
						// y_position = juce::jmap<float>(y_position, -date_max, date_max, 0, Height-ChannelIndexComboBox.getHeight());
						// x_position = juce::jmap<float>(x_position, 0, size, 0, Width);
						// waveformPath.lineTo(x_position, y_position);
						PositionMap(DrawPosition,size);
						waveformPath.lineTo(DrawPosition);
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
		int show_mod;
		juce::AudioBuffer<float> tempIncomingBuffer;
		std::vector<std::pair<std::string, int>> myvector = {
															{ "L", 1 },
															{ "R", 2 },
															{ "L+R", 3 }
														};
		juce::ComboBox ChannelIndexComboBox;
	};

}