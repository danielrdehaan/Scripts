/*
 * FMODSceneScanner.cs
 *
 * Description:
 * Scans all loaded Unity scenes for FMOD-related fields and components using reflection.
 * Detects EventReference, BankReference, legacy attributes, string paths, and built-in
 * FMOD components.
 *
 * Author: Daniel Dehaan
 * Website: danielrdehaan.com
 */

using System;
using System.Collections.Generic;
using System.Reflection;
using UnityEngine;
using UnityEngine.SceneManagement;

#if UNITY_EDITOR
using UnityEditor;
using UnityEditor.SceneManagement;
#endif

namespace FMODSceneManager
{
    /// <summary>
    /// Scans loaded scenes for FMOD-related fields and components.
    /// </summary>
    public static class FMODSceneScanner
    {
        // FMOD type names to detect
        private static readonly string[] FMODStructTypes = {
            "EventReference",
            "BankReference",
            "BusReference"
        };

        // FMOD attribute names to detect on string fields
        private static readonly string[] FMODAttributeNames = {
            "EventRefAttribute",
            "BankRefAttribute",
            "ParamRefAttribute"
        };

        // FMOD built-in component type names
        private static readonly string[] FMODComponentTypes = {
            "StudioEventEmitter",
            "StudioBankLoader",
            "StudioListener"
        };

        // String path prefixes for FMOD paths
        private static readonly Dictionary<string, FMODFieldType> PathPrefixes = new Dictionary<string, FMODFieldType>
        {
            { "event:/", FMODFieldType.EventPath },
            { "bus:/", FMODFieldType.BusPath },
            { "snapshot:/", FMODFieldType.SnapshotPath },
            { "vca:/", FMODFieldType.VCAPath },
            { "bank:/", FMODFieldType.BankPath }
        };

        /// <summary>
        /// Scans all loaded scenes for FMOD-related fields and components.
        /// </summary>
        /// <returns>List of discovered FMOD field information.</returns>
        public static List<FMODFieldInfo> ScanAllScenes()
        {
            var results = new List<FMODFieldInfo>();

#if UNITY_EDITOR
            int sceneCount = EditorSceneManager.sceneCount;

            for (int i = 0; i < sceneCount; i++)
            {
                Scene scene = EditorSceneManager.GetSceneAt(i);

                if (!scene.isLoaded)
                    continue;

                ScanScene(scene, results);
            }
#endif

            return results;
        }

        /// <summary>
        /// Scans a single scene for FMOD-related fields and components.
        /// </summary>
        private static void ScanScene(Scene scene, List<FMODFieldInfo> results)
        {
            var rootObjects = scene.GetRootGameObjects();

            foreach (var root in rootObjects)
            {
                ScanGameObjectRecursive(root, scene, results);
            }
        }

        /// <summary>
        /// Recursively scans a GameObject and its children for FMOD references.
        /// </summary>
        private static void ScanGameObjectRecursive(GameObject gameObject, Scene scene, List<FMODFieldInfo> results)
        {
            // Get all components on this GameObject
            var components = gameObject.GetComponents<Component>();

            foreach (var component in components)
            {
                if (component == null)
                    continue;

                ScanComponent(component, scene, results);
            }

            // Recurse into children
            foreach (Transform child in gameObject.transform)
            {
                ScanGameObjectRecursive(child.gameObject, scene, results);
            }
        }

        /// <summary>
        /// Scans a component for FMOD-related fields.
        /// </summary>
        private static void ScanComponent(Component component, Scene scene, List<FMODFieldInfo> results)
        {
            var componentType = component.GetType();
            var componentTypeName = componentType.Name;

            // Check if this is a built-in FMOD component
            if (IsBuiltInFMODComponent(componentTypeName, out FMODFieldType builtInType))
            {
                var info = CreateBuiltInComponentInfo(component, scene, builtInType);
                if (info != null)
                {
                    results.Add(info);
                }

                // Still scan fields of built-in components for additional references
            }

            // Scan all fields using reflection
            ScanComponentFields(component, scene, results);
        }

        /// <summary>
        /// Checks if a component type name is a built-in FMOD component.
        /// </summary>
        private static bool IsBuiltInFMODComponent(string typeName, out FMODFieldType fieldType)
        {
            switch (typeName)
            {
                case "StudioEventEmitter":
                    fieldType = FMODFieldType.StudioEmitter;
                    return true;
                case "StudioBankLoader":
                    fieldType = FMODFieldType.StudioBankLoader;
                    return true;
                case "StudioListener":
                    fieldType = FMODFieldType.StudioListener;
                    return true;
                default:
                    fieldType = FMODFieldType.EventReference;
                    return false;
            }
        }

        /// <summary>
        /// Creates an FMODFieldInfo for a built-in FMOD component.
        /// </summary>
        private static FMODFieldInfo CreateBuiltInComponentInfo(Component component, Scene scene, FMODFieldType fieldType)
        {
            string displayPath = string.Empty;
            var componentType = component.GetType();

            // Try to get the event path from StudioEventEmitter
            if (fieldType == FMODFieldType.StudioEmitter)
            {
                var eventRefField = componentType.GetField("EventReference") ??
                                    componentType.GetField("eventReference");

                if (eventRefField != null)
                {
                    var eventRef = eventRefField.GetValue(component);
                    displayPath = GetPathFromEventReference(eventRef);
                }

                // Fallback to legacy Event field
                if (string.IsNullOrEmpty(displayPath))
                {
                    var eventField = componentType.GetField("Event") ??
                                     componentType.GetField("event");
                    if (eventField != null)
                    {
                        displayPath = eventField.GetValue(component) as string ?? string.Empty;
                    }
                }
            }
            // Try to get bank paths from StudioBankLoader
            else if (fieldType == FMODFieldType.StudioBankLoader)
            {
                var banksField = componentType.GetField("Banks") ??
                                 componentType.GetField("banks");
                if (banksField != null)
                {
                    var banks = banksField.GetValue(component);
                    if (banks is IEnumerable<string> bankList)
                    {
                        displayPath = string.Join(", ", bankList);
                    }
                    else if (banks is Array bankArray)
                    {
                        var paths = new List<string>();
                        foreach (var bank in bankArray)
                        {
                            paths.Add(bank?.ToString() ?? string.Empty);
                        }
                        displayPath = string.Join(", ", paths);
                    }
                }

                if (string.IsNullOrEmpty(displayPath))
                {
                    displayPath = "(Bank Loader)";
                }
            }
            else if (fieldType == FMODFieldType.StudioListener)
            {
                displayPath = "(Listener)";
            }

            return new FMODFieldInfo
            {
                Scene = scene,
                GameObject = component.gameObject,
                Component = component,
                Field = null,
                FieldName = componentType.Name,
                FieldType = fieldType,
                Value = component,
                DisplayPath = displayPath,
                IsValid = !string.IsNullOrEmpty(displayPath) || fieldType == FMODFieldType.StudioListener
            };
        }

        /// <summary>
        /// Scans all fields of a component for FMOD references.
        /// </summary>
        private static void ScanComponentFields(Component component, Scene scene, List<FMODFieldInfo> results)
        {
            var componentType = component.GetType();
            var fields = componentType.GetFields(
                BindingFlags.Instance |
                BindingFlags.Public |
                BindingFlags.NonPublic
            );

            foreach (var field in fields)
            {
                // Skip non-serialized private fields
                if (!field.IsPublic && field.GetCustomAttribute<SerializeField>() == null)
                    continue;

                var fieldInfo = AnalyzeField(field, component, scene);
                if (fieldInfo != null)
                {
                    results.Add(fieldInfo);
                }

                // Also check for arrays/lists of FMOD types
                ScanArrayField(field, component, scene, results);
            }
        }

        /// <summary>
        /// Analyzes a single field to determine if it's FMOD-related.
        /// </summary>
        private static FMODFieldInfo AnalyzeField(FieldInfo field, Component component, Scene scene)
        {
            var fieldTypeName = field.FieldType.Name;
            var fieldTypeFullName = field.FieldType.FullName ?? string.Empty;

            // Check for FMOD struct types (EventReference, BankReference, etc.)
            foreach (var fmodType in FMODStructTypes)
            {
                if (fieldTypeName == fmodType || fieldTypeFullName.Contains($"FMODUnity.{fmodType}"))
                {
                    var value = field.GetValue(component);
                    var displayPath = GetPathFromFMODStruct(value, fmodType);
                    var fmodFieldType = GetFieldTypeFromStructName(fmodType);

                    return new FMODFieldInfo
                    {
                        Scene = scene,
                        GameObject = component.gameObject,
                        Component = component,
                        Field = field,
                        FieldName = field.Name,
                        FieldType = fmodFieldType,
                        Value = value,
                        DisplayPath = displayPath,
                        IsValid = !string.IsNullOrEmpty(displayPath)
                    };
                }
            }

            // Check for FMOD attributes on string fields
            foreach (var attrName in FMODAttributeNames)
            {
                var attrs = field.GetCustomAttributes(true);
                foreach (var attr in attrs)
                {
                    if (attr.GetType().Name == attrName)
                    {
                        var value = field.GetValue(component) as string ?? string.Empty;
                        var fmodFieldType = GetFieldTypeFromAttributeName(attrName);

                        return new FMODFieldInfo
                        {
                            Scene = scene,
                            GameObject = component.gameObject,
                            Component = component,
                            Field = field,
                            FieldName = field.Name,
                            FieldType = fmodFieldType,
                            Value = value,
                            DisplayPath = value,
                            IsValid = !string.IsNullOrEmpty(value)
                        };
                    }
                }
            }

            // Check for string fields containing FMOD paths
            if (field.FieldType == typeof(string))
            {
                var value = field.GetValue(component) as string;
                if (!string.IsNullOrEmpty(value))
                {
                    foreach (var kvp in PathPrefixes)
                    {
                        if (value.StartsWith(kvp.Key, StringComparison.OrdinalIgnoreCase))
                        {
                            return new FMODFieldInfo
                            {
                                Scene = scene,
                                GameObject = component.gameObject,
                                Component = component,
                                Field = field,
                                FieldName = field.Name,
                                FieldType = kvp.Value,
                                Value = value,
                                DisplayPath = value,
                                IsValid = true
                            };
                        }
                    }
                }

                // Check for parameter name fields (common naming conventions)
                var fieldNameLower = field.Name.ToLowerInvariant();
                if (fieldNameLower.Contains("fmodparam") ||
                    fieldNameLower.Contains("parametername") ||
                    fieldNameLower.Contains("fmod") && fieldNameLower.Contains("param"))
                {
                    var paramValue = field.GetValue(component) as string;
                    if (!string.IsNullOrEmpty(paramValue))
                    {
                        return new FMODFieldInfo
                        {
                            Scene = scene,
                            GameObject = component.gameObject,
                            Component = component,
                            Field = field,
                            FieldName = field.Name,
                            FieldType = FMODFieldType.ParameterName,
                            Value = paramValue,
                            DisplayPath = paramValue,
                            IsValid = true
                        };
                    }
                }
            }

            return null;
        }

        /// <summary>
        /// Scans array/list fields for FMOD types.
        /// </summary>
        private static void ScanArrayField(FieldInfo field, Component component, Scene scene, List<FMODFieldInfo> results)
        {
            var fieldType = field.FieldType;

            // Check if it's an array or generic list
            Type elementType = null;

            if (fieldType.IsArray)
            {
                elementType = fieldType.GetElementType();
            }
            else if (fieldType.IsGenericType && fieldType.GetGenericTypeDefinition() == typeof(List<>))
            {
                elementType = fieldType.GetGenericArguments()[0];
            }

            if (elementType == null)
                return;

            // Check if element type is a struct that might contain FMOD references
            if (elementType.IsValueType && !elementType.IsPrimitive && elementType != typeof(decimal))
            {
                var value = field.GetValue(component);
                if (value == null)
                    return;

                var enumerable = value as System.Collections.IEnumerable;
                if (enumerable == null)
                    return;

                int index = 0;
                foreach (var item in enumerable)
                {
                    if (item == null)
                    {
                        index++;
                        continue;
                    }

                    // Scan fields of each struct in the array
                    var structFields = elementType.GetFields(BindingFlags.Instance | BindingFlags.Public | BindingFlags.NonPublic);
                    foreach (var structField in structFields)
                    {
                        var structFieldInfo = AnalyzeStructField(structField, item, component, scene, field.Name, index);
                        if (structFieldInfo != null)
                        {
                            results.Add(structFieldInfo);
                        }
                    }

                    index++;
                }
            }
        }

        /// <summary>
        /// Analyzes a field within a struct (for array elements).
        /// </summary>
        private static FMODFieldInfo AnalyzeStructField(FieldInfo structField, object structInstance, Component component, Scene scene, string arrayFieldName, int index)
        {
            var fieldTypeName = structField.FieldType.Name;
            var fieldTypeFullName = structField.FieldType.FullName ?? string.Empty;

            // Check for FMOD struct types
            foreach (var fmodType in FMODStructTypes)
            {
                if (fieldTypeName == fmodType || fieldTypeFullName.Contains($"FMODUnity.{fmodType}"))
                {
                    var value = structField.GetValue(structInstance);
                    var displayPath = GetPathFromFMODStruct(value, fmodType);
                    var fmodFieldType = GetFieldTypeFromStructName(fmodType);

                    return new FMODFieldInfo
                    {
                        Scene = scene,
                        GameObject = component.gameObject,
                        Component = component,
                        Field = structField,
                        FieldName = $"{arrayFieldName}[{index}].{structField.Name}",
                        FieldType = fmodFieldType,
                        Value = value,
                        DisplayPath = displayPath,
                        IsValid = !string.IsNullOrEmpty(displayPath)
                    };
                }
            }

            // Check for string fields that might be FMOD paths or parameters
            if (structField.FieldType == typeof(string))
            {
                var value = structField.GetValue(structInstance) as string;
                if (!string.IsNullOrEmpty(value))
                {
                    // Check for path prefixes
                    foreach (var kvp in PathPrefixes)
                    {
                        if (value.StartsWith(kvp.Key, StringComparison.OrdinalIgnoreCase))
                        {
                            return new FMODFieldInfo
                            {
                                Scene = scene,
                                GameObject = component.gameObject,
                                Component = component,
                                Field = structField,
                                FieldName = $"{arrayFieldName}[{index}].{structField.Name}",
                                FieldType = kvp.Value,
                                Value = value,
                                DisplayPath = value,
                                IsValid = true
                            };
                        }
                    }

                    // Check for parameter names
                    var fieldNameLower = structField.Name.ToLowerInvariant();
                    if (fieldNameLower.Contains("param") || fieldNameLower.Contains("fmod"))
                    {
                        return new FMODFieldInfo
                        {
                            Scene = scene,
                            GameObject = component.gameObject,
                            Component = component,
                            Field = structField,
                            FieldName = $"{arrayFieldName}[{index}].{structField.Name}",
                            FieldType = FMODFieldType.ParameterName,
                            Value = value,
                            DisplayPath = value,
                            IsValid = true
                        };
                    }
                }
            }

            return null;
        }

        /// <summary>
        /// Extracts the path from an FMOD struct (EventReference, etc.).
        /// </summary>
        private static string GetPathFromFMODStruct(object value, string structTypeName)
        {
            if (value == null)
                return string.Empty;

            // Try to get Path property or field
            var valueType = value.GetType();

            var pathProperty = valueType.GetProperty("Path");
            if (pathProperty != null)
            {
                return pathProperty.GetValue(value) as string ?? string.Empty;
            }

            var pathField = valueType.GetField("Path") ?? valueType.GetField("path");
            if (pathField != null)
            {
                return pathField.GetValue(value) as string ?? string.Empty;
            }

            // Try Guid for EventReference
            if (structTypeName == "EventReference")
            {
                var guidField = valueType.GetField("Guid") ?? valueType.GetField("guid");
                if (guidField != null)
                {
                    var guid = guidField.GetValue(value);
                    if (guid != null && guid.ToString() != "00000000-0000-0000-0000-000000000000")
                    {
                        return $"(GUID: {guid})";
                    }
                }
            }

            return string.Empty;
        }

        /// <summary>
        /// Extracts the path from an EventReference object.
        /// </summary>
        private static string GetPathFromEventReference(object eventRef)
        {
            return GetPathFromFMODStruct(eventRef, "EventReference");
        }

        /// <summary>
        /// Maps struct type name to FMODFieldType.
        /// </summary>
        private static FMODFieldType GetFieldTypeFromStructName(string structName)
        {
            switch (structName)
            {
                case "EventReference":
                    return FMODFieldType.EventReference;
                case "BankReference":
                    return FMODFieldType.BankReference;
                case "BusReference":
                    return FMODFieldType.BusReference;
                default:
                    return FMODFieldType.EventReference;
            }
        }

        /// <summary>
        /// Maps attribute name to FMODFieldType.
        /// </summary>
        private static FMODFieldType GetFieldTypeFromAttributeName(string attrName)
        {
            switch (attrName)
            {
                case "EventRefAttribute":
                    return FMODFieldType.EventPath;
                case "BankRefAttribute":
                    return FMODFieldType.BankPath;
                case "ParamRefAttribute":
                    return FMODFieldType.ParameterName;
                default:
                    return FMODFieldType.EventPath;
            }
        }
    }
}
