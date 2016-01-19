import React, {Component, PropTypes} from 'react';

import classes from './FloatingButton.scss';
import iconClasses from 'styles/icons.scss';

export default class FloatingButton extends Component {
  static propTypes = {
    onClick: PropTypes.func.isRequired,
    icon: PropTypes.string,
    children: PropTypes.string,
    title: PropTypes.string,
  };

  render() {
    const {onClick, icon, children, title} = this.props;

    let className = `${classes['container']}${icon && ' ' + iconClasses[icon]}`;

    return (
      <button
        className={className}
        onClick={onClick}
        title={title}
      >
        {children}
      </button>
    );
  }
}

