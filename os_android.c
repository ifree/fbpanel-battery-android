/****************************************************************************
**
** Copyright (c) 2014 Frei Zhang <ifree0@gmail.com>
**
** Use, modification and distribution is allowed without limitation,
** warranty, liability or support of any kind.
**
****************************************************************************/


#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define AC_ONLINE_PATH "/sys/class/power_supply/ac/online"
#define USB_ONLINE_PATH "/sys/class/power_supply/usb/online"
#define BATTERY_STATUS_PATH "/sys/class/power_supply/battery/status"
#define BATTERY_HEALTH_PATH "/sys/class/power_supply/battery/health"
#define BATTERY_PRESENT_PATH "/sys/class/power_supply/battery/present"
#define BATTERY_CAPACITY_PATH "/sys/class/power_supply/battery/capacity"
#define BATTERY_VOLTAGE_PATH "/sys/class/power_supply/battery/batt_vol"
#define BATTERY_TEMPERATURE_PATH "/sys/class/power_supply/battery/batt_temp"
#define BATTERY_TECHNOLOGY_PATH "/sys/class/power_supply/battery/technology"

static gboolean
battery_update_os(battery_priv *c)
{
  GString *path;
  gchar *buf;
  gboolean ret = FALSE, exist, charging;
  int capacity;

  ENTER;
  c->exist = FALSE;
  path = g_string_sized_new(200);
  g_string_append(path, BATTERY_PRESENT_PATH);
  ret = g_file_get_contents(path->str, &buf, 0, NULL);
  g_string_truncate(path, 0);
  if(!ret)
    RET(FALSE);
  exist = atoi(buf);
  g_free(buf);
  if(!exist)
    RET(FALSE);
  
  g_string_append(path, AC_ONLINE_PATH);
  ret = g_file_get_contents(path->str, &buf, 0, NULL);
  g_string_truncate(path, 0);
  if(!ret)
    RET(FALSE);
  charging = atoi(buf);
  g_free(buf);

  if(!charging){
      g_string_append(path, USB_ONLINE_PATH);
      ret = g_file_get_contents(path->str, &buf, 0, NULL);
      g_string_truncate(path, 0);
      if(!ret)
        RET(FALSE);
      charging = atoi(buf);
      g_free(buf);
  }

  g_string_append(path, BATTERY_CAPACITY_PATH);
  ret = g_file_get_contents(path->str, &buf, 0, NULL);
  g_string_truncate(path, 0);
  if(!ret)
    RET(FALSE);
  capacity = atoi(buf);
  g_free(buf);

  g_string_free(path, TRUE);
  c->exist = exist;
  c->charging = charging;
  c->level = capacity;
  RET(TRUE);
}
