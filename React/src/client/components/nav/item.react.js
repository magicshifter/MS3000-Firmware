import Component from '../component.react';
import React from 'react';
import {FormattedHTMLMessage} from 'react-intl';
import {Link} from 'react-router';
import {msg} from '../../intl/store';
import Logo from '../logo.react';

export default class Item extends Component {
  render() {
    const styles = {
      container: {
        float: 'left',
        margin: '0 0 0 .5em',
      },
      span: {
        width: '100%',
        textAlign: 'center',
        margin: '-.3em 0 0',
        fontSize: '1.2em',
      },
    };

    return (
      <li key={this.props.key} style={styles.container}>
        <Link to={this.props.to}>
          <Logo
            src={`${this.props.to}.svg`}
            size='7vw'
            shadow='5px 5px 0px #78909C'
          />
          <span style={styles.span}>{msg(`menu.${this.props.to}`)}</span>
        </Link>
      </li>
    );
  }
}
