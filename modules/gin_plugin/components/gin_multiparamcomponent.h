#pragma once

//==============================================================================
/** A base component for controlling multiple parameters at once
*/
class MultiParamComponent : public juce::Component,
                            public Parameter::ParameterListener,
                            private juce::AsyncUpdater
{
public:
    MultiParamComponent ()
    {
        triggerAsyncUpdate ();
    }

    ~MultiParamComponent () override
    {
        unwatchParams();
    }

protected:
    void showBubble (const juce::String& text, const juce::Rectangle<int>& rc)
    {
        if (auto p = findParentComponentOfClass<juce::AudioProcessorEditor>())
        {
            if (bubble == nullptr)
            {
                bubble = std::make_unique<juce::BubbleMessageComponent> (50000);
                p->addAndMakeVisible (*bubble);
            }

            juce::AttributedString attString;
            attString.append (text, juce::Font (13.0f));
            attString.setColour (juce::Colours::white);

            auto rcg = p->getLocalArea (this, rc);

            bubble->showAt (rcg, attString, 50000, false, false);
        }
    }

    void hideBubble()
    {
        bubble = nullptr;
    }

    void watchParam (Parameter* p)
    {
        p->addListener (this);
        params.add (p);
    }

    void unwatchParams()
    {
        for (auto p : params)
            p->removeListener (this);
    }

    virtual void paramChanged ()                { repaint ();               }

    void enablementChanged() override           { repaint();                }
    void valueUpdated (Parameter*) override     { triggerAsyncUpdate ();    }
    void handleAsyncUpdate () override          { paramChanged ();          }

    juce::Array<Parameter*> params;

    std::unique_ptr<juce::BubbleMessageComponent> bubble;
};
