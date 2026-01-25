import {
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
import * as fs from "fs";
import * as path from "path";

// Electron remote for native file dialogs
// eslint-disable-next-line @typescript-eslint/no-var-requires
const { remote } = require("electron");

// ============================================================================
// Types
// ============================================================================

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
	project_name: string;
	project_path: string;
	event_count: number;
	events: FMODEvent[];
}

interface FMODProjectConfig {
	id: string;
	name: string;
	jsonFilePath: string;
	outputFolder: string;
	enabled: boolean;
}

interface FMODSyncSettings {
	projects: FMODProjectConfig[];
	mirrorFolders: boolean;
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

// ============================================================================
// Default Settings
// ============================================================================

const DEFAULT_SETTINGS: FMODSyncSettings = {
	projects: [],
	mirrorFolders: true,
};

interface ProjectPickerItem {
	type: "all" | "single";
	project?: FMODProjectConfig;
}

// ============================================================================
// Main Plugin
// ============================================================================

export default class FMODSyncPlugin extends Plugin {
	settings: FMODSyncSettings = DEFAULT_SETTINGS;

	async onload(): Promise<void> {
		await this.loadSettings();

		// Add ribbon icon
		this.addRibbonIcon("audio-file", "FMOD Sync", () => {
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
		this.addSettingTab(new FMODSyncSettingTab(this.app, this));
	}

	async loadSettings(): Promise<void> {
		this.settings = Object.assign({}, DEFAULT_SETTINGS, await this.loadData());
	}

	async saveSettings(): Promise<void> {
		await this.saveData(this.settings);
	}

	// ========================================================================
	// Main Sync Logic
	// ========================================================================

	async runSync(): Promise<void> {
		const enabledProjects = this.settings.projects.filter((p) => p.enabled);

		if (enabledProjects.length === 0) {
			new Notice(
				"FMOD Sync: No projects configured. Add projects in settings."
			);
			return;
		}

		if (enabledProjects.length === 1) {
			// Single project - sync directly
			await this.syncSingleProject(enabledProjects[0]);
		} else {
			// Multiple projects - show picker
			new ProjectPickerModal(this.app, enabledProjects, async (item) => {
				if (item.type === "all") {
					await this.syncProjects(enabledProjects);
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
	}

	async syncSingleProject(
		project: FMODProjectConfig,
		silent = false
	): Promise<SyncStats | null> {
		const { mirrorFolders } = this.settings;

		// Validate settings
		if (!project.jsonFilePath) {
			if (!silent) {
				new Notice(
					`FMOD Sync: No JSON file path configured for "${project.name}". Check settings.`
				);
			}
			return null;
		}

		if (!project.outputFolder) {
			if (!silent) {
				new Notice(
					`FMOD Sync: No output folder configured for "${project.name}". Check settings.`
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
				`FMOD Sync: Failed to read JSON for "${project.name}".\nPath: ${project.jsonFilePath}\n${error}`
			);
			return null;
		}

		// Validate JSON structure
		if (!exportData.events || !Array.isArray(exportData.events)) {
			new Notice(
				`FMOD Sync: Invalid JSON structure for "${project.name}" - missing events array.`
			);
			return null;
		}

		if (!silent) {
			new Notice(
				`FMOD Sync: Processing ${exportData.events.length} events for "${project.name}"...`
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
					mirrorFolders,
					notesByGuid,
					notesByName,
					exportData.exported_at,
					project.name,
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
				`FMOD Sync Complete for "${project.name}"!`,
				`Created: ${stats.created}`,
				`Updated: ${stats.updated}`,
				`Moved: ${stats.moved}`,
				`Skipped: ${stats.skipped}`,
				stats.errors > 0 ? `Errors: ${stats.errors}` : "",
			]
				.filter(Boolean)
				.join("\n");

			new Notice(summary, 5000);
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
		mirrorFolders: boolean,
		notesByGuid: Map<string, { path: string; content: string }>,
		notesByName: Map<string, { path: string; content: string }>,
		exportedAt: string,
		projectName: string,
		stats: SyncStats
	): Promise<void> {
		const sanitizedName = this.sanitizeFilename(event.name);

		// Calculate target path
		let targetPath: string;
		if (mirrorFolders && event.folder_path) {
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
		return item.project?.name || "";
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

	constructor(app: App, plugin: FMODSyncPlugin) {
		super(app, plugin);
		this.plugin = plugin;
	}

	display(): void {
		const { containerEl } = this;
		containerEl.empty();
		containerEl.addClass("fmod-sync-settings");

		containerEl.createEl("h2", { text: "FMOD Sync Settings" });

		// Global settings section
		containerEl.createEl("h3", { text: "Global Settings" });

		new Setting(containerEl)
			.setName("Mirror folder structure")
			.setDesc(
				"When enabled, creates subfolders matching the FMOD event folder hierarchy."
			)
			.addToggle((toggle) =>
				toggle
					.setValue(this.plugin.settings.mirrorFolders)
					.onChange(async (value) => {
						this.plugin.settings.mirrorFolders = value;
						await this.plugin.saveSettings();
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
						name: `Project ${this.plugin.settings.projects.length + 1}`,
						jsonFilePath: "",
						outputFolder: "",
						enabled: true,
					};
					this.plugin.settings.projects.push(newProject);
					await this.plugin.saveSettings();
					this.display();
				})
		);

		// Instructions section
		containerEl.createEl("h3", { text: "How to use" });

		const instructions = containerEl.createEl("ol");
		instructions.createEl("li", {
			text: 'In FMOD Studio, run "DRD > Export for Obsidian..." to generate a JSON file for each project.',
		});
		instructions.createEl("li", {
			text: "Add each FMOD project above with a unique name and its JSON file path.",
		});
		instructions.createEl("li", {
			text: 'Use the command "FMOD Sync: Import from JSON" or click the audio icon in the ribbon.',
		});
		instructions.createEl("li", {
			text: "If you have multiple projects, a picker will let you sync individual projects or all at once.",
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

		// Header with toggle and delete
		const header = card.createDiv({ cls: "fmod-project-header" });

		// Enable toggle
		new Setting(header)
			.setName("Enabled")
			.addToggle((toggle) =>
				toggle.setValue(project.enabled).onChange(async (value) => {
					project.enabled = value;
					await this.plugin.saveSettings();
				})
			);

		// Delete button
		const deleteBtn = header.createEl("button", {
			cls: "fmod-delete-btn",
			text: "Delete",
		});
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

		// Project name
		new Setting(card)
			.setName("Project name")
			.setDesc("Display name for this project.")
			.addText((text) =>
				text
					.setPlaceholder("My Game Audio")
					.setValue(project.name)
					.onChange(async (value) => {
						// Check for duplicate names
						const duplicate = this.plugin.settings.projects.find(
							(p) => p.id !== project.id && p.name === value
						);
						if (duplicate) {
							new Notice(
								"A project with this name already exists. Please choose a different name."
							);
							return;
						}
						project.name = value;
						await this.plugin.saveSettings();
					})
			);

		// Output folder
		new Setting(card)
			.setName("Output folder")
			.setDesc("Folder in your vault where event notes will be created.")
			.addText((text) =>
				text
					.setPlaceholder("FMOD Events/MyProject")
					.setValue(project.outputFolder)
					.onChange(async (value) => {
						project.outputFolder = value;
						await this.plugin.saveSettings();
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

		// JSON file path
		const jsonSetting = new Setting(card)
			.setName("JSON file path")
			.setDesc(
				"Path to the obsidian-sync.json file exported from FMOD Studio."
			);

		const jsonTextComponent = jsonSetting.addText((text) =>
			text
				.setPlaceholder("/path/to/obsidian-sync.json")
				.setValue(project.jsonFilePath)
				.onChange(async (value) => {
					project.jsonFilePath = value;
					await this.plugin.saveSettings();
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

	generateId(): string {
		return Date.now().toString(36) + Math.random().toString(36).substr(2);
	}
}
