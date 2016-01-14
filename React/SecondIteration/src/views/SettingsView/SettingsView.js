import React, {PropTypes, Component} from 'react';
import {connect} from 'react-redux';

import {actions} from 'redux/modules/settings';
import {settingsType} from 'utils/propTypes';

import classes from './SettingsView.scss';

// We define mapStateToProps where we'd normally use
// the @connect decorator so the data requirements are clear upfront, but then
// export the decorated component after the main class definition so
// the component can be tested w/ and w/o being connected.
// See: http://rackt.github.io/redux/docs/recipes/WritingTests.html
const mapStateToProps = (state) => ({settings: state.settings.toJS()});

export class SettingsView extends Component {
  static propTypes = {
    settings: settingsType,
    setSettings: PropTypes.func.isRequired,
  };

  constructor(props) {
    super(props);

    this.onInputChange = this.onInputChange.bind(this);

    this.state = props.settings || {};
  }

  onInputChange(e) {
    const {name, value} = e.target;
    this.setState({[name]: value});
  }

  render() {
    const {settings, setSettings} = this.props;
    const {protocol, host} = this.state;

    return (
      <div className={classes['container']}>
        <h2>Settings:</h2>

        <div className={classes['form']}>
          <ul>
            <li style={{display: 'none'}} key='protocol'>
              <label>protocol:</label>
              <input
                type='text'
                name='protocol'
                onChange={e => this.onInputChange(e)}
                value={protocol}
              />
            </li>

            <li key='host'>
              <label>hostname:</label>
              <input
                type='text'
                name='host'
                onChange={e => this.onInputChange(e)}
                value={host}
              />
            </li>

            <li
              key='submit'
            >
              <input
                type='submit'
                value='Save Settings'
                onClick={() => setSettings(this.state)}
              />
            </li>
          </ul>
        </div>
      </div>
    );
  }
}

export default connect(mapStateToProps, actions)(SettingsView);
