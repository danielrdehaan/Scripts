import {
	App,
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

interface FMODSyncSettings {
	jsonFilePath: string;
	outputFolder: string;
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
	jsonFilePath: "",
	outputFolder: "FMOD Events",
	mirrorFolders: true,
};

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
		const { jsonFilePath, outputFolder, mirrorFolders } = this.settings;

		// Validate settings
		if (!jsonFilePath) {
			new Notice("FMOD Sync: No JSON file path configured. Check settings.");
			return;
		}

		// Read and parse JSON
		let exportData: FMODExportData;
		try {
			exportData = await this.readJsonFile(jsonFilePath);
		} catch (error) {
			new Notice(`FMOD Sync: Failed to read JSON file.\n${error}`);
			return;
		}

		// Validate JSON structure
		if (!exportData.events || !Array.isArray(exportData.events)) {
			new Notice("FMOD Sync: Invalid JSON structure - missing events array.");
			return;
		}

		new Notice(`FMOD Sync: Processing ${exportData.events.length} events...`);

		// Ensure output folder exists
		const outputPath = normalizePath(outputFolder);
		await this.ensureFolderExists(outputPath);

		// Build index of existing notes by GUID and filename
		const existingNotes = await this.scanExistingNotes(outputPath);
		const notesByGuid = new Map<string, { path: string; content: string }>();
		const notesByName = new Map<string, { path: string; content: string }>();

		for (const [path, content] of existingNotes) {
			const frontmatter = this.parseFrontmatter(content);
			const guid = frontmatter.properties["guid"] as string | undefined;
			const filename = path.split("/").pop()?.replace(".md", "") || "";

			if (guid) {
				notesByGuid.set(guid, { path, content });
			}
			notesByName.set(filename, { path, content });
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
					stats
				);
			} catch (error) {
				console.error(`FMOD Sync: Error processing event ${event.name}:`, error);
				stats.errors++;
			}
		}

		// Show summary
		const summary = [
			`FMOD Sync Complete!`,
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
		const markdown = this.generateMarkdown(event, existingContent, exportedAt);

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
		exportedAt: string
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

		new Setting(containerEl)
			.setName("JSON file path")
			.setDesc(
				"Path to the obsidian-sync.json file exported from FMOD Studio. Can be absolute path or relative to vault."
			)
			.addText((text) =>
				text
					.setPlaceholder("/path/to/obsidian-sync.json")
					.setValue(this.plugin.settings.jsonFilePath)
					.onChange(async (value) => {
						this.plugin.settings.jsonFilePath = value;
						await this.plugin.saveSettings();
					})
			);

		new Setting(containerEl)
			.setName("Output folder")
			.setDesc("Folder within your vault where FMOD event notes will be created.")
			.addText((text) =>
				text
					.setPlaceholder("FMOD Events")
					.setValue(this.plugin.settings.outputFolder)
					.onChange(async (value) => {
						this.plugin.settings.outputFolder = value;
						await this.plugin.saveSettings();
					})
			);

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

		containerEl.createEl("h3", { text: "How to use" });

		const instructions = containerEl.createEl("ol");
		instructions.createEl("li", {
			text: 'In FMOD Studio, run "DRD > Export for Obsidian..." to generate the JSON file.',
		});
		instructions.createEl("li", {
			text: "Set the JSON file path above to point to the exported file.",
		});
		instructions.createEl("li", {
			text: 'Use the command "FMOD Sync: Import from JSON" or click the audio icon in the ribbon.',
		});
	}
}
