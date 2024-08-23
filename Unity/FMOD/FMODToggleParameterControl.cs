using UnityEngine;
using UnityEngine.UI;
using FMODUnity;

public class FMODToggleParameterControl : MonoBehaviour
{
    [System.Serializable]
    public struct ToggleParameterPair
    {
        public Toggle audioToggle;
        public string fmodParameterName;
    }

    public ToggleParameterPair[] toggleParameterPairs;

    void Start()
    {
        foreach (var pair in toggleParameterPairs)
        {
            // Initialize the FMOD parameter with the initial value of the Toggle.
            RuntimeManager.StudioSystem.setParameterByName(pair.fmodParameterName, pair.audioToggle.isOn ? 1.0f : 0.0f);

            // Setup the action to be performed whenever the Toggle value changes.
            pair.audioToggle.onValueChanged.AddListener((value) => UpdateFMODParameterValue(value, pair.fmodParameterName));
        }
    }

    // A function to update the FMOD parameter value.
    void UpdateFMODParameterValue(bool toggleValue, string parameterName)
    {
        // Set FMOD parameter to 1 if Toggle is on, and 0 if off.
        RuntimeManager.StudioSystem.setParameterByName(parameterName, toggleValue ? 1.0f : 0.0f);
    }
}