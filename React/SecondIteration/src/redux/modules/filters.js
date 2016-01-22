export const invert =
  (state, {payload: pixels}) =>
    state.set(
      'pixels',
      pixels.map(
        px => ({
          ...px,
          color: Object.keys(px.color).map(
            key =>
              key === 'a' ? px.color[key] : 255 - px.color
          ),
        })
      )
    );

export const darken =
  (state, {payload: pixels}) =>
    state.set(
      'pixels',
      pixels.map(
        px => ({
          ...px,
          color: Object.keys(px.color).map(
            key =>
              key === 'a' ? px.color[key] : Math.max(0, px.color - 10)
          ),
        })
      ));

export const lighten =
  (state, {payload: pixels}) =>
    state.set(
      'pixels',
      pixels.map(
        px => ({
          ...px,
          color: Object.keys(px.color).map(
            key =>
              key === 'a' ? px.color[key] : Math.min(255, px.color + 10)
          ),
        })
      ));

export const actions = {
  darken,
  lighten,
  invert,
};
