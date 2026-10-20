#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <algorithm>
#include <cmath>

NevGolosVoiceAudioProcessor::NevGolosVoiceAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true))
{
}

void NevGolosVoiceAudioProcessor::prepareToPlay(double sampleRate, int)
{
    currentSampleRate = sampleRate > 0.0 ? sampleRate : 44100.0;
    delayBufferL.fill(0.0f);
    delayBufferR.fill(0.0f);
    delayWriteIndex = 0;
    limiterGain = 1.0f;
}

void NevGolosVoiceAudioProcessor::releaseResources()
{
    delayBufferL.fill(0.0f);
    delayBufferR.fill(0.0f);
}

bool NevGolosVoiceAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    const auto input = layouts.getMainInputChannelSet();
    const auto output = layouts.getMainOutputChannelSet();
    if (output != juce::AudioChannelSet::mono() && output != juce::AudioChannelSet::stereo())
        return false;
    return input == output;
}

void NevGolosVoiceAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                                juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(midiMessages);
    juce::ScopedNoDenormals noDenormals;

    const int channels = buffer.getNumChannels();
    const int samples = buffer.getNumSamples();
    if (channels == 0)
        return;

    constexpr float satDrive = 1.6f;
    constexpr float satOutput = 0.92f;
    constexpr float threshold = 0.5f;
    constexpr float ceiling = 0.988553f; // approximately -0.1 dBFS
    constexpr float makeup = ceiling / threshold;
    const float releaseMs = 80.0f;
    const float releaseCoefficient = 1.0f - std::exp(
        -1.0f / (static_cast<float>(currentSampleRate) * releaseMs * 0.001f));

    auto* left = buffer.getWritePointer(0);
    auto* right = channels > 1 ? buffer.getWritePointer(1) : nullptr;

    for (int i = 0; i < samples; ++i)
    {
        const float inL = std::tanh(left[i] * satDrive) * satOutput;
        const float inR = right != nullptr ? std::tanh(right[i] * satDrive) * satOutput : inL;

        delayBufferL[static_cast<size_t>(delayWriteIndex)] = inL;
        delayBufferR[static_cast<size_t>(delayWriteIndex)] = inR;

        const float peak = std::max(std::abs(inL), std::abs(inR));
        const float targetGain = peak > threshold ? threshold / (peak + 1.0e-8f) : 1.0f;
        if (targetGain < limiterGain)
            limiterGain = targetGain;
        else
            limiterGain += (1.0f - limiterGain) * releaseCoefficient;

        const int readIndex = (delayWriteIndex - lookaheadSamples + delayLength) % delayLength;
        const float outL = delayBufferL[static_cast<size_t>(readIndex)] * limiterGain * makeup;
        const float outR = delayBufferR[static_cast<size_t>(readIndex)] * limiterGain * makeup;

        left[i] = juce::jlimit(-ceiling, ceiling, outL);
        if (right != nullptr)
            right[i] = juce::jlimit(-ceiling, ceiling, outR);

        delayWriteIndex = (delayWriteIndex + 1) % delayLength;
    }
}

juce::AudioProcessorEditor* NevGolosVoiceAudioProcessor::createEditor()
{
    return new NevGolosVoiceAudioProcessorEditor(*this);
}

void NevGolosVoiceAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    juce::XmlElement state("NevGolosVoiceState");
    state.setAttribute("version", 1);
    copyXmlToBinary(state, destData);
}

void NevGolosVoiceAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> state(getXmlFromBinary(data, sizeInBytes));
    juce::ignoreUnused(state);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NevGolosVoiceAudioProcessor();
}
