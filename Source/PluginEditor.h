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

    using SliderVts = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonVts = juce::AudioProcessorValueTreeState::ButtonAttachment;

    ScscAudioProcessor& audioProcessor;

    DecibelSlider levelSlider;
    juce::Slider  xxxSlider;
    juce::ToggleButton levelSliderButton;

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

    SliderVts sliderAttachment{ audioProcessor.apvts, "testslider", levelSlider };
    SliderVts xxxsliderAttachment{ audioProcessor.apvts, "xxxtestslider", xxxSlider };
    SliderVts EnvAttackAttachment{ audioProcessor.apvts, "EnvAttack", EnvAttack };
    SliderVts EnvDecayAttachment{ audioProcessor.apvts, "EnvDecay", EnvDecay };
    SliderVts EnvSustainAttachment{ audioProcessor.apvts, "EnvSustain", EnvSustain };
    SliderVts EnvReleaseAttachment{ audioProcessor.apvts, "EnvRelease", EnvRelease };
    SliderVts EnvLevelAttachment{ audioProcessor.apvts, "Envlevel", Envlevel };


    ButtonVts SineButtonAttachment{ audioProcessor.apvts, "SineButton", SineButton };
    ButtonVts SawButtonAttachment{ audioProcessor.apvts, "SawButton", SawButton };
    ButtonVts SquareButtonAttachment{ audioProcessor.apvts, "SquareButton", SquareButton };
    ButtonVts NoiseButtonAttachment{ audioProcessor.apvts, "NoiseButton", NoiseButton };
    ButtonVts levelSliderButtonAttachment{ audioProcessor.apvts, "levelSliderButton", levelSliderButton };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ScscAudioProcessorEditor)
};
