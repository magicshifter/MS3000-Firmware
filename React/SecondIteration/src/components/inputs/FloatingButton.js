import React, { PropTypes } from 'react';

import classes from './FloatingButton.scss';
import iconClasses from 'styles/icons.scss';

const getCssClass =
  (cssClass, icon) => {
    cssClass += ' ' + classes['container'];
    if (icon && iconClasses[icon]) {
      cssClass += ' ' + iconClasses[icon];
    }
  };

export const FloatingButton =
  ({ onClick, icon, children, title, text, cssClass }) => {
    return (
      <button
        className={getCssClass(cssClass, icon)}
        onClick={onClick}
        title={title}
      >
        {children || text}
      </button>
    );
  };

FloatingButton.propTypes = {
  onClick: PropTypes.func.isRequired,
  icon: PropTypes.string,
  children: PropTypes.string,
  title: PropTypes.string,
  text: PropTypes.string,
  cssClass: PropTypes.string,
};

FloatingButton.defaultProps = {
  cssClass: '',
  icon: '',
  title: '',
  text: '',
};

export default FloatingButton;
