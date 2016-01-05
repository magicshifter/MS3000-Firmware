#App startup

##Actions:
  1 browser request /
    index.html
      -> favicon.ico
      -> manifest.appcache

  2 Start loading animation

  3 /info/hardware
    {
      leds: int,
      ledformat: 'rgba || abgr'
      version: semver,
    }

  4*
    /settings/server
      {
        hostname: string,
        port: {int, maxLength: 5},
      }

    /settings/ap
      {
        ssid: type: string, maxLength: 31,
        pass: type: string, maxLength: 31,
      }

    /settings/wifi
      {
        accesspoints: [string],
      }
