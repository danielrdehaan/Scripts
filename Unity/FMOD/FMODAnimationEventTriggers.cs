using System.Linq;
using UnityEngine;
using FMODUnity;

public class FMODAnimationEventTriggers : MonoBehaviour
{
    [System.Serializable]
    public struct AnimationEventTriggers
    {
        public string eventName;
        public EventReference fmodEvent;
    }

    public AnimationEventTriggers[] animationEventTiggerPairs;

    public void FMODAnimationEventTrigger(string eventString)
    {
        AnimationEventTriggers evt = animationEventTiggerPairs.FirstOrDefault(e => e.eventName == eventString);

        // If the FMOD event is not null, play the FMOD event
        if (evt.fmodEvent.IsNull == false) // FMOD event is valid, play it
        {
            var instance = RuntimeManager.CreateInstance(evt.fmodEvent);
            instance.set3DAttributes(FMODUnity.RuntimeUtils.To3DAttributes(gameObject));
            instance.start();
            instance.release();
        }
    }

}