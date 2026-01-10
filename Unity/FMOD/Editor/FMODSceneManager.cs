/*
 * FMODSceneManager.cs
 *
 * Description:
 * A dockable Unity Editor window that displays all FMOD-related fields and components
 * across loaded scenes. Provides search, filtering, and navigation capabilities.
 *
 * Author: Daniel Dehaan
 * Website: danielrdehaan.com
 */

using System;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;
using UnityEditor;
using UnityEditor.SceneManagement;
using UnityEngine.SceneManagement;

namespace FMODSceneManager
{
    /// <summary>
    /// Editor window for managing and viewing FMOD references across scenes.
    /// </summary>
    public class FMODSceneManagerWindow : EditorWindow
    {
        // Scan results
        private List<FMODFieldInfo> _allResults = new List<FMODFieldInfo>();
        private List<FMODFieldInfo> _filteredResults = new List<FMODFieldInfo>();
        private bool _needsRescan = true;

        // UI State
        private Vector2 _scrollPosition;
        private string _searchFilter = "";
        private FMODFieldType? _typeFilter = null;
        private bool _showTreeView = false;
        private float _lastClickTime;
        private FMODFieldInfo _lastClickedItem;

        // Tree view state
        private Dictionary<string, bool> _sceneFoldouts = new Dictionary<string, bool>();
        private Dictionary<string, bool> _gameObjectFoldouts = new Dictionary<string, bool>();
        private Dictionary<string, bool> _componentFoldouts = new Dictionary<string, bool>();

        // Column widths
        private float _colScene = 100f;
        private float _colObject = 150f;
        private float _colComponent = 120f;
        private float _colField = 120f;
        private float _colType = 70f;

        // Sort state
        private enum SortColumn { Scene, Object, Component, Field, Type, Path }
        private SortColumn _sortColumn = SortColumn.Object;
        private bool _sortAscending = true;

        // Styles (cached)
        private GUIStyle _headerStyle;
        private GUIStyle _rowStyle;
        private GUIStyle _rowAltStyle;
        private GUIStyle _invalidRowStyle;
        private bool _stylesInitialized = false;

        // Preview state
        private FMODFieldInfo _previewingItem = null;

        [MenuItem("Tools/FMOD/Scene Manager %#f")]
        public static void ShowWindow()
        {
            var window = GetWindow<FMODSceneManagerWindow>("FMOD Scene Manager");
            window.minSize = new Vector2(600, 300);
            window.Show();
        }

        private void OnEnable()
        {
            EditorSceneManager.sceneOpened += OnSceneOpened;
            EditorSceneManager.sceneClosed += OnSceneClosed;
            EditorApplication.hierarchyChanged += OnHierarchyChanged;
            EditorApplication.playModeStateChanged += OnPlayModeChanged;

            _needsRescan = true;
        }

        private void OnDisable()
        {
            EditorSceneManager.sceneOpened -= OnSceneOpened;
            EditorSceneManager.sceneClosed -= OnSceneClosed;
            EditorApplication.hierarchyChanged -= OnHierarchyChanged;
            EditorApplication.playModeStateChanged -= OnPlayModeChanged;

            StopPreview();
        }

        private void OnSceneOpened(Scene scene, OpenSceneMode mode) => _needsRescan = true;
        private void OnSceneClosed(Scene scene) => _needsRescan = true;
        private void OnHierarchyChanged() => _needsRescan = true;
        private void OnPlayModeChanged(PlayModeStateChange state) => _needsRescan = true;

        private void InitStyles()
        {
            if (_stylesInitialized)
                return;

            _headerStyle = new GUIStyle(EditorStyles.boldLabel)
            {
                alignment = TextAnchor.MiddleLeft
            };

            _rowStyle = new GUIStyle(EditorStyles.label)
            {
                padding = new RectOffset(4, 4, 2, 2)
            };

            _rowAltStyle = new GUIStyle(_rowStyle);
            var altBg = new Texture2D(1, 1);
            altBg.SetPixel(0, 0, new Color(0, 0, 0, 0.05f));
            altBg.Apply();
            _rowAltStyle.normal.background = altBg;

            _invalidRowStyle = new GUIStyle(_rowStyle);
            var invalidBg = new Texture2D(1, 1);
            invalidBg.SetPixel(0, 0, new Color(1, 0.3f, 0.3f, 0.2f));
            invalidBg.Apply();
            _invalidRowStyle.normal.background = invalidBg;

            _stylesInitialized = true;
        }

        private void OnGUI()
        {
            InitStyles();

            if (_needsRescan)
            {
                PerformScan();
                _needsRescan = false;
            }

            DrawToolbar();
            DrawFilterBar();
            DrawContent();
            DrawStatusBar();
        }

        private void DrawToolbar()
        {
            EditorGUILayout.BeginHorizontal(EditorStyles.toolbar);

            if (GUILayout.Button("Scan", EditorStyles.toolbarButton, GUILayout.Width(50)))
            {
                PerformScan();
            }

            if (GUILayout.Button("Refresh", EditorStyles.toolbarButton, GUILayout.Width(60)))
            {
                _needsRescan = true;
                Repaint();
            }

            GUILayout.FlexibleSpace();

            // View toggle
            EditorGUILayout.LabelField("View:", GUILayout.Width(35));
            if (GUILayout.Button(_showTreeView ? "Tree" : "Table", EditorStyles.toolbarDropDown, GUILayout.Width(60)))
            {
                _showTreeView = !_showTreeView;
            }

            GUILayout.Space(10);

            // Search field
            EditorGUILayout.LabelField("Search:", GUILayout.Width(45));
            var newSearch = EditorGUILayout.TextField(_searchFilter, EditorStyles.toolbarSearchField, GUILayout.Width(200));
            if (newSearch != _searchFilter)
            {
                _searchFilter = newSearch;
                ApplyFilters();
            }

            if (GUILayout.Button("", GUI.skin.FindStyle("ToolbarSearchCancelButton") ?? EditorStyles.toolbarButton))
            {
                _searchFilter = "";
                ApplyFilters();
                GUI.FocusControl(null);
            }

            EditorGUILayout.EndHorizontal();
        }

        private void DrawFilterBar()
        {
            EditorGUILayout.BeginHorizontal(EditorStyles.toolbar);

            EditorGUILayout.LabelField("Filter:", GUILayout.Width(40));

            if (DrawFilterButton("All", _typeFilter == null))
            {
                _typeFilter = null;
                ApplyFilters();
            }

            if (DrawFilterButton("Events", IsEventFilter()))
            {
                _typeFilter = _typeFilter == FMODFieldType.EventReference ? null : FMODFieldType.EventReference;
                ApplyFilters();
            }

            if (DrawFilterButton("Banks", IsBankFilter()))
            {
                _typeFilter = _typeFilter == FMODFieldType.BankReference ? null : FMODFieldType.BankReference;
                ApplyFilters();
            }

            if (DrawFilterButton("Buses", IsBusFilter()))
            {
                _typeFilter = _typeFilter == FMODFieldType.BusReference ? null : FMODFieldType.BusReference;
                ApplyFilters();
            }

            if (DrawFilterButton("Params", _typeFilter == FMODFieldType.ParameterName))
            {
                _typeFilter = _typeFilter == FMODFieldType.ParameterName ? null : FMODFieldType.ParameterName;
                ApplyFilters();
            }

            if (DrawFilterButton("Invalid", _typeFilter == (FMODFieldType)(-1)))
            {
                _typeFilter = _typeFilter == (FMODFieldType)(-1) ? null : (FMODFieldType)(-1);
                ApplyFilters();
            }

            GUILayout.FlexibleSpace();

            // Play mode indicator
            if (EditorApplication.isPlaying)
            {
                var prevColor = GUI.backgroundColor;
                GUI.backgroundColor = new Color(0.3f, 0.8f, 0.3f);
                GUILayout.Label(" PLAY MODE ", EditorStyles.toolbarButton);
                GUI.backgroundColor = prevColor;
            }

            EditorGUILayout.EndHorizontal();
        }

        private bool DrawFilterButton(string label, bool isActive)
        {
            var prevColor = GUI.backgroundColor;
            if (isActive)
            {
                GUI.backgroundColor = new Color(0.6f, 0.8f, 1f);
            }

            bool clicked = GUILayout.Button(label, EditorStyles.toolbarButton, GUILayout.Width(60));

            GUI.backgroundColor = prevColor;
            return clicked;
        }

        private bool IsEventFilter()
        {
            return _typeFilter == FMODFieldType.EventReference ||
                   _typeFilter == FMODFieldType.EventPath ||
                   _typeFilter == FMODFieldType.StudioEmitter;
        }

        private bool IsBankFilter()
        {
            return _typeFilter == FMODFieldType.BankReference ||
                   _typeFilter == FMODFieldType.BankPath ||
                   _typeFilter == FMODFieldType.StudioBankLoader;
        }

        private bool IsBusFilter()
        {
            return _typeFilter == FMODFieldType.BusReference ||
                   _typeFilter == FMODFieldType.BusPath ||
                   _typeFilter == FMODFieldType.SnapshotPath ||
                   _typeFilter == FMODFieldType.VCAPath;
        }

        private void DrawContent()
        {
            if (_showTreeView)
            {
                DrawTreeView();
            }
            else
            {
                DrawTableView();
            }
        }

        private void DrawTableView()
        {
            // Column headers
            EditorGUILayout.BeginHorizontal(EditorStyles.toolbar);

            DrawSortableHeader("Scene", SortColumn.Scene, _colScene);
            DrawSortableHeader("Object", SortColumn.Object, _colObject);
            DrawSortableHeader("Component", SortColumn.Component, _colComponent);
            DrawSortableHeader("Field", SortColumn.Field, _colField);
            DrawSortableHeader("Type", SortColumn.Type, _colType);
            DrawSortableHeader("Path / Value", SortColumn.Path, 0); // Flexible

            EditorGUILayout.EndHorizontal();

            // Scrollable content
            _scrollPosition = EditorGUILayout.BeginScrollView(_scrollPosition);

            for (int i = 0; i < _filteredResults.Count; i++)
            {
                var item = _filteredResults[i];
                DrawTableRow(item, i);
            }

            if (_filteredResults.Count == 0)
            {
                EditorGUILayout.HelpBox(
                    _allResults.Count == 0
                        ? "No FMOD references found. Click 'Scan' to search loaded scenes."
                        : "No results match the current filter.",
                    MessageType.Info);
            }

            EditorGUILayout.EndScrollView();
        }

        private void DrawSortableHeader(string label, SortColumn column, float width)
        {
            string sortIndicator = "";
            if (_sortColumn == column)
            {
                sortIndicator = _sortAscending ? " ^" : " v";
            }

            bool clicked;
            if (width > 0)
            {
                clicked = GUILayout.Button(label + sortIndicator, EditorStyles.toolbarButton, GUILayout.Width(width));
            }
            else
            {
                clicked = GUILayout.Button(label + sortIndicator, EditorStyles.toolbarButton);
            }

            if (clicked)
            {
                if (_sortColumn == column)
                {
                    _sortAscending = !_sortAscending;
                }
                else
                {
                    _sortColumn = column;
                    _sortAscending = true;
                }
                ApplyFilters();
            }
        }

        private void DrawTableRow(FMODFieldInfo item, int index)
        {
            var style = item.IsValid
                ? (index % 2 == 0 ? _rowStyle : _rowAltStyle)
                : _invalidRowStyle;

            var rect = EditorGUILayout.BeginHorizontal(style);

            // Handle clicks
            if (Event.current.type == EventType.MouseDown && rect.Contains(Event.current.mousePosition))
            {
                HandleRowClick(item);
                Event.current.Use();
            }

            // Scene
            EditorGUILayout.LabelField(item.Scene.name, GUILayout.Width(_colScene));

            // Object
            EditorGUILayout.LabelField(item.GameObject?.name ?? "(null)", GUILayout.Width(_colObject));

            // Component
            string componentName = item.Component?.GetType().Name ?? "(null)";
            EditorGUILayout.LabelField(componentName, GUILayout.Width(_colComponent));

            // Field
            EditorGUILayout.LabelField(item.FieldName, GUILayout.Width(_colField));

            // Type
            EditorGUILayout.LabelField(item.TypeDisplayName, GUILayout.Width(_colType));

            // Path / Value + Preview button
            EditorGUILayout.BeginHorizontal();

            string displayValue = string.IsNullOrEmpty(item.DisplayPath) ? "(empty)" : item.DisplayPath;
            if (!item.IsValid && !string.IsNullOrEmpty(item.ValidationMessage))
            {
                displayValue += $" [{item.ValidationMessage}]";
            }
            EditorGUILayout.LabelField(displayValue);

            // Preview button for events
            if (item.CanPreview && !string.IsNullOrEmpty(item.DisplayPath))
            {
                bool isPreviewing = _previewingItem == item;
                string buttonLabel = isPreviewing ? "Stop" : "Play";

                if (GUILayout.Button(buttonLabel, GUILayout.Width(45)))
                {
                    if (isPreviewing)
                    {
                        StopPreview();
                    }
                    else
                    {
                        StartPreview(item);
                    }
                }
            }

            EditorGUILayout.EndHorizontal();

            EditorGUILayout.EndHorizontal();
        }

        private void DrawTreeView()
        {
            _scrollPosition = EditorGUILayout.BeginScrollView(_scrollPosition);

            // Group by scene
            var byScene = _filteredResults.GroupBy(r => r.Scene.name).OrderBy(g => g.Key);

            foreach (var sceneGroup in byScene)
            {
                string sceneKey = sceneGroup.Key;
                if (!_sceneFoldouts.ContainsKey(sceneKey))
                    _sceneFoldouts[sceneKey] = true;

                _sceneFoldouts[sceneKey] = EditorGUILayout.Foldout(_sceneFoldouts[sceneKey], $"Scene: {sceneKey}", true);

                if (_sceneFoldouts[sceneKey])
                {
                    EditorGUI.indentLevel++;

                    // Group by GameObject
                    var byObject = sceneGroup.GroupBy(r => r.GameObjectPath).OrderBy(g => g.Key);

                    foreach (var objectGroup in byObject)
                    {
                        string objectKey = $"{sceneKey}/{objectGroup.Key}";
                        if (!_gameObjectFoldouts.ContainsKey(objectKey))
                            _gameObjectFoldouts[objectKey] = true;

                        string objectName = objectGroup.First().GameObject?.name ?? "(null)";
                        _gameObjectFoldouts[objectKey] = EditorGUILayout.Foldout(_gameObjectFoldouts[objectKey], objectName, true);

                        if (_gameObjectFoldouts[objectKey])
                        {
                            EditorGUI.indentLevel++;

                            // Group by Component
                            var byComponent = objectGroup.GroupBy(r => r.Component?.GetType().Name ?? "(null)").OrderBy(g => g.Key);

                            foreach (var componentGroup in byComponent)
                            {
                                string componentKey = $"{objectKey}/{componentGroup.Key}";
                                if (!_componentFoldouts.ContainsKey(componentKey))
                                    _componentFoldouts[componentKey] = true;

                                _componentFoldouts[componentKey] = EditorGUILayout.Foldout(_componentFoldouts[componentKey], componentGroup.Key, true);

                                if (_componentFoldouts[componentKey])
                                {
                                    EditorGUI.indentLevel++;

                                    foreach (var item in componentGroup)
                                    {
                                        DrawTreeViewItem(item);
                                    }

                                    EditorGUI.indentLevel--;
                                }
                            }

                            EditorGUI.indentLevel--;
                        }
                    }

                    EditorGUI.indentLevel--;
                }
            }

            if (_filteredResults.Count == 0)
            {
                EditorGUILayout.HelpBox(
                    _allResults.Count == 0
                        ? "No FMOD references found. Click 'Scan' to search loaded scenes."
                        : "No results match the current filter.",
                    MessageType.Info);
            }

            EditorGUILayout.EndScrollView();
        }

        private void DrawTreeViewItem(FMODFieldInfo item)
        {
            EditorGUILayout.BeginHorizontal();

            // Indicator for invalid items
            if (!item.IsValid)
            {
                var prevColor = GUI.color;
                GUI.color = new Color(1f, 0.5f, 0.5f);
                EditorGUILayout.LabelField("!", GUILayout.Width(15));
                GUI.color = prevColor;
            }
            else
            {
                EditorGUILayout.LabelField("", GUILayout.Width(15));
            }

            // Field name and type
            string label = $"{item.FieldName} ({item.TypeDisplayName})";
            EditorGUILayout.LabelField(label, GUILayout.Width(200));

            // Value/Path
            string displayValue = string.IsNullOrEmpty(item.DisplayPath) ? "(empty)" : item.DisplayPath;
            EditorGUILayout.LabelField("-> " + displayValue);

            // Select button
            if (GUILayout.Button("Select", GUILayout.Width(50)))
            {
                SelectItem(item);
            }

            // Preview button for events
            if (item.CanPreview && !string.IsNullOrEmpty(item.DisplayPath))
            {
                bool isPreviewing = _previewingItem == item;
                string buttonLabel = isPreviewing ? "Stop" : "Play";

                if (GUILayout.Button(buttonLabel, GUILayout.Width(45)))
                {
                    if (isPreviewing)
                    {
                        StopPreview();
                    }
                    else
                    {
                        StartPreview(item);
                    }
                }
            }

            EditorGUILayout.EndHorizontal();
        }

        private void DrawStatusBar()
        {
            EditorGUILayout.BeginHorizontal(EditorStyles.toolbar);

            int totalCount = _allResults.Count;
            int filteredCount = _filteredResults.Count;
            int invalidCount = _allResults.Count(r => !r.IsValid);

            string statusText = $"Found {totalCount} FMOD references";
            if (filteredCount != totalCount)
            {
                statusText += $" (showing {filteredCount})";
            }

            EditorGUILayout.LabelField(statusText);

            GUILayout.FlexibleSpace();

            if (invalidCount > 0)
            {
                var prevColor = GUI.color;
                GUI.color = new Color(1f, 0.7f, 0.7f);
                EditorGUILayout.LabelField($"! {invalidCount} invalid reference(s)", GUILayout.Width(150));
                GUI.color = prevColor;
            }

            EditorGUILayout.EndHorizontal();
        }

        private void HandleRowClick(FMODFieldInfo item)
        {
            float currentTime = Time.realtimeSinceStartup;

            // Double-click detection
            if (_lastClickedItem == item && (currentTime - _lastClickTime) < 0.3f)
            {
                // Double-click: ping in scene
                PingItem(item);
            }
            else
            {
                // Single click: select
                SelectItem(item);
            }

            _lastClickedItem = item;
            _lastClickTime = currentTime;
        }

        private void SelectItem(FMODFieldInfo item)
        {
            if (item.GameObject != null)
            {
                Selection.activeGameObject = item.GameObject;
            }
        }

        private void PingItem(FMODFieldInfo item)
        {
            if (item.GameObject != null)
            {
                EditorGUIUtility.PingObject(item.GameObject);
                SceneView.lastActiveSceneView?.FrameSelected();
            }
        }

        private void PerformScan()
        {
            _allResults = FMODSceneScanner.ScanAllScenes();
            ApplyFilters();
            Repaint();
        }

        private void ApplyFilters()
        {
            _filteredResults = _allResults;

            // Apply type filter
            if (_typeFilter.HasValue)
            {
                if (_typeFilter.Value == (FMODFieldType)(-1))
                {
                    // Special case: show invalid only
                    _filteredResults = _filteredResults.Where(r => !r.IsValid).ToList();
                }
                else if (IsEventTypeFilter(_typeFilter.Value))
                {
                    _filteredResults = _filteredResults.Where(r =>
                        r.FieldType == FMODFieldType.EventReference ||
                        r.FieldType == FMODFieldType.EventPath ||
                        r.FieldType == FMODFieldType.StudioEmitter).ToList();
                }
                else if (IsBankTypeFilter(_typeFilter.Value))
                {
                    _filteredResults = _filteredResults.Where(r =>
                        r.FieldType == FMODFieldType.BankReference ||
                        r.FieldType == FMODFieldType.BankPath ||
                        r.FieldType == FMODFieldType.StudioBankLoader).ToList();
                }
                else if (IsBusTypeFilter(_typeFilter.Value))
                {
                    _filteredResults = _filteredResults.Where(r =>
                        r.FieldType == FMODFieldType.BusReference ||
                        r.FieldType == FMODFieldType.BusPath ||
                        r.FieldType == FMODFieldType.SnapshotPath ||
                        r.FieldType == FMODFieldType.VCAPath).ToList();
                }
                else
                {
                    _filteredResults = _filteredResults.Where(r => r.FieldType == _typeFilter.Value).ToList();
                }
            }

            // Apply search filter
            if (!string.IsNullOrEmpty(_searchFilter))
            {
                string search = _searchFilter.ToLowerInvariant();
                _filteredResults = _filteredResults.Where(r =>
                    (r.GameObject?.name.ToLowerInvariant().Contains(search) ?? false) ||
                    (r.Component?.GetType().Name.ToLowerInvariant().Contains(search) ?? false) ||
                    (r.FieldName?.ToLowerInvariant().Contains(search) ?? false) ||
                    (r.DisplayPath?.ToLowerInvariant().Contains(search) ?? false) ||
                    (r.Scene.name.ToLowerInvariant().Contains(search))
                ).ToList();
            }

            // Apply sorting
            _filteredResults = SortResults(_filteredResults);
        }

        private bool IsEventTypeFilter(FMODFieldType type)
        {
            return type == FMODFieldType.EventReference ||
                   type == FMODFieldType.EventPath ||
                   type == FMODFieldType.StudioEmitter;
        }

        private bool IsBankTypeFilter(FMODFieldType type)
        {
            return type == FMODFieldType.BankReference ||
                   type == FMODFieldType.BankPath ||
                   type == FMODFieldType.StudioBankLoader;
        }

        private bool IsBusTypeFilter(FMODFieldType type)
        {
            return type == FMODFieldType.BusReference ||
                   type == FMODFieldType.BusPath ||
                   type == FMODFieldType.SnapshotPath ||
                   type == FMODFieldType.VCAPath;
        }

        private List<FMODFieldInfo> SortResults(List<FMODFieldInfo> results)
        {
            IOrderedEnumerable<FMODFieldInfo> sorted;

            switch (_sortColumn)
            {
                case SortColumn.Scene:
                    sorted = _sortAscending
                        ? results.OrderBy(r => r.Scene.name)
                        : results.OrderByDescending(r => r.Scene.name);
                    break;
                case SortColumn.Object:
                    sorted = _sortAscending
                        ? results.OrderBy(r => r.GameObject?.name ?? "")
                        : results.OrderByDescending(r => r.GameObject?.name ?? "");
                    break;
                case SortColumn.Component:
                    sorted = _sortAscending
                        ? results.OrderBy(r => r.Component?.GetType().Name ?? "")
                        : results.OrderByDescending(r => r.Component?.GetType().Name ?? "");
                    break;
                case SortColumn.Field:
                    sorted = _sortAscending
                        ? results.OrderBy(r => r.FieldName)
                        : results.OrderByDescending(r => r.FieldName);
                    break;
                case SortColumn.Type:
                    sorted = _sortAscending
                        ? results.OrderBy(r => r.TypeDisplayName)
                        : results.OrderByDescending(r => r.TypeDisplayName);
                    break;
                case SortColumn.Path:
                    sorted = _sortAscending
                        ? results.OrderBy(r => r.DisplayPath ?? "")
                        : results.OrderByDescending(r => r.DisplayPath ?? "");
                    break;
                default:
                    return results;
            }

            return sorted.ToList();
        }

        private void StartPreview(FMODFieldInfo item)
        {
            StopPreview();

            if (string.IsNullOrEmpty(item.DisplayPath))
                return;

            // Try to use FMOD's editor preview API
            try
            {
                // Use reflection to call FMODUnity.EditorUtils.PreviewEvent
                var editorUtilsType = Type.GetType("FMODUnity.EditorUtils, FMODUnity");
                if (editorUtilsType != null)
                {
                    var eventManagerType = Type.GetType("FMODUnity.EventManager, FMODUnity");
                    if (eventManagerType != null)
                    {
                        // Get the event reference from path
                        var eventFromPathMethod = eventManagerType.GetMethod("EventFromPath",
                            BindingFlags.Public | BindingFlags.Static);

                        if (eventFromPathMethod != null)
                        {
                            var editorEventRef = eventFromPathMethod.Invoke(null, new object[] { item.DisplayPath });

                            if (editorEventRef != null)
                            {
                                var previewMethod = editorUtilsType.GetMethod("PreviewEvent",
                                    BindingFlags.Public | BindingFlags.Static,
                                    null,
                                    new Type[] { editorEventRef.GetType() },
                                    null);

                                if (previewMethod != null)
                                {
                                    previewMethod.Invoke(null, new object[] { editorEventRef });
                                    _previewingItem = item;
                                }
                            }
                        }
                    }
                }
            }
            catch (Exception e)
            {
                Debug.LogWarning($"FMOD Scene Manager: Could not preview event. {e.Message}");
            }
        }

        private void StopPreview()
        {
            if (_previewingItem == null)
                return;

            try
            {
                var editorUtilsType = Type.GetType("FMODUnity.EditorUtils, FMODUnity");
                if (editorUtilsType != null)
                {
                    var stopMethod = editorUtilsType.GetMethod("PreviewStop",
                        BindingFlags.Public | BindingFlags.Static);

                    if (stopMethod != null)
                    {
                        stopMethod.Invoke(null, null);
                    }
                }
            }
            catch (Exception e)
            {
                Debug.LogWarning($"FMOD Scene Manager: Could not stop preview. {e.Message}");
            }

            _previewingItem = null;
        }
    }
}
