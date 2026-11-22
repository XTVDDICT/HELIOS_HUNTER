#pragma once
#include "HunterNightMode.h"

inline String hunterNightFields(const HunterDisplayOptions& options) {
  String html = "<div class='night-settings'><label class='toggle'><input type='checkbox' id='nightEnabled' name='night_enabled' ";
  if (options.nightEnabled) html += "checked";
  html += ">Night mode</label><div class='field-grid grid'>";
  html += "<div class='field'><label class='field-label' for='nightStart'>Start time</label><input id='nightStart' name='night_start' type='time' required value='" + hunterTimeText(options.nightStart) + "'></div>";
  html += "<div class='field'><label class='field-label' for='nightEnd'>End time</label><input id='nightEnd' name='night_end' type='time' required value='" + hunterTimeText(options.nightEnd) + "'></div>";
  html += "<div class='field full'><label class='field-label' for='nightZone'>Time zone</label><select id='nightZone' name='night_zone'><option>" + options.timezone + "</option></select></div></div><div class='sub' id='nightStatus' role='status'></div></div>";
  return html;
}
