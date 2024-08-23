# FMOD Unity Audio Control Scripts

Welcome to the FMOD Unity Audio Control Scripts repository! This collection of custom scripts is designed to simplify and enhance audio implementation within Unity projects using FMOD. Each script is focused yet adaptable, ensuring they can be easily integrated into various projects to save time and improve workflow efficiency.

## Scripts Overview

This repository includes scripts for:
-    **FMODAnimationEventTriggers**: Trigger FMOD events based on Unity animations.
-    **FMODButtonEventControl**: Assign FMOD events to UI button interactions (hover and click).
-    **FMODSliderParameterControl**: Dynamically adjust FMOD parameters using Unity UI sliders.
-    **FMODToggleParameterControl**: Manage binary FMOD parameters with Unity UI toggles.

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

**General Setup:**
1. Add the `FMODAnimationEventTriggers` script to the Player GameObject.
2. In the Inspector, configure the `AnimationEventTriggerPairs`:
   - Click the plus button to add a new pair.
   - Set "Event Name" to the identifying word that will be called via a function call from the desired animation timeline.
   - Select the desired FMOD event.
3. Configure the animation:
   - Open the game object's Animator or Animation window.
   - Select the desired animation and scrub through the timeline to the desired moment for the sound to be triggered.
   - Add an animation event at this frame and set "FMODAnimationEventTrigger" as the function name with whatever string you set as the "Event Name" in step 2.

### FMODButtonEventControl

**General Setup:**
1. Attach the `FMODButtonEventControl` script to a GameObject managing UI, such as the canvas.
2. In the Inspector, configure the `buttonEventPairs` array:
   - For each button, drag it into the `button` field.
   - Assign the FMOD event for hover to `FmodHoverEvent` and for click to `FmodClickEvent`.

### FMODSliderParameterControl

**General Setup:**
1. Add the `FMODSliderParameterControl` script to a GameObject managing audio parameters.
2. In the Inspector, configure the `SliderParameterPairs` array:
   - Link each slider to an FMOD parameter, ensuring the parameter names match exactly.

### FMODToggleParameterControl

**General Setup:**
1. Attach the `FMODToggleParameterControl` script to a GameObject for managing binary audio parameters.
2. In the Inspector, configure the `ToggleParameterPairs` array:
   - Link each toggle and set the corresponding FMOD parameter name.

## Conclusion

Thank you for exploring these FMOD Unity Audio Control Scripts. These tools are crafted to provide simple yet effective solutions for audio management in Unity projects. For more information or potential collaborations, visit [www.danielrdehaan.com](http://www.danielrdehaan.com/) or connect on LinkedIn.

If you find these scripts useful, please consider starring this repository and sharing it with others interested in advanced audio implementations in Unity.

## Support

For support, feature requests, or contributions, please open an issue or pull request in this repository. Your feedback is greatly appreciated!

Let's create some awesome audio experiences together!
