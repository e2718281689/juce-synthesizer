/*
  ==============================================================================

    ProcessorGroup.h
    Created: 5 Jun 2024 7:47:10pm
    Author:  cjia

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

using Node = juce::AudioProcessorGraph::Node;

class ProcessorNode
{

public:

    juce::String ProcessorName;
    Node::Ptr ProcessorNodePtr;
    bool ProcessorStete;

    ProcessorNode()
    {
        ProcessorStete = false;
        //ProcessorName="ProcessorName";
        //ProcessorNodePtr = NULL;
    }

    ~ProcessorNode()
    {

    }
    void setName(juce::String name)
    {
        ProcessorName = name;
    }
private:

};

template <typename... Layers>
class ProcessorGroup : public juce::AudioProcessorGraph
{

public:
    ProcessorGroup(juce::AudioProcessorValueTreeState *apvts)
    {
        Apvts = apvts;
    }

    ~ProcessorGroup()
    {

    }
    void AudioGroupInit()
    {
        // std::apply([this](auto&&... args) 
        // {
        //     // 使用折叠表达式逐个元素调用 forward_element 函数
        //     (processLayer(args),...);
        // }, layers);


        clear(); 

        audioInputNode = addNode(std::make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::audioInputNode));
        audioOutputNode = addNode(std::make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::audioOutputNode));
        midiInputNode = addNode(std::make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::midiInputNode));
        midiOutputNode = addNode(std::make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::midiOutputNode));

        InputNode->ProcessorName=juce::String("InputNode");
        InputNode->ProcessorNodePtr = audioInputNode;
        InputNode->ProcessorStete = true;

        OutputNode->setName(juce::String("OutputNode"));
        OutputNode->ProcessorNodePtr = audioOutputNode;
        OutputNode->ProcessorStete = true;

        Node.push_back(InputNode);
        Node.push_back(OutputNode);

        updateConnectGroup();

    }
    Node::Ptr addProcessorNode(std::unique_ptr<ProcessorBase> newProcessor, NodeID nodeId = {}, UpdateKind = UpdateKind::sync)
    {
        Node::Ptr ProcessorNodePtr;
        std::shared_ptr<ProcessorNode> xxx= std::make_shared<ProcessorNode>();//����ָ��

        newProcessor->init(Apvts);
        xxx->setName(newProcessor->getName());
        ProcessorNodePtr = addNode(std::move(newProcessor));
        xxx->ProcessorNodePtr = ProcessorNodePtr;
        xxx->ProcessorStete = true;        
        Node.insert(Node.end() - 1, xxx);

        updateConnectGroup();
        return ProcessorNodePtr;
    }
    void updateConnectGroup()
    {
        std::shared_ptr<ProcessorNode>  previousNode = nullptr;

        for (auto connection : getConnections()) 
                removeConnection(connection);

        for (auto currentNode : Node) {
            if (previousNode!= nullptr) {
                /*
                A "channel" is a mono channel, that is:
                Stereo = 2 audio channels (usually channel 0 and channel 1)
                5.1 channel = 6 audio channels (usually 0 ~ 5)
                */
                for (int channel = 0; channel < 2; ++channel)
                {
                    juce::Logger::outputDebugString("previousNode=" + juce::String(previousNode->ProcessorName));
                    juce::Logger::outputDebugString("currentNode=" + juce::String(currentNode->ProcessorName));
                    auto previousNodeID = previousNode->ProcessorNodePtr->nodeID;
                    auto currentNodeID = currentNode->ProcessorNodePtr->nodeID;
                    addConnection({ {previousNodeID,  channel },
                                    { currentNodeID, channel } });
                }
            }
            previousNode = currentNode;
        }

        addConnection({ { midiInputNode->nodeID,  juce::AudioProcessorGraph::midiChannelIndex },
                           { midiOutputNode->nodeID, juce::AudioProcessorGraph::midiChannelIndex } });

        //juce::Logger::outputDebugString("audioisAnInputTo=" + isAnInputTo(audioInputNode->nodeID, audioOutputNode->nodeID)?"ture":"false");
        //juce::Logger::outputDebugString("midiisAnInputTo=" + isAnInputTo(midiInputNode->nodeID, midiOutputNode->nodeID) ? "ture" : "false");
    }
    

private:
    std::tuple<Layers...> layers;
    static constexpr size_t n_layers = sizeof...(Layers);

    juce::AudioProcessorValueTreeState *Apvts;

    Node::Ptr audioInputNode;
    Node::Ptr audioOutputNode;
    Node::Ptr midiInputNode;
    Node::Ptr midiOutputNode;
    std::shared_ptr<ProcessorNode> InputNode= std::make_shared<ProcessorNode>();
    std::shared_ptr<ProcessorNode> OutputNode = std::make_shared<ProcessorNode>();
    vector<std::shared_ptr<ProcessorNode>> Node;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProcessorGroup)
};
