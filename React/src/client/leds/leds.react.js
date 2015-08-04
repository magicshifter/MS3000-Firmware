import React, {PropTypes} from 'react';

import Component from '../components/component.react';
import {msg} from '../intl/store';
import * as actions from './actions';

import Led from './led.react';
import ColorPicker from 'react-color-picker';
import '../../../node_modules/react-color-picker/style/index.styl';
import ColorList from '../colorpicker/colorlist.react';
import LedsMenu from './ledsmenu.react';

import merge from 'magic-merge';
import globalStyles from '../styles/global';

export default class Leds extends Component {

  static propTypes = {
    activeColor: PropTypes.string
  }

  static defaultProps = {
    activeColor: '#ff0000',
  }

  changeActiveColor(data) {
    actions.changeActiveColor(data);
  }

  render() {
    const styles = {
      container: {
        width: '100%',
        display: 'inline-block',
      },
      list: {
        display: 'inline-block',
      },
      colorPicker: {
        backgroundColor: '#fff',
        borderRadius: '5px',
        border: '2px solid grey',
        float: 'left',
      },
      colorList: {
        float: 'left',
      },
      ledsMenu: {
        width: '100%',
        textAlign: 'center',
        display: 'block',
      }
    };

    const { leds, colorPicker } = this.props;
    const list = leds.get('list');
    const activeColor = leds.get('activeColor');
    let ledsHtml = [];
    list.map((val, key) => {
      if (val) {
        ledsHtml.push(
          <Led
            key={key}
            ledId={key}
            value={val.get('value')}
            active={val.get('active')}
          />
        );
      }
    });

    return (
      <div style={styles.container}>
        <div
          style={styles.list}
          onMouseDown={e => actions.startSelection(e)}
          onMouseUp={e => actions.stopSelection(e)}
        >
          {ledsHtml}
        </div>

        <LedsMenu
          style={globalStyles.float('left')}
          leds={leds}
        />

        <ColorPicker
          style={globalStyles.float('left')}
          onDrag={(e) => this.changeActiveColor(e)}
        />

        <ColorList
          style={globalStyles.float('left')}
          activeColor={leds.get('activeColor')}
          colorPicker={colorPicker}
        />
      </div>
    );
  }
}
