# JUCE Delay Plugin

A delay effect plugin built with the **JUCE framework**, featuring adjustable delay time, feedback, and wet/dry mix controls.

## Features

- Adjustable delay time
- Feedback control
- Wet/dry mix
- Mono/stereo processing
- Builds as VST3, AU, and Standalone

## Technical Implementation

- Circular buffer-based delay line
- Sample-by-sample processing
- Centralised parameter management
- Real-time safe audio processing
- Standard JUCE Processor/Editor architecture

## Project Structure

```
Source/
├── PluginProcessor.h / .cpp   # Audio processing and DSP logic
├── PluginEditor.h / .cpp      # GUI and parameter controls
├── Parameters.h / .cpp        # Centralised parameter definitions
```

## Building

### Requirements

- JUCE (Projucer or CMake)
- Xcode (macOS) or Visual Studio (Windows)
- DAW or plugin host for testing

### Steps

1. Open in Projucer or generate via CMake
2. Build desired target (VST3 / AU / Standalone)
3. Scan in your DAW or AudioPluginHost

> Some commercial plugins may not load under a debugger. Use a Release build or run the host outside your IDE for scanning.

## Roadmap

- Tempo-synced delay
- Parameter smoothing
- Filtered feedback path
- Ping-pong stereo mode
- Preset management

## References

Built following "The Complete Beginner's Guide to Audio Plug-in Development" 
by Matthijs Hollemans / The Audio Programmer. 
Source code used under MIT License.

## Author

**Alex Hollingsworth** — Audio developer & music producer
