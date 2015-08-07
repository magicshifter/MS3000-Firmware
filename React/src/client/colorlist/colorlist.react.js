import React from 'react';

import Component from '../components/component.react';
import * as actions from './actions';
import * as ledActions from '../leds/actions';
import {msg} from '../intl/store';
import color from 'sc-color';

import LedPreview from '../components/ledpreview.react';

import merge from 'magic-merge';

export default class ColorList extends Component {

  removeColor(e) {
    e.preventDefault();
    actions.removeColor(e.target.value);
  }

  changeLed(e) {
    const value = color(e.target.style.backgroundColor).hex6();
    const { id } = this.props;
    ledActions.changeLed({id, value});
  }


  render() {

    const styles = {
      container: merge({
        width: '10vw',
      }, this.props.style),
      li: {
        listStyle: 'none',
        display: 'inline-block',
        width: '100%',
      },
    };

    const { colorList, activeColor } = this.props;

    let colorHtmlList = [];
    const colors = colorList.get('colors')
      .sort((colorA, colorB) => color(colorA).hue() - color(colorB).hue())
      .map((val, key) => {
        colorHtmlList.push(
          <li key={key} style={styles.li}>
            <LedPreview
              value={val}
              key={key}
              onClick={(e) => this.changeLed(e)}
              size='3vw'
            />
            <button
              value={val}
              onClick={(e) => this.removeColor(e)}
              title={msg('colorList.remove.title')}>
              {msg('colorList.remove.text')}
            </button>
          </li>
        );
      });

    return (
      <div style={styles.container}>
        <ul>
          <li>
            <button
              onClick={e => actions.saveColor(activeColor)}
              title={msg('colorList.saveColorTitle')}
            >
              {msg('colorList.saveColor')}
            </button>
          </li>
          {colorHtmlList}
        </ul>
      </div>
    );
  }
}
