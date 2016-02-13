import React, { PropTypes, Component } from 'react';

import {onInputChange} from 'utils/inputs';

import classes from './Accesspoints.scss';

export class Accesspoints extends Component {

  static propTypes = {
    ssid: PropTypes.string.isRequired,
  };

  constructor(props) {
    super(props);

    this.handleSubmit = this.handleSubmit.bind(this);
    this.onInputChange = onInputChange.bind(this);
  }

  handleSubmit(e) {
    e.preventDefault();

    console.log('handleSubmit', this.state);
  }

  handleInputChange(e) {
    const {name, value} = e.target;
    console.log({name, value});
  }

  render() {
    const {ssid} = this.props;

    return (
      <form
        className={classes['container']}
        onSubmit={this.onFormSubmit}
      >
        <fieldset>
          <legend>Preferred Accesspoint:</legend>
          <div>
            <label forHtml='ssid'>ssid</label>
            <input
              name='ssid'
              value={ssid}
            />
          </div>
          <div>
            <label forHtml='password'>password</label>
            <input
              name='password'
              type='password'
            />
          </div>
          <div>
            <button
              children='save'
              type='submit'
            />
          </div>
        </fieldset>
      </form>
    );
  }
}

export default Accesspoints;
