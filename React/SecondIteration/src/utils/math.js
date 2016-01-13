export const minmax =
  (val, min, max) =>
    max < val
    ? max
    : min > val
      ? min
      : val;

export const multimax =
  (val, max, ...addMax) => {
    addMax
      .concat(max)
      .forEach(m => {
        if (m < val) {
          val = m;
        }
      });

    return val;
  };
