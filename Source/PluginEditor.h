/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "GUI/DecibelSlider.h"
#include "GUI/AudioWaveform.h"
#include "GUI/xxxsider.h"
#include "GUI/EnvWaveform.h"
//==============================================================================
/**
*/
class ScscAudioProcessorEditor  : public juce::AudioProcessorEditor
                                  ,public juce::Slider::Listener
                                  //,public juce::Timer
{
public:
    ScscAudioProcessorEditor (ScscAudioProcessor&);
    ~ScscAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged(juce::Slider* slider) override;
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ScscAudioProcessor& audioProcessor;

    DecibelSlider levelSlider;
    juce::Slider  xxxSlider;

    juce::Slider Envlevel;

    juce::Slider EnvAttack;
    juce::Slider EnvDecay;
    juce::Slider EnvSustain;
    juce::Slider EnvRelease;

    juce::ToggleButton SineButton;
    juce::ToggleButton SawButton;
    juce::ToggleButton SquareButton;
    juce::ToggleButton NoiseButton;

    Gui::AudioWaveform AudioWave;
    Gui::EnvWaveform EnvWave;

    juce::AudioProcessorValueTreeState::SliderAttachment sliderAttachment;
    juce::AudioProcessorValueTreeState::SliderAttachment xxxsliderAttachment;
    juce::AudioProcessorValueTreeState::SliderAttachment EnvAttackAttachment;
    juce::AudioProcessorValueTreeState::SliderAttachment EnvDecayAttachment;
    juce::AudioProcessorValueTreeState::SliderAttachment EnvSustainAttachment;
    juce::AudioProcessorValueTreeState::SliderAttachment EnvReleaseAttachment;
    juce::AudioProcessorValueTreeState::SliderAttachment EnvLevelAttachment;

    juce::AudioProcessorValueTreeState::ButtonAttachment SineButtonAttachment;
    juce::AudioProcessorValueTreeState::ButtonAttachment SawButtonAttachment;
    juce::AudioProcessorValueTreeState::ButtonAttachment SquareButtonAttachment;
    juce::AudioProcessorValueTreeState::ButtonAttachment NoiseButtonAttachment;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ScscAudioProcessorEditor)
};
