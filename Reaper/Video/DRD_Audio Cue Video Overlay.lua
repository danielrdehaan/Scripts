--[[
  Author: Daniel Dehaan
  Website: www.danielrdehaan.com
  Description: Burns the names of audio items into the project video so an
               audio implementer can see exactly when each file should fire.

  How it works:
    REAPER's Video Processor can't see audio items, so this script scans the
    project and bakes a cue list into a Video Processor on a "Cue Overlay"
    track at the top of the project. Each "slot" is tied to a folder track
    (e.g. "ambiance"); items on that folder or any of its children whose
    name starts with the prefix are drawn in that slot's position while they
    play. The processor also masks out (blacks) the area outside the video's
    content band so existing burned-in text is hidden.

    Each cue shows its fades/volume/pan and a progress bar (fade regions
    shaded darker), colored by category; timecode and frame number are
    burned in.

    A second processor, "Audio Cue Timeline", follows the overlay on the same
    track: one strip across the whole video with a play icon where each file
    starts and a thin line in the file's own row (overlapping files stack)
    running to where it ends. Loops also get a stop icon at their end, since
    the playback system has to stop them; one-shots just end. Colored by
    category, so it's easy to scrub to those points. It has its own knobs and can be bypassed or
    deleted independently (the main script and Refresh action add it back).

    Every rebuild also writes a .csv/.json cue sheet next to each file the
    next render will write, with the same name as the rendered file (or
    "<project> - Cue Sheet" next to the project file if there is no render
    target): start/end in seconds, frames (from project start) and timecode,
    loop/one-shot, fades, volume and pan, for loading into the playback
    system (e.g. a TouchDesigner Table DAT).

  Usage:
    1. Run the script, enter the item name prefix (e.g. GOO2595) and the
       folder track name for each slot. Settings are saved in the project.
    2. Adjust layout on the "Audio Cue Overlay" FX knobs (mask, text size,
       slot x/y/alignment, hold time, timecode, frame number, bars).
    3. Re-run the script (or use the Refresh / Auto Refresh actions) whenever
       items change, then render from the project start so video frames line
       up with the cue sheet.

  Requirements:
    - REAPER 7.x (uses the Video Processor VIDEO_CODE config parameter)
]]

local SCRIPT_NAME = "Audio Cue Video Overlay"
local EXT_SECTION = "DRD_AudioCueVideoOverlay"
local OVERLAY_TRACK_NAME = "Cue Overlay"
local OVERLAY_TITLE = "Audio Cue Overlay"   -- first line of each processor's code,
local TIMELINE_TITLE = "Audio Cue Timeline" -- also used as its FX display name
-- Bump when the generated Video Processor code changes, so auto-refresh
-- rewrites existing overlays with the new code
local CODE_VERSION = "10"

-- color is the slot's category color (file names in the overlay and the timeline)
local SLOTS = {
  { caption = "Slot 1 folder (top-left)",      default = "ambiance",   x = 0.019, y = 0.025, align = 0, color = { 0.30, 0.78, 0.72 } },
  { caption = "Slot 2 folder (top-right)",     default = "cue",        x = 0.981, y = 0.025, align = 1, color = { 0.43, 0.63, 0.94 } },
  { caption = "Slot 3 folder (bottom-left)",   default = "transition", x = 0.019, y = 0.615, align = 0, color = { 0.72, 0.52, 0.96 } },
  { caption = "Slot 4 folder (bottom-right)",  default = "ui",         x = 0.981, y = 0.615, align = 1, color = { 1.00, 0.66, 0.30 } },
  { caption = "Slot 5 folder (top-center)",    default = "",           x = 0.5,   y = 0.025, align = 0.5, color = { 0.96, 0.46, 0.66 } },
  { caption = "Slot 6 folder (bottom-center)", default = "",           x = 0.5,   y = 0.615, align = 0.5, color = { 0.56, 0.86, 0.42 } },
}

-- Video Processor knobs: { variable, display name, default, min, max, step }
local PARAMS = {
  { "size",    "Text height",          0.032, 0.01, 0.15, 0.001 },
  { "lsp",     "Line spacing",         1.15,  0.8,  3,    0.01 },
  { "maskt",   "Mask top",             0.42,  0,    1,    0.001 },
  { "maskb",   "Mask bottom",          0.42,  0,    1,    0.001 },
  { "maskl",   "Mask left",            0,     0,    1,    0.001 },
  { "maskr",   "Mask right",           0,     0,    1,    0.001 },
  { "showlbl", "Show slot labels",     1,     0,    1,    1 },
  { "showdt",  "Show fades/volume",    1,     0,    1,    1 },
  { "showbar", "Show progress bars",   1,     0,    1,    1 },
  { "showfd",  "Show fade labels",     1,     0,    1,    1 },
  { "mwid",    "Max line width",       0.47,  0.1,  1,    0.001 },
  { "bga",     "Text bg alpha",        0,     0,    1,    0.01 },
  { "hold",    "Min display (s, 0 = item length)", 0, 0, 10, 0.05 },
  { "flash",   "Highlight new (s)",    0.5,   0,    5,    0.05 },
  { "maxl",    "Max lines per slot",   6,     1,    30,   1 },
  { "showtc",  "Show timecode",        1,     0,    1,    1 },
  { "showfr",  "Show frame number",    1,     0,    1,    1 },
  { "tcx",     "Timecode x",           0.5,   0,    1,    0.001 },
  { "tcy",     "Timecode y",           0.95,  0,    1,    0.001 },
}
for i, slot in ipairs(SLOTS) do
  PARAMS[#PARAMS + 1] = { "s" .. i .. "x", "Slot " .. i .. " x",     slot.x,     0, 1, 0.001 }
  PARAMS[#PARAMS + 1] = { "s" .. i .. "y", "Slot " .. i .. " y",     slot.y,     0, 1, 0.001 }
  PARAMS[#PARAMS + 1] = { "s" .. i .. "a", "Slot " .. i .. " align", slot.align, 0, 1, 0.5 }
end

-- Knobs for the separate "Audio Cue Timeline" processor
local TIMELINE_PARAMS = {
  { "tsize",  "Text height",             0.02,  0.005, 0.1,  0.001 },
  { "ticon",  "Icon size",               0.016, 0.004, 0.08, 0.001 },
  { "tly",    "Axis y",                  0.87,  0,     1,    0.001 },
  { "tlx0",   "Left",                    0.019, 0,     1,    0.001 },
  { "tlx1",   "Right",                   0.981, 0,     1,    0.001 },
  { "tlwin",  "Span seconds (0 = all)",  0,     0,     300,  1 },
  { "tlplay", "Now position (if span)",  0.5,   0,     1,    0.01 },
  { "tlanim", "Highlight length (s)",    0.4,   0,     3,    0.05 },
  { "tleg",   "Show legend",             1,     0,     1,    1 },
  { "tlly",   "Legend y",                0.95,  0,     1,    0.001 },
}

local function trim(s)
  return (s:gsub("^%s+", ""):gsub("%s+$", ""))
end

local function split(s, sep)
  local out, start = {}, 1
  while true do
    local i = s:find(sep, start, true)
    if not i then
      out[#out + 1] = s:sub(start)
      return out
    end
    out[#out + 1] = s:sub(start, i - 1)
    start = i + #sep
  end
end

local function eel_string(s)
  s = s:gsub("[%c]", " "):gsub("\\", "\\\\"):gsub('"', '\\"')
  return '"' .. s .. '"'
end

local function num(v)
  return string.format("%.6f", v)
end

---------------------------------------------------------------------------
-- Settings dialog
---------------------------------------------------------------------------

local function get_setting(key, default)
  local ok, value = reaper.GetProjExtState(0, EXT_SECTION, key)
  if ok == 1 then return value end
  return default
end

local function prompt_settings()
  local captions = { "Item name prefix (; for several)" }
  local values = { get_setting("prefix", "") }
  for i, slot in ipairs(SLOTS) do
    captions[#captions + 1] = slot.caption
    values[#values + 1] = get_setting("slot" .. i, slot.default)
  end

  local ok, csv = reaper.GetUserInputs(SCRIPT_NAME, #captions,
    table.concat(captions, ",") .. ",extrawidth=150", table.concat(values, ","))
  if not ok then return nil end

  local fields = split(csv, ",")
  local settings = { prefix = trim(fields[1] or ""), slots = {} }
  reaper.SetProjExtState(0, EXT_SECTION, "prefix", settings.prefix)
  for i = 1, #SLOTS do
    settings.slots[i] = trim(fields[i + 1] or "")
    reaper.SetProjExtState(0, EXT_SECTION, "slot" .. i, settings.slots[i])
  end
  return settings
end

-- Saved settings, or nil if the dialog has never been confirmed in this project
local function load_settings()
  if get_setting("slot1", nil) == nil then return nil end
  local settings = { prefix = get_setting("prefix", ""), slots = {} }
  for i = 1, #SLOTS do settings.slots[i] = get_setting("slot" .. i, "") end
  return settings
end

---------------------------------------------------------------------------
-- Cue collection
---------------------------------------------------------------------------

local function item_display_name(item)
  local take = reaper.GetActiveTake(item)
  if take then
    return trim(reaper.GetTakeName(take))
  end
  -- Empty items: use the first line of the item notes
  local _, notes = reaper.GetSetMediaItemInfo_String(item, "P_NOTES", "", false)
  return trim(notes:match("[^\r\n]*") or "")
end

-- Fade lengths (auto-crossfades take precedence) and combined item * take
-- volume in dB (nil = -inf)
local function item_mix(item)
  local function fade(which)
    local auto = reaper.GetMediaItemInfo_Value(item, "D_FADE" .. which .. "LEN_AUTO")
    if auto > 0 then return auto end
    return reaper.GetMediaItemInfo_Value(item, "D_FADE" .. which .. "LEN")
  end

  local vol = reaper.GetMediaItemInfo_Value(item, "D_VOL")
  local take = reaper.GetActiveTake(item)
  if take then vol = vol * math.abs(reaper.GetMediaItemTakeInfo_Value(take, "D_VOL")) end
  local db = vol > 0 and 20 * math.log(vol, 10) or nil
  return fade("IN"), fade("OUT"), db
end

-- Pan as a signed percentage (-100 = hard left, +100 = hard right), using
-- REAPER's own formatting of the envelope value so left/right is never guessed
local function envelope_pan_percent(env, value)
  local text = reaper.Envelope_FormatValue(env, value)
  local amount, side = text:match("([%d%.]+)%%%s*([LlRr])")
  if amount then return (side:upper() == "L" and -1 or 1) * tonumber(amount) end
  return 0
end

local function envelope_active(env)
  local ok, chunk = reaper.GetEnvelopeStateChunk(env, "", false)
  return not ok or chunk:match("\nACT 0") == nil
end

-- Take pan: returns pan % at the item start and, if the take has an active Pan
-- envelope, a list of { time (relative to item start), pan % } samples. Curved
-- and square segments get extra samples so the overlay can interpolate linearly.
local function item_pan(item)
  local take = reaper.GetActiveTake(item)
  if not take then return 0, nil end
  local env = reaper.GetTakeEnvelopeByName(take, "Pan")
  if not env or reaper.CountEnvelopePoints(env) == 0 or not envelope_active(env) then
    return reaper.GetMediaItemTakeInfo_Value(take, "D_PAN") * 100, nil
  end

  local len = reaper.GetMediaItemInfo_Value(item, "D_LENGTH")
  local keys = { 0 }
  for i = 0, reaper.CountEnvelopePoints(env) - 1 do
    local _, time = reaper.GetEnvelopePoint(env, i)
    if time > keys[#keys] + 0.0001 and time < len - 0.0001 then keys[#keys + 1] = time end
  end
  keys[#keys + 1] = len

  local samples = {}
  local function sample(time)
    local _, value = reaper.Envelope_Evaluate(env, time, 48000, 1)
    samples[#samples + 1] = { time, envelope_pan_percent(env, value) }
  end
  for i = 1, #keys - 1 do
    local a, b = keys[i], keys[i + 1]
    sample(a)
    local idx = reaper.GetEnvelopePointByTime(env, a + 0.000001)
    local shape = idx >= 0 and select(4, reaper.GetEnvelopePoint(env, idx)) or 0
    if shape == 1 then
      sample(math.max(a, b - 0.0005))           -- square: hold until the next point
    elseif shape ~= 0 then
      for j = 1, 7 do sample(a + (b - a) * j / 8) end
    end
  end
  sample(len)

  -- A flat envelope is just a static pan
  local flat = true
  for _, smp in ipairs(samples) do
    if math.abs(smp[2] - samples[1][2]) >= 0.5 then flat = false break end
  end
  if flat then return samples[1][2], nil end
  return samples[1][2], samples
end

local function pan_text(pct)
  if pct >= 0.5 then return string.format("pan %d%% R", math.floor(pct + 0.5)) end
  if pct <= -0.5 then return string.format("pan %d%% L", math.floor(-pct + 0.5)) end
  return "pan C"
end

-- e.g. "in 0.50s | out 1.20s | -6.0 dB | pan 30% L". Defaults are omitted;
-- automated pan is added live by the overlay instead.
local function details_text(c)
  local parts = {}
  if c.fade_in >= 0.005 then parts[#parts + 1] = string.format("in %.2fs", c.fade_in) end
  if c.fade_out >= 0.005 then parts[#parts + 1] = string.format("out %.2fs", c.fade_out) end
  if not c.db then
    parts[#parts + 1] = "-inf dB"
  elseif math.abs(c.db) >= 0.05 then
    parts[#parts + 1] = string.format("%+.1f dB", c.db)
  end
  if not c.pan_points and math.abs(c.pan) >= 0.5 then parts[#parts + 1] = pan_text(c.pan) end
  return table.concat(parts, " | ")
end

-- "loop" if LOOP/LOOPING/LOOPED appears as its own word in the name
-- (e.g. AMBIENCE-LOOP, AMBI_LOOP), otherwise "one-shot"
local function playback_type(name)
  local words = " " .. name:upper():gsub("[^%w]", " ") .. " "
  for _, token in ipairs({ " LOOP ", " LOOPING ", " LOOPED " }) do
    if words:find(token, 1, true) then return "loop" end
  end
  return "one-shot"
end

local function make_prefix_matcher(prefix_text)
  local prefixes = {}
  for _, p in ipairs(split(prefix_text, ";")) do
    p = trim(p):lower()
    if p ~= "" then prefixes[#prefixes + 1] = p end
  end
  return function(name)
    if #prefixes == 0 then return true end
    local lower = name:lower()
    for _, p in ipairs(prefixes) do
      if lower:sub(1, #p) == p then return true end
    end
    return false
  end
end

-- Returns the slot index of the nearest ancestor (or the track itself)
-- whose name matches a slot folder name.
local function track_slot(track, slot_by_name)
  while track do
    local _, name = reaper.GetTrackName(track)
    local slot = slot_by_name[trim(name):lower()]
    if slot then return slot end
    track = reaper.GetParentTrack(track)
  end
  return nil
end

local function collect_cues(settings, overlay_track)
  local slot_by_name = {}
  for i, name in ipairs(settings.slots) do
    if name ~= "" and not slot_by_name[name:lower()] then
      slot_by_name[name:lower()] = i
    end
  end

  local matches_prefix = make_prefix_matcher(settings.prefix)
  local cues, unassigned = {}, 0

  for t = 0, reaper.CountTracks(0) - 1 do
    local track = reaper.GetTrack(0, t)
    if track ~= overlay_track then
      local slot = track_slot(track, slot_by_name)
      for i = 0, reaper.CountTrackMediaItems(track) - 1 do
        local item = reaper.GetTrackMediaItem(track, i)
        if reaper.GetMediaItemInfo_Value(item, "B_MUTE") == 0 then
          local name = item_display_name(item)
          if name ~= "" and matches_prefix(name) then
            if slot then
              local pos = reaper.GetMediaItemInfo_Value(item, "D_POSITION")
              local len = reaper.GetMediaItemInfo_Value(item, "D_LENGTH")
              local c = { start = pos, stop = pos + len, slot = slot, name = name }
              c.fade_in, c.fade_out, c.db = item_mix(item)
              c.pan, c.pan_points = item_pan(item)
              c.details = details_text(c)
              c.loop = playback_type(name) == "loop"
              cues[#cues + 1] = c
            else
              unassigned = unassigned + 1
            end
          end
        end
      end
    end
  end

  table.sort(cues, function(a, b)
    if a.start ~= b.start then return a.start < b.start end
    return a.slot < b.slot
  end)
  -- Timeline rows: overlapping cues (any category) get separate rows, and a
  -- row is reused as soon as its cue has ended. Fixed for the whole project.
  local row_ends = {}
  for _, c in ipairs(cues) do
    local row = 1
    while row_ends[row] and row_ends[row] > c.start + 0.000001 do row = row + 1 end
    row_ends[row] = c.stop
    c.row = row - 1
  end

  return cues, unassigned
end

---------------------------------------------------------------------------
-- Video Processor code generation
---------------------------------------------------------------------------

-- EEL shared by both processors: font and cue table loader
local EEL_ADD_CUE = [==[
font = "Arial";

function add_cue(st, en, k, nm, dt, fi, fo, lp, rw, pn, pc) (
  cue_s[cue_n] = st; cue_e[cue_n] = en; cue_k[cue_n] = k; cue_nm[cue_n] = nm; cue_dt[cue_n] = dt;
  cue_fi[cue_n] = fi; cue_fo[cue_n] = fo; cue_lp[cue_n] = lp; cue_rw[cue_n] = rw;
  cue_pn[cue_n] = pn; cue_pc[cue_n] = pc;
  cue_n += 1;
);
]==]

-- EEL shared by both processors: middle-truncation to fit a width
local EEL_TEXT_FUNCS = [==[
// dest = first and last keep/2 bytes of src joined by "...", without splitting
// UTF-8 characters
function cut_middle(dest, src, keep) local(n, a, b) (
  n = strlen(src);
  a = ((keep + 1) / 2) | 0; b = keep - a;
  while (a > 0 && (str_getchar(src, a, 'cu') & 192) == 128) ( a -= 1; );
  while (b > 0 && (str_getchar(src, n - b, 'cu') & 192) == 128) ( b -= 1; );
  a > 0 ? strcpy_substr(dest, src, 0, a) : strcpy(dest, "");
  strcat(dest, "...");
  strcpy_from(#cut_tail, src, n - b);
  strcat(dest, #cut_tail);
);

// dest = src, shortened in the middle if needed to fit maxw. Returns the width.
function fit_text(dest, src, maxw) local(w, h, lo, hi, mid) (
  gfx_str_measure(src, w, h);
  w <= maxw ? strcpy(dest, src) : (
    lo = 0; hi = strlen(src);
    while (lo < hi) (
      mid = ((lo + hi + 1) / 2) | 0;
      cut_middle(dest, src, mid);
      gfx_str_measure(dest, w, h);
      w <= maxw ? (lo = mid) : (hi = mid - 1);
    );
    cut_middle(dest, src, lo);
    gfx_str_measure(dest, w, h);
  );
  w;
);
]==]

-- EEL for the overlay: text layout helpers
local EEL_OVERLAY_FUNCS = [==[
// Keeps a segment of width w at anchor ax (al: 0 left, 0.5 center, 1 right) on screen
function place(ax, w, al) (
  max(pad, min((ax - w * al) | 0, fw - w - pad));
);

// Draws str at x,y (with optional background). Returns its width.
function draw_seg(str, x, y, r, g, b) local(w, h) (
  gfx_str_measure(str, w, h);
  bga > 0 ? ( gfx_set(0, 0, 0, bga); gfx_fillrect(x - pad, y, w + pad * 2, h); );
  gfx_set(r, g, b, 1);
  gfx_str_draw(str, x, y);
  w;
);

// While cue ci is fading, writes "FADE IN/OUT <remaining>" to #fdtag and
// returns 1 (in) or -1 (out); otherwise 0
function fade_tag(ci) local(dir) (
  dir = 0;
  showfd > 0.5 && t < cue_e[ci] ? (
    cue_fo[ci] >= 0.005 && t >= cue_e[ci] - cue_fo[ci] ? (
      dir = -1; sprintf(#fdtag, "FADE OUT %.1fs", cue_e[ci] - t);
    ) : (
      cue_fi[ci] >= 0.005 && t < cue_s[ci] + cue_fi[ci] ? (
        dir = 1; sprintf(#fdtag, "FADE IN %.1fs", cue_s[ci] + cue_fi[ci] - t);
      );
    );
  );
  dir;
);

// Pan % of cue ci at time tt, interpolated from its envelope samples
function pan_at(ci, tt) local(i0, n, j) (
  i0 = cue_pn[ci]; n = cue_pc[ci];
  tt <= pn_t[i0] ? pn_v[i0] : (
    tt >= pn_t[i0 + n - 1] ? pn_v[i0 + n - 1] : (
      j = i0;
      while (j < i0 + n - 2 && pn_t[j + 1] <= tt) ( j += 1; );
      pn_v[j] + (pn_v[j + 1] - pn_v[j]) * (tt - pn_t[j]) / max(pn_t[j + 1] - pn_t[j], 0.000001);
    );
  );
);

// Details text for cue ci: the static fades/volume text, plus the live pan
// value if the item has pan automation
function cue_details(ci) local(p) (
  cue_pc[ci] > 0 ? (
    p = pan_at(ci, min(max(t, cue_s[ci]), cue_e[ci]));
    p >= 0.5 ? sprintf(#pan_s, "pan %d%% R", p + 0.5) : (
      p <= -0.5 ? sprintf(#pan_s, "pan %d%% L", 0.5 - p) : strcpy(#pan_s, "pan C");
    );
    strlen(cue_dt[ci]) > 0 ? sprintf(#dt_live, "%s | %s", cue_dt[ci], #pan_s) : strcpy(#dt_live, #pan_s);
    #dt_live;
  ) : cue_dt[ci];
);

// Draws one cue, limited to "Max line width":
//   line 1: file name (shortened in the middle if too long)
//   line 2: fades/volume in grey + live fade label (smaller text)
//   progress bar with fade-in/out regions shaded darker
// Returns the height used.
function draw_cue(ci, ax, ay, al, r, g, b) local(maxw, dts, wn, wd, wt, h, dir, gap, x, y, bottom, wb, by, dur, p, fiw, fow, tr, tg, tb) (
  dts = cue_details(ci);
  maxw = max((mwid * fw) | 0, th * 4);
  wn = fit_text(#name_fit, cue_nm[ci], maxw);
  draw_seg(#name_fit, place(ax, wn, al), ay, r, g, b);
  bottom = ay + th;
  wd = 0;

  // Line 2 exists whenever the item has details or fades, so the lines below
  // don't jump when a fade label appears
  (showdt > 0.5 && strlen(dts) > 0) || (showfd > 0.5 && (cue_fi[ci] >= 0.005 || cue_fo[ci] >= 0.005)) ? (
    y = ay + lh1;
    gfx_setfont(fs2, font);
    gap = pad * 3;
    wt = 0;
    (dir = fade_tag(ci)) != 0 ? gfx_str_measure(#fdtag, wt, h);
    showdt > 0.5 && strlen(dts) > 0 ? (
      wd = fit_text(#dt_fit, dts, maxw - (wt > 0 ? wt + gap : 0));
    );
    x = place(ax, wd, al);
    wd > 0 ? draw_seg(#dt_fit, x, y, 0.65, 0.65, 0.65);
    dir != 0 ? (
      dir > 0 ? ( tr = 0.45; tg = 0.85; tb = 0.50; ) : ( tr = 1.0; tg = 0.45; tb = 0.40; );
      // Outer side of the details, so they never shift when the label appears
      al > 0.75 ?
        draw_seg(#fdtag, max(pad, x - (wd > 0 ? gap : 0) - wt), y, tr, tg, tb) :
        draw_seg(#fdtag, min(x + wd + (wd > 0 ? gap : 0), fw - wt - pad), y, tr, tg, tb);
    );
    gfx_setfont(fs1, font);
    bottom = y + th2;
  );

  showbar > 0.5 ? (
    wb = max(wn, wd);
    x = place(ax, wb, al);
    by = bottom + bgap;
    bga > 0 ? ( gfx_set(0, 0, 0, bga); gfx_fillrect(x - pad, by - bgap, wb + pad * 2, barh + bgap * 2); );
    dur = max(cue_e[ci] - cue_s[ci], 0.000001);
    p = min(max((t - cue_s[ci]) / dur, 0), 1);
    gfx_set(0.3, 0.3, 0.3, 1);
    gfx_fillrect(x, by, wb, barh);
    gfx_set(r, g, b, 1);
    p > 0 ? gfx_fillrect(x, by, max((wb * p) | 0, 1), barh);
    fiw = (min(cue_fi[ci] / dur, 1) * wb) | 0;
    fow = (min(cue_fo[ci] / dur, 1) * wb) | 0;
    fiw >= 1 ? gfx_gradrect(x, by, fiw, barh, 0, 0, 0, 0.75, 0, 0, 0, -0.75 / fiw);
    fow >= 1 ? gfx_gradrect(x + wb - fow, by, fow, barh, 0, 0, 0, 0, 0, 0, 0, 0.75 / fow);
    bottom = by + barh;
  );

  bottom - ay + (lh1 - th);
);

function draw_line(str, ax, ay, al, r, g, b) local(w) (
  w = fit_text(#line_fit, str, fw - pad * 2);
  draw_seg(#line_fit, place(ax, w, al), ay, r, g, b);
);
]==]

-- EEL for the overlay: per-frame drawing
local EEL_OVERLAY_BODY = [==[
t = project_time;
input = 0;
project_wh_valid === 0 ? input_info(input, project_w, project_h);
fw = project_w; fh = project_h;

// Source video
gfx_set(0, 0, 0, 1);
gfx_fillrect(0, 0, fw, fh);
input_info(input, iw, ih) ? gfx_blit(input, 1);

// Mask everything outside the content band
mt = (maskt * fh) | 0; mb = (maskb * fh) | 0; ml = (maskl * fw) | 0; mr = (maskr * fw) | 0;
gfx_set(0, 0, 0, 1);
mt > 0 ? gfx_fillrect(0, 0, fw, mt);
mb > 0 ? gfx_fillrect(0, fh - mb, fw, mb);
ml > 0 ? gfx_fillrect(0, mt, ml, fh - mt - mb);
mr > 0 ? gfx_fillrect(fw - mr, mt, mr, fh - mt - mb);

fs1 = max(size * fh, 6) | 0;   // file names, labels, timecode
fs2 = max(fs1 * 0.8, 6) | 0;   // fades/volume line
gfx_setfont(fs2, font);
gfx_str_measure("Ag", tw, th2);
gfx_setfont(fs1, font);
gfx_str_measure("Ag", tw, th);
pad = (th * 0.25) | 0;
barh = max(2, (th * 0.12) | 0);
bgap = max(2, (th * 0.15) | 0);
lh1 = (th * lsp) | 0;

// Gather cues that are playing (or still within the hold time)
k = 0; loop(8, cnt[k] = 0; k += 1; );
i = 0;
while (i < cue_n && cue_s[i] <= t) (
  t < max(cue_e[i], cue_s[i] + hold) ? (
    k = cue_k[i];
    act[k * cue_cap + cnt[k]] = i;
    cnt[k] += 1;
  );
  i += 1;
);

// Draw each slot: label, then the most recent cues
k = 0;
loop(6,
  slot_on[k] ? (
    ax = sx[k] * fw; ay = (sy[k] * fh) | 0; al = sa[k];
    showlbl > 0.5 ? (
      cnt[k] >= 2 ? (
        sprintf(#lbl_n, "%s (%d)", lbl[k], cnt[k]);
        draw_line(#lbl_n, ax, ay, al, cr[k], cg[k], cb[k]);
      ) : draw_line(lbl[k], ax, ay, al, cr[k], cg[k], cb[k]);
      ay += lh1;
    );
    c = cnt[k];
    j = max(0, c - (maxl | 0));
    while (j < c) (
      i = act[k * cue_cap + j];
      // Category color (same as the timeline); brightened toward white while new
      hl = t - cue_s[i] < flash ? 0.6 : 0;
      ay += draw_cue(i, ax, ay, al, cr[k] + (1 - cr[k]) * hl, cg[k] + (1 - cg[k]) * hl, cb[k] + (1 - cb[k]) * hl);
      j += 1;
    );
  );
  k += 1;
);

// Timecode and frame number (frames count from project start, matching the cue sheet)
showtc > 0.5 || showfr > 0.5 ? (
  showtc > 0.5 ? (
    tf = floor((t + project_timeoffs) * framerate + 0.0000001);
    f = ceil(framerate);
    tcdf > 0.5 && f != framerate ? (
      period = floor(framerate * 600);
      ds = floor(framerate * 60);
      ds > 0 ? tf += 18 * ((tf / period) | 0) + ((((tf % period) - 2) / ds) | 0) * 2;
    );
    sprintf(#tc, "%02d:%02d:%02d:%02d", (tf / (f * 3600)) | 0, (tf / (f * 60)) % 60, (tf / f) % 60, tf % f);
  ) : strcpy(#tc, "");
  fr = floor(t * framerate + 0.0000001);
  showfr < 0.5 ? strcpy(#tcline, #tc) : (
    showtc > 0.5 ? sprintf(#tcline, "%s  |  frame %d", #tc, fr) : sprintf(#tcline, "frame %d", fr);
  );
  draw_line(#tcline, tcx * fw, (tcy * fh) | 0, 0.5, 1, 1, 1);
);
]==]

-- EEL for the timeline processor
local EEL_TIMELINE_FUNCS = [==[
// Play icon (file in): triangle with its left edge at x.
// Stop icon (file out): square with its right edge at x. y = top, sz = size.
function draw_icon(x, y, sz, is_in) local(j, d, w) (
  sz = max(sz | 0, 2);
  is_in ? (
    j = 0;
    loop(sz,
      d = abs(j + 0.5 - sz / 2) / (sz / 2);
      w = ((1 - d) * sz * 0.9) | 0;
      w > 0 ? gfx_fillrect(x | 0, (y + j) | 0, w, 1);
      j += 1;
    );
  ) : (
    gfx_fillrect((x - sz * 0.8) | 0, (y + sz * 0.1) | 0, (sz * 0.8) | 0, (sz * 0.8) | 0);
  );
);

// One strip for all events. Each file is a thin line in its own row just under
// the axis (rows are shared by all categories and reused once a file ends),
// linking its play icon above the axis to its stop icon below the rows.
// Colored by category; playing files are thicker, finished ones dimmed.
// Covers the whole video unless a span is set (then it scrolls).
function draw_timeline() local(x0, x1, tw, ay, sz, gap, lw, thin, thick, pitch, ob, t0, t1, nx, i, k, x, xs, xe, ry, playing, lvl, side, cy, age, pop, a, ly, lx, isz, w, hh) (
  x0 = (tlx0 * fw) | 0; x1 = max((tlx1 * fw) | 0, x0 + 20); tw = x1 - x0;
  ay = (tly * fh) | 0;
  sz = max((ticon * fh) | 0, 4);
  gap = max((sz * 0.35) | 0, 2);
  lw = max(1, (fh / 540) | 0);
  thin = lw; thick = lw * 2;
  pitch = thick + max(2, lw * 2);
  ob = ay + lw + n_rows * pitch + gap;   // top of the stop-icon zone
  tlwin > 0 ? ( t0 = t - tlplay * tlwin; t1 = t0 + tlwin; ) : ( t0 = 0; t1 = max(proj_len, 1); );
  nx = x0 + (t - t0) / (t1 - t0) * tw;

  // Axis (brighter where already played) and faint row guides
  gfx_set(1, 1, 1, 0.22);
  gfx_fillrect(x0, ay, tw, lw);
  nx > x0 ? ( gfx_set(1, 1, 1, 0.6); gfx_fillrect(x0, ay, (min(nx, x1) - x0) | 0, lw); );
  gfx_set(1, 1, 1, 0.05);
  i = 0;
  loop(n_rows, gfx_fillrect(x0, (ay + lw + pitch * i + pitch / 2) | 0, tw, 1); i += 1; );

  // Span lines, one per file
  i = 0;
  while (i < cue_n && cue_s[i] <= t1) (
    k = cue_k[i];
    cue_e[i] >= t0 && slot_on[k] ? (
      xs = max(x0 + (cue_s[i] - t0) / (t1 - t0) * tw, x0);
      xe = min(x0 + (cue_e[i] - t0) / (t1 - t0) * tw, x1);
      ry = ay + lw + pitch * cue_rw[i] + pitch / 2;
      playing = t >= cue_s[i] && t < cue_e[i];
      w = playing ? thick : thin;
      gfx_set(cr[k], cg[k], cb[k], playing ? 1 : (cue_e[i] <= t ? 0.45 : 0.85));
      gfx_fillrect(xs | 0, (ry - w / 2) | 0, max((xe - xs) | 0, 1), w);
    );
    i += 1;
  );

  // Play/stop icons on stems that reach the file's row; stacked when they'd touch
  k = 0; loop(8, last_x[k] = -1000000; k += 1; );
  i = 0;
  while (i < ev_n && ev_t[i] <= t1) (
    k = ev_k[i];
    ev_t[i] >= t0 && slot_on[k] ? (
      x = x0 + (ev_t[i] - t0) / (t1 - t0) * tw;
      side = ev_in[i] ? 0 : 4;
      lvl = 0;
      while (lvl < 2 && last_x[side + lvl] > x - sz * 1.1) ( lvl += 1; );
      last_x[side + lvl] = x;
      cy = ev_in[i] ? ay - gap - sz / 2 - lvl * (sz + gap) : ob + sz / 2 + lvl * (sz + gap);
      ry = ay + lw + pitch * ev_rw[i] + pitch / 2;
      age = t - ev_t[i];
      pop = age >= 0 && age < tlanim ? 1 - age / tlanim : 0;
      a = age > 0 ? 0.5 : 1;
      gfx_set(cr[k], cg[k], cb[k], a);
      ev_in[i] ? gfx_fillrect(x | 0, cy | 0, lw, (ry - cy) | 0) : gfx_fillrect(x | 0, ry | 0, lw, (cy - ry) | 0);
      gfx_set(cr[k] + (1 - cr[k]) * pop, cg[k] + (1 - cg[k]) * pop, cb[k] + (1 - cb[k]) * pop, max(a, pop));
      isz = sz * (1 + 0.6 * pop);
      draw_icon(x, cy - isz / 2, isz, ev_in[i]);
    );
    i += 1;
  );

  // Playhead
  nx >= x0 && nx <= x1 ? (
    gfx_set(1, 1, 1, 0.95);
    gfx_fillrect((nx - lw) | 0, ay - gap * 2 - sz, lw * 2, (ob + sz + gap) - (ay - gap * 2 - sz));
  );

  // Legend: category colors on the left, icon key on the right
  tleg > 0.5 ? (
    ly = (tlly * fh) | 0;
    isz = max((th * 0.6) | 0, 4);
    lx = x0; k = 0;
    loop(6,
      slot_on[k] ? (
        gfx_set(cr[k], cg[k], cb[k], 1);
        gfx_fillrect(lx, (ly + (th - isz) / 2) | 0, isz, isz);
        gfx_str_measure(lbl[k], w, hh);
        gfx_str_draw(lbl[k], lx + isz + pad, ly);
        lx += isz + pad + w + pad * 4;
      );
      k += 1;
    );
    gfx_set(0.7, 0.7, 0.7, 1);
    gfx_str_measure("out", w, hh);
    x = x1 - w;
    gfx_str_draw("out", x, ly);
    draw_icon(x - pad, ly + (th - isz) / 2, isz, 0);
    x -= pad + isz + pad * 4;
    gfx_str_measure("in", w, hh);
    x -= w;
    gfx_str_draw("in", x, ly);
    draw_icon(x - pad - isz, ly + (th - isz) / 2, isz, 1);
  );
);
]==]

local EEL_TIMELINE_BODY = [==[
t = project_time;
input = 0;
project_wh_valid === 0 ? input_info(input, project_w, project_h);
fw = project_w; fh = project_h;

// Draw on top of the incoming picture (the overlay's output)
input_info(input, iw, ih) ? gfx_blit(input, 1) : ( gfx_set(0, 0, 0, 1); gfx_fillrect(0, 0, fw, fh); );

fs3 = max(tsize * fh, 6) | 0;
gfx_setfont(fs3, font);
gfx_str_measure("Ag", tw, th);
pad = (th * 0.3) | 0;
draw_timeline();
]==]

local function add_header(L, title, settings, hash, params)
  L[#L + 1] = "// " .. title
  L[#L + 1] = "// Generated by \"DRD_Audio Cue Video Overlay.lua\" on " .. os.date("%Y-%m-%d %H:%M:%S") .. "."
  L[#L + 1] = "// Re-run that script to refresh the cue list; edits to this code are overwritten."
  L[#L + 1] = "// Prefix: " .. (settings.prefix ~= "" and settings.prefix or "(any)")
  L[#L + 1] = "// Cue hash: " .. hash
  for i, param in ipairs(params) do
    local var, name, def, lo, hi, step = table.unpack(param)
    L[#L + 1] = string.format("//@param%d:%s '%s' %g %g %g %g %g", i, var, name, def, lo, hi, (lo + hi) / 2, step)
  end
end

-- Cue table, slot labels and lane colors, loaded once per generation
local function add_cue_table(L, settings, cues)
  local function add(s) L[#L + 1] = s end
  -- Cue table, loaded once per generation
  local gen = string.format("%d", os.time() * 1000 + math.random(0, 999))
  add("cue_gen != " .. gen .. " ? (")
  add(string.format("  cue_cap = %d;", math.max(#cues, 1)))
  add([==[
  cue_s = 0; cue_e = cue_cap; cue_k = cue_cap * 2; cue_nm = cue_cap * 3; cue_dt = cue_cap * 4;
  cue_fi = cue_cap * 5; cue_fo = cue_cap * 6; cue_lp = cue_cap * 7; cue_rw = cue_cap * 8;
  cue_pn = cue_cap * 9; cue_pc = cue_cap * 10;
  act = cue_cap * 11;
  ev_t = cue_cap * 19; ev_k = cue_cap * 21; ev_in = cue_cap * 23; ev_rw = cue_cap * 25;
  meta = cue_cap * 27;
  pn_t = meta + 200;
  lbl = meta; slot_on = meta + 8; cnt = meta + 16; sx = meta + 24; sy = meta + 32; sa = meta + 40;
  cr = meta + 56; cg = meta + 64; cb = meta + 72; last_x = meta + 80;
  cue_n = 0;]==])
  for i, name in ipairs(settings.slots) do
    if name ~= "" then
      add(string.format("  lbl[%d] = %s; slot_on[%d] = 1;", i - 1, eel_string("/" .. name), i - 1))
    else
      add(string.format("  slot_on[%d] = 0;", i - 1))
    end
    local color = SLOTS[i].color
    add(string.format("  cr[%d] = %s; cg[%d] = %s; cb[%d] = %s;", i - 1, color[1], i - 1, color[2], i - 1, color[3]))
  end
  -- Pan automation samples (absolute time, pan %) for cues with a pan envelope
  local pan_lines, pan_total = {}, 0
  for _, c in ipairs(cues) do
    local pn, pc = 0, 0
    if c.pan_points then
      pn, pc = pan_total, #c.pan_points
      for _, smp in ipairs(c.pan_points) do
        pan_lines[#pan_lines + 1] = string.format("  pn_t[%d] = %s; pn_v[%d] = %s;", pan_total, num(c.start + smp[1]), pan_total, num(smp[2]))
        pan_total = pan_total + 1
      end
    end
    add(string.format("  add_cue(%s, %s, %d, %s, %s, %s, %s, %d, %d, %d, %d);", num(c.start), num(c.stop), c.slot - 1,
      eel_string(c.name), eel_string(c.details), num(c.fade_in), num(c.fade_out), c.loop and 1 or 0, c.row, pn, pc))
  end
  add(string.format("  pn_v = pn_t + %d;", math.max(pan_total, 1)))
  for _, line in ipairs(pan_lines) do add(line) end

  -- In/out events in time order (used by the timeline). Only loops get a stop
  -- event: one-shots end on their own, while a loop has to be stopped by the
  -- playback system. Every cue's line still runs to the end of the item.
  local events = {}
  for _, c in ipairs(cues) do
    events[#events + 1] = { time = c.start, slot = c.slot, is_in = 1, row = c.row }
    if c.loop then
      events[#events + 1] = { time = c.stop, slot = c.slot, is_in = 0, row = c.row }
    end
  end
  table.sort(events, function(a, b)
    if a.time ~= b.time then return a.time < b.time end
    if a.is_in ~= b.is_in then return a.is_in < b.is_in end
    return a.slot < b.slot
  end)
  add("  ev_n = 0;")
  for _, ev in ipairs(events) do
    add(string.format("  ev_t[ev_n] = %s; ev_k[ev_n] = %d; ev_in[ev_n] = %d; ev_rw[ev_n] = %d; ev_n += 1;",
      num(ev.time), ev.slot - 1, ev.is_in, ev.row))
  end
  local rows = 0
  for _, c in ipairs(cues) do rows = math.max(rows, c.row + 1) end
  add(string.format("  n_rows = %d;", rows))
  add(string.format("  proj_len = %s;", num(reaper.GetProjectLength(0))))
  add("  cue_gen = " .. gen .. ";")
  add(");")
end

local function build_overlay_code(settings, cues, hash)
  local L = {}
  add_header(L, OVERLAY_TITLE, settings, hash, PARAMS)
  L[#L + 1] = EEL_ADD_CUE
  L[#L + 1] = EEL_TEXT_FUNCS
  L[#L + 1] = EEL_OVERLAY_FUNCS
  add_cue_table(L, settings, cues)
  for i = 1, #SLOTS do
    L[#L + 1] = string.format("sx[%d] = s%dx; sy[%d] = s%dy; sa[%d] = s%da;", i - 1, i, i - 1, i, i - 1, i)
  end
  local _, drop_frame = reaper.TimeMap_curFrameRate(0)
  L[#L + 1] = "tcdf = " .. (drop_frame and 1 or 0) .. "; // project drop-frame setting"
  L[#L + 1] = EEL_OVERLAY_BODY
  return table.concat(L, "\n")
end

local function build_timeline_code(settings, cues, hash)
  local L = {}
  add_header(L, TIMELINE_TITLE, settings, hash, TIMELINE_PARAMS)
  L[#L + 1] = EEL_ADD_CUE
  L[#L + 1] = EEL_TIMELINE_FUNCS
  add_cue_table(L, settings, cues)
  L[#L + 1] = EEL_TIMELINE_BODY
  return table.concat(L, "\n")
end


---------------------------------------------------------------------------
-- Overlay track / FX
---------------------------------------------------------------------------

local function find_overlay_track()
  for t = 0, reaper.CountTracks(0) - 1 do
    local track = reaper.GetTrack(0, t)
    local _, name = reaper.GetTrackName(track)
    if name == OVERLAY_TRACK_NAME then return track end
  end
  return nil
end

local function create_overlay_track()
  reaper.InsertTrackAtIndex(0, false)
  local track = reaper.GetTrack(0, 0)
  reaper.GetSetMediaTrackInfo_String(track, "P_NAME", OVERLAY_TRACK_NAME, true)
  return track
end

-- Finds the Video Processor whose code starts with "// <title>"
local function find_processor(track, title)
  for fx = 0, reaper.TrackFX_GetCount(track) - 1 do
    local ok, code = reaper.TrackFX_GetNamedConfigParm(track, fx, "VIDEO_CODE")
    if ok and code:sub(1, #title + 3) == "// " .. title then return fx end
  end
  return nil
end

local function param_values_by_name(track, fx)
  local values = {}
  for p = 0, reaper.TrackFX_GetNumParams(track, fx) - 1 do
    local _, name = reaper.TrackFX_GetParamName(track, fx, p)
    values[name] = reaper.TrackFX_GetParam(track, fx, p)
  end
  return values
end

local function set_param_values_by_name(track, fx, values)
  for p = 0, reaper.TrackFX_GetNumParams(track, fx) - 1 do
    local _, name = reaper.TrackFX_GetParamName(track, fx, p)
    if values[name] ~= nil then reaper.TrackFX_SetParam(track, fx, p, values[name]) end
  end
end

---------------------------------------------------------------------------
-- Cue sheet export (CSV + JSON next to the project file)
---------------------------------------------------------------------------

local CSV_COLUMNS = {
  "index", "category", "file", "playback", "start_seconds", "end_seconds", "duration_seconds",
  "start_frame", "end_frame", "start_timecode", "end_timecode",
  "fade_in_seconds", "fade_out_seconds", "fade_out_start_seconds", "fade_out_start_frame", "volume_db",
  "pan_percent", "pan_automation",
}

local function short_num(v)
  return (string.format("%.6f", v):gsub("0+$", ""):gsub("%.$", ""))
end

local function json_string(s)
  return '"' .. s:gsub('[%c"\\]', function(ch)
    local esc = { ['"'] = '\\"', ["\\"] = "\\\\", ["\n"] = "\\n", ["\r"] = "\\r", ["\t"] = "\\t" }
    return esc[ch] or string.format("\\u%04x", ch:byte())
  end) .. '"'
end

local function csv_field(s)
  if s:find('[,"\r\n]') then return '"' .. s:gsub('"', '""') .. '"' end
  return s
end

-- One row per cue; frames count from project start (frame 0 = 0.0s)
local function cue_sheet_rows(settings, cues)
  local fps = reaper.TimeMap_curFrameRate(0)
  local function frame(sec) return math.floor(sec * fps + 0.0000001) end
  local rows = {}
  for i, c in ipairs(cues) do
    rows[i] = {
      index = i,
      category = settings.slots[c.slot],
      file = c.name,
      playback = playback_type(c.name),
      start_seconds = c.start,
      end_seconds = c.stop,
      duration_seconds = c.stop - c.start,
      start_frame = frame(c.start),
      end_frame = frame(c.stop),
      start_timecode = reaper.format_timestr_pos(c.start, "", 5),
      end_timecode = reaper.format_timestr_pos(c.stop, "", 5),
      fade_in_seconds = c.fade_in,
      fade_out_seconds = c.fade_out,
      -- When to start stopping the sound (equals end_seconds if there is no fade-out)
      fade_out_start_seconds = c.stop - math.min(c.fade_out, c.stop - c.start),
      fade_out_start_frame = frame(c.stop - math.min(c.fade_out, c.stop - c.start)),
      volume_db = c.db, -- nil = -inf
      pan_percent = math.floor(c.pan * 10 + 0.5) / 10, -- at the start; negative = left
      pan_automation = c.pan_points or {},              -- { time from cue start, pan % }
    }
  end
  return rows
end

local function write_file(path, text)
  local f, err = io.open(path, "w")
  if not f then return nil, err end
  f:write(text)
  f:close()
  return true
end

local function render_targets()
  local ok, targets = reaper.GetSetProjectInfo_String(0, "RENDER_TARGETS", "", false)
  return ok and targets or ""
end

-- Where the cue sheet goes: next to each file the next render will write, with
-- the same name (wildcards already resolved by REAPER). Falls back to
-- "<project> - Cue Sheet" next to the project file if there is no render target.
local function cue_sheet_bases()
  local bases, seen = {}, {}
  for path in render_targets():gmatch("[^;]+") do
    local base = path:gsub("%.[^%.\\/]*$", "")
    if base ~= "" and not seen[base] then
      seen[base] = true
      bases[#bases + 1] = base
    end
  end
  if #bases == 0 then
    local _, project_file = reaper.EnumProjects(-1)
    if project_file and project_file ~= "" then
      bases[1] = project_file:gsub("%.[Rr][Pp][Pp]$", "") .. " - Cue Sheet"
    end
  end
  return bases
end

-- Returns the list of base paths written (without extension), or nil and a reason
local function export_cue_sheet(settings, cues)
  local bases = cue_sheet_bases()
  if #bases == 0 then
    return nil, "set a render path or save the project first"
  end
  local fps, drop = reaper.TimeMap_curFrameRate(0)
  local rows = cue_sheet_rows(settings, cues)

  local function value(row, col, for_json)
    local v = row[col]
    if v == nil then return for_json and "null" or "-inf" end
    if type(v) == "table" then
      if #v == 0 then return for_json and "null" or "" end
      local parts = {}
      for i, smp in ipairs(v) do
        local t_, pct = short_num(smp[1]), short_num(math.floor(smp[2] * 10 + 0.5) / 10)
        parts[i] = for_json and string.format('{ "time_seconds": %s, "pan_percent": %s }', t_, pct)
          or string.format("%s:%s", t_, pct)
      end
      return for_json and "[ " .. table.concat(parts, ", ") .. " ]" or table.concat(parts, ";")
    end
    if type(v) == "string" then return for_json and json_string(v) or csv_field(v) end
    if math.type(v) == "integer" then return tostring(v) end
    return short_num(v)
  end

  local csv = { table.concat(CSV_COLUMNS, ",") }
  for _, row in ipairs(rows) do
    local fields = {}
    for i, col in ipairs(CSV_COLUMNS) do fields[i] = value(row, col, false) end
    csv[#csv + 1] = table.concat(fields, ",")
  end

  local categories = {}
  for _, name in ipairs(settings.slots) do
    if name ~= "" then categories[#categories + 1] = json_string(name) end
  end
  local json_cues = {}
  for i, row in ipairs(rows) do
    local fields = {}
    for j, col in ipairs(CSV_COLUMNS) do
      fields[j] = string.format('      %s: %s', json_string(col), value(row, col, true))
    end
    json_cues[i] = "    {\n" .. table.concat(fields, ",\n") .. "\n    }"
  end
  local json = table.concat({
    "{",
    '  "project": ' .. json_string(reaper.GetProjectName(0)) .. ",",
    '  "exported_at": ' .. json_string(os.date("%Y-%m-%dT%H:%M:%S")) .. ",",
    '  "frame_rate": ' .. short_num(fps) .. ",",
    '  "drop_frame": ' .. tostring(drop) .. ",",
    '  "prefix": ' .. json_string(settings.prefix) .. ",",
    '  "categories": [' .. table.concat(categories, ", ") .. "],",
    '  "cues": [',
    table.concat(json_cues, ",\n"),
    "  ]",
    "}",
  }, "\n")

  for _, base in ipairs(bases) do
    local dir = base:match("^(.*)[\\/]")
    if dir then reaper.RecursiveCreateDirectory(dir, 0) end
    local ok, err = write_file(base .. ".csv", table.concat(csv, "\n") .. "\n")
    if ok then ok, err = write_file(base .. ".json", json .. "\n") end
    if not ok then return nil, err end
  end
  return bases
end

-- Hash of the cue list content (FNV-1a), stamped into the generated code so
-- auto-refresh only rewrites the overlay when something actually changed
local function cue_hash(settings, cues)
  local parts = { CODE_VERSION, settings.prefix, table.concat(settings.slots, "\0"), string.format("%s/%s", reaper.TimeMap_curFrameRate(0)), num(reaper.GetProjectLength(0)), render_targets() }
  for _, c in ipairs(cues) do
    parts[#parts + 1] = string.format("%.6f|%.6f|%d|%s|%s", c.start, c.stop, c.slot, c.name, c.details)
    for _, smp in ipairs(c.pan_points or {}) do
      parts[#parts + 1] = string.format("%.4f:%.1f", smp[1], smp[2])
    end
  end
  local s, h = table.concat(parts, "\n"), 2166136261
  for i = 1, #s do h = ((h ~ s:byte(i)) * 16777619) & 0xffffffff end
  return string.format("%08x", h)
end

local function current_hash(track)
  local fx = find_processor(track, OVERLAY_TITLE)
  if not fx then return nil end
  local _, code = reaper.TrackFX_GetNamedConfigParm(track, fx, "VIDEO_CODE")
  return code:match("// Cue hash: (%x+)")
end

-- Writes the cue list into the overlay's Video Processor.
-- opts.create: create the overlay track/FX if missing (otherwise returns "missing")
-- opts.undo:   wrap the change in an undo point
-- Returns status ("ok", "missing" or "error"), cues, unassigned count.
-- Writes code into the processor titled `title` (adding it at the end of the
-- chain if fx is nil), keeping knob values and defaulting any new knobs
local function write_processor(track, fx, title, code, params)
  local saved = {}
  if fx then
    saved = param_values_by_name(track, fx)
  else
    fx = reaper.TrackFX_AddByName(track, "Video processor", false, -1)
    if fx < 0 then return false end
  end
  for _, param in ipairs(params) do
    if saved[param[2]] == nil then saved[param[2]] = param[3] end
  end
  if not reaper.TrackFX_SetNamedConfigParm(track, fx, "VIDEO_CODE", code) then return false end
  set_param_values_by_name(track, fx, saved)
  reaper.TrackFX_SetNamedConfigParm(track, fx, "renamed_name", title)
  return true
end

local function rebuild(settings, opts)
  local track = find_overlay_track()
  local overlay_fx = track and find_processor(track, OVERLAY_TITLE)
  if not overlay_fx and not opts.create then return "missing" end

  if opts.undo then reaper.Undo_BeginBlock() end
  reaper.PreventUIRefresh(1)

  track = track or create_overlay_track()
  local cues, unassigned = collect_cues(settings, track)
  local hash = cue_hash(settings, cues)

  local ok = write_processor(track, overlay_fx, OVERLAY_TITLE,
    build_overlay_code(settings, cues, hash), PARAMS)
  -- The timeline goes after the overlay so it draws on top. Auto-refresh only
  -- updates an existing one, so deleting it keeps it deleted.
  local timeline_fx = find_processor(track, TIMELINE_TITLE)
  if ok and (timeline_fx or opts.create) then
    ok = write_processor(track, timeline_fx, TIMELINE_TITLE,
      build_timeline_code(settings, cues, hash), TIMELINE_PARAMS)
  end

  reaper.PreventUIRefresh(-1)
  reaper.TrackList_AdjustWindows(false)
  if opts.undo then reaper.Undo_EndBlock(SCRIPT_NAME, -1) end

  if not ok then return "error", cues, unassigned end
  local sheet, sheet_err = export_cue_sheet(settings, cues)
  return "ok", cues, unassigned, sheet, sheet_err
end

local function report_error()
  reaper.MB("Couldn't write the Video Processor code. This script needs REAPER 7 or newer.", SCRIPT_NAME, 0)
end

local function report_summary(settings, cues, unassigned, sheet, sheet_err)
  local per_slot = {}
  for _, c in ipairs(cues) do per_slot[c.slot] = (per_slot[c.slot] or 0) + 1 end
  local lines = { string.format("%d cue(s) written to the \"%s\" track.", #cues, OVERLAY_TRACK_NAME), "" }
  for i, name in ipairs(settings.slots) do
    if name ~= "" then
      lines[#lines + 1] = string.format("  Slot %d  /%s: %d", i, name, per_slot[i] or 0)
    end
  end
  if unassigned > 0 then
    lines[#lines + 1] = ""
    lines[#lines + 1] = string.format("%d matching item(s) aren't under any slot folder and were skipped.", unassigned)
  end
  lines[#lines + 1] = ""
  if sheet then
    lines[#lines + 1] = "Cue sheet (.csv / .json):"
    for _, base in ipairs(sheet) do lines[#lines + 1] = base end
  else
    lines[#lines + 1] = "Cue sheet not written: " .. tostring(sheet_err)
  end
  reaper.MB(table.concat(lines, "\n"), SCRIPT_NAME, 0)
end

---------------------------------------------------------------------------
-- Main
--
-- The companion actions load this file with the global ACVO_MODE set:
--   "refresh" - rebuild from saved settings without the dialog
--   "library" - return the functions above for the auto-refresh loop
---------------------------------------------------------------------------

local MODE = ACVO_MODE or "dialog"
ACVO_MODE = nil

if MODE == "library" then
  return {
    SCRIPT_NAME = SCRIPT_NAME,
    load_settings = load_settings,
    find_overlay_track = find_overlay_track,
    collect_cues = collect_cues,
    cue_hash = cue_hash,
    current_hash = current_hash,
    rebuild = rebuild,
  }
end

local settings = MODE == "refresh" and load_settings() or prompt_settings()
if settings then
  local status, cues, unassigned, sheet, sheet_err = rebuild(settings, { create = true, undo = true })
  if status ~= "ok" then
    report_error()
  elseif MODE ~= "refresh" then
    report_summary(settings, cues, unassigned, sheet, sheet_err)
  end
end
