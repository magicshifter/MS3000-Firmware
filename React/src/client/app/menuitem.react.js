import Component from '../components/component.react';
import React from 'react';
import {FormattedHTMLMessage} from 'react-intl';
import {Link} from 'react-router';
import {msg} from '../intl/store';
import Radium from 'radium';
import styles from './app.styles';
import Logo from '../components/logo.react';

@Radium
export default class MenuItem extends Component {
  render() {
    return (
      <li key={this.props.key} style={styles.header.li}>
        <Link to={this.props.to}>
          <Logo
            src={`${this.props.to}.svg`}
            size='7vw'
            shadow='5px 5px 0px #78909C'
          />
          <span style={styles.header.span}>{msg(`menu.${this.props.to}`)}</span>
        </Link>
      </li>
    );
  }
}
