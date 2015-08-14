import Component from '../components/component.react';
import React from 'react';
import {FormattedHTMLMessage} from 'react-intl';
import {msg} from '../intl/store';
import Loading from '../components/loading.react';

import Connector from './connector.react';

export default class StatusBar extends Component {

  render() {

    const styles = {
      wrapper: {
        backgroundColor: '#D4E157',
        fontSize: '9px',
        padding: '5px 30px',
        borderBottom: '5px solid #B0BEC5',
        width: '100%',
        bottom: 0,
        position: 'absolute',
        left: 0,
      },
      container: {
        maxWidth: '800px',
        display: 'block',
        margin: '0 auto',
      },
      title: {
        float: 'left',
        margin: '0 1em 0 0',
      },
      floatRight: {
        float: 'right',
      },
      floatLeft: {
        float: 'left',
      },
    };

    return (
      <div style={styles.wrapper}>
        <div style={styles.container}>
          <div style={styles.floatLeft}>{msg('statusbar.title', {version: '0.0.1'})}</div>
          <div style={styles.floatRight}>
            <h5 style={styles.title}>
              {msg('statusbar.connectors.title')}
            </h5>
            <Connector apiName='api' url='magicshifter.net/api/v1/' />
            <Connector apiName='magicshifter' />
          </div>
        </div>
      </div>
    );
  }
}
