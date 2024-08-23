# FMOD Unity Audio Control Scripts

Welcome to the FMOD Unity Audio Control Scripts repository! This collection of custom scripts is designed to simplify and enhance audio implementation within Unity projects using FMOD. Each script is focused yet adaptable, ensuring they can be easily integrated into various projects to save time and improve workflow efficiency.

## Scripts Overview

This repository includes scripts for:
-  **FMODAnimationEventTriggers**: Trigger FMOD events based on Unity animations.
-  **FMODButtonEventControl**: Assign FMOD events to UI button interactions (hover and click).
-  **FMODSliderParameterControl**: Dynamically adjust FMOD parameters using Unity UI sliders.
-  **FMODToggleParameterControl**: Manage binary FMOD parameters with Unity UI toggles.

These scripts are developed to be reusable components, enabling quick and efficient integration of sophisticated audio controls in any game or interactive application.

## Getting Started

### Prerequisites

Before using these scripts, ensure your Unity project is connected to an FMOD project with all necessary events and parameters set up.

### Installation

1. Clone this repository or download the ZIP file.
2. Import the scripts into your Unity project's `Assets` folder.
3. Attach the scripts to appropriate GameObjects as described in the detailed setup instructions below.

## Usage

### FMODAnimationEventTriggers

**Setup:**
1. Add the `FMODAnimationEventTriggers` script to the Player GameObject.
2. In the Inspector, configure the `AnimationEventTriggerPairs`:
   - Click the plus button to add a new pair.
   - Set the event name and select the corresponding FMOD event.

**Testing:**
-  Play your scene and observe the triggered sounds on specific animation events.

### FMODButtonEventControl

**Setup:**
1. Attach the `FMODButtonEventControl` script to a suitable GameObject (e.g., UI manager or canvas).
2. Configure the `buttonEventPairs` array in the Inspector:
   - Link each UI button and assign FMOD events for hover and click actions.

**Testing:**
-  In Play mode, interact with the buttons to hear the assigned FMOD events.

### FMODSliderParameterControl

**Setup:**
1. Add the `FMODSliderParameterControl` script to a GameObject managing audio parameters.
2. Set up the `SliderParameterPairs` array in the Inspector:
   - Link each slider to an FMOD parameter ensuring the parameter names match exactly.

**Testing:**
-  Adjust the sliders in Play mode to dynamically control the FMOD parameters.

### FMODToggleParameterControl

**Setup:**
1. Attach the `FMODToggleParameterControl` script to a GameObject for managing binary audio parameters.
2. Configure the `ToggleParameterPairs` array:
   - Link each toggle and set the corresponding FMOD parameter name.

**Testing:**
-  Toggle the UI elements in Play mode to see the real-time audio adjustments.

## Conclusion

Thank you for exploring these FMOD Unity Audio Control Scripts. These tools are crafted to provide simple yet effective solutions for audio management in Unity projects. For more information or potential collaborations, visit [www.danielrdehaan.com](http://www.danielrdehaan.com/) or connect on LinkedIn.

If you find these scripts useful, please consider starring this repository and sharing it with others interested in advanced audio implementations in Unity.

## Support

For support, feature requests, or contributions, please open an issue or pull request in this repository. Your feedback is greatly appreciated!

Let's create some awesome audio experiences together!
