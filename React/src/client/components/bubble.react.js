import Component from './component.react';
import React from 'react';
import {FormattedHTMLMessage} from 'react-intl';
import {msg} from '../intl/store';

import colors from '../styles/colors';

export default class Bubble extends Component {

  render() {
    const styles = {
      container: {
        width: '100%',
        position: 'relative',
        margin: '2em 0',
        padding: '1em 2em',
        borderRadius: '1em',
        border: `3px solid ${colors.input.borderBottom}`,
        backgroundColor: colors.input.color,
        color: colors.input.backgroundColor,
        WebkitFilter: 'drop-shadow(5px 5px 0px #78909C)',
        filter: 'drop-shadow(5px 5px 0px #78909C)',
      },
      text: {
        fontSize: '1.5em',
        fontWeight: 600,
      },
    };

    return (
      <div style={styles.container}>
        <span style={styles.text}>{this.props.text}</span>
      </div>
    );
  }
}
