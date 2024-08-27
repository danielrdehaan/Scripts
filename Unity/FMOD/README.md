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

These scripts were developed using Unity Editor version `2022.3.10f1` for FMOD `2.02.17`

### Installation

1. Clone this repository or download the ZIP file.
2. Import the scripts into your Unity project's `Assets` folder.
3. Attach the scripts to appropriate GameObjects as described in the detailed setup instructions below.

## Usage

### FMODAnimationEventTriggers

**Setup:**
1. Add the Script to the desired GameObject**:
2. Configure the Script in the Inspector:
    - In the Inspector, you'll see the `AnimationEventTriggerPairs` array. Click on the plus button to add a new element to this list.
    - For each element:
        - Enter an indentifying string as the event name (e.g. "Step").
        - Use the magnifying glass icon to locate and select the desired FMOD event.
3. Set Up Animation Events:
    - Navigate to the gameObject's Animator or Animation window (depending on your setup).
    - Select the desired animation and scrub through the animation timeline to find the moment you want the FMOD sound to be triggered.
    - Under Events in the Animation window, click the plus button to add a new event at the specific frame.
    - Enter "FMODAnimationEventTrigger" as the function name and the identifying string (e.g. "Step") as the parameter. 
    - Link the animation event to the GameObject that contains the `FMODAnimationEventTriggers` script component by dragging it from the Project panel.
4. Add any additional events to the animation timeline following these same steps.

### FMODButtonEventControl

**Setup:**
1. Attach the `FMODButtonEventControl` script to a GameObject managing UI, such as the canvas.
2. Increase the size of this array to match the number of buttons you want to enhance with sound.
3. For each element in the array:
    - Drag the corresponding UI button from the Hierarchy into the `button` field.
    - Use the magnifying glass icon to assign the FMOD event for hover to the `FmodHoverEvent` field.
    - Similarly, assign the FMOD event for click to the `FmodClickEvent` field.

### FMODSliderParameterControl

**Setup:**
1. Add the Script to a GameObject:
    - Select a GameObject in your scene where you want to manage audio parameters. This could be a dedicated audio control panel or any other GameObject. Add the `FMODSliderParameterControl` script to this GameObject.
2. Configure the Script in the Inspector:
    - With the GameObject selected, go to the Inspector where the `FMODSliderParameterControl` script is attached. You will see the `SliderParameterPairs` array. Set the size of this array to match the number of sliders you intend to use for controlling FMOD parameters.
    - For each element in the array:
        - Drag the corresponding slider from the Hierarchy into the `soundSlider` field.
        - Enter the name of the FMOD parameter that this slider will control into the `fmodParameterName` field. **It's crucial that the parameter name exactly matches the one defined in FMOD**.
        - In addition, since this script does not currently support any value scaling, care should be taken to match min/max values between the slider in Unity and the parameter in FMOD

### FMODToggleParameterControl

**Setup:**
1. Add the Script to a GameObject:
    - Select a GameObject in your scene where you want to manage audio parameters. This could be a dedicated audio control panel or any other GameObject. Add the `FMODSliderParameterControl` script to this GameObject.
2. Configure the Script in the Inspector:
    - With the GameObject selected, go to the Inspector where the `FMODSliderParameterControl` script is attached. You will see the `SliderParameterPairs` array. Set the size of this array to match the number of sliders you intend to use for controlling FMOD parameters.
    - For each element in the array:
        - Drag the corresponding slider from the Hierarchy into the `soundSlider` field.
        - Enter the name of the FMOD parameter that this slider will control into the `fmodParameterName` field. It's crucial that the parameter name exactly matches the one defined in FMOD.
        - In additions, since this script does not currently support any value scaling, care should be taken to match these values between the slider in Unity and the parameter in FMOD

## Conclusion

Thank you for exploring these FMOD Unity Audio Control Scripts. These tools are crafted to provide simple yet effective solutions for audio management in Unity projects. For more information or potential collaborations, visit [www.danielrdehaan.com](http://www.danielrdehaan.com/) or connect on LinkedIn.

If you find these scripts useful, please consider starring this repository and sharing it with others interested in advanced audio implementations in Unity.

## Support

For support, feature requests, or contributions, please open an issue or pull request in this repository. Your feedback is greatly appreciated!

Let's create some awesome audio experiences together!
