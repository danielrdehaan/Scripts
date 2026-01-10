/*
 * FMODFieldInfo.cs
 *
 * Description:
 * Data model for the FMOD Scene Manager tool. Represents discovered FMOD-related
 * fields and components found during scene scanning.
 *
 * Author: Daniel Dehaan
 * Website: danielrdehaan.com
 */

using System.Reflection;
using UnityEngine;
using UnityEngine.SceneManagement;

namespace FMODSceneManager
{
    /// <summary>
    /// Categorizes the type of FMOD reference found in a component.
    /// </summary>
    public enum FMODFieldType
    {
        // Struct types from FMODUnity
        EventReference,
        BankReference,
        BusReference,

        // Parameter references
        ParameterName,

        // String-based paths (legacy or direct)
        EventPath,
        BusPath,
        SnapshotPath,
        VCAPath,
        BankPath,

        // Built-in FMOD components
        StudioEmitter,
        StudioBankLoader,
        StudioListener
    }

    /// <summary>
    /// Represents a single FMOD-related field or component discovered in a scene.
    /// </summary>
    public class FMODFieldInfo
    {
        /// <summary>
        /// The scene containing this FMOD reference.
        /// </summary>
        public Scene Scene { get; set; }

        /// <summary>
        /// The GameObject containing the component with the FMOD reference.
        /// </summary>
        public GameObject GameObject { get; set; }

        /// <summary>
        /// The component containing the FMOD field.
        /// </summary>
        public Component Component { get; set; }

        /// <summary>
        /// The reflected field info. Null for built-in FMOD components (StudioEventEmitter, etc.).
        /// </summary>
        public FieldInfo Field { get; set; }

        /// <summary>
        /// The name of the field or property.
        /// </summary>
        public string FieldName { get; set; }

        /// <summary>
        /// The type of FMOD reference.
        /// </summary>
        public FMODFieldType FieldType { get; set; }

        /// <summary>
        /// The current value of the field.
        /// </summary>
        public object Value { get; set; }

        /// <summary>
        /// The FMOD path for display (event:/..., bus:/..., etc.).
        /// </summary>
        public string DisplayPath { get; set; }

        /// <summary>
        /// Whether the reference is valid (event exists, bank loaded, etc.).
        /// </summary>
        public bool IsValid { get; set; } = true;

        /// <summary>
        /// Validation error message if IsValid is false.
        /// </summary>
        public string ValidationMessage { get; set; }

        /// <summary>
        /// Gets the full hierarchy path of the GameObject.
        /// </summary>
        public string GameObjectPath
        {
            get
            {
                if (GameObject == null) return string.Empty;

                var path = GameObject.name;
                var parent = GameObject.transform.parent;

                while (parent != null)
                {
                    path = parent.name + "/" + path;
                    parent = parent.parent;
                }

                return path;
            }
        }

        /// <summary>
        /// Gets a display-friendly type name.
        /// </summary>
        public string TypeDisplayName
        {
            get
            {
                switch (FieldType)
                {
                    case FMODFieldType.EventReference:
                    case FMODFieldType.EventPath:
                    case FMODFieldType.StudioEmitter:
                        return "Event";
                    case FMODFieldType.BankReference:
                    case FMODFieldType.BankPath:
                    case FMODFieldType.StudioBankLoader:
                        return "Bank";
                    case FMODFieldType.BusReference:
                    case FMODFieldType.BusPath:
                        return "Bus";
                    case FMODFieldType.SnapshotPath:
                        return "Snapshot";
                    case FMODFieldType.VCAPath:
                        return "VCA";
                    case FMODFieldType.ParameterName:
                        return "Parameter";
                    case FMODFieldType.StudioListener:
                        return "Listener";
                    default:
                        return FieldType.ToString();
                }
            }
        }

        /// <summary>
        /// Whether this entry supports event preview (only Event types).
        /// </summary>
        public bool CanPreview
        {
            get
            {
                return FieldType == FMODFieldType.EventReference ||
                       FieldType == FMODFieldType.EventPath ||
                       FieldType == FMODFieldType.StudioEmitter;
            }
        }
    }
}
