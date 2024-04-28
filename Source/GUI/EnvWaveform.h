/*
  ==============================================================================

    EnvWaveform.h
    Created: 15 Apr 2024 7:07:02pm
    Author:  cjia

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
namespace Gui
{
    class EnvWaveform : public juce::Component, public juce::Timer
    {
    public:
        enum class Axis {x,y};
        juce::Path EnvPath;
        enum MoveStyle
        {
            Horizontal_Move,
            Portrait_Move,
            Free_move
        };
        class Thumb : public juce::Component //, juce::MouseListener
        {
        public:
            Thumb(int MoveStyle = Horizontal_Move)
            {
                Move_Style = MoveStyle;
                constrainer.setMinimumOnscreenAmounts(40, 40, 40, 40);

            }
            void paint(juce::Graphics& g) override
            {
                g.setColour(juce::Colours::white);
                g.fillEllipse(getLocalBounds().getX(), getLocalBounds().getY(), diameter, diameter);
                //g.drawRoundedRectangle(getLocalBounds().getX()-5, getLocalBounds().getY()-5, 10, 10, 5, 5);
            }
            void resized() override
            {
                initial_x = getX();
                initial_y = getY();
            }
            void mouseDown(const juce::MouseEvent& event) override
            {
                dragger.startDraggingComponent(this, event);
            }
            void mouseDrag(const juce::MouseEvent& event) override
            {
                //dragger.dragComponent(this, event, &constrainer);
                auto bounds = this->getBounds();
                if (this->isOnDesktop())
                    bounds += this->getLocalPoint(nullptr, event.source.getScreenPosition()).roundToInt() ;
                else
                    bounds += event.getEventRelativeTo(this).getPosition() ;
                if (Move_Style == Horizontal_Move)
                {
                    bounds.setY(initial_y);
                }
                else if (Move_Style == Portrait_Move)
                {
                    bounds.setX(initial_x);
                }
                constrainer.setBoundsForComponent(this, bounds, false, false, false, false);     
            }
            float radius = 5;
            float diameter = radius * 2;
        private:
            juce::ComponentDragger dragger;
            juce::ComponentBoundsConstrainer constrainer;
            float initial_x ;
            float initial_y ;
            int Move_Style;
            JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Thumb)
        };

        EnvWaveform(ScscAudioProcessor& p)
        {
            startTimerHz(60);
            //addAndMakeVisible(thumb);
            addAndMakeVisible(thumbDelay);
            addAndMakeVisible(thumbAttack);
            addAndMakeVisible(thumbHold);
            addAndMakeVisible(thumbDecaySustain);
            addAndMakeVisible(thumbRelease);

            EnvChain.push_back(&thumbDelay);
            EnvChain.push_back(&thumbAttack);
            EnvChain.push_back(&thumbHold);
            EnvChain.push_back(&thumbDecaySustain);
            EnvChain.push_back(&thumbRelease);

        }
        void resized() override
        {
            //thumb.setBounds(getLocalBounds().withSizeKeepingCentre(10, 10));
            //thumb.setBounds(10,10,10,10);
            juce::Logger::outputDebugString("getHeight=" + juce::String(getHeight()));
            Height = getHeight();
            Width = getWidth();

            thumbDelay.setBounds(0, Height-10, 10, 10);
            thumbAttack.setBounds(0, 0, 10, 10);
            thumbHold.setBounds(0, 0, 10, 10);
            thumbDecaySustain.setBounds(Width * 0.4, 0, 10, 10);
            thumbRelease.setBounds(Width * 0.5, Height - 10, 10, 10);
        }
        void paint(juce::Graphics& g) override
        {
            g.setColour(juce::Colour::fromRGBA(50, 50, 50, 170));
            g.fillRoundedRectangle(getLocalBounds().toFloat(), 10.f);
            g.setColour(juce::Colours::white);
            g.strokePath(EnvPath, juce::PathStrokeType(1.f));
        }
        void timerCallback() override
        {
            EnvPath.clear();
            //juce::Logger::outputDebugString("thumbDelay.getX()="+juce::String(thumbDelay.getX()));
            EnvPath.startNewSubPath(thumbDelay.getX()+ Offset, thumbDelay.getY() + Offset);
            EnvPath.lineTo(thumbAttack.getX() + Offset, thumbAttack.getY() + Offset);
            EnvPath.lineTo(thumbHold.getX() + Offset, thumbHold.getY() + Offset);
            EnvPath.lineTo(thumbDecaySustain.getX() + Offset, thumbDecaySustain.getY() + Offset);
            EnvPath.lineTo(thumbRelease.getX() + Offset, thumbRelease.getY() + Offset);
            repaint();

        }


    private:
        Thumb thumb;
        Thumb thumbDelay, thumbAttack, thumbHold, thumbDecaySustain{ Free_move }, thumbRelease;
        std::vector <Thumb *> EnvChain;
        float Offset = thumbDelay.radius;
        std::vector < juce::Slider* > xSliders,ySliders;
        float Height = 0;
        float Width = 0;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EnvWaveform)
    };

}