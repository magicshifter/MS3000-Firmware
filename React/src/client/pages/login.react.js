import Component from '../components/component.react';
import DocumentTitle from 'react-document-title';
import LoginForm from '../auth/login.react';
import React from 'react';
import {msg} from '../intl/store';
import Page from '../components/page.react';

class Login extends Component {

  render() {
    return (
      <DocumentTitle title={msg('pages.login.title')}>
        <Page page='login'></Page>
        <LoginForm {...this.props} />
      </DocumentTitle>
    );
  }

}

export default Login;
