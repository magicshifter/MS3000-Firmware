import React, {PropTypes, Component} from 'react';
import {connect} from 'react-redux';

import {actions} from 'redux/modules/views/settings';
import {settingsViewType} from 'utils/propTypes';

import ApSettings from './ap';
import UiSettings from './ui';

import {onInputChange} from 'utils/inputs';

import classes from './SettingsView.scss';

import GLOBALS from 'GLOBALS';

const mapStateToProps =
  state => ({
    settings: state.settingsView.toJS(),
  });

export class SettingsView extends Component {
  static propTypes = {
    settings: settingsViewType,
    setSettings: PropTypes.func.isRequired,
  };

  constructor(props) {
    super(props);

    // this.inputChange = onInputChange.bind(this);
    this.formSubmit = this.formSubmit.bind(this);

    this.state = props.settings || {
      ssid: GLOBALS.ssid || '',
      syslogIp: GLOBALS.syslogIp || '',
      host: GLOBALS.host || '',
      protocol: GLOBALS.protocol || '',
    };
  }

  inputChange(e) {
    onInputChange(e, this);
  }

  formSubmit(e) {
    const {setSettings} = this.props;
    setSettings(this.state);

    e.preventDefault();
    return false;
  }

  render() {
    const {protocol, host, syslogIp, ssid} = this.state;

    return (
      <div className={[classes['container'], 'container'].join(' ')}>
        <h2>Settings:</h2>

        <UiSettings
          onInputChange={e => this.inputChange(e, this)}
          protocol={protocol}
          host={host}
          syslogIp={syslogIp}
          onFormSubmit={this.formSubmit}
        />

        <ApSettings
          ssid={ssid}
          onInputChange={e => this.inputChange(e, this)}
          onFormSubmit={this.formSubmit}
        />
      </div>
    );
  }
}

export default connect(mapStateToProps, actions)(SettingsView);
