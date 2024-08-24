/*
 * FMODAnimationEventTriggers.cs
 * 
 * Description:
 * This script is used to trigger FMOD events based on animation event strings in Unity.
 * It provides a way to link animation events to specific FMOD events, allowing for 
 * dynamic audio feedback in response to animations.
 * 
 * Author: Daniel Dehaan
 * Website: danielrdehaan.com
 * Date: 2024-08-24
 * 
 * Usage:
 * 1. Attach this script to a GameObject in your Unity scene.
 * 2. In the Inspector, configure the 'animationEventTiggerPairs' array with the 
 *    appropriate animation event names and corresponding FMOD events.
 * 3. Call the 'FMODAnimationEventTrigger' method with the animation event string 
 *    to trigger the corresponding FMOD event.
 * 
 * Notes:
 * - Ensure your Unity project is connected to an FMOD project with all necessary events set up.
 * - The FMOD event names in the 'animationEventTiggerPairs' array must match the animation event strings exactly.
 */

using System.Linq;                // For LINQ operations like FirstOrDefault
using UnityEngine;                // For Unity core functionalities
using FMODUnity;                  // For integrating FMOD functionalities with Unity

public class FMODAnimationEventTriggers : MonoBehaviour
{
    [System.Serializable]
    public struct AnimationEventTriggers
    {
        public string eventName;           // The name of the animation event
        public EventReference fmodEvent;   // The corresponding FMOD event to trigger
    }

    // Array to hold pairs of animation event names and their corresponding FMOD events
    public AnimationEventTriggers[] animationEventTiggerPairs;

    /// <summary>
    /// Triggers the FMOD event corresponding to the provided animation event string.
    /// </summary>
    /// <param name="eventString">The name of the animation event to trigger</param>
    public void FMODAnimationEventTrigger(string eventString)
    {
        // Find the first pair where the animation event name matches the provided event string
        AnimationEventTriggers evt = animationEventTiggerPairs.FirstOrDefault(e => e.eventName == eventString);

        // If the FMOD event is valid, play it
        if (!evt.fmodEvent.IsNull)
        {
            var instance = RuntimeManager.CreateInstance(evt.fmodEvent); // Create an instance of the FMOD event
            instance.set3DAttributes(FMODUnity.RuntimeUtils.To3DAttributes(gameObject)); // Set the 3D attributes of the event to match the GameObject's position
            instance.start(); // Start playing the FMOD event
            instance.release(); // Release the instance to free up resources
        }
    }
}
