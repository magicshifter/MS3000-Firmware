import materialColors from './materialcolors';

export default {
  input: {
    backgroundColor: materialColors.blueGrey['50'],
    color: materialColors.blueGrey['500'],
    borderRight: materialColors.blueGrey['500'], borderBottom: '#B0BEC5',
  },
  button: {
    backgroundColor: materialColors.blueGrey['500'],
    color: materialColors.blueGrey['50'],
    borderRight: materialColors.blueGrey['50'],
    borderBottom: materialColors.blueGrey['50'],
  },
  legend: {
    color: materialColors.blueGrey['500'],
  },
  errorMessage: {
    color: materialColors.red['500'],
  },
  pages: {
    draw: materialColors.red,
    leds: materialColors.cyan,
    settings: materialColors.lightBlue,
    login: materialColors.purple,
    logout: materialColors.purple,
    notFound: materialColors.deepOrange,
  },
};
