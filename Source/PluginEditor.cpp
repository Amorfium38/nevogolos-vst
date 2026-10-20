#include "PluginEditor.h"

NevGolosVoiceAudioProcessorEditor::NevGolosVoiceAudioProcessorEditor(
    NevGolosVoiceAudioProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    setSize(800, 600);
}

void NevGolosVoiceAudioProcessorEditor::paint(juce::Graphics& g)
{
    juce::ignoreUnused(processorRef);
    const auto bounds = getLocalBounds();
    g.fillAll(juce::Colour(0xff0a0d14));

    g.setColour(juce::Colour(0xffffd36b));
    g.drawRoundedRectangle(bounds.toFloat().reduced(2.0f), 18.0f, 2.0f);

    g.setColour(juce::Colour(0xffffd36b));
    g.setFont(juce::Font(juce::FontOptions(32.0f, juce::Font::bold)));
    g.drawText("НЕВЪГОЛОС", bounds.removeFromTop(100),
               juce::Justification::centred, true);

    g.setColour(juce::Colour(0xffe2e8f0));
    g.setFont(juce::Font(juce::FontOptions(18.0f)));
    g.drawText("Вокальный процессор", getLocalBounds().removeFromTop(160),
               juce::Justification::centred, true);

    g.setColour(juce::Colour(0xfff59e0b));
    g.setFont(juce::Font(juce::FontOptions(15.0f)));
    g.drawText("TUBE WARMTH  •  LOOKAHEAD PEAK CONTROL",
               juce::Rectangle<int>(40, 280, 720, 40),
               juce::Justification::centred, true);

    g.setColour(juce::Colour(0xff94a3b8));
    g.setFont(juce::Font(juce::FontOptions(14.0f)));
    g.drawText("Текущая версия: фиксированные настройки обработки",
               juce::Rectangle<int>(40, 330, 720, 30),
               juce::Justification::centred, true);
    g.drawText("Аудиодвижок готов к дальнейшему развитию",
               juce::Rectangle<int>(40, getHeight() - 70, 720, 30),
               juce::Justification::centred, true);
}

void NevGolosVoiceAudioProcessorEditor::resized()
{
}
