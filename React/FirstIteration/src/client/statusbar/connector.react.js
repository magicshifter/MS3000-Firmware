import Component from '../components/component.react';
import React from 'react';
import {msg} from '../intl/store';
import Loading from '../components/loading.react';

import fetch from 'isomorphic-fetch';

export default class Connector extends Component {

  render() {

    const styles = {
      floater: {
        float: 'left',
      },
    };

    return (
      <span style={styles.floater}>
        <span style={styles.floater}>
          {msg(`statusbar.connectors.${this.props.apiName}`)}
        </span>
        <Loading done={this.done} />
      </span>
    );
  }
}
