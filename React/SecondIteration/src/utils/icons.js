import classes from 'styles/icons.scss';

export const getIconCssClass =
  icon => [
    classes['icon'],
    classes[icon],
  ].join(' ');

