export default {
  homepage: 'leds',
  menu: [
    {path: '#leds', text: 'leds'},
    {path: '#help', text: 'help'},
    {path: '#settings', icon: 'i i-settings'},
  ],
  defaultLanguage: 'en-US',
  languages: ['en-US', 'de'],
  settings: {
    webserver: {
      host: 'magicshifter.local',
      port: 80,
    },
    accesspoint: {
      host: 'magicshifter network',
      pass: '',
    },
    wifi: {
      accesspoints: ['wizard23'],
    },
    hardware: {
      leds: {
        layout: 'abgr',
        count: 16,
      },
    },
  },
  hardware: {
    leds: {
      colorOrder: 'abgr',
      count: 16,
    },
  },
};
