import Component from '../component.react';
import React from 'react';
import {FormattedHTMLMessage} from 'react-intl';
import {msg} from '../../intl/store';

import Logo from '../logo.react.js';

export default class Title extends Component {

  render() {
    const styles = {
      header: {
        width: '100%',
        textAlign: 'center',
        margin: '1em 0 .5em',
      },
      h2: {
        display: 'inline-block',
      },
      textSpan: {
        margin: '.2em 0 0',
        display: 'block',
        float: 'left',
      },
    };

    let title = '';
    try {
      title = msg(`pages.${this.props.page}.title`);
    }
    catch (e) {
      console.warn(e);
      console.log(msg('debug.continuehappily'));
    }

    return (
      <header style={styles.header}>
        <h2 style={styles.h2}>
          <Logo src={`${this.props.page}.svg`} size='5vw' float='left'></Logo>
          <span style={styles.textSpan}>
            {msg(`pages.${this.props.page}.title`).toUpperCase()}
          </span>
        </h2>
      </header>
    );
  }
}
