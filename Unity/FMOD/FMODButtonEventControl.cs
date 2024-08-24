/*
 * FMODButtonEventControl.cs
 * 
 * Description:
 * This script is used to trigger FMOD events based on UI button interactions in Unity.
 * It allows you to assign specific FMOD events for button hover and click actions, enhancing
 * the user interface with dynamic audio feedback.
 * 
 * Author: Daniel Dehaan
 * Website: danielrdehaan.com
 * Date: 2024-08-24
 * 
 * Usage:
 * 1. Attach the FMODButtonEventControl script to a GameObject managing UI, such as the canvas.
 * 2. In the Inspector, configure the 'buttonEventPairs' array:
 *    - Increase the size of this array to match the number of buttons you want to enhance with sound.
 *    - For each element in the array:
 *      - Drag the corresponding UI button from the Hierarchy into the 'button' field.
 *      - Use the magnifying glass icon to assign the FMOD event for hover to the 'FmodHoverEvent' field.
 *      - Similarly, assign the FMOD event for click to the 'FmodClickEvent' field.
 * 
 * Notes:
 * - Ensure your Unity project is connected to an FMOD project with all necessary events set up.
 * - The FMOD events assigned to the buttons must be properly configured in the FMOD project.
 */

using UnityEngine;                // For Unity core functionalities
using UnityEngine.UI;             // For accessing UI components like Button
using UnityEngine.EventSystems;   // For handling UI events like hover and click
using FMODUnity;                  // For integrating FMOD functionalities with Unity

public class FMODButtonEventControl : MonoBehaviour
{
    [System.Serializable]
    public struct Buttons
    {
        public Button button;                  // The UI button
        public EventReference FmodHoverEvent;  // FMOD Event when the button is hovered over
        public EventReference FmodClickEvent;  // FMOD Event when the button is clicked
    }

    // Array to hold pairs of buttons and their corresponding FMOD events
    public Buttons[] buttonEventPairs;

    private void Start()
    {
        // Iterate through each button-FMOD event pair
        foreach (var pair in buttonEventPairs)
        {
            // Add an EventTrigger component to the button's GameObject
            EventTrigger trigger = pair.button.gameObject.AddComponent<EventTrigger>();

            // Setup hover event
            EventTrigger.Entry hoverEntry = new EventTrigger.Entry
            {
                // Set the event type to PointerEnter (hover)
                eventID = EventTriggerType.PointerEnter
            };
            // Add a listener to play the FMOD hover event when the button is hovered over
            hoverEntry.callback.AddListener((data) => { RuntimeManager.PlayOneShot(pair.FmodHoverEvent, pair.button.transform.position); });
            // Add the hover event to the trigger's list of events
            trigger.triggers.Add(hoverEntry);

            // Setup click event
            EventTrigger.Entry clickEntry = new EventTrigger.Entry
            {
                // Set the event type to PointerClick (click)
                eventID = EventTriggerType.PointerClick
            };
            // Add a listener to play the FMOD click event when the button is clicked
            clickEntry.callback.AddListener((data) => { RuntimeManager.PlayOneShot(pair.FmodClickEvent, pair.button.transform.position); });
            // Add the click event to the trigger's list of events
            trigger.triggers.Add(clickEntry);
        }
    }
}
