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
    using ComboBoxVts = juce::AudioProcessorValueTreeState::ComboBoxAttachment;
    using ButtonVts = juce::AudioProcessorValueTreeState::ButtonAttachment;

    ScscAudioProcessor& audioProcessor;

    DecibelSlider levelSlider;

    juce::Label   GainsliderLabel;
    juce::Slider  GainSlider;

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

    juce::ComboBox ModIndexComboBox;
    juce::ComboBox cORcPPComboBox;

    Gui::AudioWaveform AudioWave;
    Gui::EnvWaveform EnvWave;

    SliderVts sliderAttachment{ audioProcessor.apvts, "levelSlider", levelSlider };
    SliderVts xxxsliderAttachment{ audioProcessor.apvts, "GainSlider", GainSlider };
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

    ComboBoxVts ModIndexComboBoxAttachment{ audioProcessor.apvts, "ModIndexComboBox", ModIndexComboBox };
    ComboBoxVts cORcPPComboBoxAttachment{ audioProcessor.apvts, "cORcPPComboBox", cORcPPComboBox };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ScscAudioProcessorEditor)
};
