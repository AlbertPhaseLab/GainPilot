#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "juce_audio_processors_headless/juce_audio_processors_headless.h"

//==============================================================================
AudioPluginAudioProcessor::AudioPluginAudioProcessor()
    : AudioProcessor(BusesProperties()
        #if ! JucePlugin_IsMidiEffect
         #if ! JucePlugin_IsSynth
          .withInput("Input", juce::AudioChannelSet::stereo(), true)
         #endif
          .withOutput("Output", juce::AudioChannelSet::stereo(), true)
        #endif
      ),
      apvts(*this, nullptr, "PARAMS",
      {
          std::make_unique<juce::AudioParameterFloat>("gain", "Gain",
              juce::NormalisableRange<float>(0.0f, 24.0f, 0.01f), 0.0f),
          std::make_unique<juce::AudioParameterFloat>("tone", "Tone",
              juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f), 0.5f),
            std::make_unique<juce::AudioParameterFloat>("wetDry", "Wet/Dry",
              juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f), 1.0f)
      })
{
    params.init(apvts);
}
AudioPluginAudioProcessor::~AudioPluginAudioProcessor()
{
}

//==============================================================================
const juce::String AudioPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AudioPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AudioPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AudioPluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AudioPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AudioPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AudioPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AudioPluginAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String AudioPluginAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void AudioPluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void AudioPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    // juce::ignoreUnused (sampleRate, samplesPerBlock);

    toneFilterL.prepare(sampleRate);
    toneFilterR.prepare(sampleRate);
}

void AudioPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool AudioPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void AudioPluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(midiMessages);
    juce::ScopedNoDenormals noDenormals;

    float gainLinear = juce::Decibels::decibelsToGain(params.gain->load()); // dB to linear
    float toneNorm  = params.tone->load(); // 0.0 = dark, 1.0 = bright
    float wetDryNorm = params.wetDry->load(); // 0.0 = dry, 1.0 = wet

    // Map tone 0–1 → 200–8000 Hz (log)
    float cutoff = juce::jmap(toneNorm, 0.0f, 1.0f, 200.0f, 8000.0f); // 

    toneFilterL.setCutoff(cutoff); // left channel
    toneFilterR.setCutoff(cutoff); // right channel

    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Clear any output channels that didn't contain input data
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel); // get pointer to channel data
        auto& filter = (channel == 0) ? toneFilterL : toneFilterR; // select filter

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            // --- DRY path ---
            const float dry = channelData[sample]; // input sample

            // --- WET / DSP path ---
            float wet = dry * gainLinear; // apply gain
            wet = filter.processSample(wet); // apply tone filter

            // --- MIX ---
            channelData[sample] = dry * (1.0f - wetDryNorm) + wet * wetDryNorm; // linear crossfade
        }
    }

    // input ──► DRY ───────────────┐
    //                              ├─► MIX ─► output
    // input ──► DSP (tone, gain) ──┘

}

//==============================================================================
bool AudioPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AudioPluginAudioProcessor::createEditor()
{
    return new AudioPluginAudioProcessorEditor (*this);
}

//==============================================================================
void AudioPluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::MemoryOutputStream stream(destData, true);
    apvts.state.writeToStream(stream);
}

void AudioPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    auto tree = juce::ValueTree::readFromData(data, sizeInBytes);

    if (tree.isValid())
        apvts.state = tree;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioPluginAudioProcessor();
}
