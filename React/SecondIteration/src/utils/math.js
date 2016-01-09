export function minmax(val, min, max) {
  if (val > max) {
    val = max;
  }
  if (val < min) {
    val = min;
  }
  return val;
}
