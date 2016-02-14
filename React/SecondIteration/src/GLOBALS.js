import mColors from 'material-colors';

import { colorsToObj } from 'utils/colors';

export const materialColors = colorsToObj(mColors);

export const host = 'magicshifter.local';
// export const host = '192.168.4.1';
export const protocol = 'http';

export const syslogIp = '';

export const rows = 16;
export const totalColumns = 96;
export const visibleColumns = 16;

export const fontSize = 15;

export const zoomLevel = 1;
export const minZoomLevel = 0.5;
export const maxZoomLevel = 3;

export const links = [
  { to: '/info', icon: 'info' },
  { to: '/settings', icon: 'settings' },
];

export const pixelEditorUrlBase = '/paint';

export const pixelEditorSubUrls = {
  images: {},
  fonts: {},
  effects: {},
  colors: {},
  upload: {
    color: '#fff',
  },
};

export const secondaryLinks =
  Object.keys(pixelEditorSubUrls).map(
    url => ({
      to: `${pixelEditorUrlBase}/${url}`,
      text: url,
      icon: url,
      style: { ...pixelEditorSubUrls[url]},
    })
  );

export const defaultLedColor = { r: 0, b: 0, g: 0, a: 255 };

export const defaultEditorColor = { r: 28, b: 255, g: 250, a: 255 };
export const uiColor = defaultEditorColor;

export const colorList = [
  { r: 0, g: 0, b: 0, a: 255 },
  { r: 255, g: 0, b: 0, a: 255 },
  { r: 0, g: 255, b: 0, a: 255 },
  { r: 0, g: 0, b: 255, a: 255 },
  { r: 255, g: 255, b: 0, a: 255 },
  { r: 255, g: 0, b: 255, a: 255 },
  { r: 0, g: 255, b: 255, a: 255 },
  { r: 255, g: 255, b: 255, a: 255 },
];

export const menuTextColor = materialColors['black'];

export const fonts = [
  {
    name: 'Palatino',
    css: 'Palatino Linotype, Book Antiqua3, Palatino, serif',
  },
  {
    name: 'Impact',
    css: 'Impact, Impact5, Charcoal6, sans-serif',
  },
  {
    name: 'Tahoma',
    css: 'Tahoma, Geneva, sans-serif',
  },
  {
    name: 'Monospace',
    css: 'Lucida Console, Monaco5, monospace',
  },
];

export const layout = {
  height: window.innerHeight,
  width: window.innerWidth,
};

export const sidebar = {
  width: layout.height > 500 ? 300 : 250,
  margin: 25,
};

export const text = 'Magic';

export const fontId = 0;
