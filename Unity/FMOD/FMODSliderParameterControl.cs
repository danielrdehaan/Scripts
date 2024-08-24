/*
 * FMODSliderParameterControl.cs
 * 
 * Description:
 * This script is used to control FMOD parameters using Unity UI sliders. It allows you to
 * dynamically adjust FMOD parameters in real-time by linking sliders to specific FMOD parameter names.
 * 
 * Author: Daniel Dehaan
 * Website: danielrdehaan.com
 * Date: 2024-08-24
 * 
 * Usage:
 * 1. Select a GameObject in your scene where you want to manage audio parameters. This could be a dedicated audio control panel or any other GameObject.
 * 2. Add the FMODSliderParameterControl script to this GameObject.
 * 3. With the GameObject selected, go to the Inspector where the FMODSliderParameterControl script is attached.
 * 4. Set the size of the SliderParameterPairs array to match the number of sliders you intend to use for controlling FMOD parameters.
 * 5. For each element in the array:
 *    - Drag the corresponding slider from the Hierarchy into the soundSlider field.
 *    - Enter the name of the FMOD parameter that this slider will control into the fmodParameterName field.
 * 6. Ensure the FMOD parameter names exactly match those defined in FMOD.
 * 
 * Notes:
 * - This script does not currently support any value scaling, so care should be taken to match min/max values between the slider in Unity and the parameter in FMOD.
 */

using UnityEngine;                // For Unity core functionalities
using UnityEngine.UI;             // For accessing UI components like Slider
using FMODUnity;                  // For integrating FMOD functionalities with Unity

public class FMODSliderParameterControl : MonoBehaviour
{
    // A struct to hold the slider and its corresponding FMOD parameter name.
    [System.Serializable]
    public struct SliderParameterPair
    {
        public Slider soundSlider;         // The UI slider
        public string fmodParameterName;   // The corresponding FMOD parameter name
    }

    // Array of SliderParameterPair - each element represents a slider and its corresponding FMOD parameter.
    public SliderParameterPair[] sliderParameterPairs;

    void Start()
    {
        // For each SliderParameterPair in the array, initialize the FMOD parameter and set up the listener for the slider's onValueChanged event.
        foreach (var pair in sliderParameterPairs)
        {
            // Initialize the FMOD parameter with the initial value of the Slider.
            RuntimeManager.StudioSystem.setParameterByName(pair.fmodParameterName, pair.soundSlider.value);

            // Setup the action to be performed whenever the Slider value changes.
            pair.soundSlider.onValueChanged.AddListener((value) => UpdateFMODParameterValue(value, pair.fmodParameterName));
        }
    }

    // A function to update the FMOD parameter value.
    void UpdateFMODParameterValue(float sliderValue, string parameterName)
    {
        // Update the FMOD parameter with the new slider value.
        RuntimeManager.StudioSystem.setParameterByName(parameterName, sliderValue);
    }
}
