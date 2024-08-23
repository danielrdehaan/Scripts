using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;
using FMODUnity;

public class FMODButtonEventControl : MonoBehaviour
{
    [System.Serializable]
    public struct Buttons
    {
        public Button button;
        public EventReference FmodHoverEvent; // FMOD Event when the button is hovered over
        public EventReference FmodClickEvent; // FMOD Event when the button is clicked
    }

    public Buttons[] buttonEventPairs;

    private void Start()
    {
        foreach (var pair in buttonEventPairs)
        {
            EventTrigger trigger = pair.button.gameObject.AddComponent<EventTrigger>();

            // Setup hover event
            EventTrigger.Entry hoverEntry = new EventTrigger.Entry();
            hoverEntry.eventID = EventTriggerType.PointerEnter;
            hoverEntry.callback.AddListener((data) => { RuntimeManager.PlayOneShot(pair.FmodHoverEvent, pair.button.transform.position); });
            trigger.triggers.Add(hoverEntry);

            // Setup click event
            EventTrigger.Entry clickEntry = new EventTrigger.Entry();
            clickEntry.eventID = EventTriggerType.PointerClick;
            clickEntry.callback.AddListener((data) => { RuntimeManager.PlayOneShot(pair.FmodClickEvent, pair.button.transform.position); });
            trigger.triggers.Add(clickEntry);
        }
    }
}