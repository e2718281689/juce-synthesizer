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

class ProcessorGroup : public juce::AudioProcessorGraph
{


public:
    ProcessorGroup()
    {


    }

    ~ProcessorGroup()
    {

    }
    void AudioGroupInit()
    {
        clear(); //清除所有的连接和节点

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
    Node::Ptr addProcessorNode(std::unique_ptr<AudioProcessor> newProcessor, NodeID nodeId = {}, UpdateKind = UpdateKind::sync)
    {
        Node::Ptr ProcessorNodePtr;
        std::shared_ptr<ProcessorNode> xxx= std::make_shared<ProcessorNode>();//智能指针

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

        for (auto connection : getConnections()) //清除所有连接
                removeConnection(connection);

        //连接节点，按照Node中的数据
        for (auto currentNode : Node) {
            if (previousNode!= nullptr) {
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
    Node::Ptr audioInputNode;
    Node::Ptr audioOutputNode;
    Node::Ptr midiInputNode;
    Node::Ptr midiOutputNode;
    std::shared_ptr<ProcessorNode> InputNode= std::make_shared<ProcessorNode>();
    std::shared_ptr<ProcessorNode> OutputNode = std::make_shared<ProcessorNode>();
    vector<std::shared_ptr<ProcessorNode>> Node;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProcessorGroup)
};