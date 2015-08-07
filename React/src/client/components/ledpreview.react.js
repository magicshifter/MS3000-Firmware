import React from 'react';

import Component from '../components/component.react';
import {msg} from '../intl/store';

import merge from 'magic-merge';

export default class LedPreview extends Component {

  render() {
    const {value, size} = this.props;

    const styles = {
      container: merge({
        display: 'inline-block',
        backgroundColor: value || '#000000',
        width: size ? size : '100%',
        height: size ? size : '100%',
      }, this.props.style),
    };

    return (
      <a style={styles.container}></a>
    );
  }
}
