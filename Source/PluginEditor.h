#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class NevGolosVoiceAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit NevGolosVoiceAudioProcessorEditor(NevGolosVoiceAudioProcessor&);
    ~NevGolosVoiceAudioProcessorEditor() override = default;
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    NevGolosVoiceAudioProcessor& processorRef;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NevGolosVoiceAudioProcessorEditor)
};
