import Component from '../components/component.react';
import DocumentTitle from 'react-document-title';
import LoginForm from '../auth/login.react';
import React from 'react';
import {msg} from '../intl/store';
import PageTitle from '../components/pagetitle.react';

class Login extends Component {

  render() {
    return (
      <DocumentTitle title={msg('pages.login.title')}>
        <div className="login-page">
          <PageTitle page='login'></PageTitle>
          <LoginForm {...this.props} />
        </div>
      </DocumentTitle>
    );
  }

}

export default Login;
