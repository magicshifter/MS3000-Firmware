import mColors from 'material-colors';

import {colorsToObj} from 'utils/colors';

export const materialColors = colorsToObj(mColors);

export const host = 'magicshifter.local';
export const protocol = 'http';

export const rows = 16;
export const totalColumns = 96;
export const visibleColumns = 16;

export const fontSize = 15;

export const sidebarWidth = 250;

export const links = [
  {to: '/', text: 'about'},
  {to: '/paint', text: 'paint'},
  {to: '/settings', text: 'settings'},
];

export const currentColorName = 'cyan';

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
