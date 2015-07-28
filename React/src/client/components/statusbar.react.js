import Component from '../components/component.react';
import React from 'react';
import {FormattedHTMLMessage} from 'react-intl';
import {msg} from '../intl/store';

export default class StatusBar extends Component {

  render() {

    const styles = {
      container: {
        backgroundColor: '#D4E157',
        fontSize: '9px',
        padding: '5px 5px 5px 30px',
        borderBottom: '5px solid #B0BEC5',
        width: '100%',
        bottom: 0,
        position: 'absolute',
        left: 0,
      },
      span: {
        float: 'left',
      },

    };

    return (
      <div className='statusbar' style={styles.container}>
        <span style={styles.span}>App Version 0.0.1</span>
        <span style={styles.span}></span>
      </div>
    );
  }
}
