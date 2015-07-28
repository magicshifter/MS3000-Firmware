import Component from '../component.react';
import React from 'react';
import {FormattedHTMLMessage} from 'react-intl';
import {msg} from '../../intl/store';

import Logo from '../logo.react.js';

export default class Content extends Component {

  render() {
    const styles = {
      container: {
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
    let content = '';
    let m = msg(`pages.${this.props.page}.content`);
    if (m) {
      content = m;
    }

    return (
      <div style={styles.container}>{content}</div>
    );
  }
}
