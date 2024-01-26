/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ScscAudioProcessorEditor::ScscAudioProcessorEditor (ScscAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
    , sliderAttachment(audioProcessor.apvts,"testslider", levelSlider)
    , xxxsliderAttachment(audioProcessor.apvts, "xxxtestslider", xxxSlider)
    , EnvAttackAttachment(audioProcessor.apvts, "EnvAttack", EnvAttack)
    , EnvDecayAttachment(audioProcessor.apvts, "EnvDecay", EnvDecay)
    , EnvSustainAttachment(audioProcessor.apvts, "EnvSustain", EnvSustain)
    , EnvReleaseAttachment(audioProcessor.apvts, "EnvRelease", EnvRelease)
    , EnvLevelAttachment(audioProcessor.apvts, "Envlevel", Envlevel)
    , AudioWave(p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    levelSlider.setRange(-72, 10);//滑块取值范围
    levelSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 100, 20);
    levelSlider.onValueChange = [this] { juce::Decibels::decibelsToGain((float)levelSlider.getValue()); };//可以当成回调函数


    xxxSlider.setRange(-72, 10);
    xxxSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 100, 20);

    Envlevel.setRange(-72, 10);
    Envlevel.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    Envlevel.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    Envlevel.setValue(0.5f);




    EnvAttack.setSliderStyle(juce::Slider::LinearVertical);
    EnvDecay.setSliderStyle(juce::Slider::LinearVertical);
    EnvSustain.setSliderStyle(juce::Slider::LinearVertical);
    EnvRelease.setSliderStyle(juce::Slider::LinearVertical);

    EnvAttack.setRange(0, 1000);
    EnvDecay.setRange(0, 1000);
    EnvSustain.setRange(0, 1);
    EnvRelease.setRange(0, 1000);


    //EnvAttack.setSliderStyle(juce::Slider::LinearBarVertical);
    //EnvAttack.setRange(0.0, 127.0, 1.0);
    //EnvAttack.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    //EnvAttack.setPopupDisplayEnabled(true, false, this);
    //EnvAttack.setTextValueSuffix(" Volume");
    //EnvAttack.setValue(1.0);

    addAndMakeVisible(Envlevel);

    addAndMakeVisible(EnvAttack);
    addAndMakeVisible(EnvDecay);
    addAndMakeVisible(EnvSustain);
    addAndMakeVisible(EnvRelease);

    addAndMakeVisible(levelSlider);
    addAndMakeVisible(xxxSlider);
    addAndMakeVisible(AudioWave);

    xxxSlider.addListener(this);

    setSize(600, 300);
}

ScscAudioProcessorEditor::~ScscAudioProcessorEditor()
{
}

//==============================================================================
void ScscAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    //g.setColour (juce::Colours::white);
    //g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
    
    //// 将整个窗口设置为白色
    //g.fillAll(juce::Colours::white);

    //// 将通用绘制色彩设置为黑色
    //g.setColour(juce::Colours::black);

}
void ScscAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    //juce::Logger::outputDebugString(juce::String(xxxSlider.getValue()));
}
void ScscAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    levelSlider.setBounds(100, 10, getWidth() - 110, 20);

    xxxSlider.setBounds(100, 50, getWidth() - 110, 20);


    EnvAttack.setBounds (400, 100, 20, 100);
    EnvDecay.setBounds  (450, 100, 20, 100);
    EnvSustain.setBounds(500, 100, 20, 100);
    EnvRelease.setBounds(550, 100, 20, 100);

    Envlevel.setBounds(300, 100, 100, 100);

    AudioWave.setBounds(50, 90, 200, 200);

}
