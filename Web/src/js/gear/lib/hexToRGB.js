/*
 * Function hexToRGB
 * convert hex colors to rgb
 *
 * @param {String} hex - hex color object
 *
 * @returns {Array|Boolean} - array of rgb values or false on error
 */

export function hexToRGB(hex) {
  let rgb;
  hex = hex.replace('#', '');
  if (hex.length === 6) {
    rgb = [
      hex.substring(0, 2),
      hex.substring(2, 4),
      hex.substring(4, 6),
    ];
  } else if (hex.length === 3) {
    rgb = [
      hex.substring(0, 1) + hex.substring(0, 1),
      hex.substring(1, 2) + hex.substring(1, 2),
      hex.substring(2, 3) + hex.substring(2, 3),
    ];
  } else {
    console.warn(`invalid hex passed to hexToRGB: ${hex}`);
    return false;
  }

  return rgb.map((val) => parseInt(val));
}

export default hexToRGB;
