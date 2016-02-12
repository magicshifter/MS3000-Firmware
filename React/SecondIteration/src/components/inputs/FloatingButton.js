import React, {Component, PropTypes} from 'react';

import classes from './FloatingButton.scss';
import iconClasses from 'styles/icons.scss';

export default class FloatingButton extends Component {
  static propTypes = {
    onClick: PropTypes.func.isRequired,
    icon: PropTypes.string,
    children: PropTypes.string,
    title: PropTypes.string,
    text: PropTypes.string,
    cssClass: PropTypes.string,
  };

  static defaultProps = {
    cssClass: '',
    icon: '',
    title: '',
    text: '',
  };

  render() {
    const {onClick, icon, children, title, text} = this.props;
    let {cssClass} = this.props;

    cssClass += ' ' + classes['container'];
    if (icon && iconClasses[icon]) {
      cssClass += ' ' + iconClasses[icon];
    }

    return (
      <button
        className={cssClass}
        onClick={onClick}
        title={title}
      >
        {children || text}
      </button>
    );
  }
}

