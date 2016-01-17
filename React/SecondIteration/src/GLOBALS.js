import mColors from 'material-colors';

import {colorsToObj} from 'utils/colors';

export const materialColors = colorsToObj(mColors);

export const host = '';
export const protocol = 'http';

export const rows = 16;
export const totalColumns = 96;
export const visibleColumns = 16;

export const fontSize = 15;

export const sidebarWidth = 250;

export const links = [
  {to: '/', text: 'paint'},
  {to: '/about', text: 'about'},
  {to: '/settings', text: 'settings'},
];

export const currentColorName = 'cyan';

export const colorList = [
  {r: 255, g: 255, b: 0, a: 255},
  {r: 0, g: 255, b: 255, a: 255},
  {r: 127, g: 255, b: 127, a: 255},
];

export const menuTextColor = materialColors['white'];

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

export const text = '! MAGIC !';

export const fontId = 0;

export const textColor = materialColors[currentColorName][900];
