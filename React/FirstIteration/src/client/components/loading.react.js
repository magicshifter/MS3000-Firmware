import Component from './component.react';
import React from 'react';
import {FormattedHTMLMessage} from 'react-intl';
import {Link} from 'react-router';
import {msg} from '../intl/store';

import merge from 'magic-merge';

export default class Loading extends Component {
  render() {
    const styles = {
      container: {
        float: 'left',
        margin: '-1px 5px 0 4px',
        width: '40px',
      },
      leds: {
        border: '1px solid',
        borderRadius: '5px',
        display: 'inline-block',
        float: 'left',
        height: '5px',
        margin: '5px 1px 5px',
        width: '5px',
      },
      states: [
        {
          borderColor: 'grey',
        },
        {
          borderColor: 'orange',
        },
        {
          borderColor: 'white',
        },
      ],
    };

    const ledStyle = merge(styles.leds, styles.states[this.currentState]);

    return (
      <ul style={styles.container}>
        <li style={ledStyle}></li>
        <li style={ledStyle}></li>
        <li style={ledStyle}></li>
      </ul>
    );
  }
}
