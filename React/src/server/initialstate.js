import messages from '../client/messages';

const initialLocale = 'en';
const ledCount = 16;

let ledArray = [];
for (let i = 0; i < ledCount; i++) {
  ledArray.push({
    key: i,
    value: '#000000',
    active: false,
  });
}

export default {
  auth: {
    data: null,
    form: null,
  },
  serverSettings: {
    data: null,
    form: null,
  },
  accesspointSettings: {
    data: null,
    form: null,
  },
  i18n: {
    formats: {},
    locales: initialLocale,
    messages: messages[initialLocale]
  },
  pendingActions: {},
  users: {
    viewer: null
  },
  leds: {
    settings: {
      count: 16,
      format: 'abgr',
    },
    list: ledArray,
  },
  colorPicker: {
    colors: [
      '#ff0000',
      '#00ff00',
      '#0000ff',
    ],
  },
};
