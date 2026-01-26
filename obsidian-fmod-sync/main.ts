import {
	addIcon,
	App,
	FuzzySuggestModal,
	Notice,
	Plugin,
	PluginSettingTab,
	Setting,
	TFile,
	TFolder,
	normalizePath,
} from "obsidian";

// FMOD logo SVG - cyan color (#6ECEF4)
const FMOD_ICON_SVG = `<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 512 512">
  <path fill="#6ECEF4" d="M 235.00 455.60 C178.64,446.91 128.15,416.19 103.89,375.82 C97.48,365.16 92.68,352.51 93.38,348.13 C94.34,342.09 99.87,338.00 107.08,338.00 C110.58,338.00 111.81,338.58 114.63,341.57 C116.48,343.54 118.00,345.77 118.00,346.54 C118.00,349.07 127.45,366.43 132.16,372.56 C155.41,402.78 200.08,426.38 242.88,431.05 C307.51,438.10 375.01,393.86 391.73,333.50 C394.25,324.42 394.50,321.99 394.48,307.00 C394.46,288.57 393.03,281.43 386.57,267.44 C375.72,243.96 353.61,222.46 327.49,209.99 C312.26,202.73 291.93,196.95 276.25,195.44 L 269.00 194.74 L 269.00 343.28 L 250.75 342.70 C236.81,342.26 229.66,341.53 220.47,339.60 C196.80,334.63 172.26,324.52 153.00,311.79 C140.06,303.24 121.23,284.50 113.23,272.22 C84.37,227.89 86.47,173.31 118.83,126.50 C127.20,114.40 147.34,94.85 160.41,86.13 C208.46,54.09 260.10,47.04 314.50,65.09 C349.20,76.60 381.76,98.56 400.87,123.33 C411.56,137.19 421.77,159.06 420.57,165.50 C419.31,172.21 411.05,177.16 404.65,175.05 C399.96,173.50 397.70,170.68 394.05,161.84 C379.77,127.24 342.20,98.03 296.50,86.00 C279.94,81.64 271.14,80.70 252.25,81.29 C228.58,82.02 215.31,85.28 194.50,95.47 C178.20,103.46 168.87,110.14 155.38,123.50 C129.65,148.98 118.59,173.71 118.71,205.50 C118.79,225.14 122.38,237.97 132.68,255.35 C138.64,265.43 157.14,284.39 167.92,291.47 C187.79,304.54 215.57,314.70 236.64,316.61 L 244.00 317.28 L 244.00 169.00 L 254.75 169.03 C334.67,169.22 401.89,214.64 417.11,278.73 C435.33,355.45 376.41,435.13 287.50,454.01 C274.77,456.72 247.57,457.54 235.00,455.60 Z"/>
</svg>`;
import * as fs from "fs";
import * as path from "path";

// Electron remote for native file dialogs and shell
// eslint-disable-next-line @typescript-eslint/no-var-requires
const { remote, shell } = require("electron");

// Child process for launching FMOD Studio
// eslint-disable-next-line @typescript-eslint/no-var-requires
const { exec, spawn } = require("child_process");
// eslint-disable-next-line @typescript-eslint/no-var-requires
const { promisify } = require("util");
const execAsync = promisify(exec);

// ============================================================================
// FMOD Companion Script
// ============================================================================

const FMOD_SCRIPT_FILENAME = "FMOD_Obsidian_Sync.js";
const FMOD_SCRIPT_URL = "https://raw.githubusercontent.com/danielrdehaan/Scripts/main/FMOD/FMOD_Obsidian_Sync.js";

/**
 * Get the FMOD Studio scripts folder path from the installation path.
 * - macOS: <app>/Contents/Scripts/
 * - Windows: <exe directory>/Scripts/
 */
function getFmodScriptsFolder(installationPath: string): string {
	const platform = process.platform;
	if (platform === "darwin") {
		// macOS: Scripts folder is inside the .app bundle
		if (installationPath.endsWith(".app")) {
			return path.join(installationPath, "Contents", "Scripts");
		}
	} else if (platform === "win32") {
		// Windows: Scripts folder is next to the exe
		const exeDir = path.dirname(installationPath);
		return path.join(exeDir, "Scripts");
	}
	return "";
}

/**
 * Check if the companion script exists in the FMOD Studio scripts folder.
 */
function checkCompanionScriptExists(installationPath: string): boolean {
	const scriptsFolder = getFmodScriptsFolder(installationPath);
	if (!scriptsFolder) return false;
	const scriptPath = path.join(scriptsFolder, FMOD_SCRIPT_FILENAME);
	return fs.existsSync(scriptPath);
}

/**
 * Install the companion script to the FMOD Studio scripts folder.
 * Fetches the latest version from GitHub.
 */
async function installCompanionScript(installationPath: string): Promise<{ success: boolean; error?: string }> {
	const scriptsFolder = getFmodScriptsFolder(installationPath);
	if (!scriptsFolder) {
		return { success: false, error: "Could not determine scripts folder path" };
	}

	// Ensure scripts folder exists
	try {
		if (!fs.existsSync(scriptsFolder)) {
			fs.mkdirSync(scriptsFolder, { recursive: true });
		}
	} catch (err) {
		return { success: false, error: `Could not create scripts folder: ${err}` };
	}

	// Fetch script from GitHub
	try {
		const response = await fetch(FMOD_SCRIPT_URL);
		if (!response.ok) {
			return { success: false, error: `Failed to fetch script: ${response.status} ${response.statusText}` };
		}
		const scriptContent = await response.text();

		// Write script to file
		const scriptPath = path.join(scriptsFolder, FMOD_SCRIPT_FILENAME);
		fs.writeFileSync(scriptPath, scriptContent, "utf8");

		return { success: true };
	} catch (err) {
		return { success: false, error: `Failed to install script: ${err}` };
	}
}

// ============================================================================
// Types
// ============================================================================

interface FMODInstallation {
	id: string;
	path: string;           // Path to .app (macOS) or .exe (Windows)
	version: string;        // Auto-detected version number
}

interface FMODParameter {
	name: string;
	type: string;
	min: number | string;
	max: number | string;
	initial: number | string;
	labels?: string;
}

interface FMODUserProperty {
	name: string;
	type: string;
	value: string | number | boolean;
}

interface FMODEvent {
	name: string;
	guid: string;
	full_path: string;
	folder_path: string;
	banks: string[];
	loop_type: string;
	space: string;
	max_voices: number | string;
	notes: string;
	parameters: FMODParameter[];
	user_properties: FMODUserProperty[];
}

interface FMODExportData {
	exported_at: string;
	fmod_version?: string;
	project_name: string;
	project_path: string;
	event_count: number;
	events: FMODEvent[];
}

interface FMODProjectConfig {
	id: string;
	jsonFilePath: string;
	outputFolder: string;
	// Metadata extracted from JSON on sync
	fmodProjectName?: string;  // Project name from JSON
	fmodProjectPath?: string;  // Path to .fspro file
	fmodVersion?: string;      // FMOD Studio version
	lastExportedAt?: string;   // When JSON was exported
	// Version override for opening project
	selectedFmodInstallationId?: string;  // Override version for this project
}

interface FMODSyncSettings {
	projects: FMODProjectConfig[];
	fmodInstallations: FMODInstallation[];
}

interface ParsedFrontmatter {
	properties: Record<string, string | string[] | number | boolean>;
	bodyStart: number;
}

interface SyncStats {
	created: number;
	updated: number;
	moved: number;
	skipped: number;
	errors: number;
}

interface NewerExportInfo {
	filePath: string;
	projectName: string;
	exportDate: Date;
}

// ============================================================================
// Newer Export Detection
// ============================================================================

/**
 * Parse a timestamped export filename to extract project name and date.
 * Expected format: ProjectName_YYYY-MM-DD_HHMMSS.json
 */
function parseExportFilename(filename: string): { projectName: string; date: Date } | null {
	const baseName = filename.replace(/\.json$/i, "");
	const match = baseName.match(/^(.+)_(\d{4}-\d{2}-\d{2})_(\d{6})$/);
	if (!match) return null;

	const [, projectName, dateStr, timeStr] = match;
	const year = parseInt(dateStr.substring(0, 4));
	const month = parseInt(dateStr.substring(5, 7)) - 1; // 0-indexed
	const day = parseInt(dateStr.substring(8, 10));
	const hour = parseInt(timeStr.substring(0, 2));
	const minute = parseInt(timeStr.substring(2, 4));
	const second = parseInt(timeStr.substring(4, 6));

	return { projectName, date: new Date(year, month, day, hour, minute, second) };
}

/**
 * Scan a directory for newer exports matching the same project name.
 * Returns info about the newest file if it's newer than the current file.
 */
async function findNewerExport(currentPath: string): Promise<NewerExportInfo | null> {
	if (!currentPath) return null;

	const directory = path.dirname(currentPath);
	const currentFilename = path.basename(currentPath);
	const currentParsed = parseExportFilename(currentFilename);

	if (!currentParsed) return null; // Current file doesn't match expected format

	const currentProjectName = currentParsed.projectName;
	const currentDate = currentParsed.date;

	return new Promise((resolve) => {
		fs.readdir(directory, (err, files) => {
			if (err) {
				resolve(null);
				return;
			}

			let newestExport: NewerExportInfo | null = null;

			for (const file of files) {
				if (!file.endsWith(".json")) continue;

				const parsed = parseExportFilename(file);
				if (!parsed) continue;

				// Only consider files from the same project
				if (parsed.projectName !== currentProjectName) continue;

				// Check if this file is newer than current
				if (parsed.date > currentDate) {
					if (!newestExport || parsed.date > newestExport.exportDate) {
						newestExport = {
							filePath: path.join(directory, file),
							projectName: parsed.projectName,
							exportDate: parsed.date,
						};
					}
				}
			}

			resolve(newestExport);
		});
	});
}

// ============================================================================
// Default Settings
// ============================================================================

const DEFAULT_SETTINGS: FMODSyncSettings = {
	projects: [],
	fmodInstallations: [],
};

interface ProjectPickerItem {
	type: "all" | "single";
	project?: FMODProjectConfig;
}

// ============================================================================
// Version Detection
// ============================================================================

async function detectFmodVersion(appPath: string): Promise<string> {
	const platform = process.platform;

	try {
		if (platform === "darwin") {
			// macOS: Read version from revision_studio.txt in the app bundle
			if (!appPath.endsWith(".app")) {
				return "Unknown";
			}

			const revisionPath = path.join(appPath, "Contents", "Resources", "documentation", "revision_studio.txt");

			// Read the revision file and find the first version line
			const content = await new Promise<string>((resolve, reject) => {
				fs.readFile(revisionPath, "utf8", (err: Error | null, data: string) => {
					if (err) reject(err);
					else resolve(data);
				});
			});

			// Version line format: "13/1/26 2.02.33 - Studio Tool minor release (build 160335)"
			const versionMatch = content.match(/^\d+\/\d+\/\d+\s+(\d+\.\d+\.\d+)\s+-/m);
			if (versionMatch) {
				return versionMatch[1];
			}

			return "Unknown";
		} else if (platform === "win32") {
			// Windows: Try revision file first (same location relative to exe)
			const exeDir = path.dirname(appPath);
			const revisionPath = path.join(exeDir, "documentation", "revision_studio.txt");

			try {
				const content = await new Promise<string>((resolve, reject) => {
					fs.readFile(revisionPath, "utf8", (err: Error | null, data: string) => {
						if (err) reject(err);
						else resolve(data);
					});
				});

				const versionMatch = content.match(/^\d+\/\d+\/\d+\s+(\d+\.\d+\.\d+)\s+-/m);
				if (versionMatch) {
					return versionMatch[1];
				}
			} catch {
				// Fall back to PowerShell file version
				const psCommand = `(Get-Item "${appPath}").VersionInfo.ProductVersion`;
				const { stdout } = await execAsync(`powershell -Command "${psCommand}"`);
				const version = stdout.trim();
				return version || "Unknown";
			}
		}
	} catch (error) {
		console.error("Failed to detect FMOD version:", error);
	}

	return "Unknown";
}

// ============================================================================
// Main Plugin
// ============================================================================

export default class FMODSyncPlugin extends Plugin {
	settings: FMODSyncSettings = DEFAULT_SETTINGS;
	private settingsTab: FMODSyncSettingTab | null = null;
	newerExports: Map<string, NewerExportInfo> = new Map();

	async onload(): Promise<void> {
		await this.loadSettings();

		// Check for newer exports on load (non-blocking)
		this.checkForNewerExports();

		// Register custom FMOD icon
		addIcon("fmod-logo", FMOD_ICON_SVG);

		// Add ribbon icon
		this.addRibbonIcon("fmod-logo", "FMOD Sync", () => {
			this.runSync();
		});

		// Add command
		this.addCommand({
			id: "import-from-json",
			name: "Import from JSON",
			callback: () => {
				this.runSync();
			},
		});

		// Add settings tab
		this.settingsTab = new FMODSyncSettingTab(this.app, this);
		this.addSettingTab(this.settingsTab);
	}

	async loadSettings(): Promise<void> {
		this.settings = Object.assign({}, DEFAULT_SETTINGS, await this.loadData());
	}

	async saveSettings(): Promise<void> {
		await this.saveData(this.settings);
	}

	/**
	 * Refresh the settings tab display if it's currently visible.
	 * Called after sync to update project metadata shown in the UI.
	 */
	refreshSettingsTab(): void {
		if (this.settingsTab) {
			this.settingsTab.display();
		}
	}

	/**
	 * Check all projects for newer JSON exports in their directories.
	 * Updates the newerExports Map with any found.
	 */
	async checkForNewerExports(): Promise<void> {
		for (const project of this.settings.projects) {
			const newer = await findNewerExport(project.jsonFilePath);
			if (newer) {
				this.newerExports.set(project.id, newer);
			} else {
				this.newerExports.delete(project.id);
			}
		}
	}

	// ========================================================================
	// Main Sync Logic
	// ========================================================================

	async runSync(): Promise<void> {
		const projects = this.settings.projects;

		if (projects.length === 0) {
			new Notice(
				"FMOD Sync: No projects configured. Add projects in settings."
			);
			return;
		}

		if (projects.length === 1) {
			// Single project - sync directly
			await this.syncSingleProject(projects[0]);
		} else {
			// Multiple projects - show picker
			new ProjectPickerModal(this.app, projects, async (item) => {
				if (item.type === "all") {
					await this.syncProjects(projects);
				} else if (item.project) {
					await this.syncSingleProject(item.project);
				}
			}).open();
		}
	}

	async syncProjects(projects: FMODProjectConfig[]): Promise<void> {
		const totalStats: SyncStats = {
			created: 0,
			updated: 0,
			moved: 0,
			skipped: 0,
			errors: 0,
		};

		let successCount = 0;
		let failCount = 0;

		for (const project of projects) {
			const stats = await this.syncSingleProject(project, true);
			if (stats) {
				successCount++;
				totalStats.created += stats.created;
				totalStats.updated += stats.updated;
				totalStats.moved += stats.moved;
				totalStats.skipped += stats.skipped;
				totalStats.errors += stats.errors;
			} else {
				failCount++;
			}
		}

		const summary = [
			`FMOD Sync Complete!`,
			`Projects: ${successCount} synced${failCount > 0 ? `, ${failCount} failed` : ""}`,
			`Created: ${totalStats.created}`,
			`Updated: ${totalStats.updated}`,
			`Moved: ${totalStats.moved}`,
			`Skipped: ${totalStats.skipped}`,
			totalStats.errors > 0 ? `Errors: ${totalStats.errors}` : "",
		]
			.filter(Boolean)
			.join("\n");

		new Notice(summary, 5000);

		// Refresh settings tab to show updated project metadata
		this.refreshSettingsTab();
	}

	async syncSingleProject(
		project: FMODProjectConfig,
		silent = false
	): Promise<SyncStats | null> {
		// Helper to get display name for messages
		const displayName = project.fmodProjectName || project.jsonFilePath.split("/").pop() || "Unknown";

		// Validate settings
		if (!project.jsonFilePath) {
			if (!silent) {
				new Notice(
					`FMOD Sync: No JSON file path configured. Check settings.`
				);
			}
			return null;
		}

		if (!project.outputFolder) {
			if (!silent) {
				new Notice(
					`FMOD Sync: No vault folder configured for "${displayName}". Check settings.`
				);
			}
			return null;
		}

		// Read and parse JSON
		let exportData: FMODExportData;
		try {
			exportData = await this.readJsonFile(project.jsonFilePath);
		} catch (error) {
			new Notice(
				`FMOD Sync: Failed to read JSON.\nPath: ${project.jsonFilePath}\n${error}`
			);
			return null;
		}

		// Validate JSON structure
		if (!exportData.events || !Array.isArray(exportData.events)) {
			new Notice(
				`FMOD Sync: Invalid JSON structure - missing events array.`
			);
			return null;
		}

		// Save all FMOD metadata from the JSON
		project.fmodProjectName = exportData.project_name;
		project.fmodProjectPath = exportData.project_path;
		project.fmodVersion = exportData.fmod_version;
		project.lastExportedAt = exportData.exported_at;
		await this.saveSettings();

		const projectName = project.fmodProjectName || displayName;

		if (!silent) {
			new Notice(
				`FMOD Sync: Processing ${exportData.events.length} events for "${projectName}"...`
			);
		}

		// Use project's output folder
		const outputPath = normalizePath(project.outputFolder);
		await this.ensureFolderExists(outputPath);

		// Build index of existing notes by GUID and filename
		const existingNotes = await this.scanExistingNotes(outputPath);
		const notesByGuid = new Map<string, { path: string; content: string }>();
		const notesByName = new Map<string, { path: string; content: string }>();

		for (const [notePath, content] of existingNotes) {
			const frontmatter = this.parseFrontmatter(content);
			const guid = frontmatter.properties["guid"] as string | undefined;
			const filename = notePath.split("/").pop()?.replace(".md", "") || "";

			if (guid) {
				notesByGuid.set(guid, { path: notePath, content });
			}
			notesByName.set(filename, { path: notePath, content });
		}

		// Process events
		const stats: SyncStats = {
			created: 0,
			updated: 0,
			moved: 0,
			skipped: 0,
			errors: 0,
		};

		for (const event of exportData.events) {
			try {
				await this.processEvent(
					event,
					outputPath,
					notesByGuid,
					notesByName,
					exportData.exported_at,
					projectName,
					stats
				);
			} catch (error) {
				console.error(
					`FMOD Sync: Error processing event ${event.name}:`,
					error
				);
				stats.errors++;
			}
		}

		// Show summary (only if not silent)
		if (!silent) {
			const summary = [
				`FMOD Sync Complete for "${projectName}"!`,
				`Created: ${stats.created}`,
				`Updated: ${stats.updated}`,
				`Moved: ${stats.moved}`,
				`Skipped: ${stats.skipped}`,
				stats.errors > 0 ? `Errors: ${stats.errors}` : "",
			]
				.filter(Boolean)
				.join("\n");

			new Notice(summary, 5000);

			// Refresh settings tab to show updated project metadata
			this.refreshSettingsTab();
		}

		return stats;
	}

	// ========================================================================
	// JSON Reading
	// ========================================================================

	async readJsonFile(filePath: string): Promise<FMODExportData> {
		// Handle path - could be absolute or relative to vault
		let resolvedPath = filePath;

		// If it looks like a relative path, try vault first
		if (!path.isAbsolute(filePath)) {
			const vaultFile = this.app.vault.getAbstractFileByPath(normalizePath(filePath));
			if (vaultFile instanceof TFile) {
				const content = await this.app.vault.read(vaultFile);
				return JSON.parse(content);
			}
		}

		// Read using Node.js fs (works for absolute paths outside vault)
		return new Promise((resolve, reject) => {
			fs.readFile(resolvedPath, "utf8", (err, data) => {
				if (err) {
					reject(new Error(`Failed to read file: ${err.message}`));
					return;
				}
				try {
					resolve(JSON.parse(data));
				} catch (parseErr) {
					reject(new Error(`Failed to parse JSON: ${parseErr}`));
				}
			});
		});
	}

	// ========================================================================
	// Folder Management
	// ========================================================================

	async ensureFolderExists(folderPath: string): Promise<void> {
		const folder = this.app.vault.getAbstractFileByPath(folderPath);
		if (folder instanceof TFolder) {
			return;
		}

		// Create folder and all parent folders
		const parts = folderPath.split("/");
		let currentPath = "";

		for (const part of parts) {
			currentPath = currentPath ? `${currentPath}/${part}` : part;
			const existing = this.app.vault.getAbstractFileByPath(currentPath);
			if (!existing) {
				await this.app.vault.createFolder(currentPath);
			}
		}
	}

	// ========================================================================
	// Note Scanning
	// ========================================================================

	async scanExistingNotes(basePath: string): Promise<Map<string, string>> {
		const notes = new Map<string, string>();
		const folder = this.app.vault.getAbstractFileByPath(basePath);

		if (!(folder instanceof TFolder)) {
			return notes;
		}

		const scanFolder = async (f: TFolder): Promise<void> => {
			for (const child of f.children) {
				if (child instanceof TFile && child.extension === "md") {
					const content = await this.app.vault.read(child);
					notes.set(child.path, content);
				} else if (child instanceof TFolder) {
					await scanFolder(child);
				}
			}
		};

		await scanFolder(folder);
		return notes;
	}

	// ========================================================================
	// Event Processing
	// ========================================================================

	async processEvent(
		event: FMODEvent,
		outputPath: string,
		notesByGuid: Map<string, { path: string; content: string }>,
		notesByName: Map<string, { path: string; content: string }>,
		exportedAt: string,
		projectName: string,
		stats: SyncStats
	): Promise<void> {
		const sanitizedName = this.sanitizeFilename(event.name);

		// Calculate target path (always mirrors FMOD folder structure)
		let targetPath: string;
		if (event.folder_path) {
			const sanitizedFolder = event.folder_path
				.split("/")
				.map((s) => this.sanitizeFilename(s))
				.join("/");
			targetPath = normalizePath(`${outputPath}/${sanitizedFolder}/${sanitizedName}.md`);
		} else {
			targetPath = normalizePath(`${outputPath}/${sanitizedName}.md`);
		}

		// Check for existing note by GUID
		const existingByGuid = notesByGuid.get(event.guid);
		// Check for existing note by filename
		const existingByName = notesByName.get(sanitizedName);

		let existingContent: string | null = null;
		let existingPath: string | null = null;
		let needsMove = false;

		if (existingByGuid) {
			// GUID match - this is a known event
			existingContent = existingByGuid.content;
			existingPath = existingByGuid.path;
			needsMove = existingPath !== targetPath;
		} else if (existingByName) {
			// Name match - might be a planned event or coincidence
			const frontmatter = this.parseFrontmatter(existingByName.content);
			const existingGuid = frontmatter.properties["guid"];

			if (!existingGuid) {
				// No GUID = planned event, link it
				existingContent = existingByName.content;
				existingPath = existingByName.path;
				needsMove = existingPath !== targetPath;
			} else if (existingGuid !== event.guid) {
				// Different GUID = different event with same name, skip
				stats.skipped++;
				return;
			}
		}

		// Ensure target folder exists
		const targetFolder = targetPath.substring(0, targetPath.lastIndexOf("/"));
		if (targetFolder) {
			await this.ensureFolderExists(targetFolder);
		}

		// Generate markdown content
		const markdown = this.generateMarkdown(
			event,
			existingContent,
			exportedAt,
			projectName
		);

		// Write or update file
		if (needsMove && existingPath) {
			// Delete old file
			const oldFile = this.app.vault.getAbstractFileByPath(existingPath);
			if (oldFile instanceof TFile) {
				await this.app.vault.delete(oldFile);
			}
			// Create at new location
			await this.app.vault.create(targetPath, markdown);
			stats.moved++;
		} else {
			const existingFile = this.app.vault.getAbstractFileByPath(targetPath);
			if (existingFile instanceof TFile) {
				await this.app.vault.modify(existingFile, markdown);
				stats.updated++;
			} else {
				await this.app.vault.create(targetPath, markdown);
				stats.created++;
			}
		}
	}

	// ========================================================================
	// Markdown Generation
	// ========================================================================

	generateMarkdown(
		event: FMODEvent,
		existingContent: string | null,
		exportedAt: string,
		projectName: string
	): string {
		// Parse existing frontmatter to preserve user properties
		const existing = existingContent
			? this.parseFrontmatter(existingContent)
			: { properties: {}, bodyStart: 0 };

		// Extract user-added sections
		const userSections = existingContent
			? this.extractUserSections(existingContent, existing.bodyStart)
			: {};

		// FMOD-managed properties that will be overwritten
		const fmodProperties = [
			"status",
			"guid",
			"project",
			"banks",
			"folder_path",
			"full_path",
			"loop_type",
			"space",
			"max_voices",
			"parameters",
			"last_synced",
		];

		// Build merged properties
		const mergedProps: Record<string, unknown> = {};

		// First, copy existing user properties
		for (const [key, value] of Object.entries(existing.properties)) {
			if (!fmodProperties.includes(key)) {
				mergedProps[key] = value;
			}
		}

		// Then add FMOD properties
		mergedProps["status"] = "exists";
		mergedProps["guid"] = event.guid;
		mergedProps["project"] = projectName;
		if (event.banks.length > 0) {
			mergedProps["banks"] = event.banks;
		}
		mergedProps["folder_path"] = event.folder_path;
		mergedProps["full_path"] = event.full_path;
		mergedProps["loop_type"] = event.loop_type;
		mergedProps["space"] = event.space;
		if (event.max_voices !== "" && event.max_voices !== undefined) {
			mergedProps["max_voices"] = event.max_voices;
		}
		if (event.parameters.length > 0) {
			mergedProps["parameters"] = event.parameters.map((p) => p.name);
		}
		mergedProps["last_synced"] = exportedAt;

		// Build YAML frontmatter
		let yaml = "---\n";

		// Output FMOD properties in order
		const orderedKeys = [
			"status",
			"guid",
			"project",
			"banks",
			"folder_path",
			"full_path",
			"loop_type",
			"space",
			"max_voices",
			"parameters",
			"last_synced",
		];

		const usedKeys = new Set<string>();

		for (const key of orderedKeys) {
			if (mergedProps[key] !== undefined && mergedProps[key] !== "") {
				yaml += this.formatYamlProperty(key, mergedProps[key]);
				usedKeys.add(key);
			}
		}

		// Output remaining user properties
		const remainingKeys = Object.keys(mergedProps)
			.filter((k) => !usedKeys.has(k))
			.sort();

		for (const key of remainingKeys) {
			yaml += this.formatYamlProperty(key, mergedProps[key]);
		}

		yaml += "---\n\n";

		// Build markdown body
		let md = `# ${event.name}\n\n`;

		// Parameters section
		if (event.parameters.length > 0) {
			md += "## Parameters\n";
			md += "| Name | Type | Min | Max | Initial |\n";
			md += "|------|------|-----|-----|--------|\n";
			for (const param of event.parameters) {
				md += `| ${param.name} | ${param.type} | ${param.min} | ${param.max} | ${param.initial} |\n`;
			}
			md += "\n";
		}

		// Notes section
		md += "## Notes\n";
		md += (event.notes || "") + "\n\n";

		// User Properties section
		if (event.user_properties.length > 0) {
			md += "## User Properties\n";
			for (const prop of event.user_properties) {
				md += `- ${prop.name}`;
				if (prop.type) md += ` (${prop.type})`;
				if (prop.value !== "" && prop.value !== undefined) md += ` = ${prop.value}`;
				md += "\n";
			}
			md += "\n";
		}

		// Preserved user sections
		for (const [sectionName, content] of Object.entries(userSections)) {
			md += `## ${sectionName}\n`;
			md += content + "\n\n";
		}

		return yaml + md;
	}

	formatYamlProperty(key: string, value: unknown): string {
		if (Array.isArray(value)) {
			let out = `${key}:\n`;
			for (const item of value) {
				out += `  - ${this.yamlEscape(String(item))}\n`;
			}
			return out;
		}
		return `${key}: ${this.yamlEscape(String(value))}\n`;
	}

	yamlEscape(val: string): string {
		if (val === null || val === undefined) return '""';
		const s = String(val);
		// Quote if contains special YAML characters
		if (
			s.includes(":") ||
			s.includes("#") ||
			s.includes("'") ||
			s.includes('"') ||
			s.includes("{") ||
			s.includes("}") ||
			s.includes("[") ||
			s.includes("]") ||
			s.includes("&") ||
			s.includes("*") ||
			s.includes("!") ||
			s.includes("|") ||
			s.includes(">") ||
			s.includes("%") ||
			s.includes("@") ||
			s.includes("`") ||
			s.trim() !== s ||
			s === ""
		) {
			return `"${s.replace(/\\/g, "\\\\").replace(/"/g, '\\"')}"`;
		}
		return s;
	}

	// ========================================================================
	// Frontmatter Parsing
	// ========================================================================

	parseFrontmatter(content: string): ParsedFrontmatter {
		const result: ParsedFrontmatter = { properties: {}, bodyStart: 0 };

		if (!content || !content.startsWith("---")) {
			return result;
		}

		const endMatch = content.indexOf("\n---", 3);
		if (endMatch < 0) {
			return result;
		}

		const yamlBlock = content.substring(4, endMatch);
		result.bodyStart = endMatch + 4; // Skip past closing ---\n

		const lines = yamlBlock.split("\n");
		let currentKey: string | null = null;
		let arrayValues: string[] = [];
		let inArray = false;

		for (const line of lines) {
			const trimmed = line.trim();

			if (trimmed === "" || trimmed.startsWith("#")) continue;

			// Check for array item
			if (trimmed.startsWith("- ")) {
				if (inArray && currentKey) {
					arrayValues.push(trimmed.substring(2).trim());
				}
				continue;
			}

			// Check for key: value
			const colonIdx = trimmed.indexOf(":");
			if (colonIdx > 0) {
				// Save previous array if we had one
				if (inArray && currentKey && arrayValues.length > 0) {
					result.properties[currentKey] = arrayValues;
				}

				currentKey = trimmed.substring(0, colonIdx).trim();
				let val = trimmed.substring(colonIdx + 1).trim();

				if (val === "" || val === "|" || val === ">") {
					// Could be array or multiline
					inArray = true;
					arrayValues = [];
				} else {
					inArray = false;
					// Remove quotes if present
					if (
						(val.startsWith('"') && val.endsWith('"') && val.length > 1) ||
						(val.startsWith("'") && val.endsWith("'") && val.length > 1)
					) {
						val = val.substring(1, val.length - 1);
					}
					result.properties[currentKey] = val;
				}
			}
		}

		// Save final array if we had one
		if (inArray && currentKey && arrayValues.length > 0) {
			result.properties[currentKey] = arrayValues;
		}

		return result;
	}

	extractUserSections(
		content: string,
		bodyStart: number
	): Record<string, string> {
		const body = content.substring(bodyStart);
		const sections: Record<string, string> = {};

		const lines = body.split("\n");
		let currentSection: string | null = null;
		let currentContent: string[] = [];
		const managedSections = ["parameters", "notes", "user properties"];

		for (const line of lines) {
			if (line.startsWith("## ")) {
				// Save previous section if user-added
				if (
					currentSection &&
					!managedSections.includes(currentSection.toLowerCase())
				) {
					sections[currentSection] = currentContent.join("\n").trim();
				}
				currentSection = line.substring(3).trim();
				currentContent = [];
			} else if (currentSection) {
				currentContent.push(line);
			}
		}

		// Save final section if user-added
		if (
			currentSection &&
			!managedSections.includes(currentSection.toLowerCase())
		) {
			sections[currentSection] = currentContent.join("\n").trim();
		}

		return sections;
	}

	// ========================================================================
	// Utilities
	// ========================================================================

	sanitizeFilename(name: string): string {
		let result = name;
		const badChars = ["<", ">", ":", "/", "|", "?", "*", '"', "\\"];
		for (const c of badChars) {
			result = result.split(c).join("-");
		}
		result = result.replace(/\s+/g, "_");
		result = result.replace(/-+/g, "-");
		result = result.replace(/^-|-$/g, "");
		return result;
	}
}

// ============================================================================
// Project Picker Modal
// ============================================================================

class ProjectPickerModal extends FuzzySuggestModal<ProjectPickerItem> {
	private projects: FMODProjectConfig[];
	private onChoose: (item: ProjectPickerItem) => void;

	constructor(
		app: App,
		projects: FMODProjectConfig[],
		onChoose: (item: ProjectPickerItem) => void
	) {
		super(app);
		this.projects = projects;
		this.onChoose = onChoose;
		this.setPlaceholder("Select a project to sync...");
	}

	getItems(): ProjectPickerItem[] {
		const items: ProjectPickerItem[] = [{ type: "all" }];
		for (const project of this.projects) {
			items.push({ type: "single", project });
		}
		return items;
	}

	getItemText(item: ProjectPickerItem): string {
		if (item.type === "all") {
			return `Sync All (${this.projects.length} projects)`;
		}
		return item.project?.fmodProjectName || item.project?.jsonFilePath.split("/").pop() || "Unknown";
	}

	onChooseItem(item: ProjectPickerItem): void {
		this.onChoose(item);
	}
}

// ============================================================================
// Folder Picker Modal
// ============================================================================

class FolderPickerModal extends FuzzySuggestModal<TFolder> {
	private onChoose: (folder: TFolder) => void;
	private folders: TFolder[];

	constructor(app: App, onChoose: (folder: TFolder) => void) {
		super(app);
		this.onChoose = onChoose;
		this.folders = this.getAllFolders();
		this.setPlaceholder("Select a folder...");
	}

	private getAllFolders(): TFolder[] {
		const folders: TFolder[] = [];
		const rootFolder = this.app.vault.getRoot();

		const collectFolders = (folder: TFolder): void => {
			folders.push(folder);
			for (const child of folder.children) {
				if (child instanceof TFolder) {
					collectFolders(child);
				}
			}
		};

		collectFolders(rootFolder);
		return folders.sort((a, b) => a.path.localeCompare(b.path));
	}

	getItems(): TFolder[] {
		return this.folders;
	}

	getItemText(folder: TFolder): string {
		return folder.path || "/";
	}

	onChooseItem(folder: TFolder): void {
		this.onChoose(folder);
	}
}

// ============================================================================
// Settings Tab
// ============================================================================

class FMODSyncSettingTab extends PluginSettingTab {
	plugin: FMODSyncPlugin;
	private pollingIntervalId: ReturnType<typeof setInterval> | null = null;

	constructor(app: App, plugin: FMODSyncPlugin) {
		super(app, plugin);
		this.plugin = plugin;
	}

	display(): void {
		const { containerEl } = this;
		containerEl.empty();
		containerEl.addClass("fmod-sync-settings");

		containerEl.createEl("h2", { text: "FMOD Sync Settings" });

		// FMOD Installations section
		containerEl.createEl("h3", { text: "FMOD Studio Installations" });

		const installationsContainer = containerEl.createDiv({
			cls: "fmod-installations-container",
		});

		this.renderInstallations(installationsContainer);

		// Add installation button
		new Setting(containerEl).addButton((button) =>
			button
				.setButtonText("Add Installation")
				.onClick(async () => {
					const platform = process.platform;
					const filters = platform === "darwin"
						? [{ name: "Applications", extensions: ["app"] }]
						: [{ name: "Executables", extensions: ["exe"] }];

					const result = await remote.dialog.showOpenDialog({
						title: "Select FMOD Studio Application",
						properties: platform === "darwin" ? ["openDirectory", "treatPackageAsDirectory"] : ["openFile"],
						filters: platform === "win32" ? filters : undefined,
					});

					if (!result.canceled && result.filePaths.length > 0) {
						let selectedPath = result.filePaths[0];

						// On macOS, ensure we have the .app bundle path
						if (platform === "darwin" && !selectedPath.endsWith(".app")) {
							// User might have selected something inside the bundle, try to find .app
							const appMatch = selectedPath.match(/(.+\.app)/);
							if (appMatch) {
								selectedPath = appMatch[1];
							}
						}

						// Detect version
						const version = await detectFmodVersion(selectedPath);

						const newInstallation: FMODInstallation = {
							id: this.generateId(),
							path: selectedPath,
							version: version,
						};

						this.plugin.settings.fmodInstallations.push(newInstallation);
						await this.plugin.saveSettings();
						this.display();

						if (version === "Unknown") {
							new Notice("Added FMOD installation, but version could not be detected.");
						} else {
							new Notice(`Added FMOD Studio ${version}`);
						}
					}
				})
		);

		// Projects section
		containerEl.createEl("h3", { text: "Projects" });

		const projectsContainer = containerEl.createDiv({
			cls: "fmod-projects-container",
		});

		this.renderProjects(projectsContainer);

		// Add project button
		new Setting(containerEl).addButton((button) =>
			button
				.setButtonText("Add Project")
				.setCta()
				.onClick(async () => {
					const newProject: FMODProjectConfig = {
						id: this.generateId(),
						jsonFilePath: "",
						outputFolder: "",
					};
					this.plugin.settings.projects.push(newProject);
					await this.plugin.saveSettings();
					this.display();
				})
		);

		// Start polling for newer exports AFTER initial render is complete
		this.startPolling();
	}

	renderInstallations(container: HTMLElement): void {
		container.empty();

		if (this.plugin.settings.fmodInstallations.length === 0) {
			container.createEl("p", {
				text: "No FMOD Studio installations configured. Click 'Add Installation' to add one.",
				cls: "fmod-no-installations",
			});
			return;
		}

		for (const installation of this.plugin.settings.fmodInstallations) {
			this.renderInstallationItem(container, installation);
		}
	}

	renderInstallationItem(container: HTMLElement, installation: FMODInstallation): void {
		const item = container.createDiv({ cls: "fmod-installation-item" });

		// Version badge
		item.createEl("span", {
			cls: "fmod-installation-version",
			text: installation.version,
		});

		// Check if companion script is installed
		const hasScript = checkCompanionScriptExists(installation.path);

		// Script status indicator
		if (hasScript) {
			const statusEl = item.createEl("span", {
				cls: "fmod-script-status fmod-script-installed",
				attr: { "aria-label": "Export script installed" },
			});
			// Checkmark icon
			statusEl.innerHTML = `<svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M20 6 9 17l-5-5"/></svg>`;
		} else {
			// Install script button
			const installBtn = item.createEl("button", {
				cls: "fmod-install-script-btn",
				text: "Install Export Script",
				attr: { "aria-label": "Install the FMOD export script for this version" },
			});
			installBtn.addEventListener("click", async () => {
				installBtn.disabled = true;
				installBtn.textContent = "Installing...";

				const result = await installCompanionScript(installation.path);

				if (result.success) {
					new Notice("Export script installed. Restart FMOD Studio to use it.");
					this.display(); // Refresh to show installed status
				} else {
					new Notice(`Failed to install script: ${result.error}`);
					installBtn.disabled = false;
					installBtn.textContent = "Install Export Script";
				}
			});
		}

		// Path
		item.createEl("span", {
			cls: "fmod-installation-path",
			text: installation.path,
		});

		// Delete button
		const deleteBtn = item.createEl("button", {
			cls: "fmod-action-btn fmod-action-btn-danger fmod-installation-delete",
			attr: { "aria-label": "Remove installation" },
		});
		deleteBtn.innerHTML = `<svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M3 6h18"/><path d="M19 6v14c0 1-1 2-2 2H7c-1 0-2-1-2-2V6"/><path d="M8 6V4c0-1 1-2 2-2h4c1 0 2 1 2 2v2"/></svg>`;
		deleteBtn.addEventListener("click", async () => {
			const index = this.plugin.settings.fmodInstallations.findIndex(
				(i) => i.id === installation.id
			);
			if (index >= 0) {
				this.plugin.settings.fmodInstallations.splice(index, 1);
				await this.plugin.saveSettings();
				this.display();
			}
		});
	}

	renderProjects(container: HTMLElement): void {
		container.empty();

		if (this.plugin.settings.projects.length === 0) {
			container.createEl("p", {
				text: "No projects configured. Click 'Add Project' to get started.",
				cls: "fmod-no-projects",
			});
			return;
		}

		for (const project of this.plugin.settings.projects) {
			this.renderProjectCard(container, project);
		}
	}

	renderProjectCard(container: HTMLElement, project: FMODProjectConfig): void {
		const card = container.createDiv({ cls: "fmod-project-card" });

		// Compact header row
		const header = card.createDiv({ cls: "fmod-project-header" });

		// Left side: project info
		const info = header.createDiv({ cls: "fmod-project-info" });

		// Parse JSON filename to extract project name and timestamp
		// Format: ProjectName_YYYY-MM-DD_HHMMSS.json
		const parseJsonFilename = (filePath: string): { name: string; date: Date | null } => {
			const filename = filePath.split("/").pop()?.replace(".json", "") || "";
			// Match pattern: name_YYYY-MM-DD_HHMMSS
			const match = filename.match(/^(.+)_(\d{4}-\d{2}-\d{2})_(\d{6})$/);
			if (match) {
				const [, name, dateStr, timeStr] = match;
				// Parse: YYYY-MM-DD and HHMMSS
				const year = parseInt(dateStr.substring(0, 4));
				const month = parseInt(dateStr.substring(5, 7)) - 1;
				const day = parseInt(dateStr.substring(8, 10));
				const hour = parseInt(timeStr.substring(0, 2));
				const minute = parseInt(timeStr.substring(2, 4));
				const second = parseInt(timeStr.substring(4, 6));
				return { name, date: new Date(year, month, day, hour, minute, second) };
			}
			return { name: filename, date: null };
		};

		// Get display name - prefer FMOD project name, fallback to parsed filename
		let displayName = "New Project";
		let fileDate: Date | null = null;

		if (project.fmodProjectName) {
			displayName = project.fmodProjectName;
		} else if (project.jsonFilePath) {
			const parsed = parseJsonFilename(project.jsonFilePath);
			displayName = parsed.name || "New Project";
			fileDate = parsed.date;
		}

		const nameRow = info.createDiv({ cls: "fmod-project-name-row" });
		nameRow.createEl("span", { cls: "fmod-project-name", text: displayName });

		// Check for newer export and show badge
		const newerExport = this.plugin.newerExports.get(project.id);
		if (newerExport) {
			const badge = nameRow.createEl("span", {
				cls: "fmod-new-export-badge",
				text: "New export available",
			});
			// Format the newer export date for tooltip
			const formatted = newerExport.exportDate.toLocaleString(undefined, {
				year: "numeric",
				month: "short",
				day: "numeric",
				hour: "2-digit",
				minute: "2-digit"
			});
			badge.setAttribute("aria-label", `Newer export from ${formatted}`);
		}

		// Metadata row
		if (project.fmodProjectName) {
			// Synced project - show export date and FMOD version
			const metaInfo = [];
			if (project.lastExportedAt) {
				const date = new Date(project.lastExportedAt);
				const formatted = date.toLocaleString(undefined, {
					year: "numeric",
					month: "short",
					day: "numeric",
					hour: "2-digit",
					minute: "2-digit"
				});
				metaInfo.push(`Exported: ${formatted}`);
			}
			if (project.fmodVersion) {
				metaInfo.push(`FMOD ${project.fmodVersion}`);
			}
			if (metaInfo.length > 0) {
				info.createEl("div", { cls: "fmod-project-meta", text: metaInfo.join(" | ") });
			}

			// FMOD project path
			if (project.fmodProjectPath) {
				info.createEl("div", { cls: "fmod-project-path", text: project.fmodProjectPath });
			}
		} else if (project.jsonFilePath) {
			// Not synced yet but has JSON file - show date from filename
			if (fileDate) {
				const formatted = fileDate.toLocaleString(undefined, {
					year: "numeric",
					month: "short",
					day: "numeric",
					hour: "2-digit",
					minute: "2-digit"
				});
				info.createEl("div", { cls: "fmod-project-meta", text: `Exported: ${formatted}` });
			}
		} else {
			// Not configured at all
			info.createEl("div", { cls: "fmod-project-path fmod-not-configured", text: "Not configured - select JSON file and vault folder" });
		}

		// Right side: action buttons
		const actions = header.createDiv({ cls: "fmod-project-actions" });

		// Version dropdown (only show if project has been synced and has a version)
		if (project.fmodProjectPath) {
			const versionContainer = actions.createDiv({ cls: "fmod-version-container" });

			// Find matching installation for this project's version
			const installations = this.plugin.settings.fmodInstallations;
			const projectVersion = project.fmodVersion?.trim();
			const matchingInstallation = projectVersion
				? installations.find(i => i.version?.trim() === projectVersion)
				: null;

			// Get selected installation
			// Priority: matching version > user selection > first available
			let selectedInstallation: FMODInstallation | undefined;

			if (matchingInstallation) {
				// Always prefer the matching version
				selectedInstallation = matchingInstallation;
				// Update saved selection if it doesn't match
				if (project.selectedFmodInstallationId !== matchingInstallation.id) {
					project.selectedFmodInstallationId = matchingInstallation.id;
					this.plugin.saveSettings();
				}
			} else if (project.selectedFmodInstallationId) {
				// Use user's previous selection if no match
				selectedInstallation = installations.find(i => i.id === project.selectedFmodInstallationId);
			}

			// Show warning if:
			// 1. Project version doesn't match any installation, OR
			// 2. User selected a different version than the project's version
			const versionMissing = projectVersion && !matchingInstallation && installations.length > 0;
			const versionOverridden = projectVersion && selectedInstallation && selectedInstallation.version !== projectVersion;
			const hasVersionMismatch = versionMissing || versionOverridden;

			if (hasVersionMismatch) {
				const warningMessage = versionMissing
					? `Project requires FMOD ${projectVersion} (not installed)`
					: `Project created with FMOD ${projectVersion}, using ${selectedInstallation?.version}`;
				const warningIcon = versionContainer.createEl("span", {
					cls: "fmod-version-warning",
					attr: { "aria-label": warningMessage },
				});
				warningIcon.innerHTML = `<svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="m21.73 18-8-14a2 2 0 0 0-3.48 0l-8 14A2 2 0 0 0 4 21h16a2 2 0 0 0 1.73-3Z"/><path d="M12 9v4"/><path d="M12 17h.01"/></svg>`;
			}

			// Version dropdown
			if (installations.length > 0) {
				const select = versionContainer.createEl("select", { cls: "fmod-version-select" });

				// Show "(missing)" placeholder only if project version doesn't match any installation
				if (projectVersion && !matchingInstallation) {
					const placeholderOpt = select.createEl("option", {
						text: `${projectVersion} (missing)`,
						value: "",
					});
					placeholderOpt.disabled = true;
					placeholderOpt.selected = !selectedInstallation;
				}

				// Add all installations
				for (const inst of installations) {
					const opt = select.createEl("option", {
						text: inst.version,
						value: inst.id,
					});
					if (selectedInstallation && inst.id === selectedInstallation.id) {
						opt.selected = true;
					}
				}

				select.addEventListener("change", async () => {
					project.selectedFmodInstallationId = select.value || undefined;
					await this.plugin.saveSettings();
					// Refresh to update warning icon
					this.display();
				});
			}
		}

		// Open FMOD button (only show if project path is known)
		if (project.fmodProjectPath) {
			const openFmodBtn = actions.createEl("button", {
				cls: "fmod-action-btn",
				attr: { "aria-label": "Open FMOD Studio project" },
			});
			// FMOD logo icon - cyan color
			openFmodBtn.innerHTML = `<svg xmlns="http://www.w3.org/2000/svg" width="18" height="18" viewBox="0 0 512 512"><path fill="#6ECEF4" d="M 235.00 455.60 C178.64,446.91 128.15,416.19 103.89,375.82 C97.48,365.16 92.68,352.51 93.38,348.13 C94.34,342.09 99.87,338.00 107.08,338.00 C110.58,338.00 111.81,338.58 114.63,341.57 C116.48,343.54 118.00,345.77 118.00,346.54 C118.00,349.07 127.45,366.43 132.16,372.56 C155.41,402.78 200.08,426.38 242.88,431.05 C307.51,438.10 375.01,393.86 391.73,333.50 C394.25,324.42 394.50,321.99 394.48,307.00 C394.46,288.57 393.03,281.43 386.57,267.44 C375.72,243.96 353.61,222.46 327.49,209.99 C312.26,202.73 291.93,196.95 276.25,195.44 L 269.00 194.74 L 269.00 343.28 L 250.75 342.70 C236.81,342.26 229.66,341.53 220.47,339.60 C196.80,334.63 172.26,324.52 153.00,311.79 C140.06,303.24 121.23,284.50 113.23,272.22 C84.37,227.89 86.47,173.31 118.83,126.50 C127.20,114.40 147.34,94.85 160.41,86.13 C208.46,54.09 260.10,47.04 314.50,65.09 C349.20,76.60 381.76,98.56 400.87,123.33 C411.56,137.19 421.77,159.06 420.57,165.50 C419.31,172.21 411.05,177.16 404.65,175.05 C399.96,173.50 397.70,170.68 394.05,161.84 C379.77,127.24 342.20,98.03 296.50,86.00 C279.94,81.64 271.14,80.70 252.25,81.29 C228.58,82.02 215.31,85.28 194.50,95.47 C178.20,103.46 168.87,110.14 155.38,123.50 C129.65,148.98 118.59,173.71 118.71,205.50 C118.79,225.14 122.38,237.97 132.68,255.35 C138.64,265.43 157.14,284.39 167.92,291.47 C187.79,304.54 215.57,314.70 236.64,316.61 L 244.00 317.28 L 244.00 169.00 L 254.75 169.03 C334.67,169.22 401.89,214.64 417.11,278.73 C435.33,355.45 376.41,435.13 287.50,454.01 C274.77,456.72 247.57,457.54 235.00,455.60 Z"/></svg>`;
			openFmodBtn.addEventListener("click", async () => {
				const projectPath = project.fmodProjectPath!;

				// Get the selected installation
				const installations = this.plugin.settings.fmodInstallations;
				const selectedId = project.selectedFmodInstallationId;
				const projectVersion = project.fmodVersion;

				// Find installation: first try selected, then try matching version
				let installation: FMODInstallation | undefined;
				if (selectedId) {
					installation = installations.find(i => i.id === selectedId);
				}
				if (!installation && projectVersion) {
					installation = installations.find(i => i.version === projectVersion);
				}
				if (!installation && installations.length > 0) {
					installation = installations[0]; // Fallback to first available
				}

				if (installation) {
					// Launch with specific FMOD version
					const platform = process.platform;
					try {
						if (platform === "darwin") {
							// macOS: use open -a
							spawn("open", ["-a", installation.path, projectPath], { detached: true, stdio: "ignore" });
						} else if (platform === "win32") {
							// Windows: launch exe directly with project as argument
							spawn(installation.path, [projectPath], { detached: true, stdio: "ignore" });
						}
					} catch (error) {
						new Notice(`Failed to open FMOD project: ${error}`);
					}
				} else {
					// No installations configured, fall back to default shell open
					const result = await shell.openPath(projectPath);
					if (result) {
						new Notice(`Failed to open FMOD project: ${result}`);
					}
				}
			});
		}

		// Sync button - highlight if newer export is available
		const hasNewerExport = this.plugin.newerExports.has(project.id);
		const syncBtn = actions.createEl("button", {
			cls: hasNewerExport ? "fmod-action-btn fmod-action-btn-update" : "fmod-action-btn",
			attr: { "aria-label": hasNewerExport ? "Sync with newer export" : "Sync this project" },
		});
		syncBtn.innerHTML = `<svg xmlns="http://www.w3.org/2000/svg" width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M21.5 2v6h-6M2.5 22v-6h6M2 11.5a10 10 0 0 1 18.8-4.3M22 12.5a10 10 0 0 1-18.8 4.2"/></svg>`;
		syncBtn.addEventListener("click", async () => {
			// If newer export available, update the path first
			const newerExportForSync = this.plugin.newerExports.get(project.id);
			if (newerExportForSync) {
				project.jsonFilePath = newerExportForSync.filePath;
				await this.plugin.saveSettings();
				// Clear the newer export indicator
				this.plugin.newerExports.delete(project.id);
			}
			await this.plugin.syncSingleProject(project);
		});

		// Edit button
		const editBtn = actions.createEl("button", {
			cls: "fmod-action-btn",
			attr: { "aria-label": "Edit project settings" },
		});
		editBtn.innerHTML = `<svg xmlns="http://www.w3.org/2000/svg" width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M17 3a2.85 2.83 0 1 1 4 4L7.5 20.5 2 22l1.5-5.5Z"/><path d="m15 5 4 4"/></svg>`;

		// Delete button
		const deleteBtn = actions.createEl("button", {
			cls: "fmod-action-btn fmod-action-btn-danger",
			attr: { "aria-label": "Delete project" },
		});
		deleteBtn.innerHTML = `<svg xmlns="http://www.w3.org/2000/svg" width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M3 6h18"/><path d="M19 6v14c0 1-1 2-2 2H7c-1 0-2-1-2-2V6"/><path d="M8 6V4c0-1 1-2 2-2h4c1 0 2 1 2 2v2"/><line x1="10" x2="10" y1="11" y2="17"/><line x1="14" x2="14" y1="11" y2="17"/></svg>`;
		deleteBtn.addEventListener("click", async () => {
			const index = this.plugin.settings.projects.findIndex(
				(p) => p.id === project.id
			);
			if (index >= 0) {
				this.plugin.settings.projects.splice(index, 1);
				await this.plugin.saveSettings();
				this.display();
			}
		});

		// Expandable settings section
		const settingsSection = card.createDiv({ cls: "fmod-project-settings" });
		settingsSection.style.display = "none";

		// Toggle settings visibility
		editBtn.addEventListener("click", () => {
			const isVisible = settingsSection.style.display !== "none";
			settingsSection.style.display = isVisible ? "none" : "block";
			editBtn.classList.toggle("fmod-action-btn-active", !isVisible);
		});

		// Vault folder
		const outputSetting = new Setting(settingsSection)
			.setName("Vault folder")
			.setDesc("Folder in your vault where event notes will be created.")
			.addText((text) =>
				text
					.setPlaceholder("FMOD Events/MyProject")
					.setValue(project.outputFolder)
					.onChange(async (value) => {
						project.outputFolder = value;
						await this.plugin.saveSettings();
						this.updateFolderStatus(statusEl, value);
						// Update header path info
						this.updateProjectPathInfo(card, project);
					})
			)
			.addButton((button) =>
				button
					.setButtonText("Browse")
					.onClick(() => {
						new FolderPickerModal(this.app, async (folder) => {
							project.outputFolder = folder.path || "";
							await this.plugin.saveSettings();
							this.display();
						}).open();
					})
			);

		// Add status indicator below the input field
		const statusEl = outputSetting.controlEl.createDiv({ cls: "fmod-folder-status" });
		this.updateFolderStatus(statusEl, project.outputFolder);

		// JSON file path
		const jsonSetting = new Setting(settingsSection)
			.setName("JSON file path")
			.setDesc(
				"Path to the obsidian-sync.json file exported from FMOD Studio."
			);

		jsonSetting.addText((text) =>
			text
				.setPlaceholder("/path/to/obsidian-sync.json")
				.setValue(project.jsonFilePath)
				.onChange(async (value) => {
					project.jsonFilePath = value;
					await this.plugin.saveSettings();
					// Update header path info
					this.updateProjectPathInfo(card, project);
				})
		);

		jsonSetting.addButton((button) =>
			button
				.setButtonText("Browse")
				.onClick(async () => {
					const result = await remote.dialog.showOpenDialog({
						title: "Select FMOD Export JSON File",
						properties: ["openFile"],
						filters: [
							{ name: "JSON Files", extensions: ["json"] },
							{ name: "All Files", extensions: ["*"] },
						],
					});

					if (!result.canceled && result.filePaths.length > 0) {
						project.jsonFilePath = result.filePaths[0];
						await this.plugin.saveSettings();
						this.display();
					}
				})
		);
	}

	updateProjectPathInfo(card: HTMLElement, project: FMODProjectConfig): void {
		const pathEl = card.querySelector(".fmod-project-path");
		if (!pathEl) return;

		const pathInfo = [];
		if (project.outputFolder) {
			pathInfo.push(`Vault: ${project.outputFolder}`);
		}
		if (project.jsonFilePath) {
			const jsonName = project.jsonFilePath.split("/").pop() || project.jsonFilePath;
			pathInfo.push(`JSON: ${jsonName}`);
		}
		pathEl.textContent = pathInfo.length > 0 ? pathInfo.join(" | ") : "Not configured";
	}

	updateFolderStatus(statusEl: HTMLElement, folderPath: string): void {
		statusEl.empty();

		if (!folderPath) {
			statusEl.removeClass("fmod-status-ok", "fmod-status-error");
			statusEl.addClass("fmod-status-warning");
			statusEl.setText("No folder specified");
			return;
		}

		const existing = this.app.vault.getAbstractFileByPath(folderPath);

		if (existing instanceof TFolder) {
			statusEl.removeClass("fmod-status-warning", "fmod-status-error");
			statusEl.addClass("fmod-status-ok");
			statusEl.setText("✓ Folder exists");
		} else if (existing instanceof TFile) {
			statusEl.removeClass("fmod-status-ok", "fmod-status-warning");
			statusEl.addClass("fmod-status-error");
			statusEl.setText("✗ Path is a file, not a folder");
		} else {
			statusEl.removeClass("fmod-status-ok", "fmod-status-error");
			statusEl.addClass("fmod-status-warning");
			statusEl.setText("ℹ Folder will be created on sync");
		}
	}

	hide(): void {
		this.stopPolling();
	}

	private startPolling(): void {
		// Stop any existing polling
		this.stopPolling();

		// Check immediately
		this.checkAndRefresh();

		// Poll every 30 seconds
		this.pollingIntervalId = setInterval(() => {
			this.checkAndRefresh();
		}, 30000);
	}

	private stopPolling(): void {
		if (this.pollingIntervalId) {
			clearInterval(this.pollingIntervalId);
			this.pollingIntervalId = null;
		}
	}

	private async checkAndRefresh(): Promise<void> {
		const previousCount = this.plugin.newerExports.size;
		await this.plugin.checkForNewerExports();
		const newCount = this.plugin.newerExports.size;

		// Only refresh if the count changed (avoids unnecessary re-renders)
		if (previousCount !== newCount) {
			// Re-render just the projects section
			const projectsContainer = this.containerEl.querySelector(".fmod-projects-container");
			if (projectsContainer instanceof HTMLElement) {
				this.renderProjects(projectsContainer);
			}
		}
	}

	generateId(): string {
		return Date.now().toString(36) + Math.random().toString(36).substr(2);
	}
}
