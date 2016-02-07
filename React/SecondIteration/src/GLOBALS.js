import mColors from 'material-colors';

import {colorsToObj} from 'utils/colors';

export const materialColors = colorsToObj(mColors);

export const host = '';
// export const host = '192.168.4.1';
export const protocol = 'http';

export const rows = 16;
export const totalColumns = 96;
export const visibleColumns = 16;

export const fontSize = 15;

export const zoomLevel = 1;
export const minZoomLevel = 0.5;
export const maxZoomLevel = 3;

export const links = [
  {to: '/', text: 'paint'},
  {to: '/about', text: 'about'},
  {to: '/settings', text: 'settings'},
];

export const currentColorName = 'cyan';

export const defaultLedColor = {r: 0, b: 0, g: 0, a: 255};

export const defaultEditorColor = {r: 255, b: 255, g: 255, a: 255};

export const colorList = [
  {r: 0, g: 0, b: 0, a: 255},
  {r: 255, g: 0, b: 0, a: 255},
  {r: 0, g: 255, b: 0, a: 255},
  {r: 0, g: 0, b: 255, a: 255},
  {r: 255, g: 255, b: 0, a: 255},
  {r: 255, g: 0, b: 255, a: 255},
  {r: 0, g: 255, b: 255, a: 255},
  {r: 255, g: 255, b: 255, a: 255},
];

export const menuTextColor = materialColors['black'];

export const currentColor = materialColors[currentColorName];

export const fonts = [
  {
    name: 'georgia',
    css: 'Georgia',
  },
  {
    name: 'monospace',
    css: 'Lucida Console',
  },
  {
    name: 'comic sans',
    css: 'Comic Sans MS',
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

export const textColor = materialColors[currentColorName][900];
