import Component from './component.react';
import React from 'react';
import {Link} from 'react-router';
import {msg} from '../intl/store';

export default class Logo extends Component {
  render() {
    let size = (this.props.size)
           ? this.props.size
           : '10vw';

    let style = {
      backgroundImage: `url(/assets/img/${this.props.src})`,
      backgroundSize: 'contain',
      height: `${size}`,
      width: `${size}`,
      display: 'block',
      maxHeight: '80px',
      maxWidth: '80px',
    };

    if (this.props.float) {
      style.float = this.props.float;
    }

    if (this.props.shadow) {
      style.filter = `drop-shadow(${this.props.shadow})`;
    }

    return (
      <span style={style}></span>
    );
  }
}
