/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ScscAudioProcessor::ScscAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),apvts(*this,nullptr,"Parameters", CreateParameters())
#endif
{
    //juce::AudioProcessorValueTreeState apvts{ *this, nullptr, "Parameters", CreateParameters() };

    apvts.addParameterListener("EnvAttack", this);
    apvts.addParameterListener("EnvDecay", this);
    apvts.addParameterListener("EnvSustain", this);
    apvts.addParameterListener("EnvRelease", this);
    

    synth.clearSounds();
    for (int i = 0; i < 5; i++)
    {
        synth.addVoice(new SynthVoice());
        synth.addSound(new SynthSound());
    }

    CabSimulator();



}

ScscAudioProcessor::~ScscAudioProcessor()
{
    apvts.removeParameterListener("EnvAttack", this);
    apvts.removeParameterListener("EnvDecay", this);
    apvts.removeParameterListener("EnvSustain", this);
    apvts.removeParameterListener("EnvRelease", this);
}

//==============================================================================
const juce::String ScscAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ScscAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ScscAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ScscAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ScscAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ScscAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ScscAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ScscAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ScscAudioProcessor::getProgramName (int index)
{
    return {};
}

void ScscAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ScscAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    synth.setCurrentPlaybackSampleRate(sampleRate);
    singleChannelSampleFifo.prepare(samplesPerBlock);

    const auto channels = juce::jmax(getTotalNumInputChannels(), getTotalNumOutputChannels());



    mainProcessor->setPlayConfigDetails(getMainBusNumInputChannels(),
                                        getMainBusNumOutputChannels(),
                                        sampleRate, samplesPerBlock);

    mainProcessor->prepareToPlay(sampleRate, samplesPerBlock);

    mainProcessor->clear();
    audioInputNode = mainProcessor->addNode(std::make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::audioInputNode));
    audioOutputNode = mainProcessor->addNode(std::make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::audioOutputNode));
    midiInputNode = mainProcessor->addNode(std::make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::midiInputNode));
    midiOutputNode = mainProcessor->addNode(std::make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::midiOutputNode));

    FilterNode = mainProcessor->addNode(std::make_unique < FilterProcessor > (&apvts));

    for (int channel = 0; channel < 2; ++channel)
    {
        mainProcessor->addConnection({ { audioInputNode->nodeID,  channel },{ FilterNode->nodeID, channel } });
        mainProcessor->addConnection({ { FilterNode->nodeID,  channel },{ audioOutputNode->nodeID, channel } });
    }

    mainProcessor->addConnection({ { midiInputNode->nodeID,  juce::AudioProcessorGraph::midiChannelIndex },
                                       { midiOutputNode->nodeID, juce::AudioProcessorGraph::midiChannelIndex } });


    audioNode.push_back(&audioInputNode);
    audioNode.push_back(&FilterNode);
    audioNode.push_back(&audioOutputNode);

    EnvAttackTime = apvts.getParameterAsValue("EnvAttack").getValue();
    EnvDecayTime = apvts.getParameterAsValue("EnvDecay").getValue();
    EnvSustainTime = apvts.getParameterAsValue("EnvSustain").getValue();
    EnvReleaseTime = apvts.getParameterAsValue("EnvRelease").getValue();
}

void ScscAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    mainProcessor->releaseResources();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ScscAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void ScscAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    //auto aaa = apvts.getParameterAsValue("testslider");
    //double xxx = juce::Decibels::decibelsToGain((float)aaa.getValue());
    //auto bbb = apvts.getParameterAsValue("xxxtestslider");
    //double yyy = juce::Decibels::decibelsToGain((float)bbb.getValue());
    //juce::Logger::outputDebugString("xxx=" + juce::String(xxx));
    //juce::Logger::outputDebugString("yyy=" + juce::String(yyy));

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    //for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    //    buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.


    //for (int channel = 0; channel < totalNumInputChannels; ++channel)
    //{
    //    auto* channelData = buffer.getWritePointer (channel);
    //    for (auto sample = 0; sample < buffer.getNumSamples(); ++sample)
    //    {
    //        buffer.addSample(channel, sample, (random.nextFloat() * 0.25f - 0.125f)* xxx);

    //    }

    //    // ..do something to the data...
    //}
    //Pmidi.process(midiMessages);

    //synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());


    //for (auto sample = 0; sample < buffer.getNumSamples(); ++sample)
    //{
    //    auto xxxcurrentSample = (float)std::sin(xxxcurrentAngle);
    //    xxxcurrentAngle += xxxangleDelta;
    //    xxxangleDelta = juce::Decibels::decibelsToGain((float)bbb.getValue());
    //    auto currentSample = (float)std::sin(xxxcurrentSample+currentAngle);
    //    currentAngle += angleDelta;
    //    angleDelta = juce::Decibels::decibelsToGain((float)aaa.getValue());
    //    buffer.addSample(0, sample, currentSample);
    //    buffer.addSample(1, sample, currentSample);
    //}
   


    for (int i = 0; i < synth.getNumVoices(); i++)
    {
        if ((myVoice = dynamic_cast<SynthVoice*>(synth.getVoice(i))))
        {
            //juce::Logger::outputDebugString("xxx=" + juce::String((double)apvts.getParameterAsValue("EnvAttack").getValue()));
            myVoice->getEnvAttack(EnvAttackTime, EnvDecayTime, EnvSustainTime, EnvReleaseTime, i);

            myVoice->getOsc(apvts.getParameterAsValue("SineButton").getValue(),
                            apvts.getParameterAsValue("SawButton").getValue(),
                            apvts.getParameterAsValue("SquareButton").getValue(),
                            apvts.getParameterAsValue("NoiseButton").getValue(),
                            i);

            myVoice->getLevel(apvts.getParameterAsValue("Envlevel").getValue(), i);
        }
    }

    buffer.clear();
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

    //auto inoutBlock = juce::dsp::AudioBlock<float>(buffer).getSubsetChannelBlock(0, (size_t)totalNumInputChannels);
    //processorChain.process(juce::dsp::ProcessContextReplacing<float>(inoutBlock));

    mainProcessor->processBlock(buffer, midiMessages);

    singleChannelSampleFifo.update(buffer);
}

//==============================================================================
bool ScscAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ScscAudioProcessor::createEditor()
{
    return new ScscAudioProcessorEditor (*this);
}

//==============================================================================
void ScscAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ScscAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

juce::AudioProcessorValueTreeState::ParameterLayout ScscAudioProcessor::CreateParameters()
{

    juce::AudioProcessorValueTreeState::ParameterLayout parameterLayout;
    parameterLayout.add(std::make_unique<juce::AudioParameterFloat>(
        "testslider",
        "testslider",
        juce::NormalisableRange<float>(-72.0f, 10.0f, 0.01f),
        1.0f));

    parameterLayout.add(std::make_unique<juce::AudioParameterFloat>(
        "xxxtestslider",
        "xxxtestslider",
        juce::NormalisableRange<float>(-72.0f, 10.0f, 0.01f),
        1.0f)); 

    parameterLayout.add(std::make_unique<juce::AudioParameterFloat>(
        "EnvAttack",
        "EnvAttack",
        juce::NormalisableRange<float>(0, 1000.0f, 0.01f),
        500.0f));

    parameterLayout.add(std::make_unique<juce::AudioParameterFloat>(
        "EnvDecay",
        "EnvDecay",
        juce::NormalisableRange<float>(0, 1000.0f, 0.01f),
        500.0f));

    parameterLayout.add(std::make_unique<juce::AudioParameterFloat>(
        "EnvSustain",
        "EnvSustain",
        juce::NormalisableRange<float>(0, 1.0f, 0.01f),
        0.8f));

    parameterLayout.add(std::make_unique<juce::AudioParameterFloat>(
        "EnvRelease",
        "EnvRelease",
        juce::NormalisableRange<float>(0, 1000.0f, 0.01f),
        500.0f));

    parameterLayout.add(std::make_unique<juce::AudioParameterFloat>(
        "Envlevel",
        "Envlevel",
        juce::NormalisableRange<float>(-72.0f, 10.0f, 0.01f),
        1.0f));


    parameterLayout.add(std::make_unique<juce::AudioParameterBool>(
        "SineButton",
        "SineButton",
         false));

    parameterLayout.add(std::make_unique<juce::AudioParameterBool>(
        "SawButton",
        "SawButton",
        false));

    parameterLayout.add(std::make_unique<juce::AudioParameterBool>(
        "SquareButton",
        "SquareButton",
        false));

    parameterLayout.add(std::make_unique<juce::AudioParameterBool>(
        "NoiseButton",
        "NoiseButton",
        false));

    return parameterLayout;
}

void ScscAudioProcessor::reset()
{

}

void ScscAudioProcessor::CabSimulator()
{
    juce::File impFile{ "E:\\vst_vs\\DSPConvolutionTutorial\\DSPConvolutionTutorial\\Resources\\guitar_amp.wav" };
    if (!impFile.exists()) {
        juce::Logger::outputDebugString("TestPluginAudioProcessor cons:: your impulse file does not exist " + impFile.getFullPathName());
    }
    else {
        juce::Logger::outputDebugString("Ready to load impulse response! " + impFile.getFullPathName());
    }
}

void ScscAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{

    if (parameterID.equalsIgnoreCase("EnvAttack"))
    {
        //EnvAttackTime = apvts.getParameterAsValue("EnvAttack").getValue();
        EnvAttackTime = newValue;
    }


    if (parameterID.equalsIgnoreCase("EnvDecay"))
    {
        //EnvDecayTime = apvts.getParameterAsValue("EnvDecay").getValue();
        EnvDecayTime = newValue;
    }


    if (parameterID.equalsIgnoreCase("EnvRelease"))
    {
        //EnvReleaseTime = apvts.getParameterAsValue("EnvRelease").getValue();
        EnvReleaseTime = newValue;
    }


    if (parameterID.equalsIgnoreCase("EnvSustain"))
    {
        //EnvSustainTime = apvts.getParameterAsValue("EnvSustain").getValue();
        EnvSustainTime = newValue;
    }



    //juce::Logger::outputDebugString("EnvAttackTime=" + juce::String(EnvAttackTime));
    //juce::Logger::outputDebugString("EnvDecayTime=" + juce::String(EnvDecayTime));
    //juce::Logger::outputDebugString("EnvReleaseTime=" + juce::String(EnvReleaseTime));
    //juce::Logger::outputDebugString("EnvSustainTime=" + juce::String(EnvSustainTime));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ScscAudioProcessor();
}

