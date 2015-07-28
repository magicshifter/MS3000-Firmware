import * as actions from './actions';
import Component from '../../components/component.react';
import React from 'react';
import immutable from 'immutable';
import {focusInvalidField} from '../../lib/validation';
import {msg} from '../../intl/store';

import styles from '../../styles/form.js';

export default class AccesspointForm extends Component {

  static propTypes = {
    accesspointSettings: React.PropTypes.instanceOf(immutable.Map).isRequired,
    pendingActions: React.PropTypes.instanceOf(immutable.Map).isRequired,
  };

  getForm() {
    return this.props.accesspointSettings.get('form');
  }

  onFormSubmit(e) {
    e.preventDefault();
    const fields = this.getForm().fields.toJS();
    actions.saveAccesspoint(fields)
      .catch(focusInvalidField(this));
  }

  render() {
    const form = this.getForm();
    const {pendingActions} = this.props;

    return (
      <div className='settings-accesspoint'>
        <form onSubmit={(e) => this.onFormSubmit(e)} style={styles.form}>
          <fieldset
            style={styles.fieldset}
            disabled={pendingActions.has(actions.saveAccesspoint.toString())}
          >
            <legend>
              <h2>{msg('forms.settings.accesspoint.legend')}</h2>
            </legend>
            <div style={styles.field}>
              <label forHtml='ssid'>
                {msg('forms.settings.accesspoint.label.ssid')}
              </label>
              <input
                name='ssid'
                style={styles.input}
                onChange={actions.updateFormField}
                placeholder={msg('forms.settings.accesspoint.placeholder.ssid')}
                value={form.fields.ssid}
              />
            </div>
            <div style={styles.field}>
              <label forHtml='password'>
                {msg('forms.settings.accesspoint.label.password')}
              </label>
              <input
                name='password'
                style={styles.input}
                onChange={actions.updateFormField}
                placeholder={msg('forms.settings.accesspoint.placeholder.password')}
                type='password'
                value={form.fields.password}
              />
            </div>
            <div style={styles.buttonContainer}>
              <button
                style={styles.button}
                children={msg('buttons.save')}
                type='submit'
              />
            </div>
            {form.error &&
              <span style={styles.errorMessage}>{form.error.message}</span>
            }
          </fieldset>
        </form>
      </div>
    );
  }

}
