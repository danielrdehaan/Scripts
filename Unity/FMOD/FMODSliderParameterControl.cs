using UnityEngine;
using UnityEngine.UI;
using FMODUnity;

public class FMODSliderParameterControl : MonoBehaviour
{
    // A struct to hold the slider and its corresponding FMOD parameter name.
    [System.Serializable]
    public struct SliderParameterPair
    {
        public Slider soundSlider;
        public string fmodParameterName;
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
        RuntimeManager.StudioSystem.setParameterByName(parameterName, sliderValue);
    }
}