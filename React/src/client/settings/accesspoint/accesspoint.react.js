import './accesspoint.styl';
import * as actions from './actions';
import Component from '../../components/component.react';
import React from 'react';
import immutable from 'immutable';
import {focusInvalidField} from '../../lib/validation';
import {msg} from '../../intl/store';

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
        <form onSubmit={(e) => this.onFormSubmit(e)}>
          <fieldset disabled={pendingActions.has(actions.saveAccesspoint.toString())}>
            <legend>{msg('forms.settings.accesspoint.legend')}</legend>
            <input
              name='ssid'
              onChange={actions.updateFormField}
              placeholder={msg('forms.settings.accesspoint.placeholder.ssid')}
              value={form.fields.ssid}
            />
            <br />
            <input
              name='password'
              onChange={actions.updateFormField}
              placeholder={msg('forms.settings.accesspoint.placeholder.password')}
              type='password'
              value={form.fields.password}
            />
            <button
              children={msg('buttons.save')}
              type='submit'
            />
            {form.error &&
              <span className='error-message'>{form.error.message}</span>
            }
          </fieldset>
        </form>
      </div>
    );
  }

}
