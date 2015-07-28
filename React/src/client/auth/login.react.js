import * as actions from './actions';
import Component from '../components/component.react';
import React from 'react';
import exposeRouter from '../components/exposerouter.react';
import immutable from 'immutable';
import {focusInvalidField} from '../lib/validation';
import {msg} from '../intl/store';

import formStyles from '../styles/form.js';

class Login extends Component {

  static propTypes = {
    auth: React.PropTypes.instanceOf(immutable.Map).isRequired,
    pendingActions: React.PropTypes.instanceOf(immutable.Map).isRequired,
    router: React.PropTypes.func,
  };

  getForm() {
    return this.props.auth.get('form');
  }

  onFormSubmit(e) {
    e.preventDefault();
    const fields = this.getForm().fields.toJS();
    actions.login(fields)
      .then(() => {
        this.redirectAfterLogin();
      })
      .catch(focusInvalidField(this));
  }

  redirectAfterLogin() {
    const {router} = this.props;
    const nextPath = router.getCurrentQuery().nextPath;

    router.replaceWith(nextPath || 'login');
  }

  render() {
    const form = this.getForm();
    const {pendingActions} = this.props;

    return (
      <div className='login'>
        <form onSubmit={(e) => this.onFormSubmit(e)} style={formStyles.form}>
          <fieldset
            style={formStyles.fieldset}
            disabled={pendingActions.has(actions.login.toString())}
          >
            <legend style={formStyles.legend}>
              {msg('forms.login.legend')}
            </legend>
            <div style={formStyles.field}>
              <label htmlFor="email" children={msg('forms.login.email.label')}>
              </label>
              <input
                style={formStyles.input}
                autoFocus
                name='email'
                onChange={actions.updateFormField}
                placeholder={msg('forms.login.email.placeholder')}
                value={form.fields.email}
              />
            </div>

            <div style={formStyles.field}>
              <label htmlFor='password' children={msg('forms.login.password.label')}>
              </label>
              <input
                style={formStyles.input}
                name='password'
                onChange={actions.updateFormField}
                placeholder={msg('forms.login.password.placeholder')}
                type='password'
                value={form.fields.password}
              />
            </div>

            <div style={formStyles.buttonContainer}>
              <button
                style={formStyles.button}
                children={msg('buttons.login')}
                type='submit'
              />
            </div>
            {/*
             <button type='submit'>{msg('buttons.signup')} />
            */}
            {form.error &&
              <span style={formStyles.errorMessage}>{form.error.message}</span>
            }
            <div>{msg('forms.login.hint')}</div>
          </fieldset>
        </form>
      </div>
    );
  }

}

export default exposeRouter(Login);
