import React from 'react';

import Component from '../components/component.react';
import * as actions from './actions';
import * as ledActions from '../leds/actions';

import {msg} from '../intl/store';

export default class ColorPicker extends Component {

  onChange(e) {
    actions.updateActiveLeds(e.target.value);
  }

  render() {
    const styles = {
      container: {
        display: 'inline-block',
      },
    };

    const { active, value } = this.props;

    return (
      <div style={styles.container}>
        <input
          type='color'
          value={ value }
          onChange={(e) => this.onChange(e)}
        />
        <button
          value={ value }
          title={msg('colorPicker.add.title')}
          onClick={(e) => this.saveColor(e)}
        >
          {msg('colorPicker.add.text')}
        </button>
      </div>
    );
  }
}
