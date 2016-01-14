import React, {Component, PropTypes} from 'react';

import classes from './FloatingButton.scss';
import iconClasses from 'styles/icons.scss';

export default class FloatingButton extends Component {
  static propTypes = {
    onClick: PropTypes.func.isRequired,
    icon: PropTypes.string.isRequired,
  };

  render() {
    const {onClick, icon} = this.props;

    return (
      <button
        className={classes['container'] + ' ' + iconClasses[icon]}
        onClick={onClick}
      >
      </button>
    );
  }
}

