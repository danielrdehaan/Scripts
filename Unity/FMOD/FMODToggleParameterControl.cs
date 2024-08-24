/*
 * FMODToggleParameterControl.cs
 * 
 * Description:
 * This script is used to manage binary FMOD parameters using Unity UI toggles.
 * It allows you to control FMOD parameters by linking toggles to specific FMOD parameter names.
 * 
 * Author: Daniel Dehaan
 * Website: danielrdehaan.com
 * Date: 2024-08-24
 * 
 * Usage:
 * 1. Select a GameObject in your scene where you want to manage audio parameters. This could be a dedicated audio control panel or any other GameObject.
 * 2. Add the FMODToggleParameterControl script to this GameObject.
 * 3. With the GameObject selected, go to the Inspector where the FMODToggleParameterControl script is attached.
 * 4. Set the size of the ToggleParameterPairs array to match the number of toggles you intend to use for controlling FMOD parameters.
 * 5. For each element in the array:
 *    - Drag the corresponding toggle from the Hierarchy into the 'audioToggle' field.
 *    - Enter the name of the FMOD parameter that this toggle will control into the 'fmodParameterName' field.
 * 6. Ensure the FMOD parameter names exactly match those defined in FMOD.
 * 
 * Notes:
 * - This script does not currently support any value scaling, so care should be taken to match values between the toggle in Unity and the parameter in FMOD.
 */

using UnityEngine;                // For Unity core functionalities
using UnityEngine.UI;             // For accessing UI components like Toggle
using FMODUnity;                  // For integrating FMOD functionalities with Unity

public class FMODToggleParameterControl : MonoBehaviour
{
    // A struct to hold the toggle and its corresponding FMOD parameter name.
    [System.Serializable]
    public struct ToggleParameterPair
    {
        public Toggle audioToggle;         // The UI toggle
        public string fmodParameterName;   // The corresponding FMOD parameter name
    }

    // Array of ToggleParameterPair - each element represents a toggle and its corresponding FMOD parameter.
    public ToggleParameterPair[] toggleParameterPairs;

    void Start()
    {
        // For each ToggleParameterPair in the array, initialize the FMOD parameter and set up the listener for the toggle's onValueChanged event.
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
