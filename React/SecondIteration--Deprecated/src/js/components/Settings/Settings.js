// need to import react here
import React, {Component, PropTypes} from 'react';

import Radium from 'radium';
import ColorPicker from 'react-color';

@Radium
export default class Settings extends Component {

  static propTypes = {
    settings: PropTypes.shape({
      host: PropTypes.string,
    }),
  }

  constructor(props) {
    super(props);

    this.onSubmit = this.onSubmit.bind(this);
  }

  onSubmit(e) {
    console.log('settings submit', e);
  }

  render() {
    const {settings} = this.props;

    console.log('settings', settings);

    return (
      <section className='pixelEditor'>
        <div className='settingsContainer'>
          <ul>
            { settings && Object.keys(settings).map(key => {
              <li>
                <input
                  type='text'
                  name={key}
                  value={settings[key]}
                />
              </li>
            })}
            <li>
            </li>
            <li>
              <input
                type='submit'
                onClick={this.onSubmit}
              />
            </li>
          </ul>
        </div>
      </section>
    );
  }
}
